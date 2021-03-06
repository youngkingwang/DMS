#include "datasavethread.h"
#include "../client/data.h"
#include "userdata.h"
#include "logdao.h"
#include <iostream>
#include <pthread.h>

using namespace std;


DataSaveThread::DataSaveThread()
{
    logDao.connect();
}
DataSaveThread::~DataSaveThread()
{
    logDao.disConnect();
}

//完成具体的调用dao的工作
void DataSaveThread::run()
{
    cout << "invoke database!" << endl;
    MatchedLogRec log;

    while(UserData::isEmpty() == false) // data buffer is not empty
    {
       sleep(2);
       cout << "save data to database !" << endl;

       pthread_mutex_lock(&UserData::mutex);
       if(UserData::isEmpty() == true)
       {
           pthread_cond_wait(&UserData::bufferNotEmpty, &UserData::mutex);
       }
       log = UserData::pop_data();
       logDao.saveData(log);
       pthread_mutex_unlock(&UserData::mutex);
    }

    logDao.commitData();
}

//完成线程的创建和启动线程
void DataSaveThread::start()
{
    cout << "create save data thread !" << endl
         << "start save data thread !" << endl;
    pthread_create(&threadid, 0, saveData, this);
}

//创建线程时调用的线程函数
void* DataSaveThread::saveData(void *par)
{
    cout << "save data thread function !" << endl;

    DataSaveThread *th = (DataSaveThread*)par;

    th->run();

    return nullptr;
}
