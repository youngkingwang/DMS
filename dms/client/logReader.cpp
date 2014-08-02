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
#include <ctime>
#include <list>
#include <vector>


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
    time_t tt;
    struct tm t;
    WtmpxFormat temp;
    LogRec tempuse;

    FILE *p = fopen(backFileName, "rb");
    if(p == NULL)
        throw ReadException("open backfiel fail!");

    cout << "begin to read data" << endl;
    while(true)
    {
        memset(&temp,0,sizeof(temp));
        int i=fread(&temp,sizeof(temp),1,p);
        if (i<=0)
        {
            break;
        }
        temp.type = ntohs(temp.type);
        if (temp.type!=7&&temp.type!=8)
        {
            continue;
        }
        memset(&tempuse,0,sizeof(tempuse));

        temp.pid = ntohl(temp.pid);

        temp.logtime = ntohl(temp.logtime);

        strcpy(tempuse.logname,temp.logname);
        tempuse.pid=temp.pid;
        tempuse.logtime=temp.logtime;
        strcpy(tempuse.logip,temp.logip);

        if(memcmp(tempuse.logname,".",1)==0 || strlen(tempuse.logip) < 4)
        {
            continue;
        }
        if(temp.type==7)
        {
            logins.push_back(tempuse);

        }
        if(temp.type==8)
        {
            logouts.push_back(tempuse);
        }
    }
    fclose(p);
    for (auto i=begin(logins); i != end(logins); ++i)
    {
        time_t tt=i->logtime;
        struct tm *t = localtime(&tt);

        cout << i->logname << "\t" << i->pid << "\t" << i->logip << "\t"
             << t->tm_year <<"-" << t->tm_mon << "-" << t->tm_mday
             << " " << t->tm_hour <<":" << t->tm_min << ":" << t->tm_sec
             << endl;
    }

    cout << "login size: " << logins.size() << " logout size: " << logouts.size() << endl;
    //sleep(3);

}


//将登入/登出记录匹配为完整的登录记录，logins,logouts匹配存入matches
void LogReader::matchLogRec()throw(MatchLogRecException)
{

    readFailLogins();
    size_t pos = 0;//有效区间的初始位置
    size_t loginSize = logins.size();
    size_t logoutSize = logouts.size();
    size_t maxSize;
    size_t matchSize = 0;//计数器：统计已经成功匹配的记录
    bool isMatch = false;

    maxSize = (loginSize > logoutSize) ? loginSize : logoutSize;
    cout << loginSize << " " << logoutSize << " " << maxSize << endl;

    const char *cmd = "/home/test/workspace/dms/client/getip.sh";

    for (int i=0; i < logoutSize; ++i)
    {
        isMatch = false;
        const LogRec& logout = logouts[i];
        LogRec login;
        for (int j=0; j < loginSize; ++j)
        {
            login = logins[j];
            if (login.pid ==logout.pid && strcmp(login.logip, logout.logip) == 0 && strcmp(login.logname, logout.logname) == 0)
            {
                if (login.logtime < logout.logtime)
                {
                    MatchedLogRec tempMatch;
                    strcpy(tempMatch.logname, login.logname);
                    tempMatch.pid = login.pid;
                    strcpy(tempMatch.logip, login.logip);
                    tempMatch.loginTime = login.logtime;
                    tempMatch.logoutTime = logout.logtime;
                    tempMatch.durations = logout.logtime - login.logtime;

                    execmd(cmd, tempMatch.labip);

                    matches.push_back(tempMatch);
                    ++matchSize;
                    isMatch = true;
                    cout << "-----------------match OK-----------------" << endl;
                    break;
                }

            }

        }
        if (isMatch == false){
            saveFailLogins(login);
            continue;
        }

    cout << "matches size: " << matchSize << endl;


    }
    for (auto i=begin(matches); i != end(matches); ++i)
        cout << "---test--" << i->logip << " " << i->labip << endl;

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
