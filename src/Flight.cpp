//
// Created by chenghaowang on 03/07/17.
//
#include "../include/Flight.h"

bool Flight::operator==(const Flight &rhs) const {
    return sCode == rhs.getCode();
    //return sCode == rhs.getCode() && iDepartureTime == rhs.getDepartureTime();
}

bool Flight::operator!=(const Flight &rhs) const {
    return !(rhs == *this);
}

Flight *Flight::GenerateNewFlight(Time iDepartureTime, bool bGeometricMethod){
    if (bGeometricMethod) {
        Route *pNewRoute = pRoute->GenerateNewRoute(iDepartureTime);
        if (pNewRoute != nullptr) {
            Flight *pNewFlight = new Flight(sCode, pAOrigin->clone(), pADestination->clone(), iDepartureTime, pNewRoute);
            pNewFlight->setSpFeasibleLevelSet(vpFeasibleLevelsList);
            return pNewFlight;
        }
        return nullptr;
    } else{
        Route *pNewRoute = pRoute->GenerateNewRoute2(iDepartureTime);
        Flight *pNewFlight = new Flight(sCode, pAOrigin->clone(), pADestination->clone(), iDepartureTime, pNewRoute);
        pNewFlight->setSpFeasibleLevelSet(vpFeasibleLevelsList);
        return pNewFlight;
    }
}

double Flight::getProbabilityConflict(Flight *flight, double *pdDelay, bool *pbWhoWait, bool bGeometricMethod,
                                      int iModeRandom) {
    Route *route2 = flight->getRoute();
    if (bGeometricMethod) {
        for (int i = 1; i < pRoute->getPointListSize(); i++) {
            for (int j = 1; j < route2->getPointListSize(); j++) {
                if (*pRoute->getPointAtI(i) == *route2->getPointAtI(j)) {
//                    std:: cout << "\t\tConflict at: fi, " << getCode() << "; fj, " << flight->getCode() << std::endl;
                    double prob = pRoute->CPAD3(i, route2, j, pdDelay, pbWhoWait);
                    if (prob > MIN_PROBA) {
                        return prob;
                    }
                }
            }
        }
    } else {
        for (int i = 1; i < pRoute->getPointListSize(); i++) {
            for (int j = 1; j < route2->getPointListSize(); j++) {
                if (*pRoute->getPointAtI(i) == *route2->getPointAtI(j)) {
                    double sigma1, sigma2;
                    switch (iModeRandom) {
                        // Suppose the mean value is t+(BETA-ALPHA)/2, for the data generate from the interval [t-ALPHA, t+BETA].
                        case 0:
                            sigma1 = sigma2 = getSigma(ALPHA, BETA, GAMMA);
                            break;
                            // Suppose the mean value is t, for the data generate from the interval [t-ALPHA, t+BETA].
                        case 1:
                            sigma1 = sigma2 = getSigma(ALPHA, BETA, GAMMA);
                            break;
                            // Suppose the mean value is composed by two separate mean value, one from the part [t-ALPHA, t), and another from the other part [t, t+BETA].
                        case 2:
                            sigma1 = sigma2 = HYBRID_SIGMA;
                            break;
                        default:
                            sigma1 = sigma2 = getSigma(ALPHA, BETA, GAMMA);
                            break;
                    }
                    double prob = pRoute->CPAD4(i, route2, j, sigma1, sigma2, pdDelay, pbWhoWait);
                    if (prob > MIN_PROBA) {
                        return prob;
                    }
                }
            }
        }
    }
    *pbWhoWait = false;
    *pdDelay = 0;
    return 0;
}
