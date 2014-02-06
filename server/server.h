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
    int servfd;
    DataReciveThread productor;
    DataSaveThread curstomer;
    UserData dataPoo;
};

#endif // SERVER_H
