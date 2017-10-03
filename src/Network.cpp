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

void Network::SetSigma(const vdList &vdParameters, bool modeGenerateSigma) {
    if (vdParameters.size() != 6) {
        std::cerr << "the vdParameters list invalid!" << std::endl;
        abort();
    }
    double dSigma = getSigma3(vdParameters[0], vdParameters[1], vdParameters[2], vdParameters[3], vdParameters[4],
                              vdParameters[5]);

    uni_dist UniformDist(MIN_SIGMA, dSigma);
    // Will be used to obtain a seed for the random number engine
    std::random_device rd;
    std::mt19937 gen(rd());
    for (auto &&fi : vpFlightsList) {
        if (modeGenerateSigma) {
            double sigma = UniformDist(gen);
            fi->setSigmaPrime(sigma);
            fi->setSigma(getSigmaReal(vdParameters, sigma));
        } else {
            fi->setSigmaPrime(dSigma);
            fi->setSigma(getSigmaReal(vdParameters, dSigma));
        }
    }
}

void
Network::initialize(double dCoefPi, int iFeasibleSize, vdList vdParameter, bool modeGenerateSigma, bool modeDisplay) {
    // Initialize the coefficient Pi for all flight in the network.
    this->InitCoefPi(dCoefPi);
    // Initialize the feasible flight levels for all flights in the network.
    this->InitFeasibleList(iFeasibleSize);
    // Initialize  the flight levels list of the network.
    this->InitFlightLevelsList(modeDisplay);
    // Initialize the sigma for the random departure time.

    this->SetSigma(vdParameter, modeGenerateSigma);

}


