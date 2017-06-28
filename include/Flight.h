//
// Created by martinwang on 28/04/17.
//

#ifndef FLIGHT_H
#define FLIGHT_H

#include "common.h"
#include "Route.h"

class Flight {
public:
    /**
     * Copy Constructor
     * @param flight see {@link Flight}.
     */
    Flight(const Flight &flight);

    /**
     * Constructor with parameters
     * @param sCode the flight id.
     * @param pAOrigin the original airport.
     * @param pADestination the destination airport.
     * @param iDepartureTime  the departure time.
     * @param pDefaultLevel
     * @param pDefaultRoute
     */
    Flight(const String &sCode, Airport *pAOrigin, Airport *pADestination, Time iDepartureTime, FLevel *pDefaultLevel, Route *pDefaultRoute);

    /***
     * Destructor.
     */
    virtual ~Flight();

    const String &getCode() const;

    Airport *getAirportOrigin() const;

    Airport *getAirportDestination() const;

    Time getDepartureTime() const;

    const FLevelVector &getFeasibleLevelList() const;

    Point* getRoutePointAtI(int iIndex);

    /**
     * override == operand.
     * @param rhs the other compared Flight.
     * @return true, if the two flights have same id, same departure time;false, otherwise.
     */
    bool operator==(const Flight &rhs) const;

    /**
     * override != operand.
     * @param rhs the other compared Flight.
     * @return false, if the two flights have same id, same departure time;true, otherwise.
     */
    bool operator!=(const Flight &rhs) const;

    /**
     * Clone the object itself.
     * @return the cloned object.
     */
    Flight *clone();

    void initFeasibleLevelList();

    int getDefaultLevel();

    int getNbLevels() const;

    int getLevelAtI(int index) const;
    int getCurrentLevel() const ;
    void setCurrentLevel(int iIndexLevel);
    double getCostOfCurrentLevel();
    friend std::ostream &operator<<(std::ostream &os, const FLevelVector &flightLevelVector);
    friend std::ostream &operator<<(std::ostream &os, const Flight &flight);

    bool isCurrentPreferred();
    void setAssigned(bool bAssigned);
    bool isAssigned();
    friend double erfFunction(double x);
    friend void calcluteConflictBetween(Flight &lhs, int iIndexI, Flight &rhs, int iIndexJ, bool *isIFirst, double *dOutProbability, double *dOutCost);

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
    Time iDepartureTime;

    int iDefaultLevelIndex;

    int iCurrentLevelIndex;

    /**
     * the feasible list of flight level.
     */
    FLevelVector vpFeasibleLevelList;

    Route *pRoute;

    void addNewFLevel(FLevel *pFLevel);


    bool hasAssigned = false;
};
#endif //FLIGHT_H