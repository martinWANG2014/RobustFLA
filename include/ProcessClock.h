//
// Created by chenghaowang on 23/05/17.
//

#ifndef PROCESSCLOCK_H
#define PROCESSCLOCK_H
#ifdef WIN64
#include <windows.h>
struct ProcessTime {
    FILETIME tms_utime;
    FILETIME tms_stime;
    FILETIME tms_cutime;
    FILETIME tms_cstime;
};
#else

#include <sys/times.h>

typedef struct tms ProcessTime;
#endif
#ifndef WIN64
clock_t startClock;
clock_t endClock;
#endif

/**
 * the class to describe the process clock to calculate the elapsed time by the program.
 */
class ProcessClock {
public:
    /**
     * start process.
     */
    void start();

    /**
     * terminate process.
     */
    void end();

    /**
     * Getter for User time.
     * @return the corresponding user time.
     */
    double getUserTime() const {
        return dUserTime;
    }

    /**
     * Getter for System time.
     * @return the corresponding system time.
     */
    double getSysTime() const {
        return dSysTime;
    }

    /**
     * Getter for Create time.
     * @return the corresponding create time.
     */
    double getCreateTime() const {
        return dCreateTime;
    }

    /**
     * Getter for Exit time.
     * @return the corresponding exit time.
     */
    double getExitTime() const {
        return dExitTime;
    }

    /**
     * Getter for Cpu time.
     * @return the corresponding Cpu time.
     */
    double getCpuTime() const {
        return dCpuTime;
    }

private:
    /**
     * the start time of process.
     */
    ProcessTime oStartTime;

    /**
     * the end time of process.
     */
    ProcessTime oEndTime;

    /**
	 * the user time.
	 */
    double dUserTime;

    /**
	 * the system time.
	 */
    double dSysTime;

    /**
	 * the creation time.
	 */
    double dCreateTime;

    /**
	 * the exit time.
	 */
    double dExitTime;

    /**
	 * the cpu time.
	 */
    double dCpuTime;
};

#ifdef WIN32
union b64 {
    FILETIME time;
    __int64 i64;
};

/**
 * Start the process clock.
 */
void ProcessClock::start() {
    GetProcessTimes(GetCurrentProcess(), &oStartTime.tms_cutime, &oStartTime.tms_cstime, &oStartTime.tms_stime, &oStartTime.tms_utime);
}

/**
 * Terminate the process clock, and assign the correspding value.
 */
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

/**
 * Start the process clock.
 */
void ProcessClock::start() {
    startClock = times(&oStartTime);
}

/**
 * Terminate the process clock, and assign the correspding value.
 */
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
#endif //PROCESSCLOCK_H
