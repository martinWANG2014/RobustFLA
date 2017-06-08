//
// Created by chenghaowang on 01/06/17.
//

#ifndef ROBUSTFLA_SOLVER_H
#define ROBUSTFLA_SOLVER_H
#undef IL_STD
#define  IL_STD
#include <ilcplex/ilocplex.h>
#include "Network.h"

class BasicSolver{
protected:
    IloEnv env;
    Network *pNetwork;
    double dCoefPij;
    double dCoefPi;
    double dEpsilon;
    double dFunctionObjetiveValue;
    IloNumVarArray &initDecisionVariables(int iSize);
    IloExpr &initFunctionObjective(int iProcessingLevel, IloNumVarArray &x);
    IloModel &initConstraints();
public:
    BasicSolver(IloEnv &env, Network *pNetwork, double dCoefPij, double dCoefPi,
                double dEpsilon);

    IloNumArray &solve(int iProcessingLevel);
    virtual bool checkFeasibility(IloNumArray &x)= 0;
};

#endif //ROBUSTFLA_SOLVER_H
