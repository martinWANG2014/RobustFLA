//
// Created by chenghaowang on 01/06/17.
//

#include "../include/NodePoint.h"

Point::Point(Node *pWayPoint, int iArrivingTime) : pWayPoint(pWayPoint), iArrivingTime(iArrivingTime) {}

WayPoint *Point::getWayPoint() const {
    return pWayPoint;
}

Time Point::getArrivingTime() const {
    return iArrivingTime;
}

const String &Point::getCode() const {
    return pWayPoint->getCode();
}

Position &Point::getPosition() const {
    return pWayPoint->getPosition();
}

//bool equal(const Point &lhs, const Point &rhs, Time iPeriodUnit) {
//    return  *lhs.getWayPoint() == *rhs.getWayPoint() && fabs(lhs.getArrivingTime() - rhs.getArrivingTime()) <= iPeriodUnit;
//}

bool Point::operator==(const Point &rhs) const {
    return *pWayPoint == *rhs.pWayPoint &&
           fabs(iArrivingTime - rhs.iArrivingTime) < *PERIODUNIT;
}

bool Point::operator!=(const Point &rhs) const {
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const Point &point) {
    os << "(pWayPoint: " << point.pWayPoint->getCode() << ", iArrivingTime: " << point.iArrivingTime << ")";
    return os;
}