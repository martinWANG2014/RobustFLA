//
// Created by chenghaowang on 06/07/17.
//

#include "../include/Solver.h"

void Solver::initDecisionVariables(int iSize) {
    std::cout << "\t\tInitialize decision variables... " << std::flush;
    IloNumVarArray x(env, iSize, 0, 1, ILOBOOL);
    decisionVariables = x;
    std::cout << "OK" << std::endl;
}

void Solver::initFunctionObjective(int iProcessingLevel, FlightVector &ConflictedFlightList) {
    std::cout << "\t\tInitialize function objective... " << std::flush;
    IloExpr obj(env);
    for (int i = 0; i < (int) ConflictedFlightList.size(); i++) {
        Flight *pFlight = ConflictedFlightList[i];
        if (pFlight->getDefaultLevel() == iProcessingLevel) {
            obj += 3 * decisionVariables[i];
        } else {
            obj += decisionVariables[i];
        }
    }
    functionObjective = obj;
    model.add(IloMaximize(env, obj));
    std::cout << "OK" << std::endl;
}

void Solver::initConstraints(IntVector &viConstraintList, int iNbConflictedFlights, DoubleVector &Mi, DoubleVector &Pi,
                             double **penalCost) {
    std::cout << "\t\tInitialize constraints... " << std::flush;
    for(auto &&i : viConstraintList){
        IloExpr constraint(env);
        for (int j = 0; j < iNbConflictedFlights; j++) {
            if (i == j) {
                constraint += Mi[j] * decisionVariables[j];
            } else{
                constraint += penalCost[i][j]*decisionVariables[j];
            }
        }
        IloConstraint c(constraint <= Mi[i] + Pi[i]);
        model.add(c);
    }
    std::cout << "OK" << std::endl;
}
