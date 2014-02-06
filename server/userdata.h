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

    static void push_data(MatchedLogRec rec);
    static MatchedLogRec pop_data();
private:
    static deque<MatchedLogRec> data;	//data buffer pool
    static pthread_mutex_t mutex;	//控制线程安全的互斥量
    static pthread_cond_t con_p;
    static pthread_cond_t con_c;
};

#endif // USERDATA_H
