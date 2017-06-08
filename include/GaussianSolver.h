//
// Created by chenghaowang on 07/06/17.
//

#ifndef ROBUSTFLA_GAUSSIANSOLVER_H
#define ROBUSTFLA_GAUSSIANSOLVER_H


#include "BasicSolver.h"

class GaussianSolver: BasicSolver{
public:
    GaussianSolver(IloEnv &env, Network *pNetwork, double dCoefPij, double dCoefPi,
                   double dEpsilon);

private:
    bool checkFeasibility(IloNumArray &x) override;
};


#endif //ROBUSTFLA_GAUSSIANSOLVER_H
