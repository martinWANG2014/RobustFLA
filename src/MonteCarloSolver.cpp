//
// Created by chenghaowang on 07/06/17.
//
#include "../include/MonteCarloSolver.h"

MonteCarloSolver::MonteCarloSolver(IloEnv &env, Network *pNetwork, double dCoefPij,
                                   double dCoefPi, double dEpsilon) : BasicSolver(env, pNetwork, dCoefPij,
                                                                                  dCoefPi, dEpsilon) {}

bool MonteCarloSolver::checkFeasibility(IloNumArray &x) {
    return false;
}

