//生产者
#ifndef DATARECIVETHREAD_H
#define DATARECIVETHREAD_H

#include <pthread.h>

//producer
class DataReciveThread
{
public:
    DataReciveThread();

    void run();
    void start(int arg); //arg: client fd
    static void* reciveData(void *par);
private:
    pthread_t threadid;
    int connfd;

};

#endif // DATARECIVETHREAD_H
