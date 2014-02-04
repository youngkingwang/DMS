#ifndef LOG_SENDER_H
#define LOG_SENDER_H

#include "data.h"
#include "clientException.h"
#include <string>
#include <list>

using namespace std;

class LogSender{
public:
	LogSender();
	~LogSender();

    void sendMatchs(list<MatchedLogRec>& matches)throw(SendDataException);
	void initNetWork()throw(SocketException);
    void readSendFailed(list<MatchedLogRec>& match)throw(ReadException);
	void saveSendFailed(const list<MatchedLogRec>& match)throw(SendException);
private:
    char failSendFileName[50];	//char failSendFileName[50];
    int sockfd;
	unsigned short port;
    char serverIp[32];	//char serverIp[32];
};

#endif
