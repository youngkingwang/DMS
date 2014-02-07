#include "server.h"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

Server::Server()
{
    port = 5999;
}

void Server::initSocket() //should exception
{
    cout << "initial socket: "
            << "socket -->bind-->listen-->accept" << endl;
    sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1 )
    {
        cout << "err: socket exception" << endl;
        exit(-1);
    }

    if (bind(listenfd, (sockaddr*)&servaddr, sizeof(sockaddr) ) == -1)
    {
        cout << "err: bind exception " << endl;
        exit(-1);
    }

    listen(listenfd, 1024);

    for ( ; ;)
    {
        sockaddr *cliaddr = new sockaddr;
        socklen_t clilen;

         int connfd = accept(listenfd, cliaddr, &clilen);
         productor.start(connfd);

         delete cliaddr;
    }


}

//响应客户请求，开启线程为客户服务
void Server::startService() //should exception
{
    cout << "start service !" << endl;
    //1 initSocket()
    customer.start();

    initSocket();

    //2 initial user data buffer pool

    //3 initial and start data reciver thread
    //productor.start();

    //4 initial and start data save thread


}
