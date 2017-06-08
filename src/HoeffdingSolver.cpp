//
// Created by chenghaowang on 07/06/17.
//

#include "../include/HoeffdingSolver.h"

HoeffdingSolver::HoeffdingSolver(IloEnv &env, Network *pNetwork,  double dCoefPij,
                                 double dCoefPi, double dEpsilon) : BasicSolver(env, pNetwork, dCoefPij, dCoefPi,
                                                                                dEpsilon) {}

bool HoeffdingSolver::checkFeasibility(IloNumArray &x) {
    double **probability = pNetwork->getPorbability();
    double **delay = pNetwork->getDelay();
    double *Pi = pNetwork->getPi();
    int size = pNetwork->getCurrentProcessingSize();
    for (int i = 0; i < size; i++) {
        double sum1 = 0;
        if (x[i] == 1) {
            for (int j = 0; j < size; j++) {
                if (probability[i][j] > 0) {
                    sum1 += probability[i][j] * delay[i][j] * x[j] / 2;
                }
            }
            double sum2 = 0;
            for (int j = 0; j < size; j++) {
                if (probability[i][j] > 0) {
                    sum2 += delay[i][j] * delay[i][j] * x[j];
                }
            }
            if (exp(-(2 * pow(Pi[i] - sum1, 2)) / sum2) > dEpsilon) {
                return false;
            }
        }
    }
    return true;
}