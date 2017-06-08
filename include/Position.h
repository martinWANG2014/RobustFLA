//
// Created by chenghaowang on 22/05/17.
//

#ifndef ROBUSTFLA_POSITION_H
#define ROBUSTFLA_POSITION_H

#include <ostream>

/**
 * A Position is denoted by a latitude and a longitude.
 */
class Position {
public:
    /**
     * Copy Constructor.
     * @param position see {@link Position}.
     */
    Position(const Position &position);

    /**
     * Constructor with parameters.
     * @param dLatitude the latitude of a position.
     * @param dLongitude the longitude of a position.
     */
    Position(double dLatitude, double dLongitude);

    /**
     * Destructor
     */
    virtual ~Position();

    /**
     * Getter for the latitude.
     * @return the latitude of a corresponding position.
     */
    double getLatitude() const;

    /**
     * Getter for the longitude.
     * @return the longitude of a corresponding position.
     */
    double getLongitude() const;

    /**
     * override of == operand.
     * @param rhs the other compared Position.
     * @return true, if they are the same position; false, otherwise.
     */
    bool operator==(const Position &rhs) const;

    /**
     * override of != operand.
     * @param rhs the other compared Position.
     * @return true, if they are different position; false, otherwise.
     */
    bool operator!=(const Position &rhs) const;

    /**
     * clone the object itself.
     * @return the cloned object.
     */
    Position *clone();

    friend std::ostream &operator<<(std::ostream &os, const Position &position);
    friend double distanceBetween(const Position &p1, const Position &p2);

private:
    /**
     * the latitude of a position.
     */
    double dLatitude;

    /**
     * the longitude of a position.
     */
    double dLongitude;
};


#endif //ROBUSTFLA_POSITION_H
