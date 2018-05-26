
#include "Client.h"

Client::Client()
{
    WSADATA wsa;
    int rslt=WSAStartup(WINSOCK_VERSION,&wsa);//加载WinSock DLL
    assert(rslt==0);
}
Client::~Client(void)
{
    if(m_socket!=INVALID_SOCKET)
        closesocket(m_socket);
    WSACleanup();//卸载WinSock DLL
}

void Client::init(int inet_type,char*addr,unsigned short port)
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
    if(m_type==TCP_DATA)//TCP数据
    {
        rslt=connect(m_socket,(sockaddr*)&serverAddr,sizeof(sockaddr));//客户端连接请求
        assert(rslt==0);
    }
}

void Client::sendData(const char * buff, const int len)
{
    int rslt;
    int addrLen = sizeof(sockaddr_in);
    if(m_type == TCP_DATA)//TCP数据
    {
        rslt = send(m_socket, buff, len, 0);
    } else if(m_type == UDP_DATA)//UDP数据
    {
        rslt = sendto(m_socket, buff, len, 0, (sockaddr*)&serverAddr, addrLen);
    }
    assert(rslt>0);
}

void Client::getData(char * buff,const int len)
{
    int rslt;
    int addrLen=sizeof(sockaddr_in);
    memset(buff,0,len);
    if(m_type==TCP_DATA)//TCP数据
    {
        rslt=recv(m_socket,buff,len,0);
    }
    else if(m_type==UDP_DATA)//UDP数据
    {
        rslt=recvfrom(m_socket,buff,len,0,(sockaddr*)&serverAddr,&addrLen);
    }
    assert(rslt>0);
}

char* Client::getProto()
{
    if(m_type==TCP_DATA)
        return "TCP";
    else if(m_type==UDP_DATA)
        return "UDP";
    else
        return "";
}

char* Client::getIP()
{
    return inet_ntoa(serverAddr.sin_addr);
}

unsigned short Client::getPort()
{
    return ntohs(serverAddr.sin_port);
}