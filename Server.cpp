
#include "Server.h"
#include <iostream>
using namespace std;
//服务器线程处理函数参数结构
struct SockParam
{
    SOCKET rsock;//远程的socket
    sockaddr_in *raddr;//远程地址结构
    Server*pServer;//服务器对象指针
    SockParam(SOCKET rs, sockaddr_in*ra, Server*ps)
    {
        rsock = rs;
        raddr = ra;
        pServer = ps;
    }
};

Server::Server()
{
    cs=new CRITICAL_SECTION();
    InitializeCriticalSection(cs);//初始化临界区
    WSADATA wsa;
    int rslt=WSAStartup(WINSOCK_VERSION,&wsa);//加载WinSock DLL
    assert(rslt==0);
}

Server::~Server(void)
{
    for(list<sockaddr_in*>::iterator i=clientAddrs.begin();i!=clientAddrs.end();++i)//清空客户端地址结构
    {
        delete *i;
    }
    clientAddrs.clear();
    if(m_socket!=INVALID_SOCKET)
        closesocket(m_socket);//关闭服务器socket
    WSACleanup();//卸载WinSock DLL
    DeleteCriticalSection(cs);
    delete cs;
}

char* Server::getProto()
{
    if(m_type==TCP_DATA)
        return "TCP";
    else if(m_type==UDP_DATA)
        return "UDP";
    else
        return "";
}

char* Server::getIP(sockaddr_in*addr)
{
    if(addr==NULL)
        addr=&serverAddr;
    return inet_ntoa(addr->sin_addr);
}

unsigned short Server::getPort(sockaddr_in*addr)
{
    if(addr==NULL)
        addr=&serverAddr;
    return htons(addr->sin_port);
}

sockaddr_in* Server::addClient(sockaddr_in client)
{
    sockaddr_in*pc=new sockaddr_in(client);
    clientAddrs.push_back(pc);
    return pc;
}

void Server::delClient(sockaddr_in *client)
{
    assert(client!=NULL);
    delete client;
    clientAddrs.remove(client);
}

void Server::init(int inet_type,char*addr,unsigned short port)
{
    int rslt;
    m_type=inet_type;
    if(m_type==TCP_DATA)//TCP数据
        m_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//创建TCP套接字
    else if(m_type==UDP_DATA)//UDP数据
        m_socket=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);//创建UDP套接字
    assert(m_socket!=INVALID_SOCKET);
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_addr.S_un.S_addr=inet_addr(addr);
    serverAddr.sin_port=htons(port);
    memset(serverAddr.sin_zero,0,8);
    rslt=bind(m_socket,(sockaddr*)&serverAddr,sizeof(serverAddr));//绑定地址和端口
    assert(rslt==0);
    if(m_type==TCP_DATA)//TCP需要侦听
    {
        rslt=listen(m_socket,MAX_TCP_CONNECT);//监听客户端连接
        assert(rslt==0);
    }
}

void Server::start()
{
    int rslt;
    sockaddr_in client;//客户端地址结构
    int addrLen=sizeof(client);
    SOCKET clientSock;//客户端socket
    char buff[MAX_BUFFER_LEN];//UDP数据缓存
    while(true)
    {
        if(m_type==TCP_DATA)//TCP数据
        {
            clientSock=accept(m_socket,(sockaddr*)&client,&addrLen);//接收请求
            if(clientSock==INVALID_SOCKET)
                break;
            assert(clientSock!=INVALID_SOCKET);
            sockaddr_in*pc=addClient(client);//添加一个客户端
            connect(pc);//连接处理函数
            SockParam sp(clientSock,pc,this);//参数结构
            HANDLE thread=CreateThread(NULL,0,threadProc,(LPVOID)&sp,0,NULL);//创建连接线程
            assert(thread!=NULL);
            CloseHandle(thread);//关闭线程
        }
        else if(m_type==UDP_DATA)//UDP数据
        {
            memset(buff,0,MAX_BUFFER_LEN);
            rslt=recvfrom(m_socket,buff,MAX_BUFFER_LEN,0,(sockaddr*)&client,&addrLen);
            assert(rslt>0);
            char resp[MAX_BUFFER_LEN]={0};//接收处理后的数据
            rslt=procRequest(&client,buff,rslt,resp);//处理请求
            rslt=sendto(m_socket,resp,rslt,0,(sockaddr*)&client,addrLen);//发送udp数据
        }
    }
}


DWORD WINAPI threadProc(LPVOID lpParam)//TCP线程处理函数
{
    SockParam sp=*(SockParam*)lpParam;
    Server*s=sp.pServer;
    SOCKET sock=s->m_socket;
    SOCKET clientSock=sp.rsock;
    sockaddr_in *clientAddr=sp.raddr;
    
    CRITICAL_SECTION*cs=s->cs;
    int rslt;
    char req[MAX_BUFFER_LEN+1]={0};//数据缓冲区,多留一个字节，方便输出
    do
    {
        rslt=recv(clientSock,req,MAX_BUFFER_LEN,0);//接收数据
        if(rslt<=0)
            break;
        char resp[MAX_BUFFER_LEN]={0};//接收处理后的数据
        EnterCriticalSection(cs);
        rslt=s->procRequest(clientAddr,req,rslt,resp);//处理后返回数据的长度
        LeaveCriticalSection(cs);
        assert(rslt<=MAX_BUFFER_LEN);//不会超过MAX_BUFFER_LEN
        rslt=send(clientSock,resp,rslt,0);//发送tcp数据
    }
    while(rslt!=0||rslt!=SOCKET_ERROR);
    s->delClient(clientAddr);
    s->disConnect(clientAddr);//断开连接后处理
    return 0;
}

void Server::connect(sockaddr_in*client)
{
    cout<<"client"<<getIP(client)<<"["<<getPort(client)<<"]"<<"connect."<<endl;
}

int Server::procRequest(sockaddr_in*client,const char* req,int reqLen,char*resp)
{
    cout<<getIP(client)<<"["<<getPort(client)<<"]:"<<req<<endl;
    if(m_type==TCP_DATA)
        strcpy_s(resp,10,"TCP回复");
    else if(m_type==UDP_DATA)
        strcpy_s(resp,10,"UDP回复");
    return 10;
}

void Server::disConnect(sockaddr_in*client)
{
    cout<<"client"<<getIP(client)<<"["<<getPort(client)<<"]"<<"disconnected."<<endl;
}
