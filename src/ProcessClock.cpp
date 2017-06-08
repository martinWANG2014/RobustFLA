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
    GetProcessTimes(GetCurrentProcess(), &oPCStartTime.tms_cutime, &oPCStartTime.tms_cstime, &oPCStartTime.tms_stime, &oPCStartTime.tms_utime);
}
void ProcessClock::end() {
    GetProcessTimes(GetCurrentProcess(), &oPCEndTime.tms_cutime, &oPCEndTime.tms_cstime, &oPCEndTime.tms_stime, &oPCEndTime.tms_utime);

    b64 start, end;
    start.time = oPCStartTime.tms_utime; end.time = oPCEndTime.tms_utime;
    dPCUserTime = (double)(end.i64 - start.i64)/10000000U;
    start.time = oPCStartTime.tms_stime; end.time = oPCEndTime.tms_stime;
    dPCSysTime = (double)(end.i64 - start.i64)/10000000U;
    start.time = oPCStartTime.tms_cutime; end.time = oPCEndTime.tms_cutime;
    dPCCreateTime = (double)(end.i64 - start.i64)/10000000U;
    start.time = oPCStartTime.tms_cstime; end.time = oPCEndTime.tms_cstime;
    dPCExitTime = (double)(end.i64 - start.i64)/10000000U;
    dPCCpuTime = dPCUserTime + dPCSysTime;
}

#else

#include <unistd.h>

void ProcessClock::start() {
    startClock = times(&oPCStartTime);
}

void ProcessClock::end() {
    endClock = times(&oPCEndTime);

    long tick_per_sec = sysconf(_SC_CLK_TCK);
    dPCUserTime = (double) (oPCEndTime.tms_utime - oPCStartTime.tms_utime) / tick_per_sec;
    dPCSysTime = (double) (oPCEndTime.tms_stime - oPCStartTime.tms_stime) / tick_per_sec;
    dPCCpuTime = (double) (endClock - startClock) / tick_per_sec;
    dPCCreateTime = (double) (oPCEndTime.tms_cutime - oPCStartTime.tms_cutime) / tick_per_sec;
    dPCExitTime = (double) (oPCEndTime.tms_cstime - oPCStartTime.tms_cstime) / tick_per_sec;
}

#endif

double ProcessClock::getDPCUserTime() const {
    return dPCUserTime;
}

double ProcessClock::getDPCSysTime() const {
    return dPCSysTime;
}

double ProcessClock::getDPCCreateTime() const {
    return dPCCreateTime;
}

double ProcessClock::getDPCExitTime() const {
    return dPCExitTime;
}

double ProcessClock::getDPCCpuTime() const {
    return dPCCpuTime;
}