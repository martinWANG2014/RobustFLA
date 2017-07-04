//
// Created by chenghaowang on 01/06/17.
//

#ifndef NODEPOINT_H
#define NODEPOINT_H

#include "Define.h"
#include "Node.h"

typedef int Time;
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
    Point(WayPoint *pWayPoint, Time iArrivingTime) : pWayPoint(pWayPoint), iArrivingTime(iArrivingTime){}

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

    void setArrivingTime(Time iArrivingTime){
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
typedef std::vector<Point*> PointVector;
#endif //NODEPOINT_H