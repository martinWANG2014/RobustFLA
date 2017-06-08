//
// Created by martinwang on 27/04/17.
//

#ifndef ROBUSTFLA_NETWORK_H
#define ROBUSTFLA_NETWORK_H

#include "predefine.h"
#include "Flight.h"
#include "Configuration.h"
class Network {
public:
    static Time iPeriodTime;
    static Time iNbPeriods;
    Flight *addNewFlight(Flight *pFlight);
    Airport *addNewAirport(Airport *pAirport);
    WayPoint *addNewWayPoint(WayPoint *pWayPoint);
    int getNbAirports() const ;
    int getNbWayPoints() const ;
    int getNbFlights() const ;
    Airport *findAirportByCode(String sCode);
    WayPoint *findWayPointByCode(String sCode);
    int getNbFeasibleLevels() const ;
    int getNbPeriods()  const ;
    Time getPeriodUnit() const ;
    void setConfiguration(Configuration *pConfiguration);
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
    IntVector &getProcessingLevelOrder() const;
    void initProcessingLevelOrder();
    void updateFlightsInLevel(int iLevel);
private:
    FlightVector vpFlightsList;
    AirportVector vpAirportsList;
    WayPointVector vpWayPointsList;
    Configuration *pConfiguration;
    IntVector viATMFlightLevelsList;
    LevelFlightMap mvpLevelFlightMap;
    IntVector viProcessingLevelOrder;
    int getNbLevelPreferredAtLevel(int iIndex);
    bool compare(const int i, const int j);
};
#endif //ROBUSTFLA_NETWORK_H
