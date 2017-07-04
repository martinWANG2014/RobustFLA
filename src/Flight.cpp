//
// Created by chenghaowang on 03/07/17.
//
#include "../include/Flight.h"

bool Flight::operator==(const Flight &rhs) const {
    return sCode == rhs.getCode() &&
           iDepartureTime == rhs.getDepartureTime();
}

bool Flight::operator!=(const Flight &rhs) const {
    return !(rhs == *this);
}
