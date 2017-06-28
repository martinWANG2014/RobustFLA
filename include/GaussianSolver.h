//
// Created by chenghaowang on 07/06/17.
//

#ifndef GAUSSIANSOLVER_H
#define GAUSSIANSOLVER_H

#include "BasicSolver.h"

class GaussianSolver: BasicSolver{
public:
    GaussianSolver(IloEnv &env, Network *pNetwork, double dCoefPij, double dCoefPi,
                   double dEpsilon);

private:
    bool checkFeasibility(IloNumArray &x) override;
};


#endif //GAUSSIANSOLVER_H
