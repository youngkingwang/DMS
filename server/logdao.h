#ifndef LOGDAO_H
#define LOGDAO_H

#include "../client/data.h"

using namespace std;

class LogDao
{
public:
    LogDao();

    void connect();
    void saveData(MatchedLogRec rec);
    void commitData();
    void disConnect();

};

#endif // LOGDAO_H
