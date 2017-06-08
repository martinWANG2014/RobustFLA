//
// Created by chenghaowang on 07/06/17.
//

#include "../include/GaussianSolver.h"

GaussianSolver::GaussianSolver(IloEnv &env, Network *pNetwork, double dCoefPij,
                               double dCoefPi, double dEpsilon) : BasicSolver(env, pNetwork, dCoefPij, dCoefPi,
                                                                              dEpsilon) {}

bool GaussianSolver::checkFeasibility(IloNumArray &x) {
    return false;
}