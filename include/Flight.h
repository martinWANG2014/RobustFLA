//
// Created by martinwang on 28/04/17.
//

#ifndef FLIGHT_H
#define FLIGHT_H

#include "Define.h"
#include "Node.h"
#include "NodePoint.h"
#include "Route.h"

class Flight {
public:
    Flight(const Flight &flight) {
        sCode = flight.getCode();
        pAOrigin = flight.getAirportOrigin()->clone();
        pADestination = flight.getAirportDestination()->clone();
        iDepartureTime = flight.getDepartureTime();
        pRoute = flight.getRoute()->clone();
        iCurrentLevel = flight.getCurrentLevel();
        iDefaultLevel = flight.getDefaultLevel();
    }

    /**
     * Constructor with parameters
     * @param sCode the flight id.
     * @param pAOrigin the original airport.
     * @param pADestination the destination airport.
     * @param iDepartureTime  the departure time.
     * @param iDefaultLevel
     */
    Flight(const String &sCode, Airport *pAOrigin, Airport *pADestination, Time iDepartureTime, Route* pRoute): sCode(sCode), pAOrigin(pAOrigin), pADestination(pADestination), iDepartureTime(iDepartureTime), pRoute(pRoute){
        iCurrentLevel = iDefaultLevel = pRoute->getLevel();
    }

    /***
     * Destructor.
     */
    virtual ~Flight(){
        delete pAOrigin;
        delete pADestination;
        delete pRoute;
    }

    const String &getCode() const{
        return sCode;
    }

    Airport *getAirportOrigin() const {
        return pAOrigin;
    }

    Airport *getAirportDestination() const {
        return pADestination;
    }

    Time getDepartureTime() const{
        return iDepartureTime;
    }

    Point* getRoutePointAtI(int iIndex) const {
        return pRoute->getPointAtI(iIndex);
    }

    Level getDefaultLevel() const{
        return iDefaultLevel;
    }

    Route* getRoute() const{
        return pRoute;
    }

    void setRoute(Route *pRoute){
        delete pRoute;
        Flight::pRoute = pRoute;
    }

    /**
     * override == operand.
     * @param rhs the other compared Flight.
     * @return true, if the two flights have same id, same departure time;false, otherwise.
     */
    bool operator==(const Flight &rhs) const;

    bool operator!=(const Flight &rhs) const;

    const LevelSet &getLevelSet() const{
        return spFeasibleLevelSet;
    }
    Level getCurrentLevel() const{
        return iCurrentLevel;
    }
    void setCurrentLevel(Level iCurrentLevel){
        Flight::iCurrentLevel = iCurrentLevel;
    }

    Flight *clone(){
        return new Flight(*this);
    }

    Time getArrivingTime(){
        return pRoute->getFinalArrivingTime();
    }

    double getProbabilityConflict(const Flight &flight, double *delayWithoutConflict, int *IOrJ, int modeConflict) {
        Route *route1 = pRoute;
        Route *route2 = flight.getRoute();
        if (modeConflict == 0) {
            for (int i = 1; i < route1->getPointListSize(); i++) {
                for (int j = 1; j < route2->getPointListSize(); j++) {
                    if (*route1->getPointAtI(i) == *route2->getPointAtI(j)) {
                        double prob = route1->probabilityConflict(i, route2, j);
                        if (prob > COEFPORBABILITY){
                            *delayWithoutConflict = route1->delayWithoutConflict(i, route2, j, IOrJ);
                            return prob;
                        }
                    }
                }
            }
        }else{
            for (int i = 1; i < route1->getPointListSize(); i++) {
                for (int j = 1; j < route2->getPointListSize(); j++) {
                    if (*route1->getPointAtI(i) == *route2->getPointAtI(j)) {
                        double prob = route1->probabilityConflict2(i, route2, j);
                        if (prob > COEFPORBABILITY){
                            *delayWithoutConflict = route1->delayWithoutConflict2(i, route2, j, IOrJ);
                            return prob;
                        }
                    }
                }
            }
        }
        *delayWithoutConflict = 0;
        return -1;
    }

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

    /**
     * the default flight level.
     */
    Level iDefaultLevel;

    /**
     * the current flight level.
     */
    Level iCurrentLevel;

    /**
     * the feasible set of flight level.
     */
    LevelSet spFeasibleLevelSet;

    /**
     * the chosen route of flight.
     */
    Route *pRoute;
};
typedef std::vector<Flight *> FlightVector;
typedef std::map<Flight*, bool> FlightAssignmentMap;
#endif //FLIGHT_H