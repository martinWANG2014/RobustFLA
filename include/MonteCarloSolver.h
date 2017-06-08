//
// Created by chenghaowang on 07/06/17.
//

#ifndef ROBUSTFLA_MONTECARLO_H
#define ROBUSTFLA_MONTECARLO_H


#include "BasicSolver.h"

class MonteCarloSolver: BasicSolver {
public:
    MonteCarloSolver(IloEnv &env, Network *pNetwork, double dCoefPij, double dCoefPi,
                     double dEpsilon);

private:
    bool checkFeasibility(IloNumArray &x) override;
};


#endif //ROBUSTFLA_MONTECARLO_H
