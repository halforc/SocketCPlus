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

/*******************客户端*************************/
//客户端类
class Client
{
    int m_type;//通信协议类型
    SOCKET m_socket;//本地套接字
    sockaddr_in serverAddr;//服务器地址结构
public:
    Client();
    void init(int inet_type,char*addr,unsigned short port);//初始化通信协议，地址，端口
    char*getProto();//获取通信协议类型
    char*getIP();//获取IP地址
    unsigned short getPort();//获取端口
    void sendData(const char * buff,const int len);//发送数据
    void getData(char * buff,const int len);//接收数据
    virtual ~Client(void);
};