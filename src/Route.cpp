//
// Created by chenghaowang on 23/05/17.
//

#include "../include/Route.h"
#include "../include/Input.h"
double Route::CPAD(int iIndex1, Route *pRoute2, int iIndex2, double *delay, bool *ind) {
    double prob;
    if (*getPointAtI(iIndex1) == *pRoute2->getPointAtI(iIndex2)) {
        if (iIndex1 == 0 && iIndex2 == 0) {
            Position position = getPositionAtPoint(iIndex1);
            Time t1 = getArrivingTimeAtPoint(iIndex1);
            Time t2 = pRoute2->getArrivingTimeAtPoint(iIndex2);
            double v2 = pRoute2->getVelocityFromPoint(iIndex2);
            double d_min = v2*(t1-t2);
            prob = fabs(d_min) > MIN_SEPARATION_DISTANCE*K ? 0:1;
            *delay = std::max(0.0, MIN_SEPARATION_DISTANCE*K/v2 - fabs(t1-t2));
        }
        else if (iIndex1 == 0) {
            if (iIndex2 == pRoute2->getPointListSize() - 1) {

            } else {
                Position position = getPositionAtPoint(0);
                Time t1 = getArrivingTimeAtPoint(0);
                double v1 = getVelocityFromPoint(1);
                Time t2 = pRoute2->getArrivingTimeAtPoint(iIndex2);
                Position position3 = getPositionAtPoint(1);
                Position position4 = pRoute2->getPositionAtPoint(iIndex2 + 1);
                double a0 = distanceBetween(position3, position4);
                double b0 = distanceBetween(position3, position);
                double c0 = distanceBetween(position4, position);
                double v2 = pRoute2->getVelocityFromPoint(iIndex2 + 1);
                double p = v2 / v1;
                double cosA = (pow(b0, 2) + pow(c0, 2) - pow(a0, 2)) / (2 * b0 * c0);
                double sinA = sqrt(1 - pow(cosA, 2));
                if (sinA < MIN_ANGLE) {
                    sinA = MIN_ANGLE;
                    cosA = sqrt(1 - pow(sinA, 2));
                }
                double lambda = sinA / sqrt(pow(p, 2) - 2 * p * cosA + 1);
                if (t1 > t2) {
                    double d0 = c0 - p * b0 + v2 * (t2 - t1);
                    double muy = lambda * d0 / K;
                    double sigma = lambda * A_BAR * sqrt(1 + pow(p, 2));
                    double f2 = (MIN_SEPARATION_DISTANCE - muy) / (sqrt(2.0) * sigma);
                    double f1 = (MIN_SEPARATION_DISTANCE + muy) / (sqrt(2.0) * sigma);
                    prob = fabs(0.5 * (boost::math::erf(f1) + boost::math::erf(f2)));
                    *delay = std::max(0.0, -(MIN_SEPARATION_DISTANCE * K / lambda + p * b0 - c0) / v2 - (t1 - t2));
                    *ind = true;
                } else {
                    if (v2*(t2-t1) < MIN_SEPARATION_DISTANCE * K)
                        prob = 1;
                    else{
                        double d0 = c0 - p * b0 + v2 * (t2 - t1);
                        double muy = lambda * d0 / K;
                        double sigma = lambda * A_BAR * sqrt(1 + pow(p, 2));
                        double f2 = (MIN_SEPARATION_DISTANCE - muy) / (sqrt(2.0) * sigma);
                        double f1 = (MIN_SEPARATION_DISTANCE + muy) / (sqrt(2.0) * sigma);
                        prob = fabs(0.5 * (boost::math::erf(f1) + boost::math::erf(f2)));
                    }
                    *delay = std::max(0.0, std::max(MIN_SEPARATION_DISTANCE * K / v2 - (t2 - t1),
                                                    (MIN_SEPARATION_DISTANCE * K / lambda + p * b0 - c0) / v2 -
                                                    (t2 - t1)));
                    *ind = false;
                }
            }
        }
        else if (iIndex2 == 0) {
            Position position = pRoute2->getPositionAtPoint(0);
            Time t2 = pRoute2->getArrivingTimeAtPoint(0);
            double v2 = pRoute2->getVelocityFromPoint(1);
            Time t1 = getArrivingTimeAtPoint(iIndex1);

            if (iIndex1 == getPointListSize() - 1) {
                double v1 = getVelocityFromPoint(iIndex1);
                if (t1 > t2) {
                    *delay = std::max(0.0, MIN_SEPARATION_DISTANCE * K / std::min(v2, v1) - (t1 - t2));
                    *ind = true;
                } else {
                    *delay = std::max(0.0, MIN_SEPARATION_DISTANCE * K / v2 - (t2 - t1));
                    *ind = false;
                }
                prob = *delay > 0 ? 1:0;
            } else {
                Position position3 = getPositionAtPoint(iIndex1 + 1);
                Position position4 = pRoute2->getPositionAtPoint(1);
                double a0 = distanceBetween(position3, position4);
                double b0 = distanceBetween(position3, position);
                double c0 = distanceBetween(position4, position);
                double v1 = getVelocityFromPoint(iIndex1 + 1);
                double p = v2 / v1;
                double cosA = (pow(b0, 2) + pow(c0, 2) - pow(a0, 2)) / (2 * b0 * c0);
                double sinA = sqrt(1 - pow(cosA, 2));
                if (sinA < MIN_ANGLE) {
                    sinA = MIN_ANGLE;
                    cosA = sqrt(1 - pow(sinA, 2));
                }
                double lambda = sinA / sqrt(pow(p, 2) - 2 * p * cosA + 1);
                if (t1 > t2) {
                    if (v1*(t2-t1) < MIN_SEPARATION_DISTANCE * K)
                        prob = 1;
                    else{
                        double d0 = c0 - p * b0 + v2 * (t2 - t1);
                        double muy = lambda * d0 / K;
                        double sigma = lambda * A_BAR * sqrt(1 + pow(p, 2));
                        double f2 = (MIN_SEPARATION_DISTANCE - muy) / (sqrt(2.0) * sigma);
                        double f1 = (MIN_SEPARATION_DISTANCE + muy) / (sqrt(2.0) * sigma);
                        prob = fabs(0.5 * (boost::math::erf(f1) + boost::math::erf(f2)));
                    }
                    *delay = std::max(0.0, std::max(MIN_SEPARATION_DISTANCE * K / v1 - (t1 - t2),
                                                    -(MIN_SEPARATION_DISTANCE * K / lambda + p * b0 - c0) / v2 -
                                                    (t1 - t2)));
                    *ind = true;
                } else {
                    double d0 = c0 - p * b0 + v2 * (t2 - t1);
                    double muy = lambda * d0 / K;
                    double sigma = lambda * A_BAR * sqrt(1 + pow(p, 2));
                    double f2 = (MIN_SEPARATION_DISTANCE - muy) / (sqrt(2.0) * sigma);
                    double f1 = (MIN_SEPARATION_DISTANCE + muy) / (sqrt(2.0) * sigma);
                    prob = fabs(0.5 * (boost::math::erf(f1) + boost::math::erf(f2)));
                    *delay = std::max(0.0, (MIN_SEPARATION_DISTANCE * K / lambda + p * b0 - c0) / v2 - (t2 - t1));
                    *ind = false;
                }
            }
        } else{
            Position position = getPositionAtPoint(iIndex1);
            Time t1 = getArrivingTimeAtPoint(iIndex1);
            Time t2 = pRoute2->getArrivingTimeAtPoint(iIndex2);
            Position position3 = getPositionAtPoint(iIndex1 - 1);
            Position position4 = pRoute2->getPositionAtPoint(iIndex2 - 1);
            double v1 = getVelocityFromPoint(iIndex1);
            double v2 = pRoute2->getVelocityFromPoint(iIndex2);
            if (*getPointAtI(iIndex1-1) == *pRoute2->getPointAtI(iIndex2-1)){
                if(t1>t2){
                    *ind = true;
                    *delay = std::max(0.0, MIN_SEPARATION_DISTANCE*K/v2 - (t1-t2));
                }else{
                    *ind = false;
                    *delay = std::max(0.0, MIN_SEPARATION_DISTANCE*K/v1 - (t2-t1));
                }
                prob = *delay > 0 ? 1:0;
            }else {
                double a0 = distanceBetween(position3, position4);
                double b0 = distanceBetween(position3, position);
                double c0 = distanceBetween(position4, position);
                double p = v2 / v1;
                double cosA = (pow(b0, 2) + pow(c0, 2) - pow(a0, 2)) / (2 * b0 * c0);
                double sinA = sqrt(1 - pow(cosA, 2));
                double p_2 = pow(p, 2);
                if (sinA < MIN_ANGLE) {
                    sinA = MIN_ANGLE;
                    cosA = sqrt(1 - pow(sinA, 2));
                }
                double lambda = sinA / sqrt(p_2 - 2 * p * cosA + 1);
                double d0 = c0 - p * b0 + v2 * (t2 - t1);
                double muy = lambda * d0 / K;
                double sigma = lambda * A_BAR * sqrt(1 + p_2);
                double f2 = (MIN_SEPARATION_DISTANCE - muy) / (sqrt(2.0) * sigma);
                double f1 = (MIN_SEPARATION_DISTANCE + muy) / (sqrt(2.0) * sigma);
                if (t1 > t2) {
                    *delay = std::max(0.0, -(MIN_SEPARATION_DISTANCE * K / lambda + p * b0 - c0) / v2 - (t1 - t2));
                    *ind = true;
                } else {
                    *delay = std::max(0.0, (MIN_SEPARATION_DISTANCE * K / lambda + p * b0 - c0) / v2 - (t2 - t1));
                    *ind = false;
                }
                prob = fabs(0.5 * (boost::math::erf(f1) + boost::math::erf(f2)));
            }
        }
    }
    else{
        *delay = 0;
        prob = 0;
    }

    return prob;
}

double Route::CPAD2(int iIndex1, Route *pRoute2, int iIndex2, double mu1, double sigma1, double mu2, double sigma2,
                    double *delay, bool *ind) {
    double prob;
    if (*getPointAtI(iIndex1) == *pRoute2->getPointAtI(iIndex2)) {
        if (iIndex1 == 0 && iIndex2 == 0) {
            Position position = getPositionAtPoint(0);
            Time t1 = getArrivingTimeAtPoint(0);
            Time t2 = pRoute2->getArrivingTimeAtPoint(0);
            Position position3 = getPositionAtPoint(1);
            Position position4 = pRoute2->getPositionAtPoint(1);
            double a0 = distanceBetween(position3, position4);
            double b0 = distanceBetween(position3, position);
            double c0 = distanceBetween(position4, position);
            double v1 = getVelocityFromPoint(1);
            double v2 = pRoute2->getVelocityFromPoint(1);
            double p = v2 / v1;
            double cosA = (pow(b0, 2) + pow(c0, 2) - pow(a0, 2)) / (2 * b0 * c0);
            double sinA = sqrt(1 - pow(cosA, 2));
            double p_2 = pow(p, 2);
            if (sinA < MIN_ANGLE) {
                sinA = MIN_ANGLE;
                cosA = sqrt(1 - pow(sinA, 2));
            }
            double lambda = sinA / sqrt(p_2 - 2 * p * cosA + 1);
            double muy = lambda * (c0 - p * b0 + v2 * (mu2 - mu1)) / K;
            double sigma = lambda * v2* sqrt(pow(sigma1, 2) + pow(sigma2, 2));
            double f2 = (MIN_SEPARATION_DISTANCE - muy) / (sqrt(2.0) * sigma);
            double f1 = (MIN_SEPARATION_DISTANCE + muy) / (sqrt(2.0) * sigma);
            if (t1 > t2) {
                *delay = std::max(0.0, -(MIN_SEPARATION_DISTANCE * K / lambda + p * b0 - c0) / v2 - (t1 - t2));
                *ind = true;
            } else {
                *delay = std::max(0.0, (MIN_SEPARATION_DISTANCE * K / lambda + p * b0 - c0) / v2 - (t2 - t1));
                *ind = false;
            }
            double prob = fabs(0.5 * (boost::math::erf(f1) + boost::math::erf(f2)));
            return prob;
//            if (t1 > t2) {
//                *delay = std::max(0.0, (MIN_SEPARATION_DISTANCE * K / lambda + p * b0 - c0) / v2 - (t1 - t2));
//                *ind = true;
//            } else {
//                *delay = std::max(0.0, (MIN_SEPARATION_DISTANCE * K / lambda + p * b0 - c0) / v2 - (t2 - t1));
//                *ind = false;
//            }
//            return *delay > 0 ? 1:0;
        }
        else if (iIndex1 == 0) {
            Position position = getPositionAtPoint(0);
            Time t1 = getArrivingTimeAtPoint(0);
            double v1 = getVelocityFromPoint(1);
            Time t2 = pRoute2->getArrivingTimeAtPoint(iIndex2);
            if (iIndex2 == pRoute2->getPointListSize() - 1) {
                double v2 = pRoute2->getVelocityFromPoint(iIndex2);
                if (t1 > t2) {
                    *delay = std::max(0.0, MIN_SEPARATION_DISTANCE * K / v1 - (t1 - t2));
                    *ind = true;
                } else {
                    *delay = std::max(0.0, MIN_SEPARATION_DISTANCE * K / std::min(v2, v1) - (t2 - t1));
                    *ind = false;
                }
            } else {
                Position position3 = getPositionAtPoint(1);
                Position position4 = pRoute2->getPositionAtPoint(iIndex2 + 1);
                double a0 = distanceBetween(position3, position4);
                double b0 = distanceBetween(position3, position);
                double c0 = distanceBetween(position4, position);
                double v2 = pRoute2->getVelocityFromPoint(iIndex2 + 1);
                double p = v2 / v1;
                double cosA = (pow(b0, 2) + pow(c0, 2) - pow(a0, 2)) / (2 * b0 * c0);
                double sinA = sqrt(1 - pow(cosA, 2));
                if (sinA < MIN_ANGLE) {
                    sinA = MIN_ANGLE;
                    cosA = sqrt(1 - pow(sinA, 2));
                }
                double lambda = sinA / sqrt(pow(p, 2) - 2 * p * cosA + 1);
                if (t1 > t2) {
                    *delay = std::max(0.0, (MIN_SEPARATION_DISTANCE * K / lambda + p * b0 - c0) / v2 - (t1 - t2));
                    *ind = true;
                } else {
                    *delay = std::max(0.0, std::max(MIN_SEPARATION_DISTANCE * K / v2 - (t2 - t1),
                                                    (MIN_SEPARATION_DISTANCE * K / lambda + p * b0 - c0) / v2 -
                                                    (t1 - t2)));
                    *ind = false;
                }
            }
            return *delay > 0 ? 1:0;
        }
        else if (iIndex2 == 0) {
            Position position = pRoute2->getPositionAtPoint(0);
            Time t2 = pRoute2->getArrivingTimeAtPoint(0);
            double v2 = pRoute2->getVelocityFromPoint(1);
            Time t1 = getArrivingTimeAtPoint(iIndex1);

            if (iIndex1 == getPointListSize() - 1) {
                double v1 = getVelocityFromPoint(iIndex1);
                if (t1 > t2) {
                    *delay = std::max(0.0, MIN_SEPARATION_DISTANCE * K / std::min(v2, v1) - (t1 - t2));
                    *ind = true;
                } else {
                    *delay = std::max(0.0, MIN_SEPARATION_DISTANCE * K / v2 - (t2 - t1));
                    *ind = false;
                }
            } else {
                Position position3 = getPositionAtPoint(iIndex1 + 1);
                Position position4 = pRoute2->getPositionAtPoint(1);
                double a0 = distanceBetween(position3, position4);
                double b0 = distanceBetween(position3, position);
                double c0 = distanceBetween(position4, position);
                double v1 = getVelocityFromPoint(iIndex1 + 1);
                double p = v2 / v1;
                double cosA = (pow(b0, 2) + pow(c0, 2) - pow(a0, 2)) / (2 * b0 * c0);
                double sinA = sqrt(1 - pow(cosA, 2));
                if (sinA < MIN_ANGLE) {
                    sinA = MIN_ANGLE;
                    cosA = sqrt(1 - pow(sinA, 2));
                }
                double lambda = sinA / sqrt(pow(p, 2) - 2 * p * cosA + 1);
                if (t1 > t2) {
                    *delay = std::max(0.0, std::max(MIN_SEPARATION_DISTANCE * K / v1 - (t1 - t2),
                                                    (MIN_SEPARATION_DISTANCE * K / lambda + p * b0 - c0) / v2 -
                                                    (t1 - t2)));
                    *ind = true;
                } else {
                    *delay = std::max(0.0, (MIN_SEPARATION_DISTANCE * K / lambda + p * b0 - c0) / v2 - (t1 - t2));
                    *ind = false;
                }
            }
            return *delay > 0 ? 1:0;
        } else{
            Position position = getPositionAtPoint(iIndex1);
            Time t1 = getArrivingTimeAtPoint(iIndex1-1);
            Time t2 = pRoute2->getArrivingTimeAtPoint(iIndex2-1);
            Position position3 = getPositionAtPoint(iIndex1 - 1);
            Position position4 = pRoute2->getPositionAtPoint(iIndex2 - 1);
            double v1 = getVelocityFromPoint(iIndex1);
            double v2 = pRoute2->getVelocityFromPoint(iIndex2);
            double muy, sigma;
            if (*getPointAtI(iIndex1-1) == *pRoute2->getPointAtI(iIndex2-1)){
                if(t1>t2){
                    muy = v2 * (mu1 - mu2) / K;
                    sigma = v2 * sqrt(pow(sigma1, 2) + pow(sigma2, 2));
                    *ind = true;
                    *delay = std::max(0.0, MIN_SEPARATION_DISTANCE*K/v2 - (t1-t2));
                }else{
                    muy = v1 * (mu2 - mu1) / K;
                    sigma = v1 * sqrt(pow(sigma1, 2) + pow(sigma2, 2));
                    *ind = false;
                    *delay = std::max(0.0, MIN_SEPARATION_DISTANCE*K/v1 - (t2-t1));
                }
            }else {
                double a0 = distanceBetween(position3, position4);
                double b0 = distanceBetween(position3, position);
                double c0 = distanceBetween(position4, position);
                double p = v2 / v1;
                double cosA = (pow(b0, 2) + pow(c0, 2) - pow(a0, 2)) / (2 * b0 * c0);
                double sinA = sqrt(1 - pow(cosA, 2));
                if (sinA < MIN_ANGLE) {
                    sinA = MIN_ANGLE;
                    cosA = sqrt(1 - pow(sinA, 2));
                }
                double lambda = sinA / sqrt(pow(p, 2) - 2 * p * cosA + 1);
                muy = lambda * (c0 - p * b0 + v2 * (mu2 - mu1)) / K;
                sigma = lambda * v2 * sqrt(pow(sigma1, 2) + pow(sigma2, 2));
                if (t1 > t2) {
                    *delay = std::max(0.0, -(MIN_SEPARATION_DISTANCE * K / lambda + p * b0 - c0) / v2 - (t1 - t2));
                    *ind = true;
                } else {
                    *delay = std::max(0.0, (MIN_SEPARATION_DISTANCE * K / lambda + p * b0 - c0) / v2 - (t2 - t1));
                    *ind = false;
                }
            }
            double f2 = (MIN_SEPARATION_DISTANCE - muy) / (sqrt(2.0) * sigma);
            double f1 = (MIN_SEPARATION_DISTANCE + muy) / (sqrt(2.0) * sigma);
            prob = fabs(0.5 * (boost::math::erf(f1) + boost::math::erf(f2)));
        }
    }else {
        *delay = 0;
        prob = 0;
    }
    return prob;
}

Route *Route::GenerateNewRoute(Time iDepartureTime) {
    std::default_random_engine generator;
    Route *newRoute = new Route(getDefaultLevel(), getPointAtI(0)->clone(iDepartureTime));
    for (int i = 1; i < getPointListSize(); i++) {
        double velocity = getVelocityFromPoint(i - 1);
        normal_dist NormalDistribution(velocity, velocity / 300);
        velocity = NormalDistribution(generator);
        double dDistance = distanceBetween(getPointAtI(i)->getPosition(), getPointAtI(i - 1)->getPosition());
        if (velocity > 0) {
            Time dDeltaT = (Time) (dDistance / velocity);
            newRoute->addNewPoint(getPointAtI(i)->clone(newRoute->getArrivingTimeAtPoint(i - 1) + dDeltaT));
        } else {
            delete newRoute;
            return nullptr;
        }
    }
    return newRoute;
}

Route *Route::GenerateNewRoute2(Time iDepartureTime) {
    std::default_random_engine generator;
    Route *newRoute = new Route(getDefaultLevel(), getPointAtI(0)->clone(iDepartureTime));
    Time dDeltaT = iDepartureTime - getArrivingTimeAtPoint(0);
    for (int i = 1; i < getPointListSize(); i++) {
        newRoute->addNewPoint(getPointAtI(i)->clone(newRoute->getArrivingTimeAtPoint(i - 1) + dDeltaT));
    }
    return newRoute;
}

double Route::getVelocityFromPoint(int iIndexPoint) {
    //Calculate the velocity, and it varies between -1% and 1%.
    if (iIndexPoint == 0) {
        return -1;
    }
    double dDeltaT = getArrivingTimeAtPoint(iIndexPoint) - getArrivingTimeAtPoint(iIndexPoint - 1);
    double dDistance = distanceBetween(getPositionAtPoint(iIndexPoint), getPositionAtPoint(iIndexPoint-1));
    if (dDeltaT != 0) {
        return dDistance / dDeltaT;
    }
    std::cerr << "[Fatal Error] the arriving time between two consecutive points is 0." << std::endl;
    return -1;
}

bool Route::selfCheck() {
    bool bValid = true;
    for(int i =1; i < getPointListSize(); i++){
        if (getArrivingTimeAtPoint(i) <= getArrivingTimeAtPoint(i-1))
            bValid = false;
    }
    return bValid;
}

double Route::CPAD4_11(int iIndex1, Route *pRoute2, int iIndex2, double sigma_1, double sigma_2, double *delay){
    Position position = getPositionAtPoint(iIndex1);
    Position position3 = getPositionAtPoint(iIndex1-1);
    Position position4 = pRoute2->getPositionAtPoint(iIndex2-1);
    Time time1 = getArrivingTimeAtPoint(iIndex1);
    Time time2 = pRoute2->getArrivingTimeAtPoint(iIndex2);
    Time time3 = getArrivingTimeAtPoint(iIndex1-1);
    Time time4 = pRoute2->getArrivingTimeAtPoint(iIndex2-1);
    double a0 = distanceBetween(position3, position4);
    double b0 = distanceBetween(position3, position);
    double c0 = distanceBetween(position4, position);
    double v1 = b0/(time1-time3);
    double v2 = c0/(time2-time4);
    double p = v2 / v1;
    double cosA = (pow(b0, 2) + pow(c0, 2) - pow(a0, 2)) / (2 * b0 * c0);
    double sinA = sqrt(1 - pow(cosA, 2));
    if (sinA < MIN_ANGLE) {
        sinA = MIN_ANGLE;
        if (cosA < 0){
            cosA = -sqrt(1 - pow(sinA, 2));
        }else{
            cosA = sqrt(1 - pow(sinA, 2));
        }
    }
    double lambda = sinA / sqrt(pow(p, 2) - 2 * p * cosA + 1);
    double muy = lambda*v2*(time2-time1)/K;
    double sigma = lambda*v2*sqrt(pow(sigma_1,2) + pow(sigma_2,2));
    double f1 = (MIN_SEPARATION_DISTANCE - muy) / (sqrt(2.0) * sigma);
    double f2 = (MIN_SEPARATION_DISTANCE + muy) / (sqrt(2.0) * sigma);
    double prob = fabs(0.5 * (boost::math::erf(f1) + boost::math::erf(f2)));
    *delay = std::max(MIN_SEPARATION_DISTANCE*K/(lambda*v2) - fabs(time2-time1), 0.0);
    return prob;
}
double Route::CPAD4_10(int iIndex1, Route *pRoute2, int iIndex2, double sigma_1, double sigma_2, double *delay){
    Position position = getPositionAtPoint(iIndex1);
    Position position3 = getPositionAtPoint(iIndex1-1);
    Position position4 = pRoute2->getPositionAtPoint(iIndex2+1);
    Time time1 = getArrivingTimeAtPoint(iIndex1);
    Time time2 = pRoute2->getArrivingTimeAtPoint(iIndex2);
    Time time3 = getArrivingTimeAtPoint(iIndex1-1);
    Time time4 = pRoute2->getArrivingTimeAtPoint(iIndex2+1);
    double a0 = distanceBetween(position3, position4);
    double b0 = distanceBetween(position3, position);
    double c0 = distanceBetween(position4, position);
    double v1 = b0/(time1-time3);
    double v2 = c0/(time4-time2);
    double p = v2 / v1;
    double cosA = (pow(b0, 2) + pow(c0, 2) - pow(a0, 2)) / (2 * b0 * c0);
    double sinA = sqrt(1 - pow(cosA, 2));
    if (sinA < MIN_ANGLE) {
        sinA = MIN_ANGLE;
        if (cosA < 0){
            cosA = -sqrt(1 - pow(sinA, 2));
        }else{
            cosA = sqrt(1 - pow(sinA, 2));
        }
    }
    double lambda = sinA / sqrt(pow(p, 2) + 2 * p * cosA + 1);
    double muy = lambda*v2*(time2-time1)/K;
    double sigma = lambda*v2*sqrt(pow(sigma_1,2) + pow(sigma_2,2));
    double f1 = (MIN_SEPARATION_DISTANCE - muy) / (sqrt(2.0) * sigma);
    double f2 = (MIN_SEPARATION_DISTANCE + muy) / (sqrt(2.0) * sigma);
    double prob = fabs(0.5 * (boost::math::erf(f1) + boost::math::erf(f2)));
    *delay = std::max(MIN_SEPARATION_DISTANCE*K/(lambda*v2) - fabs(time2-time1), 0.0);
    return prob;
}
double Route::CPAD4_01(int iIndex1, Route *pRoute2, int iIndex2, double sigma_1, double sigma_2,double *delay){
    Position position = getPositionAtPoint(iIndex1);
    Position position3 = getPositionAtPoint(iIndex1+1);
    Position position4 = pRoute2->getPositionAtPoint(iIndex2-1);
    Time time1 = getArrivingTimeAtPoint(iIndex1);
    Time time2 = pRoute2->getArrivingTimeAtPoint(iIndex2);
    Time time3 = getArrivingTimeAtPoint(iIndex1+1);
    Time time4 = pRoute2->getArrivingTimeAtPoint(iIndex2-1);
    double a0 = distanceBetween(position3, position4);
    double b0 = distanceBetween(position3, position);
    double c0 = distanceBetween(position4, position);
    double v1 = b0/(time3-time1);
    double v2 = c0/(time2-time4);
    double p = v2 / v1;
    double cosA = (pow(b0, 2) + pow(c0, 2) - pow(a0, 2)) / (2 * b0 * c0);
    double sinA = sqrt(1 - pow(cosA, 2));
    if (sinA < MIN_ANGLE) {
        sinA = MIN_ANGLE;
        if (cosA < 0){
            cosA = -sqrt(1 - pow(sinA, 2));
        }else{
            cosA = sqrt(1 - pow(sinA, 2));
        }
    }
    double lambda = sinA / sqrt(pow(p, 2) + 2 * p * cosA + 1);
    double muy = lambda*v2*(time2-time1)/K;
    double sigma = lambda*v2*sqrt(pow(sigma_1,2) + pow(sigma_2,2));
    double f1 = (MIN_SEPARATION_DISTANCE - muy) / (sqrt(2.0) * sigma);
    double f2 = (MIN_SEPARATION_DISTANCE + muy) / (sqrt(2.0) * sigma);
    double prob = fabs(0.5 * (boost::math::erf(f1) + boost::math::erf(f2)));
    *delay = std::max(MIN_SEPARATION_DISTANCE*K/(lambda*v2) - fabs(time2-time1), 0.0);
    return prob;
}
double Route::CPAD4_00(int iIndex1, Route *pRoute2, int iIndex2, double sigma_1, double sigma_2, double *delay){
    Position position = getPositionAtPoint(iIndex1);
    Position position3 = getPositionAtPoint(iIndex1+1);
    Position position4 = pRoute2->getPositionAtPoint(iIndex2+1);
    Time time1 = getArrivingTimeAtPoint(iIndex1);
    Time time2 = pRoute2->getArrivingTimeAtPoint(iIndex2);
    Time time3 = getArrivingTimeAtPoint(iIndex1+1);
    Time time4 = pRoute2->getArrivingTimeAtPoint(iIndex2+1);
    double a0 = distanceBetween(position3, position4);
    double b0 = distanceBetween(position3, position);
    double c0 = distanceBetween(position4, position);
    double v1 = b0/(time3-time1);
    double v2 = c0/(time4-time2);
    double p = v2 / v1;
    double cosA = (pow(b0, 2) + pow(c0, 2) - pow(a0, 2)) / (2 * b0 * c0);
    double sinA = sqrt(1 - pow(cosA, 2));
    if (sinA < MIN_ANGLE) {
        sinA = MIN_ANGLE;
        if (cosA < 0){
            cosA = -sqrt(1 - pow(sinA, 2));
        }else{
            cosA = sqrt(1 - pow(sinA, 2));
        }
    }
    double lambda = sinA / sqrt(pow(p, 2) - 2 * p * cosA + 1);
    double muy = lambda*v2*(time2-time1)/K;
    double sigma = lambda*v2*sqrt(pow(sigma_1,2) + pow(sigma_2,2));
    double f1 = (MIN_SEPARATION_DISTANCE - muy) / (sqrt(2.0) * sigma);
    double f2 = (MIN_SEPARATION_DISTANCE + muy) / (sqrt(2.0) * sigma);
    double prob = fabs(0.5 * (boost::math::erf(f1) + boost::math::erf(f2)));
    *delay = std::max(MIN_SEPARATION_DISTANCE*K/(lambda*v2) - fabs(time2-time1), 0.0);
    return prob;

}
double Route::CPAD4(int iIndex1, Route *pRoute2, int iIndex2, double sigma1, double sigma2,
                    double *delay, bool *ind) {
    if (*getPointAtI(iIndex1) != *pRoute2->getPointAtI(iIndex2)) {
        *delay =0;
        *ind = true;
        return 0;
    }
    DoubleVector prob;
    DoubleVector tau;
    double *pdDelay = new double(0.0);
    double dProbability;
    if (iIndex1 > 0 && iIndex1 < getPointListSize()-1){
        if (iIndex2 > 0 && iIndex2 < pRoute2->getPointListSize()-1){
            dProbability = CPAD4_00(iIndex1, pRoute2, iIndex2, sigma1, sigma2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
            dProbability = CPAD4_01(iIndex1, pRoute2, iIndex2, sigma1, sigma2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
            dProbability = CPAD4_10(iIndex1, pRoute2, iIndex2, sigma1, sigma2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
            dProbability = CPAD4_11(iIndex1, pRoute2, iIndex2, sigma1, sigma2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
        } else if( iIndex2 == 0){
            dProbability = CPAD4_00(iIndex1, pRoute2, iIndex2, sigma1, sigma2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
            prob.push_back(dProbability);
            dProbability = CPAD4_10(iIndex1, pRoute2, iIndex2, sigma1, sigma2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
        }else{
            dProbability = CPAD4_01(iIndex1, pRoute2, iIndex2, sigma1, sigma2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
            dProbability = CPAD4_11(iIndex1, pRoute2, iIndex2, sigma1, sigma2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
        }
    } else if (iIndex1 == 0){
        if (iIndex2 > 0 && iIndex2 < pRoute2->getPointListSize()-1){
            dProbability = CPAD4_00(iIndex1, pRoute2, iIndex2, sigma1, sigma2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
            dProbability = CPAD4_01(iIndex1, pRoute2, iIndex2, sigma1, sigma2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
        } else if( iIndex2 == 0){
            dProbability = CPAD4_00(iIndex1, pRoute2, iIndex2, sigma1, sigma2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
        }else{
            dProbability = CPAD4_01(iIndex1, pRoute2, iIndex2, sigma1, sigma2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
        }
    }else{
        if (iIndex2 > 0 && iIndex2 < pRoute2->getPointListSize()-1){
            dProbability = CPAD4_10(iIndex1, pRoute2, iIndex2, sigma1, sigma2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
            dProbability = CPAD4_11(iIndex1, pRoute2, iIndex2, sigma1, sigma2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
        } else if( iIndex2 == 0){
            dProbability = CPAD4_10(iIndex1, pRoute2, iIndex2, sigma1, sigma2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
        }else{
            dProbability = CPAD4_11(iIndex1, pRoute2, iIndex2, sigma1, sigma2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
        }
    }
    *delay = *std::max(tau.begin(), tau.end());
    *ind = getArrivingTimeAtPoint(iIndex1) > pRoute2->getArrivingTimeAtPoint(iIndex2);
    return *std::max(prob.begin(), prob.end());

}

double Route::CPAD3_11(int iIndex1, Route *pRoute2, int iIndex2, double *delay){
    Position position = getPositionAtPoint(iIndex1);
    Position position3 = getPositionAtPoint(iIndex1-1);
    Position position4 = pRoute2->getPositionAtPoint(iIndex2-1);
    Time time1 = getArrivingTimeAtPoint(iIndex1);
    Time time2 = pRoute2->getArrivingTimeAtPoint(iIndex2);
    Time time3 = getArrivingTimeAtPoint(iIndex1-1);
    Time time4 = pRoute2->getArrivingTimeAtPoint(iIndex2-1);
    double a0 = distanceBetween(position3, position4);
    double b0 = distanceBetween(position3, position);
    double c0 = distanceBetween(position4, position);
    double v1 = b0/(time1-time3);
    double v2 = c0/(time2-time4);
    double p = v2 / v1;
    double cosA = (pow(b0, 2) + pow(c0, 2) - pow(a0, 2)) / (2 * b0 * c0);
    double sinA = sqrt(1 - pow(cosA, 2));
    if (sinA < MIN_ANGLE) {
        sinA = MIN_ANGLE;
        if (cosA < 0){
            cosA = -sqrt(1 - pow(sinA, 2));
        }else{
            cosA = sqrt(1 - pow(sinA, 2));
        }
    }
    double lambda = sinA / sqrt(pow(p, 2) - 2 * p * cosA + 1);
    double muy = lambda*v2*(time2-time1)/K;
    double sigma = lambda * A_BAR * sqrt(1 + pow(p, 2));
    double f1 = (MIN_SEPARATION_DISTANCE - muy) / (sqrt(2.0) * sigma);
    double f2 = (MIN_SEPARATION_DISTANCE + muy) / (sqrt(2.0) * sigma);
    double prob = fabs(0.5 * (boost::math::erf(f1) + boost::math::erf(f2)));
    *delay = std::max(MIN_SEPARATION_DISTANCE*K/(lambda*v2) - fabs(time2-time1), 0.0);
    return prob;
}
double Route::CPAD3_10(int iIndex1, Route *pRoute2, int iIndex2, double *delay){
    Position position = getPositionAtPoint(iIndex1);
    Position position3 = getPositionAtPoint(iIndex1-1);
    Position position4 = pRoute2->getPositionAtPoint(iIndex2+1);
    Time time1 = getArrivingTimeAtPoint(iIndex1);
    Time time2 = pRoute2->getArrivingTimeAtPoint(iIndex2);
    Time time3 = getArrivingTimeAtPoint(iIndex1-1);
    Time time4 = pRoute2->getArrivingTimeAtPoint(iIndex2+1);
    double a0 = distanceBetween(position3, position4);
    double b0 = distanceBetween(position3, position);
    double c0 = distanceBetween(position4, position);
    double v1 = b0/(time1-time3);
    double v2 = c0/(time4-time2);
    double p = v2 / v1;
    double cosA = (pow(b0, 2) + pow(c0, 2) - pow(a0, 2)) / (2 * b0 * c0);
    double sinA = sqrt(1 - pow(cosA, 2));
    if (sinA < MIN_ANGLE) {
        sinA = MIN_ANGLE;
        if (cosA < 0){
            cosA = -sqrt(1 - pow(sinA, 2));
        }else{
            cosA = sqrt(1 - pow(sinA, 2));
        }
    }
    double lambda = sinA / sqrt(pow(p, 2) + 2 * p * cosA + 1);
    double muy = lambda*v2*(time2-time1)/K;
    double sigma = lambda * A_BAR * sqrt(1 + pow(p, 2));
    double f1 = (MIN_SEPARATION_DISTANCE - muy) / (sqrt(2.0) * sigma);
    double f2 = (MIN_SEPARATION_DISTANCE + muy) / (sqrt(2.0) * sigma);
    double prob = fabs(0.5 * (boost::math::erf(f1) + boost::math::erf(f2)));
    *delay = std::max(MIN_SEPARATION_DISTANCE*K/(lambda*v2) - fabs(time2-time1), 0.0);
    return prob;
}
double Route::CPAD3_01(int iIndex1, Route *pRoute2, int iIndex2, double *delay){
    Position position = getPositionAtPoint(iIndex1);
    Position position3 = getPositionAtPoint(iIndex1+1);
    Position position4 = pRoute2->getPositionAtPoint(iIndex2-1);
    Time time1 = getArrivingTimeAtPoint(iIndex1);
    Time time2 = pRoute2->getArrivingTimeAtPoint(iIndex2);
    Time time3 = getArrivingTimeAtPoint(iIndex1+1);
    Time time4 = pRoute2->getArrivingTimeAtPoint(iIndex2-1);
    double a0 = distanceBetween(position3, position4);
    double b0 = distanceBetween(position3, position);
    double c0 = distanceBetween(position4, position);
    double v1 = b0/(time3-time1);
    double v2 = c0/(time2-time4);
    double p = v2 / v1;
    double cosA = (pow(b0, 2) + pow(c0, 2) - pow(a0, 2)) / (2 * b0 * c0);
    double sinA = sqrt(1 - pow(cosA, 2));
    if (sinA < MIN_ANGLE) {
        sinA = MIN_ANGLE;
        if (cosA < 0){
            cosA = -sqrt(1 - pow(sinA, 2));
        }else{
            cosA = sqrt(1 - pow(sinA, 2));
        }
    }
    double lambda = sinA / sqrt(pow(p, 2) + 2 * p * cosA + 1);
    double muy = lambda*v2*(time2-time1)/K;
    double sigma = lambda * A_BAR * sqrt(1 + pow(p, 2));
    double f1 = (MIN_SEPARATION_DISTANCE - muy) / (sqrt(2.0) * sigma);
    double f2 = (MIN_SEPARATION_DISTANCE + muy) / (sqrt(2.0) * sigma);
    double prob = fabs(0.5 * (boost::math::erf(f1) + boost::math::erf(f2)));
    *delay = std::max(MIN_SEPARATION_DISTANCE*K/(lambda*v2) - fabs(time2-time1), 0.0);
    return prob;
}
double Route::CPAD3_00(int iIndex1, Route *pRoute2, int iIndex2, double *delay){
    Position position = getPositionAtPoint(iIndex1);
    Position position3 = getPositionAtPoint(iIndex1+1);
    Position position4 = pRoute2->getPositionAtPoint(iIndex2+1);
    Time time1 = getArrivingTimeAtPoint(iIndex1);
    Time time2 = pRoute2->getArrivingTimeAtPoint(iIndex2);
    Time time3 = getArrivingTimeAtPoint(iIndex1+1);
    Time time4 = pRoute2->getArrivingTimeAtPoint(iIndex2+1);
    double a0 = distanceBetween(position3, position4);
    double b0 = distanceBetween(position3, position);
    double c0 = distanceBetween(position4, position);
    double v1 = b0/(time3-time1);
    double v2 = c0/(time4-time2);
    double p = v2 / v1;
    double cosA = (pow(b0, 2) + pow(c0, 2) - pow(a0, 2)) / (2 * b0 * c0);
    double sinA = sqrt(1 - pow(cosA, 2));
    if (sinA < MIN_ANGLE) {
        sinA = MIN_ANGLE;
        if (cosA < 0){
            cosA = -sqrt(1 - pow(sinA, 2));
        }else{
            cosA = sqrt(1 - pow(sinA, 2));
        }
    }
    double lambda = sinA / sqrt(pow(p, 2) - 2 * p * cosA + 1);
    double muy = lambda*v2*(time2-time1)/K;
    double sigma = lambda * A_BAR * sqrt(1 + pow(p, 2));
    double f1 = (MIN_SEPARATION_DISTANCE - muy) / (sqrt(2.0) * sigma);
    double f2 = (MIN_SEPARATION_DISTANCE + muy) / (sqrt(2.0) * sigma);
    double prob = fabs(0.5 * (boost::math::erf(f1) + boost::math::erf(f2)));
    *delay = std::max(MIN_SEPARATION_DISTANCE*K/(lambda*v2) - fabs(time2-time1), 0.0);
    return prob;

}
double Route::CPAD3(int iIndex1, Route *pRoute2, int iIndex2, double *delay, bool *ind) {
    if (*getPointAtI(iIndex1) != *pRoute2->getPointAtI(iIndex2)) {
        *delay =0;
        *ind = true;
        return 0;
    }
    DoubleVector prob;
    DoubleVector tau;
    double *pdDelay = new double(0.0);
    double dProbability;
    if (iIndex1 > 0 && iIndex1 < getPointListSize()-1){
        if (iIndex2 > 0 && iIndex2 < pRoute2->getPointListSize()-1){
            dProbability = CPAD3_00(iIndex1, pRoute2, iIndex2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
            dProbability = CPAD3_01(iIndex1, pRoute2, iIndex2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
            dProbability = CPAD3_10(iIndex1, pRoute2, iIndex2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
            dProbability = CPAD3_11(iIndex1, pRoute2, iIndex2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
        } else if( iIndex2 == 0){
            dProbability = CPAD3_00(iIndex1, pRoute2, iIndex2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
            prob.push_back(dProbability);
            dProbability = CPAD3_10(iIndex1, pRoute2, iIndex2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
        }else{
            dProbability = CPAD3_01(iIndex1, pRoute2, iIndex2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
            dProbability = CPAD3_11(iIndex1, pRoute2, iIndex2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
        }
    } else if (iIndex1 == 0){
        if (iIndex2 > 0 && iIndex2 < pRoute2->getPointListSize()-1){
            dProbability = CPAD3_00(iIndex1, pRoute2, iIndex2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
            dProbability = CPAD3_01(iIndex1, pRoute2, iIndex2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
        } else if( iIndex2 == 0){
            dProbability = CPAD3_00(iIndex1, pRoute2, iIndex2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
        }else{
            dProbability = CPAD3_01(iIndex1, pRoute2, iIndex2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
        }
    }else{
        if (iIndex2 > 0 && iIndex2 < pRoute2->getPointListSize()-1){
            dProbability = CPAD3_10(iIndex1, pRoute2, iIndex2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
            dProbability = CPAD3_11(iIndex1, pRoute2, iIndex2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
        } else if( iIndex2 == 0){
            dProbability = CPAD3_10(iIndex1, pRoute2, iIndex2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
        }else{
            dProbability = CPAD3_11(iIndex1, pRoute2, iIndex2, pdDelay);
            tau.push_back(*pdDelay);
            prob.push_back(dProbability);
        }
    }
    *delay = *std::max(tau.begin(), tau.end());
    *ind = getArrivingTimeAtPoint(iIndex1) > pRoute2->getArrivingTimeAtPoint(iIndex2);
    return *std::max(prob.begin(), prob.end());

}
