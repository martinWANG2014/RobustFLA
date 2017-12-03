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
     * Getter for Cpu time.
     * @return the corresponding Cpu time.
     */
    double getCpuTime() const;

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

#ifndef WIN64
    clock_t startClock;
    clock_t endClock;
#endif
};




#endif //PROCESSCLOCK_H
