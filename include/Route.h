//
// Created by chenghaowang on 23/05/17.
//

#ifndef ROUTE_H
#define ROUTE_H

#include "common.h"

class Route {
public:
    /**
     * Default Constructor.
     */
    Route(void);

    /**
     * Copy Constructor.
     * @param route see{@link Route}.
     */
    Route(const Route &route);

    /**
     * Destructor.
     */
    virtual ~Route();

    /**
     * Add a new point into route.
     * @param pPoint the new added point.
     */
    void addNewPoint(Point *pPoint);

    /**
     * Getter for PointsList
     * @return the corresponding points that the route will pass.
     */
    const PointVector &getPointsList() const;

    /**
     * Get the ith Point in the points list.
     * @param index the index of queried Point.
     * @return the corresponding point.
     */
    Point *getPointAtI(int index);

    /**
     * override == operand
     * @param rhs the other compared Route.
     * @return true, if the two routes have same level and pass same list of points; false, otherwise.
     */
    bool operator==(const Route &rhs) const;

    /**
     * override != operand
     * @param rhs the other compared Route.
     * @return false, if the two routes have same level and pass same list of points; true, otherwise.
     */
    bool operator!=(const Route &rhs) const;

    /**
     * Clone the object itself.
     * @return the cloned object.
     */
    Route *clone();
    friend std::ostream &operator<<(std::ostream &os, const PointVector &vpPointsList);
    friend std::ostream &operator<<(std::ostream &os, const Route &route);
private:
    /**
     * the point passed in the route.
     */
    PointVector vpPointsList;
};
#endif //ROUTE_H
