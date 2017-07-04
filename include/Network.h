//
// Created by martinwang on 27/04/17.
//

#ifndef NETWORK_H
#define NETWORK_H

#include "Define.h"
#include "Flight.h"

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
    const FlightVector &getFlightList(void) const;
    const IntVector &getLevelList();
    void setLevelList(IntVector &level_list);

private:

    FlightVector vpFlightsList;
    AirportVector vpAirportsList;
    WayPointVector vpWayPointsList;
    IntVector viLevelsList;
};
#endif //NETWORK_H
