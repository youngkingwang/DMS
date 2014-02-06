#include <QCoreApplication>
#include "server.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;

    server.startService();

    return a.exec();
}
