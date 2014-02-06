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

    while(true)
    {
       sleep(2);
       cout << "save data to database !" << endl;
       UserData::pop_data();

    }
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

    return (void*)0;
}
