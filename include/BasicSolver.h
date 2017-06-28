//
// Created by chenghaowang on 01/06/17.
//

#ifndef BASICSOLVER_H
#define BASICSOLVER_H

#include "Network.h"

class BasicSolver{
protected:
    IloEnv env;
    Network *pNetwork;
    double dCoefPij;
    double dCoefPi;
    double dEpsilon;
    double dFunctionObjetiveValue;
//    IloNumVarArray &initDecisionVariables(int iSize);
//    IloExpr &initFunctionObjective(int iProcessingLevel, IloNumVarArray &x);
//    IloModel &initConstraints();
public:
    BasicSolver(IloEnv &env, Network *pNetwork, double dCoefPij, double dCoefPi,
                double dEpsilon);
//    IloNumArray &solve(int iProcessingLevel);
    virtual bool checkFeasibility(IloNumArray &x)= 0;
};

#endif //ROBUSTFLA_SOLVER_H
