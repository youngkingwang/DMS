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
    long logtime;	//登入/登出时刻（秒）
    char logip[257];	//host name


};
bool operator== (const LogRec& arg1, const LogRec& arg2 );

struct MatchedLogRec{
    char logname[32];	//char logname[32];
	int pid;
    long loginTime;
    long logoutTime;
	int durations;	//用户在线时间长度（秒）
    char logip[257];	//remote host name
    char labip[257];	//dababase
};

bool operator< (const LogRec& l, const LogRec& r);

struct WtmpxFormat
{
    char logname[32];
    char none1[36];
    unsigned int pid;
    unsigned short type;
    short none2;
    int exitnum;
    long logtime;
    char none3[30];
    char logip[258];
};

ostream& operator<< (ostream& os, const MatchedLogRec& matchLog);

#endif
