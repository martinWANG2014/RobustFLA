//
// Created by chenghaowang on 03/07/17.
//
#include "../include/Flight.h"

bool Flight::operator==(const Flight &rhs) const {
    return sCode == rhs.getCode();
}

bool Flight::operator!=(const Flight &rhs) const {
    return !(rhs == *this);
}

void Flight::GenerateNewFlight(Time iNewDepartureTime) {
    pRoute->GenerateNewRoute(iNewDepartureTime);
}

double
Flight::CalculateProbabilityConflictAndDelayForFlight(Flight *pFlight2, double *pdDiffTime, double *pdWaitingTimeMax,
                                                      bool deterministic) {
    Route *route2 = pFlight2->getRoute();
    for (int i = 0; i < pRoute->getPointListSize(); i++) {
        for (int j = 0; j < route2->getPointListSize(); j++) {
            if (*pRoute->getPointAtI(i) == *route2->getPointAtI(j)) {
                double prob = pRoute->CalculationProbabilityAndDelayAtPoint(i, route2, j, pdDiffTime, pdWaitingTimeMax,
                                                                            deterministic);
                if (prob > MIN_PROBA) {
                    return prob;
                }
            }
        }
    }
    *pdDiffTime = 0;
    *pdWaitingTimeMax = 0;
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

bool Flight::selfCheck() {
    return pRoute->selfCheck();
}

void Flight::setFeasibleLevelList(const LevelVector levelList) {
    Flight::viFeasibleLevelList = levelList;
}

Time Flight::getArrivingTime() const {
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
    auto position = std::find(pRoute->getVpPointsList().begin() + 1, pRoute->getVpPointsList().end(),
                              pRoute->getPointAtI(0));
    if (position == pRoute->getVpPointsList().end()) {
        pRoute->setNbPointsPerFlight(pRoute->getPointListSize());
    } else {
        pRoute->setNbPointsPerFlight((int) (position - pRoute->getVpPointsList().begin()));
    }
    Flight::iCurrentLevel = pRoute->getDefaultLevel();
}

Point *Flight::getPointAtI(int indexI) const {
    return pRoute->getPointAtI(indexI);
}

String Flight::getDestAirportName() const {
    return pADestination->getCode();
}

String Flight::getOrigAirportName() const {
    return pAOrigin->getCode();
}

int Flight::getNbPoints() const {
    return pRoute->getPointListSize();
}

void Flight::extendRoute(Time offset) {
    int nbPoints = pRoute->getPointListSize();
    for (int i = 0; i < nbPoints; i++) {
        pRoute->addNewPoint(pRoute->getPointAtI(i)->clone(pRoute->getPointAtI(i)->getArrivingTime() + offset));
    }
}

double Flight::getDuration() const {
    return getArrivingTime() - getDepartureTime();
}



