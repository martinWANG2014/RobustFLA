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
     * @param dDeltaT           The delay time
     * @return the velocity between the current point and its precedent one.
     */
    double getVelocityFromPoint(int iIndexPoint, double dDeltaT);

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
     * Geometric method: Generate a new route with different times compared with current route.
     * @param iNewDepartureTime    The new departure time
     * @return A new route.
     */
    void GenerateNewRoute(Time iNewDepartureTime);

    /**
     * Gaussian method: Generate a new route with different times compared with current route.
     * @param dNewDepartureTime    The new departure time
     * @return A new route.
     */
    void GenerateNewRoute2(Time dNewDepartureTime);

    /**
     * Check whether the route is valid or not.
     * @return true, if the route is valide; false, otherwise.
     */
    bool selfCheck();

    /**
     * Calculate the conflict probability and delay for two points.
     * @param iIndex1           The index of point in route 1
     * @param pRoute2           The another route 2
     * @param iIndex2           The index of point in route 2
     * @param pdDelayTime       Output: The delay time to avoid the conflict
     * @param pbWait            Output: Which flight will wait for another one. True, current flight wait for a given flight; False, A given flight wait for current one.
     * @param bGeometricMethod  Whether use the geometric method to estimate the conflict probability
     * @param dSigma1           The sigma for flight 1 to generate its departure time
     * @param dSigma2           The sigma for flight 2 to generate its departure time
     * @return the conflict probability for two points.
     */
    double CalculationProbabilityAndDelayAtPoint(int iIndex1, Route *pRoute2, int iIndex2, double *pdDelayTime,
                                                 bool *pbWait,
                                                 bool bGeometricMethod, double dSigma1, double dSigma2);

    /**
     * Initialize the route time list.
     */
    void initTimeList();

    /**
     * Reset the route time list.
     */
    void resetTimeList();
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

    /**
     * Calculate conflict probability and delay time.
     * @param dT1               The arriving time of flihgt 1 at the crossing point
     * @param dT2               The arriving time of flight 2 at the crossing point
     * @param dV1               The velocity of flight 1
     * @param dV2               The velocity of flight 2
     * @param dCosA             The cosine theta for the crossing angle of two conflict route
     * @param pdDelayTime       Output: The delay time to avoid the conflict
     * @param bFlag             The flag indicate whether is the conflict case that one flight arrive, another leave
     * @param bGeometricMethod  Whether use the geometric method to estimate the conflict probability
     * @param dSigma1           The sigma value for flight 1 to generate its departure time
     * @param dSigma2           The sigma value for flight 2 to generate its departure time
     * @return the conflict probability for two points.
     */
    double CalculateProbabilityAndDelayForPartA(double dT1, double dT2, double dV1, double dV2, double dCosA,
                                                double *pdDelayTime, bool bGeometricMethod, double dSigma1,
                                                double dSigma2);

    /**
     * Calculate conflict probability and delay time.
     * @param dT1               The arriving time of flihgt 1 at the crossing point
     * @param dT2               The arriving time of flight 2 at the crossing point
     * @param dV1               The velocity of flight 1
     * @param dV2               The velocity of flight 2
     * @param dCosA             The cosine theta for the crossing angle of two conflict route
     * @param pdDelayTime       Output: The delay time to avoid the conflict
     * @param bFlag             The flag indicate whether is the conflict case that one flight arrive, another leave
     * @param bGeometricMethod  Whether use the geometric method to estimate the conflict probability
     * @param dSigma1           The sigma value for flight 1 to generate its departure time
     * @param dSigma2           The sigma value for flight 2 to generate its departure time
     * @return the conflict probability for two points.
     */
    double CalculateProbabilityAndDelayForPartB(double dT1, double dT2, double dV1, double dV2, double dCosA,
                                                double *pdDelayTime, bool bGeometricMethod, double dSigma1,
                                                double dSigma2);

    /**
     * Calculate conflict probability and delay time.
     * @param dT1               The arriving time of flihgt 1 at the crossing point
     * @param dT2               The arriving time of flight 2 at the crossing point
     * @param dV1               The velocity of flight 1
     * @param dV2               The velocity of flight 2
     * @param dCosA             The cosine theta for the crossing angle of two conflict route
     * @param pdDelayTime       Output: The delay time to avoid the conflict
     * @param bFlag             The flag indicate whether is the conflict case that one flight arrive, another leave
     * @param bGeometricMethod  Whether use the geometric method to estimate the conflict probability
     * @param dSigma1           The sigma value for flight 1 to generate its departure time
     * @param dSigma2           The sigma value for flight 2 to generate its departure time
     * @return the conflict probability for two points.
     */
    double CalculateProbabilityAndDelayForPartC(double dT1, double dT2, double dV1, double dV2, double dCosA,
                                                double *pdDelayTime, bool bGeometricMethod, double dSigma1,
                                                double dSigma2);

    /**
     * Calculate conflict probability and delay time.
     * @param dT1               The arriving time of flihgt 1 at the crossing point
     * @param dT2               The arriving time of flight 2 at the crossing point
     * @param dV1               The velocity of flight 1
     * @param dV2               The velocity of flight 2
     * @param dCosA             The cosine theta for the crossing angle of two conflict route
     * @param pdDelayTime       Output: The delay time to avoid the conflict
     * @param bFlag             The flag indicate whether is the conflict case that one flight arrive, another leave
     * @param bGeometricMethod  Whether use the geometric method to estimate the conflict probability
     * @param dSigma1           The sigma value for flight 1 to generate its departure time
     * @param dSigma2           The sigma value for flight 2 to generate its departure time
     * @return the conflict probability for two points.
     */
    double CalculateProbabilityAndDelayForPartD(double dT1, double dT2, double dV1, double dV2, double dCosA,
                                                double *pdDelayTime, bool bGeometricMethod, double dSigma1,
                                                double dSigma2);

    /**
     * Calculate conflict probability and delay time.
     * @param iIndex1           The index of point in route 1
     * @param pRoute2           The another route 2
     * @param iIndex2           The index of point in route 2
     * @param pdDelayTime       Output: The delay time to avoid the conflict
     * @param bFlag1            The flag indicate whether the point in route 1 has a precedent point
     * @param bFlag2            The flag indicate whether the point in route 2 has a precedent point
     * @param bGeometricMethod  Whether use the geometric method to estimate the conflict probability
     * @param dSigma1           The sigma value for flight 1 to generate its departure time
     * @param dSigma2           The sigma value for flight 2 to generate its departure time
     * @return the conflict probability for two points.
     */
    double CalculatePartialProbabilityAndDelay(int iIndex1, Route *pRoute2, int iIndex2, double *pdDelayTime,
                                               bool bFlag1,
                                               bool bFlag2, bool bGeometricMethod, double dSigma1, double dSigma2);
};

#endif //ROUTE_H
