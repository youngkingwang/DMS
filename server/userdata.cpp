#include "userdata.h"
#include <iostream>

using namespace std;

//static variable and member function
deque<MatchedLogRec> data;
pthread_mutex_t mutex;
pthread_cond_t con_p;
pthread_mutex_t con_c;

UserData::UserData()
{
    cout << "initial data buffer !" << endl;


}

UserData::~UserData()
{

}

//向缓冲池中加入数据
void UserData::push_data(MatchedLogRec rec)
{
    cout << "push data to data buffer pool !" << endl;

}

//从缓冲池取出数据
MatchedLogRec UserData::pop_data()
{
    cout << "fetch data from data buffer pool !" << endl;

}
