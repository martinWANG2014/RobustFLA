//
// Created by chenghaowang on 23/05/17.
//

#include "../include/Route.h"
#include "../include/Input.h"


void Route::GenerateNewRoute(Time dNewDepartureTime) {
    vdTimeList[0] = dNewDepartureTime;
    Time dDeltaT = dNewDepartureTime - vpPointsList[0]->getArrivingTime();
    for (int i = 1; i < getPointListSize(); i++) {
        vdTimeList[i] = vpPointsList[i]->getArrivingTime() + dDeltaT;
    }
}


double Route::getVelocityFromPoint(int iIndexPoint) {
    if (iIndexPoint % getNbPointsPerFlight() == 0) {
        return -1;
    }
    double dDeltaT = vdTimeList[iIndexPoint] - vdTimeList[iIndexPoint - 1];
    double dDistance = distanceBetween(getPositionAtPoint(iIndexPoint), getPositionAtPoint(iIndexPoint - 1));
    if (dDeltaT != 0) {
        return dDistance / dDeltaT;
    }
    std::cerr << "[Fatal Error] the arriving time between two consecutive points is 0." << std::endl;
    return -1;
}

bool Route::selfCheck() {
    bool bValid = true;
    for (int i = 1; i < getPointListSize(); i++) {
        if (vpPointsList[i]->getArrivingTime() <= vpPointsList[i - 1]->getArrivingTime()) {
            bValid = false;
        }
    }
    return bValid;
}


double Route::CalculationProbabilityAndDelayAtPoint(int iIndex1, Route *pRoute2, int iIndex2, double *pdDiffTime,
                                                    double *pdWaitingTimeMax, bool deterministic) {
    vdList vdConflictProbabilityList;
    vdList vdDelayTime;
    double dT1 = getArrivingTimeAtPoint(iIndex1);
    double dT2 = pRoute2->getArrivingTimeAtPoint(iIndex2);
    double dV1 = getVelocityFromPoint(iIndex1 + (iIndex1 % getNbPointsPerFlight() == 0 ? 1 : 0));
    double dV2 = pRoute2->getVelocityFromPoint(iIndex2 + (iIndex2 % pRoute2->getNbPointsPerFlight() == 0 ? 1 : 0));
    double dCosA = getCosA(getPositionAtPoint(iIndex1),
                           getPositionAtPoint(iIndex1 + (iIndex1 % getNbPointsPerFlight() == 0 ? +1 : -1)),
                           pRoute2->getPositionAtPoint(
                                   iIndex2 + (iIndex2 % pRoute2->getNbPointsPerFlight() == 0 ? +1 : -1)));
    double dRho = dV2 / dV1;
    if (fabs(dCosA) > MIN_ANGLE) {
        dCosA = (dCosA > 0) ? MIN_ANGLE : -MIN_ANGLE;
    }
    double dSinA = sqrt(1 - pow(dCosA, 2));
    double dLambda = dSinA / sqrt(pow(dRho, 2) - 2 * dRho * dCosA + 1);
    double dProbabilityConflict;
    *pdDiffTime = dT1 - dT2;
    *pdWaitingTimeMax = MIN_SEPARATION_DISTANCE * K / (dLambda * dV2);
    if (deterministic) {
        dProbabilityConflict = fabs(dLambda * dV2 * (dT2 - dT1)) < MIN_SEPARATION_DISTANCE * K ? 1 : 0;
    } else {
        dProbabilityConflict = getConflictProbability(dLambda * dV2, dT2, dT1, -MIN_SEPARATION_DISTANCE * K,
                                                      MIN_SEPARATION_DISTANCE * K);
    }
    return dProbabilityConflict;
}

void Route::resetTimeList() {
    for (int i = 0; i < getPointListSize(); i++) {
        vdTimeList[i] = vpPointsList[i]->getArrivingTime();
    }
}

void Route::initTimeList() {
    vdTimeList.clear();
    for (int i = 0; i < getPointListSize(); i++) {
        vdTimeList.push_back(vpPointsList[i]->getArrivingTime());
    }
}

Time Route::getFinalArrivingTime() {
    return getArrivingTimeAtPoint((int) vpPointsList.size() - 1);
}

int Route::getPointListSize() {
    return (int) vpPointsList.size();
}

const Position &Route::getPositionAtPoint(int iIndex) {
    return vpPointsList[iIndex]->getPosition();
}

Time Route::getArrivingTimeAtPoint(int iIndex) {
    return vdTimeList[iIndex];
}

const Level Route::getDefaultLevel() const {
    return iDefaultLevel;
}

void Route::addNewPoint(Point *pPoint) {
    vpPointsList.push_back(pPoint);
}

Route::~Route() {
    for (auto &&point: vpPointsList) {
        delete point;
    }
    vpPointsList.clear();
}

Route::Route(Level iDefaultLevel, Point *pPoint) : iDefaultLevel(iDefaultLevel), vpPointsList(), vdTimeList() {
    vpPointsList.push_back(pPoint);
}

Route::Route(Level iDefaultLevel, Airport *pAirOrigin, Time iDepartureTime) : iDefaultLevel(iDefaultLevel),
                                                                              vpPointsList(), vdTimeList() {
    vpPointsList.push_back(new Point(pAirOrigin, iDepartureTime));
}

Point *Route::getPointAtI(int iIndex) {
    return vpPointsList[iIndex];
}

int Route::getNbPointsPerFlight() const {
    return nbPointsPerFlight;
}

void Route::setNbPointsPerFlight(int nbPointsPerFlight) {
    Route::nbPointsPerFlight = nbPointsPerFlight;
}

const PointVector &Route::getVpPointsList() const {
    return vpPointsList;
}


double Route::getConflictProbability(double coefficient, double t1, double t2, double dLB, double dUB) {
    return P_1 * getIntervalProbability(coefficient * (t1 - t2), pow(coefficient, 2) * SIGMA_2_1 * 2, dLB, dUB) +
           P_2 * getIntervalProbability(coefficient * (t1 - t2), pow(coefficient, 2) * SIGMA_2_2 * 2, dLB, dUB) +
           P_3 * getIntervalProbability(coefficient * (t1 - t2), pow(coefficient, 2) * SIGMA_2_3 * 2, dLB, dUB) +
           P_4 * getIntervalProbability(coefficient * (t1 - t2), pow(coefficient, 2) * SIGMA_2_4 * 2, dLB, dUB);
}

