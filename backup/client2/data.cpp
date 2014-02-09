#include "data.h"

using namespace std;

bool operator== (const LogRec& arg1, const LogRec& arg2 ) {
    if (strcmp(arg1.logname, arg2.logname) == 0  &&
                    strcmp(arg1.logip, arg2.logip) == 0 &&
                    (arg1.pid == arg2.pid))
    {
        if(arg1.logtime > arg2.logtime) {
            return true;
        }
    }
    else
    {
        return false;
    }

}

ostream& operator<< (ostream& os, const MatchedLogRec& matchLog)
{
    os << matchLog.logname << " "
            << matchLog.pid << " "
            << matchLog.logip << " "
            << matchLog.loginTime << " "
            << matchLog.logoutTime << " "
            << matchLog.durations  << " "
            << matchLog.labip;
    return os;

}
