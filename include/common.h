//
// Created by chenghaowang on 23/05/17.
//

#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <random>
#include <algorithm>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "Node.h"
#include "NodePoint.h"
#include "FLevel.h"
#include "Flight.h"
#undef IL_STD
#define  IL_STD
#include <ilcplex/ilocplex.h>

typedef std::string String;
typedef int Time;
typedef std::vector<int> IntVector;
typedef std::uniform_real_distribution<double> uni_dist;
typedef std::normal_distribution<double> normal_dist;
typedef Node WayPoint, Airport;
typedef std::vector<Node *> WayPointVector, AirportVector;
typedef std::vector<Point*> PointVector;
typedef std::vector<FLevel *> FLevelVector;
typedef std::vector<std::tuple<Point*, Point*, double>> conflictMatrix;
typedef std::vector<Flight *> FlightVector;
typedef std::map<int, std::vector<std::pair<Flight *, int>>> LevelFlightMap;
typedef std::vector<std::pair<Flight *, int>> FlightPVector;
typedef std::map<Flight *, bool> FlightAssignmentMap;
#ifdef WIN64
#include <windows.h>
    struct ProcessTime {
        FILETIME tms_utime;
        FILETIME tms_stime;
        FILETIME tms_cutime;
        FILETIME tms_cstime;
    };
#else
#include <sys/times.h>
typedef struct tms ProcessTime;
#endif
#ifndef WIN64
clock_t startClock;
clock_t endClock;
#endif
const double k = 1.8520108;              // constant : 1(nmi)=k*1(km)
const double min_separation = 5.0;
const double PI = 3.141592653589793;
const double EARTH_RADIUS = 6378.16;
const double EPSILON = 1.0e-10; // precision of double variables
const int ITMAX = 1000;
const Time* PERIODUNIT = new Time(30);
const int* NBPERIODS = new int(96);
const int* LEVELSIZE = new int(3);
static IntVector LevelIFRA{10, 30, 50, 70, 90, 110, 130, 150, 170, 190, 210, 230, 250, 270, 290, 330, 370, 410, 450, 490};
static IntVector LevelIFRB{20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 310, 350, 390, 430, 470, 510};
static IntVector LevelVFRA{35, 55, 75, 95, 115, 135, 155, 175, 195};
static IntVector LevelVFRB{45, 65, 85, 105, 125, 145, 165, 185};

std::default_random_engine generator;
uni_dist rand_2(0.0, 2.0);
uni_dist rand_3(0.0, 3.0);
uni_dist rand_5(0.0, 5.0);
uni_dist rand_100(0.0, 100.0);

template<typename T> T* findByCode(std::vector<T*> vpObjectsVector, const String &sCode){
    if (vpObjectsVector.size() < 1) return nullptr;
    for (auto &&item : vpObjectsVector) {
        if ( item->getCode() == sCode) {
            return item;
        }
    }
    return nullptr;
}

template<typename T> bool contains(std::vector<T*> vpObjectsVector, T* pObject){
    if (vpObjectsVector.size() < 1) return false;
    for (auto &&item : vpObjectsVector) {
        if (*item == *pObject) {
            return true;
        }
    }
    return false;
}

template<typename T> bool contains(std::vector<T> vpObjectsVector, T object){
    if (vpObjectsVector.size() < 1) return false;
    for (auto &&item : vpObjectsVector) {
        if (item == object) {
            return true;
        }
    }
    return false;
}

IntVector findFeasibleLevels(int iDefaultLevel){
    IntVector feasibleList;
    //auto position;
    if (iDefaultLevel < 290){
        auto position = LevelIFRA.end();
        switch (iDefaultLevel % 4){
            // in IFR B group
            case 0:
                position = std::find(LevelIFRB.begin(), LevelIFRB.end(), iDefaultLevel);
                if(position == LevelIFRB.begin()){
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position + 2));
                }else{
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position - 1));
                }
                break;
                // in VFR B group
            case 1:
                position = std::find(LevelVFRB.begin(), LevelVFRB.end(), iDefaultLevel);
                if(position == LevelVFRB.begin()){
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position + 2));
                }else if(position == LevelVFRB.end() - 1){
                    feasibleList.push_back(*(position - 1));
                    feasibleList.push_back(*(position - 2));
                }else{
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position - 1));
                }
                break;
                // in IFR A group
            case 2:
                position = std::find(LevelIFRA.begin(), LevelIFRA.end(), iDefaultLevel);
                if(position == LevelIFRA.begin()){
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position + 2));
                }else{
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position - 1));
                }
                break;
                // in VFR A group
            case 3:
                position = std::find(LevelVFRA.begin(), LevelVFRA.end(), iDefaultLevel);
                if(position == LevelVFRA.begin()){
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position + 2));
                }else if(position == LevelVFRA.end() - 1){
                    feasibleList.push_back(*(position - 1));
                    feasibleList.push_back(*(position - 2));
                }else{
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position - 1));
                }
                break;
        }
    }
    else if (iDefaultLevel % 40 == 10){
        // in IFR A group
        auto position = std::find(LevelIFRA.begin(), LevelIFRA.end(), iDefaultLevel);
        if(position == LevelIFRA.end() - 1){
            feasibleList.push_back(*(position - 1));
            feasibleList.push_back(*(position - 2));
        }else{
            feasibleList.push_back(*(position + 1));
            feasibleList.push_back(*(position - 1));
        }
    }else{
        // in IFR B group
        auto position = std::find(LevelIFRB.begin(), LevelIFRB.end(), iDefaultLevel);
        if(position == LevelIFRB.end() - 1){
            feasibleList.push_back(*(position - 1));
            feasibleList.push_back(*(position - 2));
        }else{
            feasibleList.push_back(*(position + 1));
            feasibleList.push_back(*(position - 1));
        }
    }
    return feasibleList;
}

/**
 * Generate a random departure time which follows the piecewise uniform laws.
 * @param dDepartureTime the initial departure time.
 * @return the random departure time.
 */
double randDepartureTimeInterval(double dDepartureTime){
    normal_dist NormalDistribution1(dDepartureTime, 3/3);
    normal_dist NormalDistribution2(dDepartureTime, 2/3);
    double dOffset;
    return dDepartureTime + (rand_100(generator) > 50)
           ? (dOffset = NormalDistribution2(generator), dOffset < 0 ? -dOffset : dOffset)
           : (dOffset = NormalDistribution1(generator), dOffset > 0 ? -dOffset : dOffset);
}

/**
 * Generate a random departure time which follows the piecewise normal laws.
 * @param dDepartureTime the initial departure time.
 * @return the random departure time.
 */
double randDepartureTimeNormalDist(double dDepartureTime){
    double dSeed;
    double dSeed_2;
    return dDepartureTime + (dSeed = rand_100(generator), dSeed_2 = rand_100(generator), dSeed < 83)
           ? (dSeed < 63
              // n1~[0, 19)
              ? (dSeed < 19
                 ? (dSeed_2 < 30 ? (rand_3(generator) + (dSeed_2 < 10
                                                         // n2~[0, 10)
                                                         ? -15
                                                         // n2~[10, 30)
                                                         : -12))
                                 : (dSeed_2 < 60
                                    // n2~[30, 60)                                                                                       // n2~(30, 60]
                                    ? (rand_2(generator) - 9)
                                    // n2~[60, 100)
                                    : (rand_3(generator) - 7)))
                 // n1~[19, 63)
                 : (rand_3(generator) + (dSeed_2 < 75
                                         ? (dSeed_2 < 25
                                            // n2~[0, 25)
                                            ? -4
                                            // n2~[25, 75)
                                            : -1)
                                         // n2~[75, 100)
                                         : 2)))
              // n1~[63, 83)
              : (dSeed_2 < 70
                 ? (dSeed_2 < 40
                    // n2~[0, 40)
                    ? (rand_3(generator) + 5)
                    // n2~[40, 70)
                    : (rand_2(generator) + 8))
                 : (rand_3(generator) + (dSeed_2 > 90
                                         // n2~[70, 90)
                                         ? 10
                                         // n2~[90, 100)
                                         : 13))))
           // n1~[83, 93)
           : (rand_5(generator) + (dSeed < 93
                                   ? (dSeed_2 < 80
                                      ? (dSeed_2 < 50
                                         // n2~[0, 50)
                                         ? 16
                                         // n2~[50, 80)
                                         : 21)
                                      // n2~[80, 100)
                                      : 26)
                                   // n1~[93, 100)
                                   : (dSeed_2 < 75
                                      ? (dSeed_2 < 55
                                         ? (dSeed_2 < 30
                                            // n2~[0, 30)
                                            ? 31
                                            // n2~[30, 55)
                                            : 36)
                                         // n2~[55, 75)
                                         : 41)
                                      : (dSeed_2 < 95
                                         ? (dSeed_2 < 85
                                            // n2~[75, 85)
                                            ? 46
                                            // n2~[85, 95)
                                            : 51)
                                         // n2~[95, 100)
                                         : 56)
                                   )));
}

#endif //COMMON_H
