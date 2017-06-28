//
// Created by chenghaowang on 07/06/17.
//

#ifndef MONTECARLOSOLVER_H
#define MONTECARLOSOLVER_H

#include "BasicSolver.h"

class MonteCarloSolver: BasicSolver {
public:
    MonteCarloSolver(IloEnv &env, Network *pNetwork, double dCoefPij, double dCoefPi,
                     double dEpsilon);

private:
    bool checkFeasibility(IloNumArray &x) override;
};


#endif //MONTECARLOSOLVER_H
