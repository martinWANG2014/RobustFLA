//
// Created by martinwang on 27/04/17.
//

#ifndef NETWORK_H
#define NETWORK_H

#include "Flight.h"

class Network {
public:
    /**
     * The static coefficient: PERIOD_LENGTH, the length of period that manage the two conflict aircrafts.
     */
    static double PERIOD_LENGTH;
    /**
     * Default Constructor.
     */
    Network();

    /**
     * Destructor.
     */
    virtual ~Network();

    /**
     * Add a new flight in the air traffic management network.
     * @param pFlight     The pointer of a new flight
     */
    void addNewFlight(Flight *pFlight);

    /**
     * Add a new airport in the air traffic management network.
     * @param pAirport     The pointer of a new airport
     */
    void addNewAirport(Airport *pAirport);

    /**
     * Add a new wayPoint in the air traffic management network.
     * @param pWayPoint     The pointer of a new wayPoint
     */
    void addNewWayPoint(WayPoint *pWayPoint);

    /**
     * Get the number of airports in the air traffic management network.
     * @return the number of airports in the air traffic management network.
     */
    int getNbAirports() const;

    /**
     * Get the number of wayPoints in the air traffic management network.
     * @return the number of wayPoints in the air traffic management network.
     */
    int getNbWayPoints() const;

    /**
     * Get the number of flights in the air traffic management network.
     * @return the number of flights in the air traffic management network.
     */
    int getNbFlights() const;

    /**
     * Get the number of levels in the air traffic management network.
     * @return the number of levels in the air traffic management network.
     */
    int getNbLevels() const;

    /**
     * Find the airport object by its given code.
     * @param sCode     The code of an airport.
     * @return The pointer of a corresponding airport.
     */
    Airport *findAirportByCode(String sCode);

    /**
     * Find the wayPoint object by its given code.
     * @param sCode     The code of a wayPoint.
     * @return  The pointer of a corresponding wayPoint.
     */
    WayPoint *findWayPointByCode(String sCode);

    /**
     * Getter for vpFlightsList.
     * @return the list of flights in the air traffic management network.
     */
    const FlightVector &getFlightsList() const;

    /**
     * Getter for viFlightLevelsList.
     * @return the list of flight levels in the air traffic management network.
     */
    const LevelVector &getFlightLevelsList() const;

    /**
     * Getter for vpAirportsList.
     * @return the list of airports in the air traffic management network.
     */
    const AirportVector &getAirportsList() const;

    /**
     * Getter for vpWayPonitsList.
     * @return the list of wayPoints in the air traffic management network.
     */
    const WayPointVector &getWayPointsList() const;

    /**
     * Initialize the flight level list.
     */
    void InitFlightLevelsList();

    /**
     * Initialize the feasible flight levels list with the list size.
     * @param iSize     The feasible flight levels list size
     */
    void InitFeasibleList(int iSize);

    /**
     * Initialize the coefficient of admissible cost
     * @param dCoefPi       The coefficient of admissible cost
     */
    void InitCoefPi(double dCoefPi);

    /**
     * Set the sigma value.
     * @param vdParameters         The parameter list for distribution
     * @param iRandomMode       The mode of distribution
     */
    void SetSigma(const vdList &vdParameters, int iRandomMode, bool modeGenerateSigma);
private:
    /**
     * The flight list in the air traffic management network.
     */
    FlightVector vpFlightsList;

    /**
     * The airport list in the air traffic management network.
     */
    AirportVector vpAirportsList;

    /**
     * The wayPoint list in the air traffic management network.
     */
    WayPointVector vpWayPointsList;

    /**
     * The flight levels list in the air traffic management network.
     */
    LevelVector viFlightLevelsList;
};
#endif //NETWORK_H
