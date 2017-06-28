//
// Created by martinwang on 28/04/17.
//

#include "../include/Flight.h"

Flight::Flight(const Flight &flight) : sCode(flight.getCode()), pAOrigin(flight.getAirportOrigin()),
                                       pADestination(flight.getAirportDestination()),
                                       iDepartureTime(flight.getDepartureTime()),
                                       vpFeasibleLevelList(flight.getFeasibleLevelList()) {}


Flight::Flight(const String &sCode, Airport *pAOrigin, Airport *pADestination, Time iDepartureTime,
               FLevel *pDefaultLevel, Route *pDefaultRoute)
        : sCode(sCode), pAOrigin(pAOrigin), pADestination(pADestination), iDepartureTime(iDepartureTime),
          pRoute(pDefaultRoute) {
    addNewFLevel(pDefaultLevel);
    iCurrentLevelIndex = iDefaultLevelIndex = 0;
}

Flight::~Flight() {

}

bool Flight::operator==(const Flight &rhs) const {
    return sCode == rhs.getCode() &&
           iDepartureTime == rhs.getDepartureTime();
}

bool Flight::operator!=(const Flight &rhs) const {
    return !(rhs == *this);
}

Flight *Flight::clone() {
    return new Flight(*this);
}

const String &Flight::getCode() const {
    return sCode;
}

Airport *Flight::getAirportOrigin() const {
    return pAOrigin;
}

Airport *Flight::getAirportDestination() const {
    return pADestination;
}

Time Flight::getDepartureTime() const {
    return iDepartureTime;
}

const FLevelVector &Flight::getFeasibleLevelList() const {
    return vpFeasibleLevelList;
}

std::ostream &operator<<(std::ostream &os, const FLevelVector &fLevelVector) {
    for (auto &&item : fLevelVector) {
        os << *item << " ";
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const Flight &flight) {
    os << "sCode: " << flight.sCode << " pAOrigin: " << flight.pAOrigin->getCode() << " pADestination: "
       << flight.pADestination->getCode()
       << " iDepartureTime: " << flight.iDepartureTime << " iDefaultRoute: " << *flight.pRoute << std::endl
       << flight.vpFeasibleLevelList << std::endl << std::endl;
    return os;
}

void Flight::initFeasibleLevelList() {
    IntVector feasibleList = findFeasibleLevels(getDefaultLevel());
    for (auto &&item : feasibleList) {
        addNewFLevel(new FLevel(item));
    }
}

int Flight::getDefaultLevel() {
    return vpFeasibleLevelList[iDefaultLevelIndex]->getFlightLevel();
}

void Flight::addNewFLevel(FLevel *pFLevel) {
    if (pFLevel == nullptr) {
        std::cout << "at: void Flight::addNewFLevel(FLevel *pFLevel)" << std::endl
                  << "warning: FLevel is null, the insertion is ignored automatically." << std::endl;
    } else if (contains<FLevel>(vpFeasibleLevelList, pFLevel)) {
        std::cout << "at: void Flight::addNewFLevel(FLevel *pFLevel)" << std::endl
                  << "warning: FLevel has been in the list, the insertion is ignored automatically." << std::endl;
    } else {
        vpFeasibleLevelList.push_back(pFLevel);
    }
}

int Flight::getNbLevels() const {
    return vpFeasibleLevelList.size();
}

int Flight::getLevelAtI(int index) const {
    return vpFeasibleLevelList[index]->getFlightLevel();
}

int Flight::getCurrentLevel() const {
    return vpFeasibleLevelList[iCurrentLevelIndex]->getFlightLevel();
}

void Flight::setCurrentLevel(int iIndexLevel) {
    Flight::iCurrentLevelIndex = iIndexLevel;
}

double Flight::getCostOfCurrentLevel() {
    return vpFeasibleLevelList[iCurrentLevelIndex]->getPenalCost();
}

bool Flight::isCurrentPreferred() {
    return iCurrentLevelIndex == iDefaultLevelIndex;
}

void Flight::setAssigned(bool bAssigned) {
    Flight::hasAssigned = bAssigned;
}

bool Flight::isAssigned() {
    return hasAssigned;
}

double erfFunction(double x) {
    double a = 0;
    double b = fabs(x);

    double h = (b - a) / ITMAX;
    double s = exp(-pow(a, 2)) + exp(-pow(b, 2));

    for (int i = 1; i <ITMAX; i++) {
        s += 2 * exp(-pow(a + i * h, 2));
    }
    double t = s * h / 2;
    if (x > 0) {
        return t;
    } else {
        return -t;
    }
}

void calcluteConflictBetween(Flight &lhs, int iIndexI, Flight &rhs, int iIndexJ, bool *isIFirst, double *dOutProbability = 0,
                             double *dOutCost = 0) {
    Point *p1 = lhs.getRoutePointAtI(iIndexI);
    Point *p2 = rhs.getRoutePointAtI(iIndexJ);
    if (*p1 == *p2) {
        Position n1 = p1->getWayPoint()->getPosition();
        Time t1 = p1->getArrivingTime();
        Time t2 = p2->getArrivingTime();
        *isIFirst = (t1 <= t2);

        Position n3 = (iIndexI == 0) ? lhs.getAirportOrigin()->getPosition() : lhs.getRoutePointAtI(
                iIndexI - 1)->getWayPoint()->getPosition();
        Position n4 = (iIndexJ == 0) ? rhs.getAirportOrigin()->getPosition() : rhs.getRoutePointAtI(
                iIndexJ - 1)->getWayPoint()->getPosition();
        Time t3 = (iIndexI == 0) ? lhs.getDepartureTime() : lhs.getRoutePointAtI(iIndexI - 1)->getArrivingTime();
        Time t4 = (iIndexJ == 0) ? rhs.getDepartureTime() : rhs.getRoutePointAtI(iIndexJ - 1)->getArrivingTime();
        const double a0 = distanceBetween(n3, n4);
        const double b0 = distanceBetween(n3, n1);
        const double c0 = distanceBetween(n4, n1);
        const double dVelocity1 = fabs(b0 / (t3 - t1));
        const double dVelocity2 = fabs(c0 / (t4 - t2));
        const double dRho = dVelocity2 / dVelocity1;
        const double dCosA = (b0 * b0 + c0 * c0 - a0 * a0) / (2 * b0 * c0);
        const double dSinA = sqrt(1 - dCosA * dCosA);
        const double d1 = (t1 > t2) ? b0 : b0 - dVelocity1 * (t2 - t1);
        const double d2 = (t1 > t2) ? c0 - dVelocity2 * (t1 - t2) : c0;

        if ((pow(dRho, 2) - 2 * dRho * dCosA + 1) < EPSILON) {
            *dOutProbability = 1;
            *dOutCost = 1;
            return;
        }
        const double dLambda = dSinA / sqrt(pow(dRho, 2) - 2 * dRho * dCosA + 1);
        if (dSinA < EPSILON) {
            const double muy = (d2 - dRho * d1) / k;
            *dOutProbability = (fabs(muy) > min_separation) ? 0 : 1;
            *dOutCost = (t1 > t2) ? (min_separation - d1) / dVelocity1 : (min_separation - d2) / dVelocity2;
            return;
        }
        *dOutCost = (t1 > t2) ? ((min_separation * k / dLambda + c0 - dRho * d1) / dVelocity2 + t2 - t1) : (
                (min_separation * k / dLambda + dRho * b0 - c0) / dVelocity2 + t1 - t2);

        const double muy = dLambda * (d2 - dRho * d1) / k;// en nmi
        const double sigma = 0.5 * dLambda * sqrt(10 * (1 + pow(dRho, 2)));// 10???

        if (sigma < EPSILON) {
            *dOutProbability = (fabs(muy) > min_separation) ? 0 : 1;
            return;
        }
        const double f2 = (min_separation - muy) / (sqrt(2.0) * sigma);
        const double f1 = (-min_separation - muy) / (sqrt(2.0) * sigma);
        const double prob = (erfFunction(f2) - erfFunction(f1)) / sqrt(PI);
        *dOutProbability = (prob > 1) ? 1 : fabs(prob);
    }
}

Point *Flight::getRoutePointAtI(int iIndex) {
    return pRoute->getPointAtI(iIndex);
}




