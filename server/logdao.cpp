#include "logdao.h"

LogDao::LogDao()
{

}

//连接数据库
void LogDao::connect()
{

}

/*存储数据，根据日志记录的推出时间放入相应的日志表中
 *如推出时间是8日，则放入logdetail08日志表中
*/
void LogDao::saveData(MatchedLogRec rec)
{

}

//提交事务
void LogDao::commitData()
{

}

//断开连接
void LogDao::disConnect()
{

}
