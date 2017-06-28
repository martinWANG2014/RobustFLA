//
// Created by chenghaowang on 31/05/17.
//


#include "../include/FLevel.h"

FLevel::FLevel(const FLevel &level): iFLightLevel(level.getFlightLevel()), dPenalCost(level.getPenalCost()) {}

FLevel::FLevel(int iFlightLevel): iFLightLevel(iFlightLevel), dPenalCost(0.0) {}

FLevel::FLevel(int iFlightLevel, double dPenalCost): iFLightLevel(iFlightLevel), dPenalCost(dPenalCost)  {}

FLevel::~FLevel() {}

int FLevel::getFlightLevel() const {
    return iFLightLevel;
}

double FLevel::getPenalCost() const {
    return dPenalCost;
}

void FLevel::setPenalCost(double dPenalCost) {
    FLevel::dPenalCost = dPenalCost;
}

bool FLevel::operator==(const FLevel &rhs) const {
    return iFLightLevel == rhs.iFLightLevel;
}

bool FLevel::operator!=(const FLevel &rhs) const {
    return !(rhs == *this);
}

FLevel *FLevel::clone() {
    return new FLevel(*this);
}

std::ostream &operator<<(std::ostream &os, const FLevel &level) {
    os << "(iFLightLevel: " << level.iFLightLevel << " dPenalCost: " << level.dPenalCost << ")";
    return os;
}
