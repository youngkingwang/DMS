//采用并发服务器，每个线程一个accept模式
#ifndef SERVER_H
#define SERVER_H

#include "datarecivethread.h"
#include "datasavethread.h"
#include "userdata.h"

class Server
{
public:
    Server();

    void initSocket();	//throw ServerException
    void startService();	//throw ServerException
private:
    char labIp[20];
    int listenfd;
    unsigned short port;
    DataReciveThread productor;
    DataSaveThread customer;
    UserData dataPoo;
};

#endif // SERVER_H
