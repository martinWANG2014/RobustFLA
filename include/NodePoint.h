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
     * @param pWayPoint         A pointer links to the given wayPoint.
     * @param iArrivingTime     The arriving time at the given wayPoint.
     */
    Point(WayPoint *pWayPoint, Time iArrivingTime) : pWayPoint(pWayPoint), iArrivingTime(iArrivingTime) {}

    virtual ~Point();

    /**
     * Getter for pWayPoint.
     * @return the corresponding pointer links to the wayPoint.
     */
    WayPoint *getWayPoint() const {
        return pWayPoint;
    }

    /**
     * Getter for iArriving time.
     * @return the corresponding arriving time.
     */
    Time getArrivingTime() const {
        return iArrivingTime;
    }

    /**
     * Setter for iArriving time
     * @param iArrivingTime the arriving time for a given point.
     */
    void setArrivingTime(Time iArrivingTime) {
        Point::iArrivingTime = iArrivingTime;
    }

    /**
     * Get the code of wayPoint.
     * @return the wayPoint code.
     */
    const String &getCode() const {
        return pWayPoint->getCode();
    }

    /**
     * Getter for position of wayPoint.
     * @return the wayPoint position.
     */
    const Position &getPosition() const {
        return pWayPoint->getPosition();
    }

    /**
     * Override of == operand
     * @param rhs       the compared Point.
     * @return true, if the difference of the arriving time of two points less than Peiod Unit;
     * false, otherwise.
     */
    bool operator==(const Point &rhs) const;

    /**
     * Override of != operand
     * @param rhs       the compared Point.
     * @return false, if the difference of the arriving time of two points less than Peiod Unit;
     * true, otherwise.
     */
    bool operator!=(const Point &rhs) const;

    /**
     * Clone a point with new arriving time.
     * @param iArrivingTime         the new arriving time.
     * @return The pointer of a new Point object.
     */
    Point *clone(Time iArrivingTime) {
        return new Point(this->pWayPoint->clone(), iArrivingTime);
    }

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

typedef std::vector<Point *> PointVector;
#endif //NODEPOINT_H