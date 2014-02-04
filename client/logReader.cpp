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
    sprintf(cmdback, "/home/oracle/dms/client/backup.sh %s %s",
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

        read(fd, logname, 32);

        lseek(fd, 36, SEEK_CUR);

        pid = ntohl(pid);

        read(fd, &pid, sizeof(pid));
        if(read(fd, &type, sizeof(type)) == -1)
            cout << "err: read type" << endl, exit(-1);
        type = ntohs(type);

        lseek(fd, 6, SEEK_CUR);

        read(fd, &logtime, sizeof(logtime));

        lseek(fd, 28, SEEK_CUR);

        read(fd, &len, sizeof(len));
        len = ntohs(len);

        read(fd, logip, sizeof(logip));
        logip[len] = 0;

        //cout << logname << ", " << type << ", " << logip << endl;

        if (memcmp(logname, ".", 1) != 0)
        {
            LogRec log;
            memcpy(log.logname, logname, sizeof(logname));
            memcpy(log.logip, logip, 32);
            log.logtime = logtime;
            if(type == 7) {
                logins.push_back(log);
//            cout << "======test " << log.logname << endl;
           }
            if (type == 8) {
//            cout << "======test " << log.logname << endl;
                logouts.push_back((log));
            }
           // cout << log.logname << endl;
        //cout << logname << ", " << type << ", " << logip << endl;
//            cout << "------------------test....." <<log.type << endl;
        }

    }
    cout <<"login " << logins.size()
                << ", log out " << logouts.size() << endl;

    list<LogRec>::iterator it;

    if(logins.empty() == true)
        cout << "logins is empty!" << endl;
    //for (it=logins.begin(); it != logins.end(); ++it) {
    //cout << it->logname << endl;
   // }
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
   const char *cmd = "/home/oracle/dms/client/getip.sh";

   for(auto i=begin(logins); i != end(logins); ++i)
   {
       result = find(begin(logouts), end(logouts), *i);
       if(result != last)
       {
           bzero(&matchedLog, 0);
           strcpy(matchedLog.logname, i->logname);
           matchedLog.pid = getpid();
           strcpy(matchedLog.logip , i->logip);
           matchedLog.loginTime = i->logtime;
           matchedLog.logoutTime = result->logtime;
           matchedLog.durations = matchedLog.logoutTime - matchedLog.loginTime;
           execmd(cmd, matchedLog.labip);
           //cout << matchedLog << endl;
           //cout << matchedLog.logname << endl;
           matches.push_back(matchedLog);
           //cout << matchedLog << endl;

       }
       else
       {
           saveFailLogins(*i);
       }
       cout << "matches size: " << matches.size() << endl;
       //cout << "-------------test-----------" << endl;
       //cout << logins.size() << " " << logouts.size() << endl;


       //bool isMatch = false;
       //result = find(begin(logouts), end(logouts), *i);
       /*
       for (auto j=begin(logouts); j != end(logouts); ++j)
       {
           //	3.1 如果一个元素匹配成功，则将该元素存入matches中
           if (*i == *j)
           {
               //matches.push_back(static_cast<MatchedLogRec>(i));
               bzero(&matchedLog, 0);
               strcpy(matchedLog.logname, i->logname);
               matchedLog.pid = getpid();
               strcpy(matchedLog.logip , i->logip);
               matchedLog.loginTime = i->logtime;
               matchedLog.logoutTime = j->logtime;
               matchedLog.durations = matchedLog.logoutTime - matchedLog.loginTime;
               execmd(cmd, matchedLog.labip);
               //cout << matchedLog << endl;
               //cout << matchedLog.logname << endl;
               matches.push_back(matchedLog);
               //cout << matchedLog << endl;
               isMatch = true;
               break;

               /*
           cout << matchedLog.logname << " "
                << matchedLog.pid << " "

                << matchedLog.logip << " "
                << matchedLog.loginTime << " "
                << matchedLog.logoutTime << " "
                << matchedLog.durations  << " "
                << matchedLog.labip;
                */
       }


   /*
       cout << "matches size: " << matches.size() << endl;
       if (!isMatch)
           saveFailLogins(*i);
           */

       //	3.2 否则如果失败，则将该元素存入文件logins.dat
           /*
       else
       {
           /*
       fd = open("logins.dat", O_WRONLY|O_TRUNC);
       if (fd == -1)
       {
       throw MatchLogRecException("err:open logins.dat fail!");
       }
       write(fd, *i, logRecSize);
       close(fd);
       /
           saveFailLogins(*i);

       }
       */

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
