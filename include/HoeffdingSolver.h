//
// Created by chenghaowang on 07/06/17.
//

#ifndef HOEFFDINGSOLVER_H
#define HOEFFDINGSOLVER_H

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


#endif //HOEFFDINGSOLVER_H
