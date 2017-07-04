//
// Created by martinwang on 27/04/17.
//

#include "../include/Network.h"

Flight *Network::addNewFlight(Flight *pFlight) {
    vpFlightsList.push_back(pFlight);
    return pFlight;
}

Airport *Network::addNewAirport(Airport *pAirport) {
    vpAirportsList.push_back(pAirport);
    return pAirport;
}

WayPoint *Network::addNewWayPoint(WayPoint *pWayPoint) {
    vpWayPointsList.push_back(pWayPoint);
    return  pWayPoint;
}

int Network::getNbAirports() const {
    return (int)vpAirportsList.size();
}

int Network::getNbWayPoints() const {
    return (int)vpWayPointsList.size();
}

int Network::getNbFlights() const {
    return (int)vpFlightsList.size();
}

Airport *Network::findAirportByCode(String sCode) {
    return findByCode<Airport>(vpAirportsList, sCode);
}

WayPoint *Network::findWayPointByCode(String sCode) {
    return findByCode<WayPoint>(vpWayPointsList, sCode);
}


const FlightVector &Network::getFlightList(void) const {
    return vpFlightsList;
}


const IntVector &Network::getLevelList() {
    return viLevelsList;
}


void Network::setLevelList(IntVector &level_list) {
    Network::viLevelsList = level_list;
}
