#ifndef DATARECIVETHREAD_H
#define DATARECIVETHREAD_H

#include <pthread.h>

//producer
class DataReciveThread
{
public:
    DataReciveThread();

    void run();
    void start();
    static void* reciveData(void *par);
private:
    pthread_t threadid;

};

#endif // DATARECIVETHREAD_H
