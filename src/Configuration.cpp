//
// Created by chenghaowang on 02/06/17.
//

#include "../include/Configuration.h"

Configuration::Configuration(int iNbPeriods, int iPeriodUnit, int iNbFeasibleLevels)
        : iNbPeriods(iNbPeriods), iPeriodUnit(iPeriodUnit),iNbFeasibleLevels(iNbFeasibleLevels) {}

Configuration::Configuration(Time iPeriodUnit, int iNbFeasibleLevels)
        : iPeriodUnit(iPeriodUnit), iNbFeasibleLevels(iNbFeasibleLevels) {}

Configuration::~Configuration() {

}

int Configuration::getNbPeriods() const {
    return iNbPeriods;
}

Time Configuration::getPeriodUnit() const {
    return iPeriodUnit;
}

int Configuration::getNbFeasibleLevels() const {
    return iNbFeasibleLevels;
}
