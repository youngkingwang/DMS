#include "datarecivethread.h"
#include "../client/data.h"
#include "userdata.h"
#include <iostream>
#include <pthread.h>
#include "logdao.h"
#include "unistd.h"

using namespace std;

DataReciveThread::DataReciveThread()
{
}

//完成具体的数据接收工作
void DataReciveThread::run()	
{
    cout << "run data reciver thread!" << endl;

    MatchedLogRec log;
    bool isempty = true;

    while(read(this->connfd, &log, sizeof(log)) > 0)
    {
        sleep(2);
        cout << "recive data from client" << endl;

        pthread_mutex_lock(&UserData::mutex);
        isempty = UserData::isEmpty();
        UserData::push_data(log);
        pthread_mutex_unlock(&UserData::mutex);

        pthread_mutex_lock(&UserData::mutex);
        if(isempty == true)
        {
            pthread_cond_signal(&UserData::bufferNotEmpty);
            isempty = false;
        }
        pthread_mutex_unlock(&UserData::mutex);
    }

}

//完成线程的创建和启动工作
void DataReciveThread::start(int arg)
{
    cout << "start data recive thread!" << endl;
    this->connfd = arg;
    pthread_create(&threadid, 0, reciveData, this);

}

//创建线程时调用的线程函数
void* DataReciveThread::reciveData(void *par)
{
    cout << "recive data thread  function!" << endl;
    DataReciveThread *th = (DataReciveThread*)par;

    pthread_detach(pthread_self());
    th->run();
    close(th->connfd);

    return nullptr;

}

