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
        std::cerr << std::endl << "[ERROR] Not exist " << sWayPointPath << std::endl;
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
            std::cerr << std::endl << "[ERROR] at: " << sWayPointPath << std::endl
                      << "\tat: " << i << "th wayPoint" << std::endl
                      << "\tthe wayPoint's code is unknown, it is ignored automatically!" << std::endl;
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
        std::cerr << std::endl << "[ERROR] Not exist " << sAirportPath << std::endl;
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
            std::cerr << "[ERROR] at: " << sAirportPath << std::endl
                      << "\tat: " << i << "th airport" << std::endl
                      << "\tthe airport icao code is unknown, it is ignored automatically!" << std::endl;
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
        std::cerr << std::endl << "[ERROR] Not exist " << sFlightPath << std::endl;
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
            std::cerr << std::endl << "[ERROR] at: " << sFlightPath << std::endl
                      << "\tat: " << i << "th flight" << std::endl
                      << "\tthe flight code is unknown, it is ignored automatically!" << std::endl;
        }
            // Verify the flight departure time, if it is not positive, then pop up an error message.
        else if (iDepartureTime < 0) {
            std::cerr << std::endl << "[ERROR] at: " << sFlightPath << std::endl
                      << "\tat: " << i << "th flight" << std::endl
                      << "\tthe flight departure time is not valid, it is ignored automatically!" << std::endl;
        }
            // Verify flight depart airport, if it is empty, then pop up an error message.
        else if (pAirportOrigin == nullptr) {
            std::cerr << std::endl << "[ERROR] at: " << sFlightPath << std::endl
                      << "\tat: " << i << "th flight" << std::endl
                      << "\tthe flight's origin airport is unknown, it is ignored automatically!" << std::endl;
        }
            // Verify flight destination airport, if it is empty, then pop up an error message.
        else if (pAirportDestination == nullptr) {
            std::cerr << std::endl << "[ERROR] at: " << sFlightPath << std::endl
                      << "\tat: " << i << "th flight" << std::endl
                      << "\tthe flight's destination is unknown, it is ignored automatically!" << std::endl;
        }
            // Verify whether the departure airport is the same as the destination airport or not, if yes then pop up an error message.
        else if (*pAirportOrigin == *pAirportDestination) {
            std::cerr << std::endl << "[ERROR] at: " << sFlightPath << std::endl
                      << "\tat: " << i << "th flight" << std::endl
                      << "\tthe flight's destination is identical with the departure, it is ignored automatically!"
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
                WayPoint *pWayPoint = pNetwork->findWayPointByCode(root.get<String>(sPrefixedPath + ".Code"));
                // Verify the wayPoint, if it is empty, then pop up an error message.
                if (pWayPoint == nullptr) {
                    std::cerr << std::endl << "[ERROR] at: " << sFlightPath << std::endl
                              << "\tat: " << i << "th pFlight" << std::endl
                              << "\tat: " << iIndexPoint << "th point of route" << std::endl
                              << "\tthe code of wayPoint in pFlight's route is unknown, it is ignored automatically!"
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
                if (pFlight->selfCheck() && !contains(pNetwork->getFlightsList(), pFlight)) {
                    IntVector feasibleList = findFeasibleLevels(iLevel);
                    pFlight->setSpFeasibleLevelSet(feasibleList);
                    pNetwork->addNewFlight(pFlight);
                }
            }
        }
    }
    std::cout << "OK" << std::endl
              << "\tFlights file data:" << std::endl
              << "\tFlights: " << nbFlights << std::endl
              << "\tValid Flights: " << pNetwork->getNbFlights() << std::endl;
}

LevelVector Input::findFeasibleLevels(Level iDefaultLevel) {
    LevelVector feasibleList;
    feasibleList.push_back(iDefaultLevel);
#ifdef NRVSM
    if (iDefaultLevel < 290){
#else
    if (iDefaultLevel < 410) {
#endif
        auto position = LevelIFRB.end();
        switch (iDefaultLevel % 4) {
            // in IFR B group
            case 0:
                position = std::find(LevelIFRB.begin(), LevelIFRB.end(), iDefaultLevel);
                if (position== LevelIFRB.begin()) {
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position + 2));
                } else {
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position - 1));
                }
                break;
                // in VFR B group
            case 1:
                position = std::find(LevelVFRB.begin(), LevelVFRB.end(), iDefaultLevel);
                if (position == LevelVFRB.begin()) {
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position + 2));
                } else if (position == LevelVFRB.end() - 1) {
                    feasibleList.push_back(*(position - 1));
                    feasibleList.push_back(*(position - 2));
                } else {
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position - 1));
                }
                break;
                // in IFR A group
            case 2:
                position = std::find(LevelIFRA.begin(), LevelIFRA.end(), iDefaultLevel);
                if (position == LevelIFRA.begin()) {
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position + 2));
                } else {
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position - 1));
                }
                break;
                // in VFR A group
            default:
                position = std::find(LevelVFRA.begin(), LevelVFRA.end(), iDefaultLevel);
                if (position == LevelVFRA.begin()) {
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position + 2));
                } else if (position== LevelVFRA.end() - 1) {
                    feasibleList.push_back(*(position - 1));
                    feasibleList.push_back(*(position - 2));
                } else {
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position - 1));
                }
                break;
        }
    } else if (iDefaultLevel % 40 == 10) {
        // in IFR A group
        auto position = std::find(LevelIFRA.begin(), LevelIFRA.end(), iDefaultLevel);
        if (position == LevelIFRA.end() - 1) {
            feasibleList.push_back(*(position - 1));
            feasibleList.push_back(*(position - 2));
        } else {
            feasibleList.push_back(*(position + 1));
            feasibleList.push_back(*(position - 1));
        }
    } else {
        // in IFR B group
        auto position = std::find(LevelIFRB.begin(), LevelIFRB.end(), iDefaultLevel);
        if (position == LevelIFRB.end() - 1) {
            feasibleList.push_back(*(position - 1));
            feasibleList.push_back(*(position - 2));
        } else {
            feasibleList.push_back(*(position + 1));
            feasibleList.push_back(*(position - 1));
        }
    }
    return feasibleList;
}