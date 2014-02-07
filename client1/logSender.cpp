#include "logSender.h"
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

LogSender::LogSender()
{
    memcpy(failSendFileName,"failsend.dat", sizeof("failsend.dat"));
    memcpy(serverIp, "127.0.0.1", sizeof("127.0.0.1"));
    port = 5999;	//match server port
}

LogSender::~LogSender()
{
}

//发送匹配的记录到服务器, 这是总调用函数
void LogSender::sendMatchs(list<MatchedLogRec>& matches)throw(SendDataException)
{
    try
    {
        //initNetWorK();
        cout << "初始化网络" << endl;
        initNetWork();

        try
        {
            readSendFailed(matches);
            for(auto it=begin(matches); it != end(matches); ++it)
            {
                cout << "send: " << it->logname << " "
                        << it->logip << endl;
                int w = write(sockfd, &(*it), sizeof(MatchedLogRec));
                if(w == -1)
                {
                    saveSendFailed(matches);
                    throw SendDataException("sendDataException: write failed");
                }
                matches.pop_back();
            }
            cout << "发送匹配的记录到服务器" << endl;
        }

        catch(SendDataException e)
        {cout << e.what() << endl;}
    }
    catch(SocketException e)
    {
        cout << e.what() << endl;
    }
}

//根据ip port及serverIp建立到服务器端的socket连接,初始化网络
void LogSender::initNetWork()throw(SocketException)
{
    cout << "初始化网络" << endl;
    try{
        sockfd = socket(AF_INET, SOCK_STREAM, NULL);
        if (sockfd == -1)
            throw SocketException("socket err: cannot build socket!");
    }
    catch (SocketException e)
    {
        cout << e.what() << endl;
    }

    sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, serverIp, &servaddr.sin_addr);

    try{
        if(connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr)))
            throw SocketException("connect err: cannot connect server");
    }
    catch (SocketException e)
    {
        cout << e.what() << endl;
    }

}

//读取发送失败的数据文件
void LogSender::readSendFailed(list<MatchedLogRec>& match)throw(ReadException)
{
    cout << "读取发送失败的数据文件" << endl;
    int fd = open(failSendFileName, O_RDONLY);
    if (fd == -1)
        throw ReadException("err: cannot open failSendFile!");
    struct stat st;
    if(fstat(fd, &st) == -1) {
        throw ReadException("Cannot get failsend.dat size");
    }
    size_t matchedLogRecSize = sizeof(MatchedLogRec);
    int count = st.st_size / matchedLogRecSize;

    cout << "=======begin to read failsend.dat======" << endl;
    MatchedLogRec log;
    for (int i=0; i<count; ++i)
    {
        lseek(fd, i*matchedLogRecSize, SEEK_SET);
        if (read(fd, &log, matchedLogRecSize) != -1)
        {
            match.push_back(log);
        }
    }

    close(fd);
}

//存储发送失败的匹配记录，以备下次发送
void LogSender::saveSendFailed(const list<MatchedLogRec>& match)throw(SendException)
{
    cout << "存储发送失败的匹配记录" << endl;
    int fd = open(failSendFileName, O_WRONLY|O_TRUNC);
    if (fd == -1)
        throw SendException("saveSendFailed err: open failsend.dat faill!");
    for (auto it=begin(match); it != end(match); ++it)
    {
        write(fd, &(*it), sizeof(MatchedLogRec));
    }
    close(fd);
}
