//
// Created by martinwang on 27/04/17.
//

#ifndef NETWORK_H
#define NETWORK_H

#include "Flight.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

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
    void InitFlightLevelsList(bool modeDisplay);

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
     * Initialize the network
     * @param dCoefPi the coefficient of Pi
     * @param iFeasibleSize  the feasible flight level list size
     * @param modeDisplay the mode of display detail information.
     */
    void initialize(double dCoefPi, int iFeasibleSize, bool modeDisplay);

    /**
     * Get ith flight of flight list.
     * @param indexI the index of flight
     * @return the ith flight.
     */
    Flight *getFlightAtI(int indexI) const;

    /**
     * save the modified flight data into file.
     * @param pNetwork the corresponding network.
     * @param percentileSup  the percentage of supplementary flight data.
     * @return the saved flight file name.
     */
    String writeFlightsSup(const Network *pNetwork, int percentileSup);

    /**
     * Generate the supplementary flights
     * @param percentileSup the percentage of supplementary flights.
     * @param offset the offset of duplicating the route.
     * @return  the saved flight file name.
     */
    String generateFlightsSup(int percentileSup, Time offset);

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
