#include "logReader.h"
#include "data.h"
#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include<cstdio>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <algorithm>

using namespace std;

LogReader::LogReader()
{
    memcpy(logFileName, "wtmpx", sizeof("wtmpx"));
    memcpy(failLoginsFileName , "logins.dat", sizeof("logins.dat"));
}

LogReader::~LogReader()
{
}

/*读取日志文件总调动函数，得到匹配好的集合*/
list<MatchedLogRec>& LogReader::readLogs()throw(MatchLogRecException)
{
	cout << "开始读取日志" << endl;
	backup();
	readBackupFile();
	matchLogRec();
	cout << "读取日志->得到匹配好的日志" << endl;	
		
	return matches;
	
}

/*备份日志文件，把变化的文件处理成不变的，把日志文件改名即可，
 *系统会自动生成wtmpx
 */
void LogReader::backup()throw(BackupException)
{
	cout << "开始备份文件->备份文件完成" << endl;
	char cmdback[70];
	time_t tt;
	struct tm *t;

    tt = time(NULL);
    t = localtime(&tt);
    sprintf(backFileName, "wtmpx%04d%02d%02d%02d%02d%02d",
            t->tm_year + 1900,
            t->tm_mon + 1,
            t->tm_mday,
            t->tm_hour,
            t->tm_min,
            t->tm_sec);
    sprintf(cmdback, "/home/test/workspace/dms/client/backup.sh %s %s",
            logFileName,
            backFileName);
    system("pwd");
    cout << cmdback << endl;
    int r = system(cmdback);
    if (WEXITSTATUS(r) == 1 || WEXITSTATUS(r) == 2)
    {
        throw BackupException("日志拷贝错误");
    }
    cout << "-----------备份日志成功" << endl;


}

//读取上一次没有匹配的登入记录
void LogReader::readFailLogins()throw(ReadFailLoginException)
{
    cout << "读取匹配失败的登录记录" << endl;
    int fd = open(failLoginsFileName,O_RDONLY);
    if( fd == -1) {
        throw MatchLogRecException("Exception: Cannot open logins.dat ");
    }

    struct stat st;
    if(fstat(fd, &st) == -1) {
        throw MatchLogRecException("Cannot get logins.dat size");
    }
    size_t logRecSize = sizeof(LogRec);
    int count = st.st_size / logRecSize;

    cout << "=======begin to read logins.dat======" << endl;
    LogRec log;
    for (int i=0; i<count; ++i)
    {
        lseek(fd, i*logRecSize, SEEK_SET);
        if (read(fd, &log, logRecSize) != -1)
        {
           logins.push_back(log);
        }
    }
    cout << "=====end to read logins.dat" << endl;
    close(fd);
}

//读取备份日志文件，把读取到的数据存储到对应的属性中
void LogReader::readBackupFile()throw(ReadException)
{
    /**
         * 数据格式说明
         * 位置范围	字节长度	含义
         * 000-031	32	 user login name
         * 032-035	4	 inittab id
         * 036-067	32	 device name (console, lnxx)
         * 068-071	4	 process id
         * 072-073	2	 type of entry
         * 074-075	2	 process termination
         * 076-077	2	 exit status
         *          2	 这是C数据类型补齐产生的空位
         * 080-083	4	 time entry was made  seconds
         * 084-087	4	 and microseconds
         * 088-091	4	 session ID, used for windowing
         * 092-111	20	 reserved for future use
         * 112-113	2	 significant length of ut_host
         * 114-371	257	 remote host name
         */
    cout << "读取备份的文件" << endl;
    int fd;
    int count;
    struct stat st;
    char logname[34];
    char logip[257];
    short type;	//2 log type 7: log, 8: logout
    long logtime;	//log time:4
    short len;	//ip length:2
    pid_t pid;	//process ID:4

    fd = open(backFileName, O_RDONLY);
    if(fd == -1) {
        throw ReadException("Cannot open backup file!");
    }

    //2 get count of recorde
    if (fstat(fd, &st) == -1) {
       throw ReadException("Cannot get backup file size!") ;
    }

    count = st.st_size / 372;

    //3 read data
    cout << "========= read log ============" << endl;
    for (int i = 0; i < count; ++i)
    {
        lseek(fd, i*372, SEEK_SET);

        //read logname
        read(fd, logname, 32);

        //skip 36bytes
        lseek(fd, 36, SEEK_CUR);

        //read pid
        pid = ntohl(pid);

        read(fd, &pid, sizeof(pid));

        //read type:login 7, logout 8
        if(read(fd, &type, sizeof(type)) == -1)
            cout << "err: read type" << endl, exit(-1);
        type = ntohs(type);

        //skip 6 bytes
        lseek(fd, 6, SEEK_CUR);

        //read login time or logout time
        read(fd, &logtime, sizeof(logtime));

        //skip 28 bytes
        lseek(fd, 28, SEEK_CUR);

        //read the length of IP
        read(fd, &len, sizeof(len));
        len = ntohs(len);

        //read the IP address
        read(fd, logip, sizeof(logip));
        logip[len] = 0;


        //ignore the record which logname begin with "." token
        if ((memcmp(logname, ".", 1) != 0) &&
                memcmp(logname, "root", sizeof("root")) &&
                (type == 7 || type == 8))
        {
            LogRec log;
            memcpy(log.logname, logname, sizeof(logname));
            memcpy(log.logip, logip, 32);
            log.logtime = logtime;
            log.type = type;
            log.pid = pid;

            if(type == 7) {//login type
                logins.push_back(log);
           }
            if (type == 8) {//logout type
                logouts.push_back((log));
            }
        } else {
            continue;
        }

    }
    cout <<"login " << logins.size()
                << ", log out " << logouts.size() << endl;
}

//将登入/登出记录匹配为完整的登录记录，logins,logouts匹配存入matches
void LogReader::matchLogRec()throw(MatchLogRecException)
{

    cout << "开始匹配记录" << endl;
    //1、备份logins.dat文件
    //2、读取logins.dat文件，将读到元素存入logins,然后清空logins.dat
   readFailLogins();
        //make logins.dat empty

    //3、将logins中每个元素与logouts进行循环比较
   auto last = end(logouts);
   auto result = end(logouts);
   MatchedLogRec matchedLog;
   const char *cmd = "/home/test/workspace/dms/client/getip.sh";

   for(auto i=begin(logins); i != end(logins); ++i)
   {
       /* find the logout record matching with login record
        * here, overloading operator== which compare login and logout
        * record whether  login match with logout or not*/
       result = find(begin(logouts), end(logouts), *i);
       //matched
       if(result != last)
       {
           bzero(&matchedLog, 0);

           strcpy(matchedLog.logname, i->logname);
           matchedLog.pid = getpid();
           strcpy(matchedLog.logip , i->logip);
           matchedLog.loginTime = i->logtime;
           matchedLog.logoutTime = result->logtime;
           matchedLog.durations = matchedLog.logoutTime - matchedLog.loginTime;
           //cout << "login time: " << i->logtime <<"logout: "
            //       << result->logtime << endl;

           execmd(cmd, matchedLog.labip);//get labip
           matches.push_back(matchedLog);

       }
       else
       {
           saveFailLogins(*i);
       }
       cout << "matches size: " << matches.size() << endl;

             }


   cout << "matches size: " << matches.size() << endl;

}

//将匹配的失败的登入记录存到文件logins.dat
void LogReader::saveFailLogins(const LogRec& logfail)throw(ClientException)
{
    cout << "保存匹配失败的登入记录到文件logins.dat" << endl;
    int fd = open(failLoginsFileName, O_WRONLY|O_TRUNC);
    if (fd == -1)
    {
        throw MatchLogRecException("err:open logins.dat fail!");
    }
    write(fd, &logfail,sizeof(logfail));
    close(fd);

}

list<MatchedLogRec>& LogReader::getMatches()
{
    //cout << "getMatches ---------test-------" << endl;
    return matches;
}

bool LogReader::execmd(const char* cmd, char* result)
{
    char buffer[124];
    bzero(result, sizeof(result));
    bzero(buffer, sizeof(buffer));
    FILE *pipe = popen(cmd, "r");
    if (!pipe)
        return false;
    while(!feof(pipe)) {
        if(fgets(buffer, sizeof(buffer), pipe))
            strcat(result, buffer);
    }
    pclose(pipe);

    return true;
}
