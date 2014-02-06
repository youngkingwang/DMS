#ifndef USERDATA_H
#define USERDATA_H

#include "../client/data.h"
#include <queue>
#include <pthread.h>

using namespace std;

//producer--consumer model
class UserData
{
public:
    UserData();
    ~UserData();

    void push_data(MatchedLogRec rec);
    MatchedLogRec pop_data();
private:
    deque<MatchedLogRec> data;	//data cache pool
    pthread_mutex_t mutex;	//控制线程安全的互斥量
    pthread_cond_t con_p;
    pthread_cond_t con_c;
};

#endif // USERDATA_H
