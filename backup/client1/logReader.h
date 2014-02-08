#ifndef LOG_READER_H
#define LOG_READER_H

#include "data.h"
#include "clientException.h"
#include<list>
#include<string>

using namespace std;

class LogReader{
public:
	LogReader();
	~LogReader();

    list<MatchedLogRec>& readLogs()throw(MatchLogRecException);//读取日志
	void readFailLogins()throw(ReadFailLoginException);	//读取匹配失败的登入记录
	void readBackupFile() throw (ReadException);	//读取备份文件
	void matchLogRec() throw (MatchLogRecException);	//匹配logins & logout
	void saveFailLogins(const LogRec& logfail) throw (ClientException);	//将匹配失败的登入记录保存到logins.dat

	list<MatchedLogRec>& getMatches();
private:
    char logFileName[50];	//char logFileName[50];
    char backFileName[50];	//char backfileName[50];
    char failLoginsFileName[50];	//char failLoginsFileName[50];
	list<LogRec> logins;
	list<LogRec> logouts;
	list<MatchedLogRec> matches;

	void backup() throw(BackupException);	//备份日志文件wtmpx
        bool execmd(const char* cmd, char* result);
};
#endif
