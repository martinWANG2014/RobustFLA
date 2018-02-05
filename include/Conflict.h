//
// Created by chenghaowang on 02/02/18.
//

#ifndef ROBUSTFLA_CONFLICT_H
#define ROBUSTFLA_CONFLICT_H


#include <map>

class Conflict {
private:
    double dDiffTime;
    double dAverageWaitTime;
    double dMaxWaitTime;
    double dConflictProbability;
    bool bIsAverageCalculated;
public:
    Conflict(double dDiffTime, double dMaxWaitTime, double dConflictProbability);

    double getDiffTime() const;

    double getAverageWaitTime() const;

    double getMaxWaitTime() const;

    double getConflictProbability() const;

    bool isAverageCalculated() const;

    void setAverageWaitTime(double dAverageWaitTime);
};

typedef std::map<std::pair<std::string, std::string>, Conflict *> ConflictMap;
typedef std::map<std::string, std::pair<double, double>> MiPiMap;
#endif //ROBUSTFLA_CONFLICT_H
