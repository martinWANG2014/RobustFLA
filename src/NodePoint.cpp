//
// Created by chenghaowang on 01/06/17.
//

#include "../include/NodePoint.h"
#include "../include/Input.h"

bool Point::operator==(const Point &rhs) const {
    return (*pWayPoint == *rhs.pWayPoint) &&
           fabs(iArrivingTime - rhs.iArrivingTime) < Network::PERIOD_LENGTH;
}

bool Point::operator!=(const Point &rhs) const {
    return !(rhs == *this);
}

Point::~Point() {
    delete pWayPoint;
}

const Position &Point::getPosition() const {
    return pWayPoint->getPosition();
}

Time Point::getArrivingTime() const {
    return iArrivingTime;
}

Point::Point(WayPoint *pWayPoint, Time iArrivingTime) : pWayPoint(pWayPoint), iArrivingTime(iArrivingTime) {}

String Point::getPointName() const {
    return pWayPoint->getCode();
}

Point *Point::clone(Time arrivingTime) {
    return new Point(pWayPoint->clone(), arrivingTime);
}

std::ostream &operator<<(std::ostream &os, const Point &point) {
    os << "pWayPoint: " << *point.pWayPoint << " iArrivingTime: " << point.iArrivingTime;
    return os;
}



