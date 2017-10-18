//
// Created by martinwang on 27/04/17.
//

#ifndef INPUT_H
#define INPUT_H
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "Flight.h"
#include "Network.h"
/**
 * The input class for a purpose to parse the data file, then to initialize the network object with these parsed data.
 */
class Input {
public:
    /**
     * Constructor with parameter
     * @param sAirportPath      The filename of airport
     * @param sWayPointPath     The filename of wayPoint
     * @param sFlightPath       The filename of flight
     */
    explicit Input(const String &sAirportPath, const String &sWayPointPath, const String &sFlightPath);

    /**
     * Destructor.
     */
    ~Input();

    /**
     * Initialize the air management network
     * @param pNetwork      A pointer of network
     */
    void initNetwork(Network *pNetwork, bool modeDisplay, bool flightFileModified);

private:
    /**
     * The airport filename.
     */
    String sAirportPath;

    /**
     * The wayPoint filename.
     */
    String sWayPointPath;

    /**
     * The flight filename.
     */
    String sFlightPath;

    /**
     * Parse the flight file.
     * @param pNetwork      A pointer of network
     */
    void parseFlights(Network *pNetwork, bool modeDisplay, bool flightFileModified);

    /**
     * Parse the airport file.
     * @param pNetwork      A pointer of network
     */
    void parseAirports(Network *pNetwork, bool modeDisplay);

    /**
     * Parse the wayPoint file.
     * @param pNetwork      A pointer of network
     */
    void parseWayPoints(Network *pNetwork, bool modeDisplay);


};

#endif //INPUT_H
