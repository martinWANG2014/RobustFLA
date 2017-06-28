//
// Created by chenghaowang on 07/06/17.
//

#ifndef ENUMERATIONSOLVER_H
#define ENUMERATIONSOLVER_H

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


#endif //ENUMERATIONSOLVER_H
