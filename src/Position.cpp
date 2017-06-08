//
// Created by chenghaowang on 22/05/17.
//

#include "../include/Position.h"
#include "../include/predefine.h"

Position::Position(const Position &position): dLatitude(position.getLatitude()), dLongitude(position.getLongitude()) {}

Position::Position(double dLatitude, double dLongitude) : dLatitude(dLatitude),dLongitude(dLongitude) {}

Position::~Position() {

}

double Position::getLatitude() const {
    return dLatitude;
}

double Position::getLongitude() const {
    return dLongitude;
}

bool Position::operator==(const Position &rhs) const {
    return dLatitude == rhs.getLatitude() &&
           dLongitude == rhs.getLongitude();
}

bool Position::operator!=(const Position &rhs) const {
    return !(rhs == *this);
}

Position *Position::clone() {
    return new Position(*this);
}

std::ostream &operator<<(std::ostream &os, const Position &position) {
    os << "dLatitude: " << position.dLatitude << " dLongitude: " << position.dLongitude;
    return os;
}

double distanceBetween(const Position &p1, const Position &p2) {
    double dLat1 = p1.getLatitude();
    double dLat2 = p2.getLatitude();
    double dLng1 = p1.getLongitude();
    double dLng2 = p2.getLongitude();

    double dDeltaLat = dLat1 - dLat2;
    double dDeltaLng = dLng1 - dLng2;
    double dDeltaLatRadian = dDeltaLat * PIx / 180;
    double dDeltaLngRadian = dDeltaLng * PIx / 180;
    double dLat1Radian = dLat1 * PIx / 180;
    double dLat2Radian = dLat2 * PIx / 180;
    double a = sin(dDeltaLatRadian / 2) * sin(dDeltaLatRadian / 2) +
               cos(dLat1Radian) * cos(dLat2Radian) * sin(dDeltaLngRadian / 2) * sin(dDeltaLngRadian / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double d = RADIO * c;
    return d;
}


