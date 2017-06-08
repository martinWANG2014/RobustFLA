//
// Created by chenghaowang on 01/06/17.
//

#ifndef ROBUSTFLA_NODEPOINT_H
#define ROBUSTFLA_NODEPOINT_H

#include <ostream>
#include "predefine.h"
#include "Node.h"

class Point{
public:
    Point(WayPoint *pWayPoint, Time iArrivingTime);

    WayPoint *getWayPoint() const;

    Time getArrivingTime() const;

    const String &getCode() const ;

    friend bool equal(const Point &lhs, const Point &rhs, Time iPeriodUnit);

    friend std::ostream &operator<<(std::ostream &os, const Point &point);

private:
    WayPoint *pWayPoint;
    Time iArrivingTime;
};
typedef std::vector<Point*> PointVector;
#endif //ROBUSTFLA_NODEPOINT_H
