//
// Created by chenghaowang on 02/06/17.
//

#ifndef ROBUSTFLA_CONFIGURATION_H
#define ROBUSTFLA_CONFIGURATION_H


#include "predefine.h"

class Configuration {
public:
    Configuration(int iNbPeriods, Time iPeriodUnit, int iSizeFeasibleLevel);

    Configuration(Time iPeriodUnit, int iSizeFeasibleLevel);

    virtual ~Configuration();

    int getNbPeriods() const;

    Time getPeriodUnit() const;

    int getNbFeasibleLevels() const;

private:
    int iNbPeriods;

    Time iPeriodUnit;

    int iNbFeasibleLevels;
};


#endif //ROBUSTFLA_CONFIGURATION_H
