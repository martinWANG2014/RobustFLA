//
// Created by martinwang on 27/04/17.
//

#include "../include/Network.h"

void Network::InitFlightLevelsList() {
    std::cout << "[INFO] Initialize Processing Flight Level..." << std::flush;
    FlightLevelMap levelMap;
    for(auto &&flight: vpFlightsList){
        for(auto && level: flight->getFeasibleLevelsList()){
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
    std::cout << "\tFlightLevel: NbFeasible, NbPreferred" << std::endl;
    for(auto item=levelsList.begin(); item!=levelsList.end();item++){
        std::cout << "\t"<< (*item).first << ": " << (*item).second.first << "," <<(*item).second.second << std::endl;
    }

    std::transform(levelsList.begin(), levelsList.end(), std::back_inserter(viFlightLevelsList), retrieve_key<Level, int, int>());
}
