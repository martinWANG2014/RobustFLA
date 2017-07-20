//
// Created by chenghaowang on 06/07/17.
//

#include "../include/Solver.h"

void Solver::initDecisionVariables(int iSize) {
//    std::cout << "\t\tInitialize decision variables... " << std::flush;
    IloNumVarArray x(env, iSize, 0, 1, ILOBOOL);
    decisionVariables = x;
//    std::cout << "OK" << std::endl;
}

void Solver::initFunctionObjective(int iProcessingLevel, FlightVector &ConflictedFlightList) {
//    std::cout << "\t\tInitialize function objective... " << std::flush;
    IloExpr obj(env);
    for (int i = 0; i < (int) ConflictedFlightList.size(); i++) {
        Flight *pFlight = ConflictedFlightList[i];
        auto index = std::find(pFlight->getFeasibleLevelList().begin(), pFlight->getFeasibleLevelList().end(),
                               iProcessingLevel);
        if (index != pFlight->getFeasibleLevelList().end()) {
            long lIndex = index - pFlight->getFeasibleLevelList().begin();
            obj += decisionVariables[i] * exp(pFlight->getFeasibleLevelList().size() - lIndex);
//            switch (lIndex) {
//                case 0:
//                    obj += 10 * decisionVariables[i];
//                    break;
//                case 1:
//                    obj += 3 * decisionVariables[i];
//                    break;
//                case 2:
//                    obj += 1 * decisionVariables[i];
//                    break;
//                default:
//                    std::cerr << "[Fatal Error]: the feasible flight level list size is not correct!" << std::endl;
//                    abort();
//            }
        } else {
            std::cerr << std::endl << "[Fatal Error]: the flight is processing out of it feasible flight level list !"
                      << std::endl;
            abort();
        }
//        if (pFlight->getDefaultLevel() == iProcessingLevel) {
//            obj += 3 * decisionVariables[i];
//        } else {
//            obj += decisionVariables[i];
//        }
    }
    functionObjective = obj;
    model.add(IloMaximize(env, functionObjective));
//    std::cout << "OK" << std::endl;
}

void Solver::initConstraints(viList &viConstraintList, int iNbConflictedFlights, vdList &Mi, vdList &Pi,
                             double **penalCost) {
//    std::cout << "\t\tInitialize constraints... " << std::flush;
    for(auto &&i : viConstraintList){
        IloExpr constraint(env);
        for (int j = 0; j < iNbConflictedFlights; j++) {
            if (i == j) {
                constraint += Mi[j] * decisionVariables[j];
            } else{
                constraint += penalCost[i][j]*decisionVariables[j];
            }
        }
//        std::cout << Mi[i] + Pi[i] << "\n";
        IloConstraint c(constraint <= Mi[i] + Pi[i]);
        model.add(c);
    }
//    std::cout << "OK" << std::endl;
}
