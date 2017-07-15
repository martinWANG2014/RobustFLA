//
// Created by martinwang on 28/04/17.
//

#ifndef FLIGHT_H
#define FLIGHT_H

#include "Node.h"
#include "NodePoint.h"
#include "Route.h"
#include "Configuration.h"
#include <set>
#include <map>
#include <jmorecfg.h>
#include <ostream>
class Flight {
public:
    /**
     * Constructor with parameters
     * @param sCode             The flight id.
     * @param pAOrigin          The flight departure airport.
     * @param pADestination     The flight destination airport.
     * @param iDepartureTime    The flight departure time.
     * @param pRoute            The flight default route.
     */
    Flight(const String &sCode, Airport *pAOrigin, Airport *pADestination, Time iDepartureTime, Route *pRoute,
           const LevelVector &viFeasibleLevelList, double dSigma) {
        Flight::sCode = sCode;
        Flight::pAOrigin = pAOrigin;
        Flight::pADestination = pADestination;
        Flight::dDepartureTime = iDepartureTime;
        Flight::pRoute = pRoute;
        Flight::iCurrentLevel = pRoute->getDefaultLevel();
        Flight::viFeasibleLevelList = viFeasibleLevelList;
        Flight::dSigma = dSigma;
    }

    Flight(const String &sCode, Airport *pAOrigin, Airport *pADestination, Time iDepartureTime, Route *pRoute){
        Flight::sCode = sCode;
        Flight::pAOrigin  = pAOrigin;
        Flight::pADestination = pADestination;
        Flight::dDepartureTime = iDepartureTime;
        Flight::pRoute = pRoute;
        Flight::iCurrentLevel = pRoute->getDefaultLevel();
    }

    /***
     * Destructor.
     */
    virtual ~Flight() {
        delete pAOrigin;
        delete pADestination;
        delete pRoute;
        viFeasibleLevelList.clear();
    }

    /**
     * Getter for sCode.
     * @return the flight code.
     */
    const String &getCode() const {
        return sCode;
    }

    /**
     * Getter for iDepartureTime.
     * @return the flight departure time.
     */
    Time getDepartureTime() const {
        return dDepartureTime;
    }

    /**
     * Get flight default flight level.
     * @return the flight default flight level.
     */
    Level getDefaultLevel() const {
        return pRoute->getDefaultLevel();
    }

    /**
     * Getter for pRoute.
     * @return the flight default route.
     */
    Route *getRoute() const {
        return pRoute;
    }

    /**
     * Getter for vpFeasibleLevelsList.
     * @return the feasible list of flight level.
     */
    const LevelVector &getFeasibleLevelList() const {
        return viFeasibleLevelList;
    }

    /**
     * Getter for iCurrentLevel.
     * @return the flight current flight level.
     */
    Level getCurrentLevel() const {
        return iCurrentLevel;
    }

    /**
     * Setter for iCurrentLevel.
     * @param iCurrentLevel     The new currrent flight level value.
     */
    void setCurrentLevel(Level iCurrentLevel) {
        Flight::iCurrentLevel = iCurrentLevel;
    }

    /**
     * Get the arriving time for a flight.
     * @return  the arriving time of a flight
     */
    Time getArrivingTime() {
        return pRoute->getFinalArrivingTime();
    }

    /**
     * Setter for vpFeasibleLevelsList.
     * @param levelList     The new feasible flight levels list.
     */
    void setFeasibleLevelList(const LevelVector levelList) {
        Flight::viFeasibleLevelList = levelList;
    }

    /**
     * Get the conflict probability between two flights.
     * @param pFlight2      The pointer of a compared flight
     * @param pdDelay       The delay time to avoid the
     * @param pbWait        Output: Which flight will wait for another one. True, current flight wait for a given flight; False, A given flight wait for current one.
     * @param bGeometricMethod       Whether use the geometric method to estimate the conflict probability
     * @return the conflict probability between two flights.
     */
    double getProbabilityConflictAndDelay(Flight *pFlight2, double *pdDelay, bool *pbWait, bool bGeometricMethod);


    /**
     * Generate a new flight with a different departure time.
     * @param iNewDepartureTime        The new flight departure time
     * @param bGeometricMethod      Whether use the geometric method
     * @return  The pointer of a new flight.
     */
    void GenerateNewFlight(Time iNewDepartureTime, bool bGeometricMethod);

    /**
     * override == operand.
     * @param rhs       the other compared Flight
     * @return true, if the two flights have same id, same departure time;false, otherwise.
     */
    bool operator==(const Flight &rhs) const;

    /**
     * override != operand.
     * @param rhs       the other compared Flight
     * @return false, if the two flights have same id, same departure time;true, otherwise.
     */
    bool operator!=(const Flight &rhs) const;

    bool selfCheck(){
        return pRoute->selfCheck();
    }

    double getSigma() const {
        return dSigma;
    }

    void setSigma(double dSigma) {
        Flight::dSigma = dSigma;
    }

    const vdList &getTimeList() const;


private:
    /**
     * the flight id
     */
    String sCode;

    /**
     * the original airport.
     */
    Airport *pAOrigin;

    /**
     * the destination airport.
     */
    Airport *pADestination;

    /**
     * the departure time of flight.
     */
    Time dDepartureTime;

    /**
     * the current flight level.
     */
    Level iCurrentLevel;

    /**
     * the feasible list of flight levels.
     */
    LevelVector viFeasibleLevelList;

    /**
     * the chosen route of flight.
     */
    Route *pRoute;

    double dSigma;
};
typedef std::vector<Flight *> FlightVector;
typedef std::map<Flight *, bool> FlightAssignmentMap;
#endif //FLIGHT_H