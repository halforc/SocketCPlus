// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../../Inet.h"
#include <iostream>
using namespace std;

void testServer()
{
    int type;
    cout << "ѡ��ͨ������(TCP=0/UDP=1):";
    cin >> type;
    Server s;
    if(type == 1)
        s.init(UDP_DATA, "127.0.0.1", 60090);
    else
        s.init(TCP_DATA, "127.0.0.1", 60080);
    cout << s.getProto() << "������" << s.getIP() << "[" << s.getPort() << "]" << "�����ɹ���" << endl;
    s.start();
}

void testClient()
{
    int type;
    cout << "ѡ��ͨ������(TCP=0/UDP=1):";
    cin >> type;
    Client c;
    if(type == 1)
        c.init(UDP_DATA, "127.0.0.1", 60090);
    else
        c.init(TCP_DATA, "127.0.0.1", 60080);
    cout << "�ͻ��˷����" << c.getIP() << "[" << c.getPort() << "]��" << c.getProto() << "���ӡ�" << endl;
    char buff[MAX_BUFFER_LEN];
    while(true) {
        cout << "����" << c.getProto() << "���ݵ�" << c.getIP() << "[" << c.getPort() << "]:";
        cin >> buff;
        if(strcmp(buff, "q") == 0)
            break;
        c.sendData(buff, MAX_BUFFER_LEN);
        c.getData(buff, MAX_BUFFER_LEN);
        cout << "����" << c.getProto() << "���ݴ�" << c.getIP() << "[" << c.getPort() << "]:" << buff << endl;
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    int flag;
    cout << "����������/�ͻ��ˣ�0-������|1-�ͻ��ˣ�:";
    cin >> flag;
    if(flag == 0)
        testServer();
    else
        testClient();
    system("pause");
    return 0;
}

