#include "logdao.h"
#include "../client/data.h"
#include <iostream>
#include <QtSql/QSql>
#include <QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QDateTime>
#include <QDebug>
#include <arpa/inet.h>

using namespace std;

LogDao::LogDao()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("test");
    db.setUserName("root");
    db.setPassword("wang");

}
LogDao::~LogDao()
{
    db.commit();
    db.close();
}

//连接数据库
void LogDao::connect()
{
    cout << "begin to connect database---------- !" << endl;

    if (!db.open())
        qDebug() << "Failed to connect to root mysql admin";
    else
        qDebug() << "Connect OK!";
}

/*存储数据，根据日志记录的推出时间放入相应的日志表中
 *如推出时间是8日，则放入logdetail08日志表中
*/
void LogDao::saveData(const MatchedLogRec& rec)
{
    //cout << "save data and into database !" << endl;
    //cout << "=========  save data test ========" << endl;

    cout << "save to database: " << rec.logname
         << " " << rec.logip << endl;
    QSqlQuery query;
    QDateTime logintime, logouttime;
    logintime.setTime_t(rec.loginTime);
    logouttime.setTime_t(rec.logoutTime);

    //get table name
    int logoutday = logouttime.date().day();
    char buf[20];
    sprintf(buf, " details_%02d ", logoutday);
    QString table(buf);
    QString fieldname(" (loginname,loginip,logindate,logoutdate,labip,duration)");
    //QString value(" VALUES (:rec.logname,:rec.logip,:logintime,:logouttime,:rec.labip,:rec.duration)");
    QString value(" VALUES (?, ?, ?, ?, ?, ?) ");
    QString insert("INSERT INTO " + table);


    query.prepare(insert + fieldname + value );
    query.addBindValue(rec.logname);
    query.addBindValue(rec.logip);
    query.addBindValue(logintime);
    query.addBindValue(logouttime);
    query.addBindValue(rec.labip);
    query.addBindValue(rec.durations);

    qDebug() << insert + fieldname + value;
    cout << rec.logip << endl;
    bool state = query.exec();
    if (state == true)
    {
        qDebug() << "Insert OK!" << endl;
    } else{
        qDebug() << "Insert Fail!";
        exit(-1);
    }

}

//提交事务
void LogDao::commitData()
{
    cout << "commit data !" << endl;
    if (db.commit() == false)
        qDebug() << "commit fail!";
}

//断开连接
void LogDao::disConnect()
{
    cout << "disconnect database !" << endl;
    db.close();

}
