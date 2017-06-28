//
// Created by chenghaowang on 23/05/17.
//

#ifndef PROCESSCLOCK_H
#define PROCESSCLOCK_H

#include "common.h"

/**
 * the class to describe the process clock to calculate the elapsed time by the program.
 */
class ProcessClock {
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
    double getUserTime() const;

    /**
     * Getter for System time.
     * @return the corresponding system time.
     */
    double getSysTime() const;

    /**
     * Getter for Create time.
     * @return the corresponding create time.
     */
    double getCreateTime() const;

    /**
     * Getter for Exit time.
     * @return the corresponding exit time.
     */
    double getExitTime() const;

    /**
     * Getter for Cpu time.
     * @return the corresponding Cpu time.
     */
    double getCpuTime() const;
};


#endif //PROCESSCLOCK_H
