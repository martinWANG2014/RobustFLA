//
// Created by chenghaowang on 01/06/17.
//
#include "../include/BasicSolver.h"

BasicSolver::BasicSolver(IloEnv &env, Network *pNetwork, double dCoefPij=1,
                         double dCoefPi=5, double dEpsilon=0.05) : env(env), pNetwork(pNetwork),
                                                            dCoefPij(dCoefPij), dCoefPi(dCoefPi), dEpsilon(dEpsilon) {
//    clock.start();
//    do{
//        IloNumArray x = solve(0);
//    }while(!checkFeasibility(x));
//    env.end();
//    clock.end();
}


//IloNumVarArray &BasicSolver::initDecisionVariables(int iSize) {
//    IloNumVarArray x(env, iSize, 0, 1, ILOBOOL);
//    return x;
//}

//IloExpr &BasicSolver::initFunctionObjective(int iProcessingLevel, IloNumVarArray &x) {
//    IloExpr obj(env);
//    for(int i = 0; i < pNetwork->getCurrentProcessingSize(); i++){
//        obj += pNetwork->getPenalCostOfFlightAtLevel(i, iProcessingLevel)*x[i];
//    }
//    return obj;
////    model.add(IloMinimize(env, obj));
//}

//IloModel &BasicSolver::initConstraints() {
//    IloModel model(env);
//    for(int i = 0; i < 1; i++){
//        IloExpr constraint(env);
//        //....
////        int k = Contrainte_list[i];
////        IloExpr contraint(env);
////        for (int j = 0; j < size; j++) {
////            if (k == j) {
////                contraint += Mi[j] * x[j];
////            } else if (probability[k][j] > 0) {
////                //contraint+=Coef_Pij*x[j]*probability[k][j];
////                contraint += Coef_Pij * x[j] * delai[k][j];
////            }
////        }
////        model.add(contraint <= Mi[k] + Pi[k]);
//        model.add(constraint);
//    }
//    return model;
//}

//IloNumArray &BasicSolver::solve(int iProcessingLevel) {
//    IloNumVarArray x = initDecisionVariables(pNetwork->getCurrentProcessingSize());
//    IloModel model = initConstraints();
//    IloExpr obj = initFunctionObjective(iProcessingLevel, x);
//    model.add(IloMinimize(env, obj));
//    IloCplex solver(model); // solver
//    solver.setOut(env.getNullStream()); // no output
//    solver.setParam(IloCplex::Threads, 2); // no multi-threading (less performance)
//    solver.solve();
//    solver.exportModel("model.lp");
//    solver.writeSolution("sol.sl");
//    IloNumArray values(env);
//    solver.getValues(values, x);
//    dFunctionObjetiveValue = solver.getObjValue();
//    model.end();
//    solver.end();
//    return values;
//}

