//
// Created by martinwang on 28/04/17.
//

#ifndef FLIGHT_H
#define FLIGHT_H

#include "Node.h"
#include "NodePoint.h"
#include "Route.h"
#include <set>
#include <map>
#include <jmorecfg.h>

typedef int Level;
typedef std::set<Level> LevelSet;
typedef std::vector<Level> LevelVector;

class Flight {
public:
    /**
     * A coefficient parameter: MIN_PORBA, the mininal considering conflict probability.
     */
    static double MIN_PROBA;

    /**
     * A coefficient parameter: BETA, the upper bound in an interval [t-ALPHA, t+BETA].
     */
    static double BETA;

    /**
     * A coefficient parameter: ALPHA, the lower bound in an interval [t-ALPHA, t+BETA].
     */
    static double ALPHA;

    /**
     * A coefficient parameter: GAMMA, the probability for -ALPHA<= y <= BETA
     */
    static double GAMMA;

    /**
     * A coefficient parameter: HYBRID_MU, mean value for hybrid folded normal distribution.
     */
    static double HYBRID_MU;

    /**
     * A coefficient parameter: HYBRID_SIGMA, deviation for hybrid folded normal distribution.
     */
    static double HYBRID_SIGMA;

    static double W1;

    static double W2;

    static double RATIO;

    static double mu_1_1;
    static double mu_1_2;
    static double mu_1;
    static double mu_2;
    static double sigma_1_1;
    static double sigma_1_2;
    static double sigma_1;
    static double sigma_2;


    /**
     * Constructor with parameters
     * @param sCode             The flight id.
     * @param pAOrigin          The flight departure airport.
     * @param pADestination     The flight destination airport.
     * @param iDepartureTime    The flight departure time.
     * @param pRoute            The flight default route.
     */
    Flight(const String &sCode, Airport *pAOrigin, Airport *pADestination, Time iDepartureTime, Route *pRoute){
        Flight::sCode = sCode;
        Flight::pAOrigin  = pAOrigin;
        Flight::pADestination = pADestination;
        Flight::iDepartureTime = iDepartureTime;
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
        return iDepartureTime;
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
    const LevelVector &getFeasibleLevelsList() const {
        return vpFeasibleLevelsList;
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
    void setSpFeasibleLevelSet(const LevelVector levelList) {
        Flight::vpFeasibleLevelsList = levelList;
    }

    /**
     * Generate the sigma value that makes:
     * P(a <= x <= b) >= gamma, where a-b=alpha+beta,
     * x follows a normal distribution, where the mean mu=(a+b)/2.
     * @param alpha     the expected absolute of lower bound far from the mean of random variables.
     * @param beta      the expected upper bound far from the mean of random variables.
     * @param gamma     the confidence that random variable will fall in interval [a, b]
     * @return the sigma of random varaibles.
     */
    double getSigma(double alpha, double beta, double gamma) {
        return (alpha + beta) / (2 * sqrt(2) * boost::math::erf_inv(gamma));
    }

    /**
     * Get the conflict probability between two flights.
     * @param flight                The pointer of a compared flight
     * @param pdDelay               The delay time to avoid the
     * @param pbWhoWait             Output: Which flight will wait for another one. True, current flihgt wait for a given flight; False, A given flight wait for current one.
     * @param bGeometricMethod      Whether use the geometric method to estimate the conflict probability
     * @param iModeRandom           The mode to generate the departure time from a given interval
     * @return the conflict probability between two flights.
     */
    double
    getProbabilityConflict(Flight *flight, double *pdDelay, bool *pbWhoWait, bool bGeometricMethod, int iModeRandom);

    /**
     * Generate a new flight with a different departure time.
     * @param iDepartureTime        The new flight departure time
     * @param bGeometricMethod      Whether use the geometric method
     * @return  The pointer of a new flight.
     */
    Flight *GenerateNewFlight(Time iDepartureTime, bool bGeometricMethod);

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
     * the current flight level.
     */
    Level iCurrentLevel;

    /**
     * the feasible list of flight levels.
     */
    LevelVector vpFeasibleLevelsList;

    /**
     * the chosen route of flight.
     */
    Route *pRoute;
};

typedef std::vector<Flight *> FlightVector;
typedef std::map<Flight *, bool> FlightAssignmentMap;
#endif //FLIGHT_H