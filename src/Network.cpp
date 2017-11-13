//
// Created by martinwang on 27/04/17.
//


#include "../include/Network.h"

double Network::PERIOD_LENGTH = 30;

Network::Network() : vpFlightsList(), vpAirportsList(), vpWayPointsList(), viFlightLevelsList() {}

Network::~Network() {
    vpFlightsList.clear();
    vpAirportsList.clear();
    vpWayPointsList.clear();
    viFlightLevelsList.clear();
}

void Network::addNewFlight(Flight *pFlight) {
    vpFlightsList.push_back(pFlight);
}

void Network::addNewAirport(Airport *pAirport) {
    vpAirportsList.push_back(pAirport);
}

void Network::addNewWayPoint(WayPoint *pWayPoint) {
    vpWayPointsList.push_back(pWayPoint);
}

int Network::getNbAirports() const {
    return (int) vpAirportsList.size();
}

int Network::getNbWayPoints() const {
    return (int) vpWayPointsList.size();
}

int Network::getNbFlights() const {
    return (int) vpFlightsList.size();
}

void Network::InitFlightLevelsList(bool modeDisplay) {
    std::cout << "[INFO] Initialize Processing Flight Level..." << std::flush;
    FlightLevelMap levelMap;
    for(auto &&flight: vpFlightsList){
        for (auto &&level: flight->getFeasibleLevelList()) {
            auto it = levelMap.find(level);
            if (level == flight->getDefaultLevel()){
                if (it != levelMap.end()){
                    levelMap[level].first = levelMap[level].first+1;
                    levelMap[level].second = levelMap[level].second+1;
                }else{
                    levelMap.insert(std::make_pair(level, std::make_pair(1, 1)));
                }
            }else{
                if (it != levelMap.end()){
                    levelMap[level].first = levelMap[level].first+1;
                }else{
                    levelMap.insert(std::make_pair(level, std::make_pair(1, 0)));
                }
            }

        }
    }
    LevelNumberList levelsList(levelMap.begin(), levelMap.end());
    std::sort(levelsList.begin(), levelsList.end(), greater_second<Level, int, int>());
    std::cout << "OK" << std::endl;
    if (modeDisplay) {
        std::cout << "\tFlightLevel: NbFeasible, NbPreferred" << std::endl;
        for (auto item = levelsList.begin(); item != levelsList.end(); item++) {
            std::cout << "\t" << (*item).first << ": " << (*item).second.first << "," << (*item).second.second
                      << std::endl;
        }
    }
    viFlightLevelsList.clear();
    std::transform(levelsList.begin(), levelsList.end(), std::back_inserter(viFlightLevelsList), retrieve_key<Level, int, int>());

    std::cout << "\tFlight Level size: " << getNbLevels() << std::endl;
}

int Network::getNbLevels() const {
    return (int) viFlightLevelsList.size();
}

Airport *Network::findAirportByCode(String sCode) {
    return findByCode<Airport>(vpAirportsList, sCode);
}

WayPoint *Network::findWayPointByCode(String sCode) {
    return findByCode<WayPoint>(vpWayPointsList, sCode);
}

const FlightVector &Network::getFlightsList() const {
    return vpFlightsList;
}

const LevelVector &Network::getFlightLevelsList() const {
    return viFlightLevelsList;

}

const AirportVector &Network::getAirportsList() const {
    return vpAirportsList;
}

const WayPointVector &Network::getWayPointsList() const {
    return vpWayPointsList;
}

void Network::InitFeasibleList(int iSize) {
    for (auto &&fi: vpFlightsList) {
        fi->setFeasibleLevelList(findFeasibleLevels(fi->getDefaultLevel(), iSize));
    }
}

void Network::InitCoefPi(double dCoefPi) {
    for (auto &&fi: vpFlightsList) {
        fi->setCoefPi(dCoefPi);
    }
}

void Network::initialize(double dCoefPi, int iFeasibleSize, bool modeDisplay) {
    // Initialize the coefficient Pi for all flight in the network.
    this->InitCoefPi(dCoefPi);
    // Initialize the feasible flight levels for all flights in the network.
    this->InitFeasibleList(iFeasibleSize);
    // Initialize  the flight levels list of the network.
    this->InitFlightLevelsList(modeDisplay);
}

Flight *Network::getFlightAtI(int indexI) const {
    return vpFlightsList[indexI];
}

String Network::generateFlightsSup(int percentileSup, Time offset) {
    int nbFlights = getNbFlights();
    int nbAdditionalFlights = (int) (nbFlights * percentileSup / 100.0);
    if (nbAdditionalFlights > 0) {
        viList flightIdList;
        for (int i = 0; i < nbFlights; i++) {
            flightIdList.push_back(i);
        }
        std::random_shuffle(flightIdList.begin(), flightIdList.end());
        for (int i = 0; i < nbAdditionalFlights; i++) {
            int index = flightIdList[i];
            vpFlightsList[index]->extendRoute(vpFlightsList[index]->getDuration() + offset);
        }
        for (auto &&fj: vpFlightsList) {
            fj->initRouteTimeList();
        }
    }
    return writeFlightsSup(this, percentileSup);
}

String Network::writeFlightsSup(const Network *pNetwork, int percentileSup) {
    using boost::property_tree::ptree;
    using boost::property_tree::write_json;
    // Write the json file with boost property tree.
    ptree root;
    // get the target filename.
    std::stringstream outFilename;
    outFilename << "../data/flight_" << percentileSup << "_-.json";
    String filename = outFilename.str();
    //write the flight list in a json format.
    root.put("FN", pNetwork->getNbFlights());
    for (int i = 0; i < pNetwork->getNbFlights(); i++) {
        Flight *flight = pNetwork->getFlightAtI(i);
        ptree rootFlight;
        rootFlight.put("Origin", flight->getOrigAirportName());
        rootFlight.put("Dest", flight->getDestAirportName());
        rootFlight.put("FCode", flight->getCode());
        rootFlight.put("DTime", flight->getDepartureTime());
        rootFlight.put<int>("FLevel", flight->getDefaultLevel());
        ptree rootPointsList;
        for (int j = 0; j < flight->getRoute()->getPointListSize(); j++) {
            ptree point;
            point.put("Code", flight->getPointAtI(j)->getPointName());
            point.put<double>("Time", flight->getPointAtI(j)->getArrivingTime());
            rootPointsList.put_child(std::to_string(j), point);
        }
        rootPointsList.put<int>("PLN", flight->getNbPoints());
        rootFlight.put_child("PointList", rootPointsList);
        root.put_child(std::to_string(i), rootFlight);
    }
    write_json(filename, root);
    return filename;
}

