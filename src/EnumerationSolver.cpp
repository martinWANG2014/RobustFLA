//
// Created by chenghaowang on 07/06/17.
//
#include "../include/EnumerationSolver.h"

EnumerationSolver::EnumerationSolver(IloEnv &env, Network *pNetwork, double dCoefPij,
                                     double dCoefPi, double dEpsilon) : BasicSolver(env, pNetwork, dCoefPij,
                                                                                    dCoefPi, dEpsilon) {}

bool EnumerationSolver::checkFeasibility(IloNumArray &x) {
//    int *ind;
//    int size = pNetwork->getCurrentProcessingSize();
//    double **probability = pNetwork->getPorbability();
//    double *Pi = pNetwork->getPi();
//    *ind = -1;
//    double max_somme = -1;
//    for (int i = 0; i < size; i++) {
//        if (x[i] == 1) {
//            std::vector<int> first;
//            std::vector <std::vector<int>> result;
//            int nombre = 0;
//            double somme = 0;
//            double temp_somme = 0;
//            for (int j = 0; j < size; j++) {
//                if (probability[i][j] != 0) {
//                    temp_somme = temp_somme + dCoefPij * x[j];
//                    first.push_back(j);
//                    nombre++;
//                }
//            }
//            if (temp_somme > Pi[i]) {
//                int k = (int) ceil(Pi[i] / dCoefPij);
//
//                for (unsigned int l = 0; l < result.size(); l++) {
//                    double temp_prod = 1;
//                    for (int j = 0; j < size; j++) {
//                        if (probability[i][j] != 0) {
//                            //if (contains<int>(result, j)){
//                            if (appartient(j, result[l])) {
//                                temp_prod = temp_prod * probability[i][j];
//                            } else {
//                                temp_prod = temp_prod * (1 - probability[i][j]);
//                            }
//                        }
//                    }
//                    somme = somme + temp_prod;
//                }
//                if (somme > dEpsilon) {
//                    return false;
//                }
//
//            }
//            if (somme != 0 && somme > max_somme && !appartient(i, Contrainte_list)) {
//                max_somme = somme;
//                *ind = i;
//            }
//        }
//    }
    return true;
}
