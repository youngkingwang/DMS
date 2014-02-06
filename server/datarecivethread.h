#ifndef DATARECIVETHREAD_H
#define DATARECIVETHREAD_H

class DataReciveThread
{
public:
    DataReciveThread();

    void run();
    void start();
    static void* reciveData(void *par);
private:
    int threadid;

};

#endif // DATARECIVETHREAD_H
