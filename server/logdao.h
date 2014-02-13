#ifndef LOGDAO_H
#define LOGDAO_H

#include "../client/data.h"
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>

using namespace std;

class LogDao
{
public:
    LogDao();
    ~LogDao();

    void connect();
    void saveData(MatchedLogRec rec);
    void commitData();
    void disConnect();
private:
    QSqlDatabase db;
};

#endif // LOGDAO_H
