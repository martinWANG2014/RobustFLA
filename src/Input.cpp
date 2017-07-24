//
// Created by chenghaowang on 06/07/17.
//
#include "../include/Input.h"
void Input::parseWayPoints(Network *pNetwork) {
    using boost::property_tree::ptree;
    using boost::property_tree::read_json;

    std::cout << "[INFO] Parsing wayPoints file... " << std::flush;

    // Verify whether the data file exists or not: if not, pop up an error message.
    if (!exists(sWayPointPath)) {
        std::cerr << "[ERROR] Not exist " << sWayPointPath << std::endl;
        abort();
    }

    // Parse the json file with boost property tree.
    ptree root;
    // Read the json file by the read_json method offered by the boost library.
    read_json(sWayPointPath, root);

    int nbWayPoints = root.get<int>("BN");
    for (int i = 0; i < nbWayPoints; i++) {
        String sPrefixed(std::to_string(i));
        // Parse the wayPoint code.
        String sCode = root.get<String>(sPrefixed + ".Code");
        // Verify the wayPoint code, if it's empty, then pop up an error message.
        if (sCode.empty()) {
            std::cout << std::endl << "[Warning]: the " << i
                      << "th wayPoint's code is unknown, it is ignored automatically!" << std::endl;
        } else {
            // Parse the wayPoint name and its position.
            WayPoint *wayPoint = new WayPoint(sCode,
                                              root.get<String>(sPrefixed + ".Name", ""),
                                              root.get<double>(sPrefixed + ".Lat", 0.0),
                                              root.get<double>(sPrefixed + ".Lng", 0.0));

            // Verify whether the network has contained the wayPoint or not. If not then add it into network.
            if (!contains(pNetwork->getWayPointsList(), wayPoint)) {
                pNetwork->addNewWayPoint(wayPoint);
            }
        }
    }
    std::cout << "OK" << std::endl
              << "\tBeacon file data:" << std::endl
              << "\tWayPoints: " << nbWayPoints << std::endl
              << "\tValid WayPoints: " << pNetwork->getNbWayPoints() << std::endl;
}

void Input::parseAirports(Network *pNetwork) {
    using boost::property_tree::ptree;
    using boost::property_tree::read_json;

    std::cout << "[INFO] Parsing airports file... " << std::flush;

    // Verify whether the data file exists or not: if not, pop up an error message.
    if (!exists(sAirportPath)) {
        std::cerr << "[ERROR] Not exist " << sAirportPath << std::endl;
        abort();
    }

    // Parse the json file with boost property tree.
    ptree root;
    // Read the json file by the read_json method offered by the boost library.
    read_json(sAirportPath, root);

    int nbAirports = root.get<int>("AN");
    for (int i = 0; i < nbAirports; i++) {
        String sPrefixed(std::to_string(i));
        // Parse the airport icao code.
        String sCode = root.get<String>(sPrefixed + ".ICAO");
        // Verify the airport icao code, if it's empty, then pop up an error message.
        if (sCode.empty()) {
            std::cout << "[Warning]: the " << i << "th airport's icao code is unknown, it is ignored automatically!"
                      << std::endl;
        } else {
            // Parse the airport name and its position.
            Airport *airport = new Airport(sCode,
                                           root.get<String>(sPrefixed + ".Name", ""),
                                           root.get<double>(sPrefixed + ".Lat", 0.0),
                                           root.get<double>(sPrefixed + ".Lng", 0.0));
            // Verify whether the network has contained the airport or not. If not, then add it into network.
            if (!contains(pNetwork->getAirportsList(), airport)) {
                pNetwork->addNewAirport(airport);
            }
        }
    }
    std::cout << "OK" << std::endl
              << "\tAirports file data:" << std::endl
              << "\tAirports: " << nbAirports << std::endl
              << "\tValid Airports: " << pNetwork->getNbAirports() << std::endl;
}

void Input::parseFlights(Network *pNetwork) {
    using boost::property_tree::ptree;
    using boost::property_tree::read_json;
    std::cout << "[INFO] Parsing flights file... " << std::flush;
    // Verify whether the data file exists or not: if not, pop up an error message.
    if (!exists(sFlightPath)) {
        std::cerr << "[ERROR] Not exist " << sFlightPath << std::endl;
        abort();
    }
    // Parse the json file with boost property tree.
    ptree root;
    // Read the json file by the read_json method offered by the boost library.
    read_json(sFlightPath, root);
    // Start to parse the content of json object.
    int nbFlights = root.get<int>("FN");
    for (int i = 0; i < nbFlights; i++) {
        String sPrefixed(std::to_string(i));
        // Parse the flight icao code.
        String sCode(root.get<String>(sPrefixed + ".FCode"));
        // Parse the flight departure airport.
        Airport *pAirportOrigin = pNetwork->findAirportByCode(root.get<String>(sPrefixed + ".Origin"));
        // Parse the flight destination airport.
        Airport *pAirportDestination = pNetwork->findAirportByCode(root.get<String>(sPrefixed + ".Dest"));
        // Parse the flight departure time.
        Time iDepartureTime = root.get<Time>(sPrefixed + ".DTime", -1);
        // Verify the icao code of a flight, if it is empty, then pop up an error message.
        if (sCode.empty()) {
            std::cout << std::endl << "[Warning]: the " << i
                      << "th flight's code is unknown, it is ignored automatically!" << std::endl;
        }
            // Verify the flight departure time, if it is not positive, then pop up an error message.
        else if (iDepartureTime < 0) {
            std::cout << std::endl << "[Warning]: the " << i
                      << "th flight's departure time is not valid, it is ignored automatically!" << std::endl;
        }
            // Verify flight depart airport, if it is empty, then pop up an error message.
        else if (pAirportOrigin == nullptr) {
            std::cout << std::endl << "[Warning]: the " << i
                      << "th flight's origin airport is unknown, it is ignored automatically!" << std::endl;
        }
            // Verify flight destination airport, if it is empty, then pop up an error message.
        else if (pAirportDestination == nullptr) {
            std::cout << std::endl << "[Warning]: the " << i
                      << "th flight's destination is unknown, it is ignored automatically!" << std::endl;
        }
            // Verify whether the departure airport is the same as the destination airport or not, if yes then pop up an error message.
        else if (*pAirportOrigin == *pAirportDestination) {
            std::cout << std::endl << "[Warning]: the " << i
                      << "th flight's destination is identical with the departure, it is ignored automatically!"
                      << std::endl;
        } else {
            // Parse the default flight level.
            Level iLevel = root.get<int>(sPrefixed + ".FLevel");
            Route *pRoute = new Route(iLevel, pAirportOrigin, iDepartureTime);
            int nbPoints = root.get<int>(sPrefixed + ".PointList.PLN");
            bool bValid = true;
            int iIndexPoint = 0;

            // Parse the flight default route.
            while (bValid && iIndexPoint < nbPoints) {
                String sPrefixedPath(sPrefixed + ".PointList." + std::to_string(iIndexPoint));
                // Parse the wayPoint in the route.
                WayPoint*pWayPoint = pNetwork->findWayPointByCode(root.get<String>(sPrefixedPath + ".Code"));
                // Verify the wayPoint, if it is empty, then pop up an error message.
                if (pWayPoint == nullptr) {
                    std::cout << std::endl << "[Warning]: the code of " << iIndexPoint << "th point in " << i
                              << "th flight's route is unknown, it is ignored automatically!"
                              << std::endl;
                    bValid = false;
                } else {
                    // Parse the arriving time at a corresponding wayPoint.
                    Point *point = new Point(pWayPoint, root.get<Time>(sPrefixedPath + ".Time"));
                    pRoute->addNewPoint(point);
                }
                iIndexPoint++;
            }
            // If the route is correct, then create a flight object.
            if (bValid) {
                Flight *pFlight = new Flight(sCode, pAirportOrigin, pAirportDestination, iDepartureTime, pRoute);
                // Verify whether the network has contained the flight or not. If not, then add it into network.
                bool bIsRouteValid = pFlight->selfCheck();
                if (bIsRouteValid && !contains(pNetwork->getFlightsList(), pFlight)) {
                    pFlight->initRouteTimeList();
                    pNetwork->addNewFlight(pFlight);
                }
//                else if (bIsRouteValid) {
//                    std::cout << std::endl
//                              << "[Warning]: the route of " << i
//                              << " flight is not correct, it is ignored automatically!"
//                              << std::endl;
//                } else {
//                    std::cout << std::endl
//                              << "[Warning]: the " << i << " flight was redundant, it is ignored automatically!"
//                              << std::endl;
//                }
            }
//            else {
//                std::cout << std::endl
//                          << "[Warning]: the route of " << i << " flight is not complete, it is ignored automatically!"
//                          << std::endl;
//            }
        }
    }
    std::cout << "OK" << std::endl
              << "\tFlights file data:" << std::endl
              << "\tFlights: " << nbFlights << std::endl
              << "\tValid Flights: " << pNetwork->getNbFlights() << std::endl;
}

Input::Input(const String &sAirportPath, const String &sWayPointPath, const String &sFlightPath)
        : sAirportPath(sAirportPath), sWayPointPath(sWayPointPath), sFlightPath(sFlightPath) {}

Input::~Input() {}

void Input::initNetwork(Network *pNetwork) {
    parseAirports(pNetwork);
    parseWayPoints(pNetwork);
    parseFlights(pNetwork);
}

bool Input::exists(const String &name) {
    std::ifstream f(name.c_str());
    return f.good();
}
