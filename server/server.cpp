#include "server.h"
#include <iostream>

using namespace std;

Server::Server()
{
}

void Server::initSocket() //should exception
{
    cout << "initial socket: "
            << "socket -->bind-->listen-->accept" << endl;
    //1 initial server socket
    //2

}

//响应客户请求，开启线程为客户服务
void Server::startService() //should exception
{
    cout << "start service !" << endl;
    //1 initSocket()
    initSocket();

    //2 initial user data buffer pool

    //3 initial and start data reciver thread
    productor.start();

    //4 initial and start data save thread
    customer.start();


}
