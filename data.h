#ifndef DATA_H
#define DATA_H
#include <unistd.h>
#include <string>
#include <iostream>
#include <cstring>

using namespace std;

struct LogRec{
    char logname[32];	//char logname[32];
	int pid;
	short type;
	int logtime;	//登入/登出时刻（秒）
    char logip[25];	//char logip[25];


};
bool operator== (const LogRec& arg1, const LogRec& arg2 );

struct MatchedLogRec{
    char logname[32];	//char logname[32];
	int pid;
	int loginTime;   
	int logoutTime;
	int durations;	//用户在线时间长度（秒）
    char logip[257];	//char logip[257];
    char labip[257];	//char labip[257];
};

ostream& operator<< (ostream& os, const MatchedLogRec& matchLog);

#endif
