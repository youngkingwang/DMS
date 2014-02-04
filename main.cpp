#include <QCoreApplication>
#include "logReader.h"
#include "logSender.h"
#include "clientException.h"
#include "client.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    LogReader logRead;
	LogSender logSend;

	Client client;

	try
	{
		client.sendLogs();

	}
	catch(ClientException e)
	{
		cout << e.what() << endl;
    }


    return a.exec();
}
