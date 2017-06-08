//
// Created by martinwang on 27/04/17.
//

#ifndef ROBUSTFLA_INPUT_H
#define ROBUSTFLA_INPUT_H

#include "predefine.h"
#include "Flight.h"
#include "Network.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class Input {
public:
    explicit Input(const String &sAirportPath, const String &sWayPointPath, const String &sFlightPath, const String &sConfigPath);

    ~Input();
    /**
     * initialize the air management network
     * and the feasible flight level list for each flight.
     */
    void initNetwork(Network *pNetwork);

private:
    String sAirportPath;
    String sWayPointPath;
    String sFlightPath;
    String sConfigPath;
    void parseFlights(Network *pNetwork);
    void parseAirports(Network *pNetwork);
    void parseWayPoints(Network *pNetwork);
    void parseConfiguration(Network *pNetwork);
    inline bool exists (const String& name) {
        std::ifstream f(name.c_str());
        return f.good();
    }
};

#endif //ROBUSTFLA_INPUT_H
