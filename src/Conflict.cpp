//
// Created by chenghaowang on 02/02/18.
//

#include "../include/Conflict.h"

Conflict::Conflict(double dDiffTime, double dMaxWaitTime, double dConflictProbability) : dDiffTime(dDiffTime),
                                                                                         dMaxWaitTime(dMaxWaitTime),
                                                                                         dConflictProbability(
                                                                                                 dConflictProbability) { bIsAverageCalculated = false; }

double Conflict::getDiffTime() const {
    return dDiffTime;
}

double Conflict::getAverageWaitTime() const {
    return dAverageWaitTime;
}

double Conflict::getMaxWaitTime() const {
    return dMaxWaitTime;
}

double Conflict::getConflictProbability() const {
    return dConflictProbability;
}

bool Conflict::isAverageCalculated() const {
    return bIsAverageCalculated;
}

void Conflict::setAverageWaitTime(double dAverageWaitTime) {
    Conflict::dAverageWaitTime = dAverageWaitTime;
    bIsAverageCalculated = true;
}
