//
// Created by chenghaowang on 01/06/17.
//

#ifndef NODEPOINT_H
#define NODEPOINT_H

#include "Node.h"


/**
 * the Point contains the wayPoint and the arriving time of a given wayPoint.
 */
class Point {
public:
    /**
     * Constructor with parameters
     * @param pWayPoint         A pointer links to the given wayPoint
     * @param iArrivingTime     The arriving time at the given wayPoint
     */
    Point(WayPoint *pWayPoint, Time iArrivingTime);

    virtual ~Point();

    /**
     * Getter for iArriving time.
     * @return the corresponding arriving time.
     */
    Time getArrivingTime() const;

    /**
     * Getter for position of wayPoint.
     * @return the wayPoint position.
     */
    const Position &getPosition() const;

    /**
     * Override of == operand
     * @param rhs       The compared Point
     * @return true, if the difference of the arriving time of two points less than Peiod Unit;
     * false, otherwise.
     */
    bool operator==(const Point &rhs) const;

    /**
     * Override of != operand
     * @param rhs       The compared Point
     * @return false, if the difference of the arriving time of two points less than Peiod Unit;
     * true, otherwise.
     */
    bool operator!=(const Point &rhs) const;

private:
    /**
     * The wayPoint of the node point.
     */
    WayPoint *pWayPoint;

    /**
     * The arriving time of corresponding node point.
     */
    Time iArrivingTime;
};

typedef std::vector<Point *> PointVector;
#endif //NODEPOINT_H