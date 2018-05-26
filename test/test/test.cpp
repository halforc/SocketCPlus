// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../../Inet.h"
#include <iostream>
using namespace std;

void testServer()
{
    int type;
    cout << "选择通信类型(TCP=0/UDP=1):";
    cin >> type;
    Server s;
    if(type == 1)
        s.init(UDP_DATA, "127.0.0.1", 60090);
    else
        s.init(TCP_DATA, "127.0.0.1", 60080);
    cout << s.getProto() << "服务器" << s.getIP() << "[" << s.getPort() << "]" << "启动成功。" << endl;
    s.start();
}

void testClient()
{
    int type;
    cout << "选择通信类型(TCP=0/UDP=1):";
    cin >> type;
    Client c;
    if(type == 1)
        c.init(UDP_DATA, "127.0.0.1", 60090);
    else
        c.init(TCP_DATA, "127.0.0.1", 60080);
    cout << "客户端发起对" << c.getIP() << "[" << c.getPort() << "]的" << c.getProto() << "连接。" << endl;
    char buff[MAX_BUFFER_LEN];
    while(true) {
        cout << "发送" << c.getProto() << "数据到" << c.getIP() << "[" << c.getPort() << "]:";
        cin >> buff;
        if(strcmp(buff, "q") == 0)
            break;
        c.sendData(buff, MAX_BUFFER_LEN);
        c.getData(buff, MAX_BUFFER_LEN);
        cout << "接收" << c.getProto() << "数据从" << c.getIP() << "[" << c.getPort() << "]:" << buff << endl;
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    int flag;
    cout << "构建服务器/客户端（0-服务器|1-客户端）:";
    cin >> flag;
    if(flag == 0)
        testServer();
    else
        testClient();
    system("pause");
    return 0;
}

