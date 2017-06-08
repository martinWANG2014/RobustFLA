//
// Created by chenghaowang on 07/06/17.
//

#ifndef ROBUSTFLA_ENUMERATION_H
#define ROBUSTFLA_ENUMERATION_H


#include "BasicSolver.h"

class EnumerationSolver: BasicSolver {
public:

private:
public:
    EnumerationSolver(IloEnv &env, Network *pNetwork, double dCoefPij, double dCoefPi,
                      double dEpsilon);

private:
    bool checkFeasibility(IloNumArray &x) override;
};


#endif //ROBUSTFLA_ENUMERATION_H
