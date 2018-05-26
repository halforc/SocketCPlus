/*********************服务器********************/
//服务器类
#pragma once
/********************公用数据预定义***************************/

//WinSock必须的头文件和库
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

//辅助头文件
#include <assert.h>

//网络数据类型
#define TCP_DATA 1
#define UDP_DATA 2

//TCP连接限制
#define MAX_TCP_CONNECT 10

//缓冲区上限
#define MAX_BUFFER_LEN 1024

#include <list>
using namespace std;

class Server
{
    CRITICAL_SECTION *cs;//临界区对象
    int m_type;//记录数据包类型
    SOCKET m_socket;//本地socket
    sockaddr_in serverAddr;//服务器地址
    list<sockaddr_in*> clientAddrs;//客户端地址结构列表
    sockaddr_in* addClient(sockaddr_in client);//添加客户端地址结构
    void delClient(sockaddr_in *client);//删除客户端地址结构
    friend DWORD WINAPI threadProc(LPVOID lpParam);//线程处理函数作为友元函数
public:
    Server();
    void init(int inet_type,char*addr,unsigned short port);
    void start();//启动服务器
    char* getProto();//获取协议类型
    char* getIP(sockaddr_in*serverAddr=NULL);//获取IP
    unsigned short getPort(sockaddr_in*serverAddr=NULL);//获取端口
    virtual void connect(sockaddr_in*client);//连接时候处理
    virtual int procRequest(sockaddr_in*client,const char* req,int reqLen,char*resp);//处理客户端请求
    virtual void disConnect(sockaddr_in*client);//断开时候处理
    virtual ~Server(void);
};