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
    explicit Input(const String &sAirportPath, const String &sWayPointPath, const String &sFlightPath)
            : sAirportPath(sAirportPath), sWayPointPath(sWayPointPath), sFlightPath(sFlightPath){}

    /**
     * Destructor.
     */
    ~Input() {}

    /**
     * Initialize the air management network
     * @param pNetwork      A pointer of network
     */
    void initNetwork(Network *pNetwork, int iRandomMode, const vdList &vdParameter) {
        parseAirports(pNetwork);
        parseWayPoints(pNetwork);
        parseFlights(pNetwork, iRandomMode, vdParameter);
        pNetwork->InitFlightLevelsList();
        std::cout << "\tFlight Level size: " << pNetwork->getNbLevels() << std::endl;
    }
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
    void parseFlights(Network *pNetwork, int iRandomMode, const vdList &vdParameter);

    /**
     * Parse the airport file.
     * @param pNetwork      A pointer of network
     */
    void parseAirports(Network *pNetwork);

    /**
     * Parse the wayPoint file.
     * @param pNetwork      A pointer of network
     */
    void parseWayPoints(Network *pNetwork);

    /**
     * Verify whether the file exists.
     * @param name      The filename
     * @return true, if exists; false, otherwise.
     */
    bool exists(const String &name) {
        std::ifstream f(name.c_str());
        return f.good();
    }

    /**
     * Find the feasible flight level list.
     * @param iDefaultLevel     A given default flight level
     * @return A three elements list.
     */
    viList findFeasibleLevels(int iDefaultLevel);
};

#endif //INPUT_H
