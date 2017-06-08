//
// Created by chenghaowang on 25/05/17.
//

#ifndef ROBUSTFLA_MATHFUNCTION_H
#define ROBUSTFLA_MATHFUNCTION_H

#include "../include/predefine.h"

std::default_random_engine generator;
typedef std::uniform_real_distribution<double> uni_dist;
typedef std::normal_distribution<double> normal_dist;

uni_dist rand_2(0.0, 2.0);
uni_dist rand_3(0.0, 3.0);
uni_dist rand_5(0.0, 5.0);
uni_dist rand_100(0.0, 100.0);

/**
 * Generate a random departure time which follows the piecewise uniform laws.
 * @param dDepartureTime the initial departure time.
 * @return the random departure time.
 */
double randDepartureTimeInterval(double dDepartureTime);

/**
 * Generate a random departure time which follows the piecewise normal laws.
 * @param dDepartureTime the initial departure time.
 * @return the random departure time.
 */
double randDepartureTimeNormalDist(double dDepartureTime);


//bool feasibilityHoeffding(IloNumArray x, DoubleVector vPi, double ** dpProbability, double ** costForConflict, int iSize, double dEpsilon);


#endif //ROBUSTFLA_MATHFUNCTION_H
