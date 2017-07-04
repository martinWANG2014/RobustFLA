//
// Created by chenghaowang on 01/06/17.
//

#ifndef SOLVER_H
#define SOLVER_H

#include "Network.h"

class Solver{
private:
    IloEnv *cplex;
    IloNumVarArray *decisionVariables;
    IloNumArray *decisionVariablesValues;
    IloExpr *functionObjective;
    IloModel *model;

    double dFunctionObjectiveValue;

    void initDecisionVariables(int iSize) {
        IloNumVarArray x(*cplex, iSize, 0, 1, ILOBOOL);
        *decisionVariables = x;
    }

    void initFunctionObjective(int iProcessingLevel, FlightVector &flCandLev_list) {
        IloModel model_temp(*cplex);
        *model = model_temp;
        IloExpr obj(*cplex);
        for(int i = 0; i < (int)flCandLev_list.size(); i++){
            Flight *pFlight = flCandLev_list[i];
            if (pFlight->getDefaultLevel() == iProcessingLevel) {
                obj += 3 * (*decisionVariables)[i];
            }
            else {
                obj += (*decisionVariables)[i];
            }
        }
        *functionObjective = obj;
        model->add(IloMaximize(*cplex, obj));
    }

    void initConstraints(IntVector &viConstraintList, int sizeCandidateFlights, DoubleVector &Mi, DoubleVector &Pi, double** probability, double** delay) {
        for (unsigned int i = 0; i < viConstraintList.size(); i++) {
            int k = viConstraintList[i];
            IloExpr constraint(*cplex);
            for (int j = 0; j < sizeCandidateFlights; j++) {
                if (k == j) {
                    constraint += Mi[j] * (*decisionVariables)[j];
                }
                else if (probability[k][j]>0) {
                    constraint += COEFPIJ*(*decisionVariables)[j] * delay[k][j];
                }
            }
            model->add(constraint <= Mi[k] + Pi[k]);
        }
    }

public:
    Solver(IloEnv &env, FlightVector &flCandLev_list, int iProcessingLevel, DoubleVector &Mi, DoubleVector &Pi, double** probability, double** delay, IntVector &viConstraintList) {
        *cplex= env;
        initDecisionVariables((int)flCandLev_list.size());
        initConstraints(viConstraintList, (int)flCandLev_list.size(), Mi, Pi, probability, delay);
        initFunctionObjective(iProcessingLevel, flCandLev_list);
    }

    void solve() {
        // solver
        IloCplex solver(*model);
        // no output
        solver.setOut(cplex->getNullStream());
        // no multi-threading (less performance)
        solver.setParam(IloCplex::Threads, 1);
        solver.solve();
        solver.exportModel("model.lp");
        solver.writeSolution("sol.sl");
        IloNumArray values(*cplex);
        solver.getValues(values, *decisionVariables);
        *decisionVariablesValues = values;
        dFunctionObjectiveValue = solver.getObjValue();
        solver.end();
        model->end();
        functionObjective->end();
        decisionVariables->end();
    }

    IloNumArray *getDecisionVariablesValues() const {
        return decisionVariablesValues;
    }

    double getFunctionObjectiveValue() const {
        return dFunctionObjectiveValue;
    }
};

#endif //SOLVER_H
