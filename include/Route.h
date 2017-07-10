//
// Created by chenghaowang on 23/05/17.
//

#ifndef ROUTE_H
#define ROUTE_H

#include <iostream>
#include <random>
#include <boost/math/special_functions/erf.hpp>
#include "NodePoint.h"

typedef std::normal_distribution<double> normal_dist;
typedef int Level;

/**
 * The route class is composed by a default flight level and a list of point.
 */
class Route {
public:
    /**
     * A coefficient parameter: K, 1(nmi)=K*1(km)
     */
    static double K;

    /**
     * A coefficient parameter: MIN_SEPARATION_DISTANCE, =5.0 nmi.
     */
    static double MIN_SEPARATION_DISTANCE;

    /**
     * A coefficient parameter: MIN_ANGLE, the protected angle of motion for the flight.
     */
    static double MIN_ANGLE;

    /**
     * A coefficient parameter: A_BAR, from article "A Geometrical Approach to Conflict Probability Estimation, Richard Irvine"
     */
    static double A_BAR;

    /**
     * Constructor with parameters.
     * @param iDefaultLevel     The default flight level
     * @param pAirOrigin        A pointer of departure airport
     * @param iDepartureTime    The departure time for a given flight
     * @param iPeriodUnit       The length of a period
     */
    Route(Level iDefaultLevel, Airport *pAirOrigin, Time iDepartureTime) : iDefaultLevel(iDefaultLevel),
                                                                           vpPointsList() {
        vpPointsList.push_back(new Point(pAirOrigin, iDepartureTime));
    }

    /**
     * Constructor with parameters.
     * @param iDefaultLevel     The default flight level
     * @param pPoint            A pointer of departure point.
     */
    Route(Level iDefaultLevel, Point *pPoint) : iDefaultLevel(iDefaultLevel), vpPointsList() {
        vpPointsList.push_back(pPoint);
    }

    /**
     * Destructor.
     */
    virtual ~Route() {
        for (auto &&point: vpPointsList) {
            delete point;
        }
        vpPointsList.clear();
    }

    /**
     * Add a new point into route.
     * @param pPoint        the new added point.
     */
    void addNewPoint(Point *pPoint) {
        vpPointsList.push_back(pPoint);
    }

    /**
     * Getter for iDefaultLevel.
     * @return the default flight level of current route.
     */
    const Level getDefaultLevel(void) const {
        return iDefaultLevel;
    }

    /**
     * Getter for vpPointsList.
     * @return the corresponding points that the route will pass.
     */
    const PointVector &getPointsList() const {
        return vpPointsList;
    }

    /**
     * Get the ith Point in the points list.
     * @param iIndex        the index of queried Point.
     * @return the corresponding point.
     */
    Point *getPointAtI(int iIndex) {
        return vpPointsList[iIndex];
    }

    /**
     * Set the ith Point's arriving time.
     * @param iIndex            The index of queried Point.
     * @param iArrivingTime     The new arriving time value
     */
    void setArrivingTimeAtPoint(int iIndex, Time iArrivingTime) {
        vpPointsList[iIndex]->setArrivingTime(iArrivingTime);
    }

    /**
     * Get the ith Point's arriving time.
     * @param iIndex         The index of queried Point.
     * @return the ith Point's arriving time.
     */
    Time getArrivingTimeAtPoint(int iIndex) {
        return vpPointsList[iIndex]->getArrivingTime();
    }

    /**
     * Get the ith Point's position.
     * @param iIndex        The index of queried Point.
     * @return the ith Point's position.
     */
    const Position &getPositionAtPoint(int iIndex) {
        return vpPointsList[iIndex]->getPosition();
    }

    /**
     * Get the size of Points list.
     * @return the size of Points list.
     */
    int getPointListSize() {
        return (int) vpPointsList.size();
    }

    /**
     * Get the velocity between the current point and its precedent one.
     * @param iIndexPoint       The index of queried Point.
     * @return the velocity between the current point and its precedent one.
     */
    double getVelocityFromPoint(int iIndexPoint);

    /**
     * Get the final arriving time of current route.
     * @return the final arriving time of current route.
     */
    Time getFinalArrivingTime() {
        return getArrivingTimeAtPoint((int) vpPointsList.size() - 1);
    }

    /**
     * Geometric method: Generate a new route with different times compared with current route.
     * @param iDepartureTime    The new departure time.
     * @return A new route.
     */
    Route *GenerateNewRoute(Time iDepartureTime);

    /**
     * Gaussian method: Generate a new route with different times compared with current route.
     * @param iDepartureTime    The new departure time.
     * @return A new route.
     */
    Route *GenerateNewRoute2(Time iDepartureTime);

    /**
     * Geometric method: Calculate the conflict probability and the delay time to avoid this conflict.
     * @param iIndex1       The index of point in current route
     * @param pRoute2       The pointer of a given route
     * @param iIndex2       The index of point in given route
     * @param delay         output: The delay to avoid the conflict for the two point.
     * @return the conflict probability between the two point in different route.
     */
    double CPAD(int iIndex1, Route *pRoute2, int iIndex2, double *delay, bool *ind);
    double CPAD3(int iIndex1, Route *pRoute2, int iIndex2, double *delay, bool *ind);
    double CPAD3_11(int iIndex1, Route *pRoute2, int iIndex2, double *delay);
    double CPAD3_10(int iIndex1, Route *pRoute2, int iIndex2, double *delay);
    double CPAD3_01(int iIndex1, Route *pRoute2, int iIndex2, double *delay);
    double CPAD3_00(int iIndex1, Route *pRoute2, int iIndex2, double *delay);
    double CPAD4(int iIndex1, Route *pRoute2, int iIndex2, double sigma1, double sigma2,
                 double *delay, bool *ind);
    double CPAD4_11(int iIndex1, Route *pRoute2, int iIndex2,  double sigma_1, double sigma_2, double *delay);
    double CPAD4_10(int iIndex1, Route *pRoute2, int iIndex2,  double sigma_1, double sigma_2, double *delay);
    double CPAD4_01(int iIndex1, Route *pRoute2, int iIndex2,  double sigma_1, double sigma_2, double *delay);
    double CPAD4_00(int iIndex1, Route *pRoute2, int iIndex2,  double sigma_1, double sigma_2, double *delay);
    /**
     * Gaussian method: Calculate the conflict probability and the delay time to avoid this conflict.
     * @param iIndex1       The index of point in current route
     * @param pRoute2       The pointer of a given route
     * @param iIndex2       The index of point in given route
     * @param mu1           The mean value of current route departure time
     * @param sigma1        The deviation of current route departure time
     * @param mu2           The mean value of a given route departure time
     * @param sigma2        The deviation of a given route departure time
     * @param delay         output: The delay to avoid the conflict for the two point.
     * @return
     */
    double CPAD2(int iIndex1, Route *pRoute2, int iIndex2, double mu1, double sigma1, double mu2, double sigma2,
                 double *delay, bool *ind);
    bool selfCheck();
private:
    /**
     * each route have a constant altitude.
     */
    Level iDefaultLevel;

    /**
     * the point passed in the route.
     */
    PointVector vpPointsList;
};

#endif //ROUTE_H
