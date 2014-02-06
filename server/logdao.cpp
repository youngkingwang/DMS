#include "logdao.h"
#include <iostream>

using namespace std;

LogDao::LogDao()
{
}
LogDao::~LogDao()
{
}

//连接数据库
void LogDao::connect()
{
    cout << "connect database !" << endl;

}

/*存储数据，根据日志记录的推出时间放入相应的日志表中
 *如推出时间是8日，则放入logdetail08日志表中
*/
void LogDao::saveData(MatchedLogRec rec)
{
    cout << "save data and into database !" << endl;

}

//提交事务
void LogDao::commitData()
{
    cout << "commit data !" << endl;

}

//断开连接
void LogDao::disConnect()
{
    cout << "disconnect database !" << endl;

}
