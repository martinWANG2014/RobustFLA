//
// Created by chenghaowang on 01/06/17.
//

#include "../include/NodePoint.h"
bool Point::operator==(const Point &rhs) const {
    return *pWayPoint == *rhs.pWayPoint &&
           fabs(iArrivingTime - rhs.iArrivingTime) < *PERIODUNIT;
}

bool Point::operator!=(const Point &rhs) const {
    return !(rhs == *this);
}


