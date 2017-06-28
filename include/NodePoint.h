//
// Created by chenghaowang on 01/06/17.
//

#ifndef NODEPOINT_H
#define NODEPOINT_H

#include "common.h"

/**
 * the Point contains the wayPoint and the arriving time of a given wayPoint.
 */
class Point{
public:
    /**
     * Constructor with parameters
     * @param pWayPoint a pointer links to the given wayPoint.
     * @param iArrivingTime the arriving time at the given wayPoint.
     */
    Point(WayPoint *pWayPoint, Time iArrivingTime);

    /**
     * Getter for pWayPoint.
     * @return the corresponding pointer links to the wayPoint.
     */
    WayPoint *getWayPoint() const;

    /**
     * Getter for iArriving time.
     * @return the corresponding arriving time.
     */
    Time getArrivingTime() const;

    /**
     * Get the code of wayPoint.
     * @return the wayPoint code.
     */
    const String &getCode() const ;

    /**
     * Getter for position of wayPoint.
     * @return the wayPoint position.
     */
    Position &getPosition() const;

    /**
     * Override of == operand
     * @param rhs the compared Point.
     * @return true, if the difference of the arriving time of two points less than Peiod Unit;
     * false, otherwise.
     */
    bool operator==(const Point &rhs) const;

    /**
     * Override of != operand
     * @param rhs the compared Point.
     * @return false, if the difference of the arriving time of two points less than Peiod Unit;
     * true, otherwise.
     */
    bool operator!=(const Point &rhs) const;

//    /**
//     * Compaere the two NodePoints will have conflct or not in a ginven Period Unit.
//     * @param lhs the one Point.
//     * @param rhs the another Point.
//     * @param iPeriodUnit the given Period Unit.
//     * @return true, if the difference between the arriving time of two points less than a given period unit;
//     * false, otherwise.
//     */
//    friend bool equal(const Point &lhs, const Point &rhs, Time iPeriodUnit);

    /**
     * Override the output stream to print the NodePoint object.
     * @param os the standard output stream.
     * @param point the NodePoint object.
     * @return redirected output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const Point &point);
private:
    /**
     * the wayPoint of the node point.
     */
    WayPoint *pWayPoint;

    /**
     * the arriving time of corresponding node point.
     */
    Time iArrivingTime;
};
#endif //NODEPOINT_H