//
// Created by chenghaowang on 22/05/17.
//

#ifndef POSITION_H
#define POSITION_H

#include <math.h>

/**
 * A Position is denoted by a latitude and a longitude.
 */
class Position {
public:
    /**
     * A coefficient parameter: earth radius.
     */
    static double EARTH_RADIUS;

    /**
     * Copy Constructor.
     * @param position      The position object.
     */
    Position(const Position &position) : dLatitude(position.getLatitude()), dLongitude(position.getLongitude()) {}

    /**
     * Constructor with parameters.
     * @param dLatitude     the latitude of a position.
     * @param dLongitude    the longitude of a position.
     */
    Position(double dLatitude, double dLongitude) : dLatitude(dLatitude), dLongitude(dLongitude) {}

    /**
     * Destructor
     */
    virtual ~Position() {}

    /**
     * Getter for the latitude.
     * @return the latitude of a corresponding position.
     */
    double getLatitude() const {
        return dLatitude;
    }

    /**
     * Getter for the longitude.
     * @return the longitude of a corresponding position.
     */
    double getLongitude() const {
        return dLongitude;
    }

    /**
     * Override of == operand.
     * @param rhs       the other compared Position.
     * @return true, if they are the same position; false, otherwise.
     */
    bool operator==(const Position &rhs) const;

    /**
     * Override of != operand.
     * @param rhs       the other compared Position.
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
     * @param p1        the one position.
     * @param p2        the another position.
     * @return the distance between two positions.
     */
    friend double distanceBetween(const Position &p1, const Position &p2);

    friend double getCosA(const Position &position1, const Position &position3, const Position &position4) {
        double b = distanceBetween(position1, position3);
        double c = distanceBetween(position1, position4);
        double a = distanceBetween(position3, position4);
        return (pow(b, 2) + pow(c, 2) - pow(a, 2)) / (2 * b * c);
    }
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

#endif //POSITION_H
