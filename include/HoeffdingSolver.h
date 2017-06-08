//
// Created by chenghaowang on 07/06/17.
//

#ifndef ROBUSTFLA_HOEFFDING_H
#define ROBUSTFLA_HOEFFDING_H


#include "BasicSolver.h"

class HoeffdingSolver: BasicSolver {
public:

private:
public:
    HoeffdingSolver(IloEnv &env, Network *pNetwork, double dCoefPij, double dCoefPi,
                    double dEpsilon);

private:
    bool checkFeasibility(IloNumArray &x) override;
};


#endif //ROBUSTFLA_HOEFFDING_H
