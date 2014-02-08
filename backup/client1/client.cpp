#include "client.h"
#include <iostream>

using namespace std;

//执行采集客户端的完整过程，logReader负责读取，logSender负责发送
void Client::sendLogs()throw(ClientException)
{
	cout << "开始采集数据....." << endl;
	//logReader.readLogs();
	logSender.sendMatchs(logReader.readLogs());
}
void Client::setReader(const LogReader& read)
{
	logReader = read;
}
void Client::setSender(const LogSender& send)
{
	logSender = send;
}
