//
// Created by martinwang on 27/04/17.
//

#ifndef NETWORK_H
#define NETWORK_H

#include "common.h"

class Network {
public:
    Flight *addNewFlight(Flight *pFlight);
    Airport *addNewAirport(Airport *pAirport);
    WayPoint *addNewWayPoint(WayPoint *pWayPoint);
    int getNbAirports() const ;
    int getNbWayPoints() const ;
    int getNbFlights() const ;
    Airport *findAirportByCode(String sCode);
    WayPoint *findWayPointByCode(String sCode);
    void initFeasibleLevelList();
    void initATMFlightLevelList();
    void initFlightsInLevel();
    FlightPVector getFlightsAtL(int iLevel);
    void displayFlightsInfo();
    void displayFlightLevelsInfo();
    void displayLevelFlightsMap();
    FlightVector getProcessingFlightsAtLevel(int iProcessingLevel);
    int getCurrentProcessingSize();
    double getPenalCostOfFlightAtLevel(int iIndexFlight, int iProcessingLevel);
    double **getPorbability();
    double **getDelay();
    double *getPi();

    const IntVector &getProcessingLevelOrder() const;

    void initProcessingLevelOrder();
    void updateFlightsInLevel(int iLevel);
private:
    FlightVector vpFlightsList;
    AirportVector vpAirportsList;
    WayPointVector vpWayPointsList;
    IntVector viATMFlightLevelsList;
    LevelFlightMap mvpLevelFlightMap;
    IntVector viProcessingLevelOrder;
    int getNbLevelPreferredAtLevel(int iIndex);
    bool compare(const int i, const int j);
};
#endif //NETWORK_H
