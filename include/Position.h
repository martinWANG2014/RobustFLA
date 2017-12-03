//
// Created by chenghaowang on 22/05/17.
//

#ifndef POSITION_H
#define POSITION_H

#include <math.h>
#include <iostream>

/**
 * A Position is denoted by a latitude and a longitude.
 */
class Position {
public:
    /**
     * Copy Constructor.
     * @param position      The position object
     */
    Position(const Position &position);

    /**
     * Constructor with parameters.
     * @param dLatitude     The latitude of a position
     * @param dLongitude    The longitude of a position
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
     * @param rhs       The other compared Position
     * @return true, if they are the same position; false, otherwise.
     */
    bool operator==(const Position &rhs) const;

    /**
     * Override of != operand.
     * @param rhs       The other compared Position
     * @return true, if they are different position; false, otherwise.
     */
    bool operator!=(const Position &rhs) const;

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
     * @param p1        The one position
     * @param p2        The another position
     * @return the distance between two positions.
     */
    friend double distanceBetween(const Position &p1, const Position &p2);

    /**
     * Get the cosine theta between two routes composed by three points.
     * @param position1     The crossing point position
     * @param position3     The one position from route 1
     * @param position4     The another position from route 2
     * @return
     */
    friend double getCosA(const Position &position1, const Position &position3, const Position &position4);
private:
    /**
     * The latitude of a position.
     */
    double dLatitude;

    /**
     * The longitude of a position.
     */
    double dLongitude;
};

#endif //POSITION_H
