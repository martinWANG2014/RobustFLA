//
// Created by martinwang on 27/04/17.
//

#include "../include/Input.h"
#include "../include/NodePoint.h"

Input::Input(const String &sAirportPath, const String &sWayPointPath, const String &sFlightPath, const String &sConfigPath)
        :sAirportPath(sAirportPath), sWayPointPath(sWayPointPath), sFlightPath(sFlightPath), sConfigPath(sConfigPath){}

Input::~Input() {}

void Input::parseFlights(Network *pNetwork) {
    using boost::property_tree::ptree;
    using boost::property_tree::read_json;

    std::cout << "Parsing flights file... " << std::flush;
    if (!exists(sFlightPath)){
        std::cerr << "not exist " << sFlightPath << std::endl;
        abort();
    }
    ptree root;
    read_json(sFlightPath, root);

    int nbFlights = root.get<int>("FN");
    for(int i  = 0; i < nbFlights; i++){
        String sPrefixed(std::to_string(i));
        String sCode(root.get<String>(sPrefixed + ".FCode"));
        Airport *pAirportOrigin = pNetwork->findAirportByCode(root.get<String>(sPrefixed + ".Origin"));
        Airport *pAirportDestination = pNetwork->findAirportByCode(root.get<String>(sPrefixed + ".Dest"));
        Time iDepartureTime = root.get<Time>(sPrefixed + ".DTime", -1);
        if (sCode.empty()) {
            std::cerr << "at: "<< sFlightPath << std::endl
                      << "at: " << i << "th flight" << std::endl
                      << "the flight code is unknown, it is ignored automatically!" << std::endl;
        }
        else if(iDepartureTime < 0) {
            std::cerr << "at: "<< sFlightPath << std::endl
                      << "at: " << i << "th flight" << std::endl
                      << "the flight departure time is not valid, it is ignored automatically!" << std::endl;
        }
        else if(pAirportOrigin == nullptr) {
            std::cerr << "at: "<< sFlightPath << std::endl
                      << "at: " << i << "th flight" << std::endl
                      << "the flight's origin airport is unknown, it is ignored automatically!" << std::endl;
        }
        else if(pAirportDestination == nullptr) {
            std::cerr << "at: "<< sFlightPath << std::endl
                      << "at: " << i << "th flight" << std::endl
                      << "the flight's destination is unknown, it is ignored automatically!" << std::endl;
        }
        else if(*pAirportOrigin == *pAirportDestination){
            std::cerr << "at: "<< sFlightPath << std::endl
                      << "at: " << i << "th flight" << std::endl
                      << "the flight's destination is identical with the departure, it is ignored automatically!" << std::endl;
        }
        else{
            FLevel *pLevel = new FLevel(root.get<int>(sPrefixed + ".FLevel"));
            Route *pDefaultRoute = new Route();
            int nbPoints = root.get<int>(sPrefixed + ".PointList.PLN");
            bool bValid = true;
            int iIndexPoint = 0;
            while(bValid && iIndexPoint < nbPoints){
                String sPrefixedPath(sPrefixed + ".PointList." + std::to_string(iIndexPoint));
                WayPoint *pWayPoint = pNetwork->findWayPointByCode(root.get<String>(sPrefixedPath + ".Code"));
                if (pWayPoint == nullptr){
                    std::cerr << "at: "<< sFlightPath << std::endl
                              << "at: " << i << "th flight" << std::endl
                              << "at: " << iIndexPoint << "th point of route" << std::endl
                              << "the code of wayPoint in flight's route is unknown, it is ignored automatically!" << std::endl;
                    bValid = false;
                }
                else{
                    Point *point = new Point(pWayPoint, root.get<Time>(sPrefixedPath + ".Time"));
//                    std::cout << *point << std::endl;
                    pDefaultRoute->addNewPoint(point);
                }
                iIndexPoint++;
            }
            if (bValid) {
//                std::cout << *pDefaultRoute;
                Flight *flight = new Flight(sCode, pAirportOrigin, pAirportDestination, iDepartureTime, pLevel, pDefaultRoute);
//                std::cout << *flight;
                pNetwork->addNewFlight(flight);
            }
        }
    }
    std::cout << "OK" << std::endl
              << "Flights file data:" << std::endl
              << "Flights: " << nbFlights << std::endl
              << "Valid Flights: " << pNetwork->getNbFlights() << std::endl;
}

void Input::parseAirports(Network *pNetwork) {
    using boost::property_tree::ptree;
    using boost::property_tree::read_json;

    std::cout << "Parsing airports file... " << std::flush;
    if (!exists(sAirportPath)){
        std::cerr << "not exist " << sAirportPath << std::endl;
        abort();
    }

    ptree root;
    read_json(sAirportPath, root);

    int nbAirports = root.get<int>("AN");
    for(int i  = 0; i < nbAirports; i++) {
        String sPrefixed(std::to_string(i));
        String sCode = root.get<String>(sPrefixed + ".ICAO");
        if (sCode.empty()) {
            std::cerr << "at: " << sAirportPath << std::endl
                      << "at: " << i << "th airport" << std::endl
                      << "the airport icao code is unknown, it is ignored automatically!" << std::endl;
        } else {
            Airport * airport = new Airport(sCode,
                                            root.get<String>(sPrefixed + ".Name", ""),
                                            root.get<double>(sPrefixed + ".Lat", 0.0),
                                            root.get<double>(sPrefixed + ".Lng", 0.0));
            //std::cout << *airport;
            pNetwork->addNewAirport(airport->clone());
        }
    }
    std::cout << "OK" << std::endl
              << "Airports file data:" << std::endl
              << "Airports: " << nbAirports << std::endl
              << "Valid Airports: " << pNetwork->getNbAirports() << std::endl;
}

void Input::parseWayPoints(Network *pNetwork) {
    using boost::property_tree::ptree;
    using boost::property_tree::read_json;

    std::cout << "Parsing wayPoints file... " << std::flush;
    if (!exists(sWayPointPath)){
        std::cerr << "not exist " << sWayPointPath << std::endl;
        abort();
    }
    ptree root;
    read_json(sWayPointPath, root);

    int nbWayPoints = root.get<int>("BN");
    for(int i  = 0; i < nbWayPoints; i++){
        String sPrefixed(std::to_string(i));
        String sCode = root.get<String>(sPrefixed + ".Code");
        if (sCode.empty()) {
            std::cerr << "at: " << sWayPointPath << std::endl
                      << "at: " << i << "th wayPoint" << std::endl
                      << "the wayPoint's code is unknown, it is ignored automatically!" << std::endl;
        }
        else {
            WayPoint *wayPoint = new WayPoint(sCode,
                                              root.get<String>(sPrefixed  + ".Name", ""),
                                              root.get<double>(sPrefixed  + ".Lat", 0.0),
                                              root.get<double>(sPrefixed  + ".Lng", 0.0));
            //std::cout << *wayPoint;
            pNetwork->addNewWayPoint(wayPoint);
        }
    }
    std::cout << "OK" << std::endl
              << "Beacon file data:" << std::endl
              << "WayPoints: " << nbWayPoints << std::endl
              << "Valid WayPoints: " << pNetwork->getNbWayPoints() << std::endl;
}

void Input::parseConfiguration(Network *pNetwork) {
    using boost::property_tree::ptree;
    using boost::property_tree::read_json;

    std::cout << "Parsing configuration file... " << std::flush;
    if (!exists(sConfigPath)){
        std::cerr << "not exist " << sConfigPath << std::endl;
        abort();
    }
    ptree root;
    read_json(sConfigPath, root);
    Configuration *configuration = new Configuration(root.get<int>("NbPeriods"),
                                                     root.get<Time>("PeriodUnit"),
                                                     root.get<int>("NbFeasibleLevels"));
    pNetwork->setConfiguration(configuration);
    std::cout << "OK" << std::endl
              << "Configuration file data:" << std::endl
              << "NbPeriods: " << pNetwork->getNbPeriods() << std::endl
              << "PeriodUint: " << pNetwork->getPeriodUnit() << std::endl
              << "NbFeasibleLevels: " << pNetwork->getNbFeasibleLevels() << std::endl;
}

void Input::initNetwork(Network *pNetwork) {
    parseAirports(pNetwork);
    parseWayPoints(pNetwork);
    parseFlights(pNetwork);
    parseConfiguration(pNetwork);
}



