#include "userdata.h"
#include <iostream>

using namespace std;

//static variable and member function
deque<MatchedLogRec> UserData::data;
pthread_mutex_t UserData::mutex;
pthread_cond_t UserData::bufferNotEmpty;
pthread_cond_t UserData::bufferNotFull;

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
    data.push_back(rec);

}

//从缓冲池取出数据
MatchedLogRec UserData::pop_data()
{
    cout << "fetch data from data buffer pool !" << endl;

    MatchedLogRec temp;
    if (data.empty() == false)
    {
        temp = data.back();
        data.pop_back();
    }
    return temp;
}

bool UserData::isEmpty()
{
    return data.empty();
}
