#include "datarecivethread.h"
#include "../client/data.h"
#include "userdata.h"
#include <iostream>
#include <pthread.h>
#include <logdao.h>

using namespace std;

DataReciveThread::DataReciveThread()
{
}

//完成具体的数据接收工作
void DataReciveThread::run()	
{
    cout << "run data reciver thread!" << endl;

    MatchedLogRec log;

    while(true)
    {
        sleep(2);
        cout << "recive data from client" << endl;
        UserData::push_data(log);
    }

}

//完成线程的创建和启动工作
void DataReciveThread::start()
{
    cout << "start data recive thread!" << endl;
    pthread_create(&threadid, 0, reciveData, this);

}

//创建线程时调用的线程函数
void* DataReciveThread::reciveData(void *par)
{
    cout << "recive data thread  function!" << endl;
    DataReciveThread *th = (DataReciveThread*)par;

    th->run();

    return (void*)0;

}

