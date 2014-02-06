#ifndef DATASAVETHREAD_H
#define DATASAVETHREAD_H

#include "logdao.h"


class DataSaveThread
{
public:
    DataSaveThread();

    void run();
    void start();
    void* saveData(void *par);
    

private:
    int threadid;
    LogDao logDao;
};

#endif // DATASAVETHREAD_H
