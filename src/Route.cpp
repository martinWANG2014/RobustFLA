//
// Created by chenghaowang on 23/05/17.
//

#include "../include/Route.h"
double Route::probabilityConflict(int iIndex1, Route *pRoute2, int iIndex2) {
    if (iIndex1==0 || iIndex2==0){
        return 0;
    }
    Point *pPoint1 = getPointAtI(iIndex1);
    Point *pPoint2 = pRoute2->getPointAtI(iIndex2);
    Point *pPoint3 = getPointAtI(iIndex1-1);
    Point *pPoint4 = pRoute2->getPointAtI(iIndex2-1);
    if (*pPoint1==*pPoint2) {
        Position position = pPoint1->getPosition();
        Time t1 = pPoint1->getArrivingTime();
        Time t2 = pPoint2->getArrivingTime();
        Position position3 = pPoint3->getPosition();
        Position position4 = pPoint4->getPosition();
        Time t3 = pPoint3->getArrivingTime();
        Time t4 = pPoint4->getArrivingTime();
        // distance between two precedent nodes in routes r1,r2 of conflict note
        const double a0 = distanceBetween(position3, position4);
        // distance of precedent node in route r1 from the conflict node
        const double b0 = distanceBetween(position3, position);
        // distance of precedent node in route r2 from the conflict node
        const double c0 = distanceBetween(position4, position);
        // velocity of aircraft on route r1
        const double v1 = fabs(b0 / (t3 - t1));
        // velocity of aircraft on route r2
        const double v2 = fabs(c0 / (t4 - t2));
        const double p = v2 / v1;
        // distance at moment t of 2 aircraft from the conflict point
        double d1 = (t1 > t2) ? b0: b0 - v1 * (t2 - t1);
        double d2 = (t1 > t2) ?c0 - v2 * (t1 - t2):c0;
        // cos of angle between  two routes
        const double cosA = (pow(b0, 2) + pow(c0, 2) - pow(a0, 2)) / (2 * b0 * c0);
        // sin of angle between two routes
        const double sinA = sqrt(1 - cosA * cosA);
        const double p_2 = pow(p, 2);
        const double lambda = sinA / sqrt(p_2 - 2 * p * cosA + 1);
        const double muy = lambda * (d2 - p * d1) / K;
        // ????why the parameter is 0.5 * sqrt(10) for the growth of standard deviations of along track
        // distance error through time.
        const double sigma = 0.5 * lambda * sqrt(10 * (1 + p_2));
        const double f2 = (MIN_SEPARATION_DISTANCE - muy) / (sqrt(2.0) * sigma);
        const double f1 = (MIN_SEPARATION_DISTANCE + muy) / (sqrt(2.0) * sigma);
        return fabs(0.5 * (boost::math::erf(f1) + boost::math::erf(f2)));
    }
    return 0;
}

double Route::probabilityConflict2(int iIndex1, Route *pRoute2, int iIndex2) {
    if (iIndex1 == 0 || iIndex2 == 0) {
        return 0;
    }
    Point *pPoint1 = getPointAtI(iIndex1);
    Point *pPoint2 = pRoute2->getPointAtI(iIndex2);
    Point *pPoint3 = getPointAtI(iIndex1 - 1);
    Point *pPoint4 = pRoute2->getPointAtI(iIndex2 - 1);
    if (*pPoint1 == *pPoint2) {
        Position position = pPoint1->getPosition();
        Time t1 = pPoint1->getArrivingTime();
        Time t2 = pPoint2->getArrivingTime();
        Position position3 = pPoint3->getPosition();
        Position position4 = pPoint4->getPosition();
        Time t3 = pPoint3->getArrivingTime();
        Time t4 = pPoint4->getArrivingTime();
        // distance between two precedent nodes in routes r1,r2 of conflict note
        const double a0 = distanceBetween(position3, position4);
        // distance of precedent node in route r1 from the conflict node
        const double b0 = distanceBetween(position3, position);
        // distance of precedent node in route r2 from the conflict node
        const double c0 = distanceBetween(position4, position);
        // velocity of aircraft on route r1
        const double v1 = fabs(b0 / (t3 - t1));
        // velocity of aircraft on route r2
        const double v2 = fabs(c0 / (t4 - t2));
        const double p = v2 / v1;
        // cos of angle between  two routes
        const double cosA = (b0 * b0 + c0 * c0 - a0 * a0) / (2 * b0 * c0);
        // sin of angle between two routes
        const double sinA = sqrt(1 - cosA * cosA);
        const double p_2 = pow(p, 2);
        const double lambda = (p * sinA) / sqrt(p_2 - 2 * p * cosA + 1);
        const double m = (p * cosA - 1) / (p * sinA);
        const double mu1 = t3 + (BETA + ALPHA) / 2;
        const double mu2 = t4 + (BETA + ALPHA) / 2;
        const double sigmaMax = getSigma(ALPHA, BETA, CONFIDNECE);
        const double sigma1 = randomDevice() * 1.0 / randomDevice.max() * sigmaMax;
        const double sigma2 = randomDevice() * 1.0 / randomDevice.max() * sigmaMax;
        const double muy = lambda * ((m * (-c0 * sinA) - (b0 - c0 * cosA)) + v1 * (mu2 - mu1)) / K;
        const double sigma = lambda * v1 * (sqrt(pow(sigma1, 2) + pow(sigma2, 2)));
        const double f2 = (MIN_SEPARATION_DISTANCE - muy) / (sqrt(2.0) * sigma);
        const double f1 = (MIN_SEPARATION_DISTANCE + muy) / (sqrt(2.0) * sigma);
        return fabs(0.5 * (boost::math::erf(f1) + boost::math::erf(f2)));
    }
    return 0;
}

double Route::delayWithoutConflict(int iIndex1, Route *pRoute2, int iIndex2, int *ind) {
    if (iIndex1 == 0 || iIndex2 == 0) {
        return 0;
    }
    Point *pPoint1 = getPointAtI(iIndex1);
    Point *pPoint2 = pRoute2->getPointAtI(iIndex2);
    Point *pPoint3 = getPointAtI(iIndex1 - 1);
    Point *pPoint4 = pRoute2->getPointAtI(iIndex2 - 1);
    if (*pPoint1 == *pPoint2) {
        Position position = pPoint1->getPosition();
        Time t1 = pPoint1->getArrivingTime();
        Time t2 = pPoint2->getArrivingTime();
        Position position3 = pPoint3->getPosition();
        Position position4 = pPoint4->getPosition();
        Time t3 = pPoint3->getArrivingTime();
        Time t4 = pPoint4->getArrivingTime();
        // distance between two precedent nodes in routes r1,r2 of conflict note
        const double a0 = distanceBetween(position3, position4);
        // distance of precedent node in route r1 from the conflict node
        const double b0 = distanceBetween(position3, position);
        // distance of precedent node in route r2 from the conflict node
        const double c0 = distanceBetween(position4, position);
        // velocity of aircraft on route r1
        const double v1 = fabs(b0 / (t3 - t1));
        // velocity of aircraft on route r2
        const double v2 = fabs(c0 / (t4 - t2));
        const double p = v2 / v1;

        // cos of angle between  two routes
        const double cosA = (b0 * b0 + c0 * c0 - a0 * a0) / (2 * b0 * c0);
        // sin of angle between two routes
        const double sinA = sqrt(1 - cosA * cosA);
        const double p_2 = pow(p, 2);
        if (t1 > t2){
            *ind=1;
        }else{
            *ind=2;
        }
        if(sinA < 0){
            return fabs((b0-c0-MIN_SEPARATION_DISTANCE*K)/(v2-v1));
        }
        const double lambda = (p*sinA)/sqrt(p_2 - 2*p*cosA + 1);
        const double m = (p*cosA-1)/(p*sinA);
        const double deltaT=fabs((MIN_SEPARATION_DISTANCE*K/lambda-(m*(-c0*sinA)-(b0-c0*cosA)))/v1);
        if (deltaT < fabs(t3-t4+t2-t1)){
            return 0;
        }
        else{
            return deltaT - fabs(t3-t4+t2-t1);
        }
    }
    return 0;
}
double Route::delayWithoutConflict2(int iIndex1, Route *pRoute2, int iIndex2, int *ind) {
    if (iIndex1 == 0 || iIndex2 == 0) {
        return 0;
    }
    Point *pPoint1 = getPointAtI(iIndex1);
    Point *pPoint2 = pRoute2->getPointAtI(iIndex2);
    Point *pPoint3 = getPointAtI(iIndex1 - 1);
    Point *pPoint4 = pRoute2->getPointAtI(iIndex2 - 1);
    if (*pPoint1 == *pPoint2) {
        Position position = pPoint1->getPosition();
        Time t1 = pPoint1->getArrivingTime();
        Time t2 = pPoint2->getArrivingTime();
        Position position3 = pPoint3->getPosition();
        Position position4 = pPoint4->getPosition();
        Time t3 = pPoint3->getArrivingTime();
        Time t4 = pPoint4->getArrivingTime();
        // distance between two precedent nodes in routes r1,r2 of conflict note
        const double a0 = distanceBetween(position3, position4);
        // distance of precedent node in route r1 from the conflict node
        const double b0 = distanceBetween(position3, position);
        // distance of precedent node in route r2 from the conflict node
        const double c0 = distanceBetween(position4, position);
        // velocity of aircraft on route r1
        const double v1 = fabs(b0 / (t3 - t1));
        // velocity of aircraft on route r2
        const double v2 = fabs(c0 / (t4 - t2));
        const double p = v2 / v1;

        // cos of angle between  two routes
        const double cosA = (b0 * b0 + c0 * c0 - a0 * a0) / (2 * b0 * c0);
        // sin of angle between two routes
        const double sinA = sqrt(1 - cosA * cosA);
        const double p_2 = pow(p, 2);
        if (t1 > t2){
            *ind=1;
        }else{
            *ind=2;
        }
        if(sinA < 0){
            return fabs((b0-c0-MIN_SEPARATION_DISTANCE*K)/(v2-v1));
        }
        const double lambda = (p*sinA)/sqrt(p_2 - 2*p*cosA + 1);
        const double m = (p*cosA-1)/(p*sinA);
        const double deltaT=fabs((MIN_SEPARATION_DISTANCE*K/lambda-(m*(-c0*sinA)-(b0-c0*cosA)))/v1);
        if (deltaT < fabs(t3-t4+t2-t1)){
            return 0;
        }
        else{
            return deltaT - fabs(t3-t4+t2-t1);
        }
    }
    return 0;
}


