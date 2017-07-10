//
// Created by chenghaowang on 01/06/17.
//

#include "../include/NodePoint.h"
#include "../include/Input.h"

bool Point::operator==(const Point &rhs) const {
    return *pWayPoint == *rhs.pWayPoint &&
           fabs(iArrivingTime - rhs.iArrivingTime) < Input::PERIOD_UNIT;
}

bool Point::operator!=(const Point &rhs) const {
    return !(rhs == *this);
}


