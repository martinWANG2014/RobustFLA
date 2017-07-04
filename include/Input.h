//
// Created by martinwang on 27/04/17.
//

#ifndef INPUT_H
#define INPUT_H

#include "Define.h"
#include "Flight.h"
#include "Network.h"

class Input {
public:
    explicit Input(const String &sAirportPath, const String &sWayPointPath, const String &sFlightPath, const String &sConfigPath) :sAirportPath(sAirportPath), sWayPointPath(sWayPointPath), sFlightPath(sFlightPath), sConfigPath(sConfigPath){}

    ~Input(){}
    /**
     * initialize the air management network
     * and the feasible flight level list for each flight.
     */
    void initNetwork(Network *pNetwork) {
        parseAirports(pNetwork);
        parseWayPoints(pNetwork);
        parseFlights(pNetwork);
        parseConfiguration(pNetwork);
    }

private:
    String sAirportPath;
    String sWayPointPath;
    String sFlightPath;
    String sConfigPath;
    void parseFlights(Network *pNetwork) {
        using boost::property_tree::ptree;
        using boost::property_tree::read_json;

        LevelSet level_temp;
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
                Level iLevel = root.get<int>(sPrefixed + ".FLevel");
                Route *pRoute = new Route(iLevel, pAirportOrigin, iDepartureTime);
                int nbPoints = root.get<int>(sPrefixed + ".PointList.PLN");
                bool bValid = true;
                int iIndexPoint = 0;
                while(bValid && iIndexPoint < nbPoints){
                    String sPrefixedPath(sPrefixed + ".PointList." + std::to_string(iIndexPoint));
                    WayPoint *pWayPoint = pNetwork->findWayPointByCode(root.get<String>(sPrefixedPath + ".Code"));
                    if (pWayPoint == nullptr){
                        std::cerr << "at: "<< sFlightPath << std::endl
                                  << "at: " << i << "th pFlight" << std::endl
                                  << "at: " << iIndexPoint << "th point of route" << std::endl
                                  << "the code of wayPoint in pFlight's route is unknown, it is ignored automatically!" << std::endl;
                        bValid = false;
                    }
                    else{
                        Point *point = new Point(pWayPoint, root.get<Time>(sPrefixedPath + ".Time"));
                        pRoute->addNewPoint(point);
                    }
                    iIndexPoint++;
                }
                if(bValid){
                    Flight *pFlight = new Flight(sCode, pAirportOrigin, pAirportDestination, iDepartureTime, pRoute);
                    pNetwork->addNewFlight(pFlight);
                }
            }
        }
        int* pNbFlights=new int[level_temp.size()];
        FlightVector vpFlightList=pNetwork->getFlightList();
        int indexL=0;
        for (LevelSet::iterator itA = level_temp.begin() ; itA != level_temp.end() ; itA++,indexL++){
            Level iLevel=(*itA);
            pNbFlights[indexL]=0;
            for (int i=0;i< (int)vpFlightList.size();i++){
                Flight *pFlight=vpFlightList[i];
                Level iLevelDefault = pFlight->getDefaultLevel();
                if (iLevel==iLevelDefault){
                    pNbFlights[indexL]=pNbFlights[indexL]+1;
                }
            }
        }
        IntVector temp_list2;
        IntVector temp_list1;
        while (temp_list1.size()!=level_temp.size()){
            int k=maxLevel(pNbFlights,temp_list1,level_temp);
            if (k!=-1){
                temp_list1.push_back(k);
                indexL=0;
                for (LevelSet::iterator itB=level_temp.begin();itB!=level_temp.end();itB++,indexL++){
                    if (indexL==k){
                        int m=(*itB);
                        temp_list2.push_back(m);
                    }
                }
            }
        }
        pNetwork->setLevelList(temp_list2);
        std::cout << "OK" << std::endl
                  << "Flights file data:" << std::endl
                  << "Flights: " << nbFlights << std::endl
                  << "Valid Flights: " << pNetwork->getNbFlights() << std::endl;
    }
    void parseAirports(Network *pNetwork) {
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
                pNetwork->addNewAirport(airport);
            }
        }
        std::cout << "OK" << std::endl
                  << "Airports file data:" << std::endl
                  << "Airports: " << nbAirports << std::endl
                  << "Valid Airports: " << pNetwork->getNbAirports() << std::endl;
    }
    void parseWayPoints(Network *pNetwork) {
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
    void parseConfiguration(Network *pNetwork) {
        using boost::property_tree::ptree;
        using boost::property_tree::read_json;

        std::cout << "Parsing configuration file... " << std::flush;
        if (!exists(sConfigPath)){
            std::cerr << "not exist " << sConfigPath << std::endl;
            abort();
        }
        ptree root;
        read_json(sConfigPath, root);
        int iNbPeriods = root.get<int>("NbPeriods");
        Time iPeriodUnit = root.get<Time>("PeriodUnit");
        int iNbFeasibleLevels = root.get<int>("NbFeasibleLevels");
        if (iNbPeriods != *NBPERIODS){
            delete(NBPERIODS);
            NBPERIODS = new int(iNbPeriods);
        }
        if (iNbFeasibleLevels != *LEVELSIZE){
            delete(LEVELSIZE);
            LEVELSIZE = new int(iNbFeasibleLevels);
        }
        if (iPeriodUnit != *PERIODUNIT){
            delete(PERIODUNIT);
            PERIODUNIT = new int(iPeriodUnit);
        }
        std::cout << "OK" << std::endl
                  << "Configuration file data:" << std::endl
                  << "NbPeriods: " << *NBPERIODS << std::endl
                  << "PeriodUint: " << *PERIODUNIT << std::endl
                  << "NbFeasibleLevels: " << *LEVELSIZE << std::endl;
    }
    inline bool exists (const String& name) {
        std::ifstream f(name.c_str());
        return f.good();
    }
    int maxLevel(int *pNbFlights, IntVector temp_list, LevelSet level) {
        int maxV=-1;
        int maxIndice=-1;
        int index=0;
        for (LevelSet::iterator itB=level.begin();itB!=level.end();itB++,index++){
            if (pNbFlights[index] > maxV && !contains(temp_list, index)){
                maxV=pNbFlights[index];
                maxIndice=index;
            }
        }
        if (maxIndice!=-1){
            return maxIndice;
        }
        return -1;
    }
};

#endif //ROBUSTFLA_INPUT_H
