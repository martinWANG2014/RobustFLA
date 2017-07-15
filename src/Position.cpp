//
// Created by chenghaowang on 22/05/17.
//

#include "../include/Position.h"
#include "../include/Configuration.h"

bool Position::operator==(const Position &rhs) const {
    return dLatitude == rhs.getLatitude() &&
           dLongitude == rhs.getLongitude();
}

bool Position::operator!=(const Position &rhs) const {
    return !(rhs == *this);
}

double distanceBetween(const Position &p1, const Position &p2) {
    double dLat1 = p1.getLatitude();
    double dLat2 = p2.getLatitude();
    double dLng1 = p1.getLongitude();
    double dLng2 = p2.getLongitude();

    double dDeltaLat = dLat1 - dLat2;
    double dDeltaLng = dLng1 - dLng2;
    double dDeltaLatRadian = dDeltaLat * M_PI / 180;
    double dDeltaLngRadian = dDeltaLng * M_PI / 180;
    double dLat1Radian = dLat1 * M_PI / 180;
    double dLat2Radian = dLat2 * M_PI / 180;
    double a = pow(sin(dDeltaLatRadian / 2), 2) +
               cos(dLat1Radian) * cos(dLat2Radian) * pow(sin(dDeltaLngRadian / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS * c;
}


