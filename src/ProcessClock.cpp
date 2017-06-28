//
// Created by chenghaowang on 23/05/17.
//

#include "../include/ProcessClock.h"

#ifdef WIN32
union b64 {
    FILETIME time;
    __int64 i64;
};

void ProcessClock::start() {
    GetProcessTimes(GetCurrentProcess(), &oStartTime.tms_cutime, &oStartTime.tms_cstime, &oStartTime.tms_stime, &oStartTime.tms_utime);
}
void ProcessClock::end() {
    GetProcessTimes(GetCurrentProcess(), &oEndTime.tms_cutime, &oEndTime.tms_cstime, &oEndTime.tms_stime, &oEndTime.tms_utime);

    b64 start, end;
    start.time = oStartTime.tms_utime; end.time = oEndTime.tms_utime;
    dUserTime = (double)(end.i64 - start.i64)/10000000U;
    start.time = oStartTime.tms_stime; end.time = oEndTime.tms_stime;
    dSysTime = (double)(end.i64 - start.i64)/10000000U;
    start.time = oStartTime.tms_cutime; end.time = oEndTime.tms_cutime;
    dCreateTime = (double)(end.i64 - start.i64)/10000000U;
    start.time = oStartTime.tms_cstime; end.time = oEndTime.tms_cstime;
    dExitTime = (double)(end.i64 - start.i64)/10000000U;
    dCpuTime = dUserTime + dSysTime;
}
#else
#include <unistd.h>
void ProcessClock::start() {
    startClock = times(&oStartTime);
}

void ProcessClock::end() {
    endClock = times(&oEndTime);

    long tick_per_sec = sysconf(_SC_CLK_TCK);
    dUserTime = (double) (oEndTime.tms_utime - oStartTime.tms_utime) / tick_per_sec;
    dSysTime = (double) (oEndTime.tms_stime - oStartTime.tms_stime) / tick_per_sec;
    dCpuTime = (double) (endClock - startClock) / tick_per_sec;
    dCreateTime = (double) (oEndTime.tms_cutime - oStartTime.tms_cutime) / tick_per_sec;
    dExitTime = (double) (oEndTime.tms_cstime - oStartTime.tms_cstime) / tick_per_sec;
}
#endif

double ProcessClock::getUserTime() const {
    return dUserTime;
}

double ProcessClock::getSysTime() const {
    return dSysTime;
}

double ProcessClock::getCreateTime() const {
    return dCreateTime;
}

double ProcessClock::getExitTime() const {
    return dExitTime;
}

double ProcessClock::getCpuTime() const {
    return dCpuTime;
}