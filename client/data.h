#ifndef DATA_H
#define DATA_H
#include <unistd.h>
#include <string>
#include <iostream>
#include <cstring>

using namespace std;

struct LogRec{
    char logname[32];	//user login/logout name(IP)
    int pid;	//process id
    short type;	// login:7, logout:8
	int logtime;	//登入/登出时刻（秒）
    char logip[25];	//host name


};
bool operator== (const LogRec& arg1, const LogRec& arg2 );

struct MatchedLogRec{
    char logname[32];	//char logname[32];
	int pid;
	int loginTime;   
	int logoutTime;
	int durations;	//用户在线时间长度（秒）
    char logip[257];	//remote host name
    char labip[257];	//dababase
};

ostream& operator<< (ostream& os, const MatchedLogRec& matchLog);

#endif
