//
// Created by chenghaowang on 31/05/17.
//

#ifndef FLEVEL_H
#define FLEVEL_H

#include "common.h"

class FLevel {
public:
    /**
     * Copy Constructor
     * @param level see{@link FLevel}.
     */
    FLevel(const FLevel &level);

    /**
     * Constructor with parameters.
     * @param iFlightLevel the flight level.
     */
    FLevel(int iFlightLevel);

    /**
     * Constructor with parameters.
     * @param iFlightLevel the flight level.
     * @param dPenalCost the penal cost.
     */
    FLevel(int iFlightLevel, double dPenalCost);

    /**
     * Destructor.
     */
    virtual ~FLevel();

    /**
     * Getter for the flight level.
     * @return the corresponding flight level.
     */
    int getFlightLevel() const;

    /**
     * Getter for the penal cost of corresponding level.
     * @return the penal cost that the aircraft flies at the corresponding level.
     */
    double getPenalCost() const;

    /**
     * Setter for the penal cost of corresponding level.
     * @param dPenalCost the penal cost that the aircraft flies at the corresponding level.
     */
    void setPenalCost(double dPenalCost);

    /**
     * override of == operand.
     * @param rhs the other compared FLevel.
     * @return true, if two the flight levels are in the same level; false, otherwise.
     */
    bool operator==(const FLevel &rhs) const;

    /**
     * override of != operand.
     * @param rhs the other compared FLevel.
     * @return true, if the two flight levels are not in the same level; false, otherwise.
     */
    bool operator!=(const FLevel &rhs) const;

    /**
     * Clone the object itself.
     * @return the cloned object.
     */
    FLevel *clone();

    friend std::ostream &operator<<(std::ostream &os, const FLevel &level);

private:
    int iFLightLevel;
    double dPenalCost;
};
#endif //FLEVEL_H
