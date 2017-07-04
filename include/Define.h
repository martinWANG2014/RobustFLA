//
// Created by chenghaowang on 23/05/17.
//

#ifndef DEFINE_H
#define DEFINE_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <random>
#include <algorithm>
//#include <boost/python.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/math/special_functions/erf.hpp>
#undef IL_STD
#define  IL_STD
#include <ilcplex/ilocplex.h>

typedef std::string String;
typedef int Time;
typedef int Level;
typedef std::set<Level> LevelSet;
typedef std::map<Level, bool> LevelExamine;
typedef std::vector<int> IntVector;
typedef std::vector<double> DoubleVector;
typedef std::uniform_real_distribution<double> uni_dist;
typedef std::normal_distribution<double> normal_dist;
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
const double K = 1.8520108;              // constant : 1(nmi)=K*1(km)
const double MIN_SEPARATION_DISTANCE = 5.0;
const double PI = 3.141592653589793;
const double EARTH_RADIUS = 6378.16;
const double PRECISION = 1.0e-10; // precision of double variables
const int ITERATIONCOUNT = 1000;
const double  COEFPORBABILITY = 0.001;
const double CONFIDNECE = 0.95;
const double ALPHA=15;
const double BETA=60;
const double COEFPI = 5;
const double COEFPIJ = 1;//0.7
const double EPSILON = 0.05;
const Time* PERIODUNIT = new Time(30);
const int* NBPERIODS = new int(96);
const int* LEVELSIZE = new int(3);
#ifdef NRVSM
static IntVector LevelIFRA{10, 30, 50, 70, 90, 110, 130, 150, 170, 190, 210, 230, 250, 270, 290, 330, 370, 410, 450, 490};
static IntVector LevelIFRB{20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 310, 350, 390, 430, 470, 510};
#else
static IntVector LevelIFRA{10, 30, 50, 70, 90, 110, 130, 150, 170, 190, 210, 230, 250, 270, 290, 310, 330, 350, 370, 390, 410, 450, 490};
static IntVector LevelIFRB{20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 300, 320, 340, 360, 380, 400, 430, 470, 510};
#endif
static IntVector LevelVFRA{35, 55, 75, 95, 115, 135, 155, 175, 195};
static IntVector LevelVFRB{45, 65, 85, 105, 125, 145, 165, 185};

std::default_random_engine generator;
std::random_device randomDevice;
uni_dist rand_2(0.0, 2.0);
uni_dist rand_3(0.0, 3.0);
uni_dist rand_5(0.0, 5.0);
uni_dist rand_100(0.0, 100.0);
uni_dist rand_1(0.0, 1.0);

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
    feasibleList.push_back(iDefaultLevel);
    //auto position;
#ifdef NRVSM
    if (iDefaultLevel < 290){
#else
    if (iDefaultLevel < 410){
#endif
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

double randNormalDataFromInterval1(double dValue, double dSigma){
    normal_dist NormalDistribution((ALPHA+BETA)/2, dSigma);
    return dValue+NormalDistribution(generator);
}

double randNormalDataFromInterval2(double dValue, double dSigma){
    normal_dist NormalDistribution(0, dSigma);
    return dValue+NormalDistribution(generator);
}

double randNormalDataFromInterval3(double dValue, double dW1, double dW2, double dSigma1, double dSigma2, double dRatio){
    normal_dist NormalDistributionLeft(-dW1*ALPHA, dSigma1);
    normal_dist NormalDistributionRight(dW2*BETA, dSigma2);
    double dOffset;
    return dValue + (rand_1(generator) > dRatio)
           ? (dOffset = NormalDistributionRight(generator), dOffset < 0 ? -dOffset : dOffset)
           : (dOffset = NormalDistributionLeft(generator), dOffset > 0 ? -dOffset : dOffset);
}

/**
 * Generate a random departure time which follows the piecewise normal laws.
 * @param dDepartureTime the initial departure time.
 * @return the random departure time.
 */
double randPiecewiseData(double dDepartureTime){
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

/**
 * Generate the sigma value that makes:
 * P(a <= x <= b) >= gamma, where a-b=alpha+beta,
 * x follows a normal distribution, where the mean mu=(a+b)/2.
 * @param alpha the expected absolute of lower bound far from the mean of random variables.
 * @param beta the expected upper bound far from the mean of random variables.
 * @param gamma the confidence that random variable will fall in interval [a, b]
 * @return the sigma of random varaibles.
 */
double getSigma(double alpha, double beta, double gamma){
    return (alpha+beta)/(2*sqrt(2)*boost::math::erf_inv(gamma));
}
#endif //DEFINE_H
