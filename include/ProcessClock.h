//
// Created by chenghaowang on 23/05/17.
//

#ifndef ROBUSTFLA_PROCESSCLOCK_H
#define ROBUSTFLA_PROCESSCLOCK_H


#include "predefine.h"
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
private:
    /**
     * the start time of process.
     */
    ProcessTime oPCStartTime;

    /**
     * the end time of process.
     */
    ProcessTime oPCEndTime;

    /**
	 * the user time.
	 */
    double dPCUserTime;

    /**
	 * the system time.
	 */
    double dPCSysTime;

    /**
	 * the creation time.
	 */
    double dPCCreateTime;

    /**
	 * the exit time.
	 */
    double dPCExitTime;

    /**
	 * the cpu time.
	 */
    double dPCCpuTime;
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
    double getDPCUserTime() const;

    /**
     * Getter for System time.
     * @return the corresponding system time.
     */
    double getDPCSysTime() const;

    /**
     * Getter for Create time.
     * @return the corresponding create time.
     */
    double getDPCCreateTime() const;

    /**
     * Getter for Exit time.
     * @return the corresponding exit time.
     */
    double getDPCExitTime() const;

    /**
     * Getter for Cpu time.
     * @return the corresponding Cpu time.
     */
    double getDPCCpuTime() const;
};


#endif //ROBUSTFLA_PROCESSCLOCK_H
