#ifndef CLIENT_H
#define CLIENT_H

#include "logReader.h"
#include "logSender.h"
#include "clientException.h"

class Client
{
public:
	void sendLogs()throw(ClientException);
	void setReader(const LogReader& read);
	void setSender(const LogSender& send);
private:
	LogReader logReader;
	LogSender logSender;
};
#endif

