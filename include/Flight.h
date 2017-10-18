//
// Created by martinwang on 28/04/17.
//

#ifndef FLIGHT_H
#define FLIGHT_H

#include <ostream>
#include "Node.h"
#include "NodePoint.h"
#include "Route.h"
#include "Configuration.h"
class Flight {
public:
    /**
     * Constructor with parameters
     * @param sCode             The flight id
     * @param pAOrigin          The flight departure airport
     * @param pADestination     The flight destination airport
     * @param iDepartureTime    The flight departure time
     * @param pRoute            The flight default route
     */
    Flight(const String &sCode, Airport *pAOrigin, Airport *pADestination, Time iDepartureTime, Route *pRoute);

    /***
     * Destructor.
     */
    virtual ~Flight();

    /**
     * Getter for sCode.
     * @return the flight code.
     */
    const String &getCode() const;

    /**
     * Getter for iDepartureTime.
     * @return the flight departure time.
     */
    Time getDepartureTime() const;

    /**
     * Get flight default flight level.
     * @return the flight default flight level.
     */
    Level getDefaultLevel() const;

    /**
     * Getter for pRoute.
     * @return the flight default route.
     */
    Route *getRoute() const;

    /**
     * Getter for vpFeasibleLevelsList.
     * @return the feasible list of flight level.
     */
    const LevelVector &getFeasibleLevelList() const;

    /**
     * Getter for iCurrentLevel.
     * @return the flight current flight level.
     */
    Level getCurrentLevel() const;

    /**
     * Setter for iCurrentLevel.
     * @param iCurrentLevel     The new current flight level value.
     */
    void setCurrentLevel(Level iCurrentLevel);

    /**
     * Get the arriving time for a flight.
     * @return  the arriving time of a flight
     */
    Time getArrivingTime() const;

    /**
     * Setter for vpFeasibleLevelsList.
     * @param levelList     The new feasible flight levels list.
     */
    void setFeasibleLevelList(const LevelVector levelList);

    /**
     * Get the conflict probability between two flights.
     * @param pFlight2              The pointer of a compared flight
     * @param pdDelay               Output: The delay time to avoid the conflict
     * @param pbWait                Output: Which flight will wait for another one. True, current flight wait for a given flight; False, A given flight wait for current one
     * @param bGeometricMethod      Whether use the geometric method to estimate the conflict probability
     * @return the conflict probability between two flights.
     */
    double CalculateProbabilityConflictAndDelayForFlight(Flight *pFlight2, double *pdDelay, double *pdDelayMax,
                                                         bool *pbWait, bool deterministic);


    /**
     * Generate a new flight with a different departure time.
     * @param iNewDepartureTime     The new flight departure time
     * @return  the pointer of a new flight.
     */
    void GenerateNewFlight(Time iNewDepartureTime);

    /**
     * override == operand.
     * @param rhs       The other compared Flight
     * @return true, if the two flights have same id, same departure time;false, otherwise.
     */
    bool operator==(const Flight &rhs) const;

    /**
     * override != operand.
     * @param rhs       The other compared Flight
     * @return false, if the two flights have same id, same departure time;true, otherwise.
     */
    bool operator!=(const Flight &rhs) const;

    /**
     * Check whether the route of flight is valid.
     * @return true, if the route is valid; false, otherwise.
     */
    bool selfCheck();

    /**
     * Getter for dSigma.
     * @return the  sigma value.
     */
    double getSigma() const;

    double getSigmaPrime() const;

    /**
     * Setter for dSigma.
     * @param dSigma    The new sigma value
     */
    void setSigma(double dSigma);

    void setSigmaPrime(double dSigmaPrime);

    /**
     * Initialize the route time list.
     */
    void initRouteTimeList();

    /**
     * Reset the route time list.
     */
    void resetRouteTimeList();

    /**
     * Getter for dCoefPi.
     * @return the coefficient of admissible cost
     */
    double getCoefPi() const;

    /**
     * Setter for dCoefPi.
     * @param dCoefPi A new value of coefficient of admissible cost
     */
    void setCoefPi(double dCoefPi);

    /**
     * Reset the current flight level as the default one.
     */
    void resetLevel();

    /**
     * Get the last feasible flight level in the feasible list.
     * @return the last feasible flight level in the feasible list.
     */
    Level getLastFeasibleLevel();

    /**
     * Add a new feasible flight level.
     * @param newLevel      The new feasible flight level
     */
    void addNewFeasibleLevel(Level newLevel);

    String getOrigAirportName() const;

    String getDestAirportName() const;

    int getNbPoints() const;

    Point *getPointAtI(int indexI) const;

    void extendRoute(Time offset);

    double getDuration() const;

    friend std::ostream &operator<<(std::ostream &os, const Flight &flight);

private:
    /**
     * The flight id
     */
    String sCode;

    /**
     * The original airport.
     */
    Airport *pAOrigin;

    /**
     * The destination airport.
     */
    Airport *pADestination;

    /**
     * The departure time of flight.
     */
    Time dDepartureTime;

    /**
     * The current flight level.
     */
    Level iCurrentLevel;

    /**
     * The feasible list of flight levels.
     */
    LevelVector viFeasibleLevelList;

    /**
     * The chosen route of flight.
     */
    Route *pRoute;

    /**
     * The deviation of departure time.
     */
    double dSigma;

    double dSigmaPrime;

    /**
     * The coefficient of admissible cost.
     */
    double dCoefPi;
};
typedef std::vector<Flight *> FlightVector;
typedef std::map<Level, std::vector<Flight *>> FlightsLevelMap;
typedef std::map<Flight *, std::pair<bool, Level>> FlightLevelAssignmentMap;
#endif //FLIGHT_H