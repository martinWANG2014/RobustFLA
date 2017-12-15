//
// Created by chenghaowang on 23/05/17.
//

#ifndef ROUTE_H
#define ROUTE_H

#include <iostream>
#include <random>
#include <boost/math/special_functions/erf.hpp>
#include "NodePoint.h"


/**
 * The route class is composed by a default flight level and a list of point.
 */
class Route {
public:
    /**
     * Constructor with parameters.
     * @param iDefaultLevel     The default flight level
     * @param pAirOrigin        A pointer of departure airport
     * @param iDepartureTime    The departure time for a given flight
     * @param iPeriodUnit       The length of a period
     */
    Route(Level iDefaultLevel, Airport *pAirOrigin, Time iDepartureTime);

    /**
     * Constructor with parameters.
     * @param iDefaultLevel     The default flight level
     * @param pPoint            A pointer of departure point
     */
    Route(Level iDefaultLevel, Point *pPoint);

    /**
     * Destructor.
     */
    virtual ~Route();

    /**
     * Add a new point into route.
     * @param pPoint        The new added point
     */
    void addNewPoint(Point *pPoint);

    /**
     * Getter for iDefaultLevel.
     * @return the default flight level of current route.
     */
    const Level getDefaultLevel(void) const;

    /**
     * Get the ith Point in the points list.
     * @param iIndex        The index of queried Point
     * @return the corresponding point.
     */
    Point *getPointAtI(int iIndex);

    /**
     * Get the ith Point's arriving time.
     * @param iIndex         The index of queried Point
     * @return the ith Point's arriving time.
     */
    Time getArrivingTimeAtPoint(int iIndex);

    /**
     * Get the ith Point's position.
     * @param iIndex        The index of queried Point
     * @return the ith Point's position.
     */
    const Position &getPositionAtPoint(int iIndex);

    /**
     * Get the size of Points list.
     * @return the size of Points list.
     */
    int getPointListSize();

    /**
     * Get the velocity between the current point and its precedent one.
     * @param iIndexPoint       The index of queried Point
     * @return the velocity between the current point and its precedent one.
     */
    double getVelocityFromPoint(int iIndexPoint);

    /**
     * Get the final arriving time of current route.
     * @return the final arriving time of current route.
     */
    Time getFinalArrivingTime();


    /**
     * Gaussian method: Generate a new route with different times compared with current route.
     * @param dNewDepartureTime    The new departure time
     * @return A new route.
     */
    void GenerateNewRoute(Time dNewDepartureTime);

    /**
     * Check whether the route is valid or not.
     * @return true, if the route is valid; false, otherwise.
     */
    bool selfCheck();

    /**
     * Calculate the conflict probability and delay for two points.
     * @param iIndex1           The index of point in route 1
     * @param pRoute2           The another route 2
     * @param iIndex2           The index of point in route 2
     * @param pdDiffTime       Output: The delay time to avoid the conflict
     * @param pbWait            Output: Which flight will wait for another one. True, current flight wait for a given flight; False, A given flight wait for current one.
     * @param bGeometricMethod  Whether use the geometric method to estimate the conflict probability
     * @param dSigma1           The sigma for flight 1 to generate its departure time
     * @param dSigma2           The sigma for flight 2 to generate its departure time
     * @return the conflict probability for two points.
     */
    double CalculationProbabilityAndDelayAtPoint(int iIndex1, Route *pRoute2, int iIndex2, double *pdDiffTime,
                                                 double *pdWaitingTimeMax,
                                                 double *pdWait, bool deterministic);

    /**
     * Initialize the route time list.
     */
    void initTimeList();

    /**
     * Reset the route time list.
     */
    void resetTimeList();

    int getNbPointsPerFlight() const;

    void setNbPointsPerFlight(int nbPointsPerFlight);

    const PointVector &getVpPointsList() const;


private:
    /**
     * Each route have a constant altitude.
     */
    Level iDefaultLevel;

    /**
     * The point passed in the route.
     */
    PointVector vpPointsList;

    /**
     * The time list for each point in the route.
     */
    vdList vdTimeList;

    int nbPointsPerFlight;


    double getConflictProbability(double coefficient, double t1, double t2, double dLB, double dUB);

    double getWaitProbability(double t1, double t2, double dLB);
};

#endif //ROUTE_H
