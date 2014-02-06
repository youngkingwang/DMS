//消费者
#ifndef DATASAVETHREAD_H
#define DATASAVETHREAD_H

#include "logdao.h"

//consumer
class DataSaveThread
{
public:
    DataSaveThread();
    ~DataSaveThread();

    void run();
    void start();
    static void* saveData(void *par);
    

private:
    pthread_t threadid;
    LogDao logDao;
};

#endif // DATASAVETHREAD_H
