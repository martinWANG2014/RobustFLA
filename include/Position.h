//
// Created by chenghaowang on 22/05/17.
//

#ifndef ROBUSTFLA_POSITION_H
#define ROBUSTFLA_POSITION_H

#include "common.h"

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
     * Override of == operand.
     * @param rhs the other compared Position.
     * @return true, if they are the same position; false, otherwise.
     */
    bool operator==(const Position &rhs) const;

    /**
     * Override of != operand.
     * @param rhs the other compared Position.
     * @return true, if they are different position; false, otherwise.
     */
    bool operator!=(const Position &rhs) const;

    /**
     * Clone the object itself.
     * @return the cloned object.
     */
    Position *clone();

    /**
     * Override the output stream to print the Position object.
     * @param os the standard output stream.
     * @param position the Position Object.
     * @return redirected output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const Position &position);

    /**
     * Calculate the distance between the two positions by Haversine formula.
     * Haversine formula:
     *                  R = earth’s radius (mean radius = 6,378.16km)
	 *				    delta_lat = lat2− lat1
     *					delta_lng = lng2− lng1
     *					a = sin²(delta_lat/2) + cos(lat1)*cos(lat2)*sin²(delta_lng/2)
     *					c = 2*atan2(√a, √(1−a))
     *					d = R*c
     * (Note that angles need to be in radians to pass to trig functions).
     * @param p1 the one position.
     * @param p2 the another position.
     * @return the distance between two positions.
     */
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
