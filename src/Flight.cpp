//
// Created by chenghaowang on 03/07/17.
//
#include "../include/Flight.h"

bool Flight::operator==(const Flight &rhs) const {
    return sCode == rhs.getCode();
    //return sCode == rhs.getCode() && dDepartureTime == rhs.getDepartureTime();
}

bool Flight::operator!=(const Flight &rhs) const {
    return !(rhs == *this);
}

void Flight::GenerateNewFlight(Time iNewDepartureTime, bool bGeometricMethod) {
    (bGeometricMethod) ? pRoute->GenerateNewRoute(iNewDepartureTime) : pRoute->GenerateNewRoute2(
            iNewDepartureTime);
}

double Flight::CalculateProbabilityConflictAndDelayForFlight(Flight *pFlight2, double *pdDelay, bool *pbWait,
                                                             bool bGeometricMethod) {
    Route *route2 = pFlight2->getRoute();
        for (int i = 1; i < pRoute->getPointListSize(); i++) {
            for (int j = 1; j < route2->getPointListSize(); j++) {
                if (*pRoute->getPointAtI(i) == *route2->getPointAtI(j)) {
                    double prob = pRoute->CalculationProbabilityAndDelayAtPoint(i, route2, j, pdDelay, pbWait,
                                                                                bGeometricMethod,
                                                                                dSigma,
                                                                                pFlight2->getSigma());
                    if (prob > MIN_PROBA) {
                        return prob;
                    }
                }
            }
        }
    *pbWait = true;
    *pdDelay = 0;
    return 0;
}

void Flight::addNewFeasibleLevel(Level newLevel) {
    viFeasibleLevelList.push_back(newLevel);
}

Level Flight::getLastFeasibleLevel() {
    return viFeasibleLevelList[viFeasibleLevelList.size() - 1];
}

void Flight::resetLevel() {
    iCurrentLevel = getDefaultLevel();
}

void Flight::setCoefPi(double dCoefPi) {
    Flight::dCoefPi = dCoefPi;
}

double Flight::getCoefPi() const {
    return dCoefPi;
}

void Flight::resetRouteTimeList() {
    pRoute->resetTimeList();
}

void Flight::initRouteTimeList() {
    pRoute->initTimeList();
}

void Flight::setSigma(double dSigma) {
    Flight::dSigma = dSigma;
}

double Flight::getSigma() const {
    return dSigma;
}

bool Flight::selfCheck() {
    return pRoute->selfCheck();
}

void Flight::setFeasibleLevelList(const LevelVector levelList) {
    Flight::viFeasibleLevelList = levelList;
}

Time Flight::getArrivingTime() {
    return pRoute->getFinalArrivingTime();
}

void Flight::setCurrentLevel(Level iCurrentLevel) {
    Flight::iCurrentLevel = iCurrentLevel;
}

Level Flight::getCurrentLevel() const {
    return iCurrentLevel;
}

const LevelVector &Flight::getFeasibleLevelList() const {
    return viFeasibleLevelList;
}

Route *Flight::getRoute() const {
    return pRoute;
}

Level Flight::getDefaultLevel() const {
    return pRoute->getDefaultLevel();
}

Time Flight::getDepartureTime() const {
    return dDepartureTime;
}

const String &Flight::getCode() const {
    return sCode;
}

Flight::~Flight() {
    delete pAOrigin;
    delete pADestination;
    delete pRoute;
    viFeasibleLevelList.clear();
}

Flight::Flight(const String &sCode, Airport *pAOrigin, Airport *pADestination, Time iDepartureTime, Route *pRoute) {
    Flight::sCode = sCode;
    Flight::pAOrigin = pAOrigin;
    Flight::pADestination = pADestination;
    Flight::dDepartureTime = iDepartureTime;
    Flight::pRoute = pRoute;
    Flight::iCurrentLevel = pRoute->getDefaultLevel();
}

