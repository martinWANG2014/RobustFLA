//
// Created by chenghaowang on 14/07/17.
//

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//#include <Python.h>
#include <cmath>
#include <vector>
#include <set>
#include <cstdio>
#include <map>
#include <random>
#include <boost/math/special_functions.hpp>
#include <queue>

#ifndef K_CONST
#define K_CONST
const double K = 1.8520108;
#endif
#ifndef EARTH_RADIUS_CONST
#define EARTH_RADIUS_CONST
const double EARTH_RADIUS = 6378.16;
#endif
#ifndef MIN_SEPARATION_DISTANCE_CONST
#define MIN_SEPARATION_DISTANCE_CONST
const double MIN_SEPARATION_DISTANCE = 5.0;
#endif
#ifndef MIN_ANGLE_CONST
#define MIN_ANGLE_CONST
const double MIN_ANGLE = cos(15.0 / 180.0 * M_PI);
#endif
#ifndef MIN_PROBA_CONST
#define MIN_PROBA_CONST
/**
* A coefficient parameter: MIN_PORBA, the minimal considering conflict probability.
*/
const double MIN_PROBA = 0.0001;
#endif

#ifndef PARAMETERS_GAUSSIAN_CONST
#define PARAMETERS_GAUSSIAN_CONST
const double P_1=0.37;
const double P_2=0.40;
const double P_3=0.15;
const double P_4=0.08;
const double MU_1=-17.15;
const double MU_2=-7.31;
const double MU_3=19.57;
const double MU_4=69.13;
const double SIGMA_2_1= 88.20;
const double SIGMA_2_2= 89.33;
const double SIGMA_2_3= 1007.73;
const double SIGMA_2_4= 3926.00;
#endif

#ifndef TIMEOFFSETSUP_CONST
#define TIMEOFFSETSUP_CONST
const int TIMEOFFSETSUP = 300;
#endif

typedef std::vector<int> viList;
typedef std::vector<double> vdList;
typedef int Level;
typedef double Time;
typedef std::string String;

typedef std::queue<Level> LevelQueue;
typedef std::vector<Level> LevelVector;
typedef std::map<Level, std::pair<int, int>> FlightLevelMap;
typedef std::map<Level, bool> LevelExamine;
typedef std::vector<std::pair<Level, std::pair<int, int>>> LevelNumberList;
typedef std::normal_distribution<double> normal_dist;
typedef std::uniform_real_distribution<double> uni_dist;

////////////Definition of Flight Level/////////////////////////////////////////
#ifndef LEVEL_LIST_CONST
#define LEVEL_LIST_CONST
#ifdef NRVSM
static viList const LevelIFRA{10, 30, 50, 70, 90, 110, 130, 150, 170, 190, 210, 230, 250, 270, 290, 330, 370, 410, 450, 490};
static viList const LevelIFRB{20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 310, 350, 390, 430, 470, 510};
#else
const viList LevelIFRA{10, 30, 50, 70, 90, 110, 130, 150, 170, 190, 210, 230, 250, 270, 290, 310, 330, 350, 370, 390,
                       410, 450, 490};
const viList LevelIFRB{20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 300, 320, 340, 360, 380,
                       400, 430, 470, 510};
#endif
const viList LevelVFRA{35, 55, 75, 95, 115, 135, 155, 175, 195};
const viList LevelVFRB{45, 65, 85, 105, 125, 145, 165, 185};
#endif
////////////Definition of Flight Level/////////////////////////////////////////

////////////Method template////////////////////////////////////////////////////
//template <typename T>
//struct my_id_translator
//{
//    typedef T internal_type;
//    typedef T external_type;
//
//    boost::optional<T> get_value(const T &v) { return  v.substr(1, v.size() - 2) ; }
//    boost::optional<T> put_value(const T &v) { return  v; }
//};
/**
 * A template of the method to verify whether the a vector contains an element.
 * @param T                    the general type of data
 * @param vObjectsVector        the target vector
 * @param object                the queried object
 * @return true, if the vector contains the queried object; false, otherwise.
 */
template<typename T>
bool contains(std::vector<T> vObjectsVector, T object) {
    if (vObjectsVector.size() < 1) return false;
    for (auto &&item : vObjectsVector) {
        if (item == object) {
            return true;
        }
    }
    return false;
}

/**
 * A template of the method to verify whether the a vector contains an element, where the element is a pointer.
 * @tparam T                    The general type of data
 * @param vpObjectsVector       The target vector
 * @param pObject               The queried object
 * @return true, if the vector contains the queried object; false, otherwise.
 */
template<typename T>
bool contains(std::vector<T *> vpObjectsVector, T *pObject) {
    if (vpObjectsVector.size() < 1) return false;
    for (auto &&item : vpObjectsVector) {
        if (*item == *pObject) {
            return true;
        }
    }
    return false;
}

/**
 * A template of the method to find an object by providing its code.
 * @tparam T                    The general type of object
 * @param vpObjectsVector       The object list
 * @param sCode                 The given code of an object
 * @return The pointer of corresponding object.
 */
template<typename T>
T *findByCode(std::vector<T *> vpObjectsVector, const String &sCode) {
    if (vpObjectsVector.size() < 1) return nullptr;
    for (auto &&item : vpObjectsVector) {
        if (item->getCode() == sCode) {
            return item;
        }
    }
    return nullptr;
}


/**
 * A template of Comparator for a pair object in a vector.
 * @tparam T1   The type of key in a pair object
 * @tparam T2   The type of value in a pair object
 */
template<typename T1, typename T2, typename T3>
struct greater_second {
    typedef std::pair<T1, std::pair<T2, T3>> type;

    bool operator()(type const &a, type const &b) const {
//        return ((a.second.first > b.second.first)||(a.second.first == b.second.first &&a.second.second > b.second.second));
        return (a.second.second > b.second.second) ||
               (a.second.second == b.second.second && a.second.first > b.second.first);
    }
};


/**
 * A template to retrieve key from a pair object in a vector.
 * @tparam T1   The type of key in a pair object
 * @tparam T2   The type of value in a pair object
 */
template<typename T1, typename T2, typename T3>
struct retrieve_key {
    typedef std::pair<T1, std::pair<T2, T3>> type;

    T1 operator()(type const &a) const {
        return a.first;
    }
};



////////////Method template////////////////////////////////////////////////////

/**
 * Find the feasible flight level list.
 * @param iDefaultLevel     A given default flight level
 * @return A three elements list.
 */
viList findFeasibleLevels(int iDefaultLevel, int iSize);


/**
 * Find the next feasible flight level
 * @param iDefaultLevel     A given default flight level
 * @param lastLevel         The last flight level in the feasible flihgt levels list
 * @return the next feasible flight level
 */
Level findNextFeasibleLevel(int iDefaultLevel, Level lastLevel);

/**
 * Get the probability of Pr(LB <= X <= UB)
 * @param mu the mean value of random distribution.
 * @param sigma_2 the variance value of random distribution
 * @param dLB  the lower bound
 * @param dUB the upper bound
 * @return the difference of cdf at the specified interval.
 */
double getIntervalProbability(double mu, double sigma_2, double dLB, double dUB);

/**
 * Get the probability of Pr(x=UB)-Pr(x=LB)
 * @param mu the mean value of random distribution.
 * @param sigma_2 the variance value of random distribution
 * @param dLB the lower bound
 * @param dUB the upper bound
 * @return the difference of pdf at specified interval.
 */
double getIntervalDensityProbability(double mu, double sigma_2, double dLB, double dUB);

/**
 * Get the expected value of waiting time
 * @param max_value the maximal waiting time
 * @param mu the mean value of waiting time's distribution model
 * @return the expected waiting time
 */
double getExpectedValue(double max_value, double mu);

bool exists(String filename);
#endif //CONFIGURATION_H
