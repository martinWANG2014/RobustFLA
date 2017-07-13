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

Flight *Flight::GenerateNewFlight(Time iNewDepartureTime, bool bGeometricMethod) {
    Route *pNewRoute = (bGeometricMethod) ? pRoute->GenerateNewRoute(iNewDepartureTime) : pRoute->GenerateNewRoute2(
            iNewDepartureTime);
    Flight *pNewFlight = new Flight(sCode, pAOrigin->clone(), pADestination->clone(), iNewDepartureTime, pNewRoute,
                                    viFeasibleLevelList, dSigma);
    return pNewFlight;
}

double Flight::getProbabilityConflictAndDelay(Flight *pFlight2, double *pdDelay, bool *pbWait, bool bGeometricMethod) {
    Route *route2 = pFlight2->getRoute();
        for (int i = 1; i < pRoute->getPointListSize(); i++) {
            for (int j = 1; j < route2->getPointListSize(); j++) {
                if (*pRoute->getPointAtI(i) == *route2->getPointAtI(j)) {
                    double prob = pRoute->CalculationProbabilityAndDelay(i, route2, j, pdDelay, pbWait,
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