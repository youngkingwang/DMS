#include <QCoreApplication>
#include "server.h"
#include <iostream>
#include <signal.h>
#include <unistd.h>

using namespace std;

void sig_int(int signo)
{
    cout << "server stop !" << endl;
    exit(0);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    signal(SIGINT, sig_int);
    Server server;

    server.startService();

    return a.exec();
}
