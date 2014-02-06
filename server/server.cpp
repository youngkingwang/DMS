#include "server.h"
#include <iostream>

using namespace std;

Server::Server()
{
}

void Server::initSocket()
{
    cout << "initial socket: "
            << "socket -->bind-->listen-->accept" << endl;

}

//响应客户请求，开启线程为客户服务
void Server::startService()
{
    cout << "start service !" << endl;

}
