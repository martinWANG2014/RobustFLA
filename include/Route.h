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
        vdTimeList[iIndex] = iArrivingTime;//vpPointsList[iIndex]->setArrivingTime(iArrivingTime);
    }

    /**
     * Get the ith Point's arriving time.
     * @param iIndex         The index of queried Point.
     * @return the ith Point's arriving time.
     */
    Time getArrivingTimeAtPoint(int iIndex) {
        return vdTimeList[iIndex];//vpPointsList[iIndex]->getArrivingTime();
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
    double getVelocityFromPoint(int iIndexPoint, double dDeltaT);
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
     * @param iNewDepartureTime    The new departure time.
     * @return A new route.
     */
    void GenerateNewRoute(Time iNewDepartureTime);

    /**
     * Gaussian method: Generate a new route with different times compared with current route.
     * @param dNewDepartureTime    The new departure time.
     * @return A new route.
     */
    void GenerateNewRoute2(Time dNewDepartureTime);
    bool selfCheck();

    double CalculationProbabilityAndDelay(int iIndex1, Route *pRoute2, int iIndex2, double *pdDelayTime, bool *pbWait,
                                          bool bGeometricMethod, double dSigma1, double dSigma2);

    const vdList &getTimeList() const {
        return vdTimeList;
    }

    void setTimeList(const vdList &vdTimeList) {
        Route::vdTimeList = vdTimeList;
    }

    void initTimeList() {
        for (int i = 0; i < getPointListSize(); i++) {
            vdTimeList[i] = vpPointsList[i]->getArrivingTime();
        }
    }
private:
    /**
     * each route have a constant altitude.
     */
    Level iDefaultLevel;

    /**
     * the point passed in the route.
     */
    PointVector vpPointsList;

    vdList vdTimeList;

    double
    probabilityAndDelay(double dT1, double dT2, double dV1, double dV2, double dCosA, double *pdDelayTime, bool bFlag,
                        bool bGeometricMethod, double dSigma1, double dSigma2);

    double calculateProbabilityAndDelay(int iIndex1, Route *pRoute2, int iIndex2, double *pdDelayTime, bool bFlag1,
                                        bool bFlag2, bool bGeometricMethod, double dSigma1, double dSigma2);
};

#endif //ROUTE_H
