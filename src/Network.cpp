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
    return vpAirportsList.size();
}

int Network::getNbWayPoints() const {
    return vpWayPointsList.size();
}

int Network::getNbFlights() const {
    return vpFlightsList.size();
}

Airport *Network::findAirportByCode(String sCode) {
    return findByCode<Airport>(vpAirportsList, sCode);
}

WayPoint *Network::findWayPointByCode(String sCode) {
    return findByCode<WayPoint>(vpWayPointsList, sCode);
}

void Network::initFeasibleLevelList() {
    for(auto iterator = vpFlightsList.begin(); iterator != vpFlightsList.end(); iterator++){
        (*iterator)->initFeasibleLevelList();
    }
}

void Network::displayFlightsInfo() {
    for(auto &&item : vpFlightsList){
        std::cout << *item;
    }
}

void Network::initATMFlightLevelList() {
    for(auto &&item : vpFlightsList) {
        for(auto &&level : item->getFeasibleLevelList()){
            if (!contains<int>(viATMFlightLevelsList, level->getFlightLevel())){
                viATMFlightLevelsList.push_back(level->getFlightLevel());
            }
        }
    }
}

void Network::displayFlightLevelsInfo() {
    std::cout << "All Levels that are used in this network: ";
    for(auto &&item : viATMFlightLevelsList){
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

void Network::initFlightsInLevel() {
    for(auto &&flight: vpFlightsList){
        for(int i = 0; i < flight->getNbLevels(); i++){
            if (mvpLevelFlightMap.find(flight->getLevelAtI(i)) == mvpLevelFlightMap.end()) {
                mvpLevelFlightMap[flight->getLevelAtI(i)] = FlightPVector();
            }
            mvpLevelFlightMap[flight->getLevelAtI(i)].push_back(std::make_pair(flight, i));
        }
    }
}

FlightPVector Network::getFlightsAtL(int iLevel){
    if (mvpLevelFlightMap.find(iLevel) != mvpLevelFlightMap.end()){
        return mvpLevelFlightMap[iLevel];
    }else{
        std::cerr << "at: FlightVector Network::getFlightsAtL(int iLevel)" << std::endl
                  << "the level not valid!" << std::endl;
    }
    return FlightPVector();
}

void Network::displayLevelFlightsMap() {
    for(auto &&level:mvpLevelFlightMap){
        std::cout << "level: " << level.first
                  << " flights: ";
        for(auto &&flight:level.second){
            std::cout << "(flightCode: " << flight.first->getCode()
                      << " IndexFeasible:" << flight.second << ") ";
        }
        std::cout << std::endl;
    }
}

FlightVector Network::getProcessingFlightsAtLevel(int iProcessingLevels) {
    return FlightVector();
}

int Network::getCurrentProcessingSize() {
    return 0;
}

double Network::getPenalCostOfFlightAtLevel(int iIndexFlight, int iProcessingLevel) {
    return 0;
}

double **Network::getPorbability() {
    return nullptr;
}

double **Network::getDelay() {
    return nullptr;
}

double *Network::getPi() {
    return nullptr;
}

bool Network::compare(const int i, const int j)
{
    return getNbLevelPreferredAtLevel(i) > getNbLevelPreferredAtLevel(j);
}

void Network::initProcessingLevelOrder() {
    for(auto &&element: mvpLevelFlightMap){
        viProcessingLevelOrder.push_back(element.first);
    }
    std::sort(viProcessingLevelOrder.begin(), viProcessingLevelOrder.end(), compare);
}

int Network::getNbLevelPreferredAtLevel(int iIndex) {
    int iCount = 0;
    for(auto &&element: mvpLevelFlightMap[iIndex]){
        if (element.second == 0){
            iCount++;
        }
    }
    return iCount;
}

void Network::updateFlightsInLevel(int iLevel) {

}

const IntVector &Network::getProcessingLevelOrder() const {
    return viProcessingLevelOrder;
}
