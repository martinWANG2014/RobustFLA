//
// Created by chenghaowang on 01/06/17.
//

#ifndef SOLVER_H
#define SOLVER_H
#undef IL_STD
#define  IL_STD

#include <ilcplex/ilocplex.h>
#include "Flight.h"

typedef std::vector<int> viList;
typedef std::vector<double> vdList;

/**
 * The solver class for purpose to solve the ILP model, then give a corresponding result.
 */
class Solver {
private:
    /**
     * Cplex environment.
     */
    IloEnv env;

    /**
     * The model entity.
     */
    IloModel model;

    /**
     * Cplex Solver.
     */
    IloCplex solver;

    /**
     * Decision variables array.
     */
    IloNumVarArray decisionVariables;

    /**
     * Decision variables's value array.
     */
    IloNumArray decisionVariablesValues;

    /**
     * Function objective expression.
     */
    IloExpr functionObjective;

    /**
     * Function objective value.
     */
    double dFunctionObjectiveValue;

    /**
     * Initialize the decision variables array.
     * @param iSize     the array size.
     */
    void initDecisionVariables(int iSize);

    /**
     * Initialize the function objective.
     * @param iProcessingLevel      the current processing flight level
     * @param ConflictedFlightList  the conflicted flights list
     */
    void initFunctionObjective(int iProcessingLevel, FlightVector &ConflictedFlightList);

    /**
     * Initialize the constraints.
     * @param viConstraintList      A list of flight index, for which should respect the constraint.
     * @param iNbConflictedFlights  The number of conflicted flights.
     * @param Mi                    A list of Mi, which is a sufficient large value to assure the constraint for a given flight i should be satisfied at any condition.
     * @param Pi                    A list of Pi, which means the admissible cumulative cost for a given flight i at current flight level.
     * @param penalCost
     */
    void initConstraints(viList &viConstraintList, int iNbConflictedFlights, vdList &Mi, vdList &Pi,
                         double **penalCost);

    /**
     * Configure the solver.
     * @param log       The log file stream
     */
    void configureSolver(std::ofstream &log, bool display) {
        if (display) {
            env.setOut(log);
            solver.setOut(log);
        } else {
            env.setOut(env.getNullStream());
            solver.setOut(env.getNullStream());
        }
        solver.setParam(IloCplex::Threads, 1);
    }

public:
    /**
     * Constructor with parameters.
     * @param env                   Cplex environment
     * @param ConflictedFlightList  A list of conflicted flights
     * @param iProcessingLevel      The current processing flight level
     * @param Mi                    A list of Mi, which is a sufficient large value to assure the constraint for a given flight i should be satisfied at any condition.
     * @param Pi                    A list of Pi, which means the admissible cumulative cost for a given flight i at current flight level.
     * @param penalCost
     * @param viConstraintList      A list of flight index, for which should respect the constraint.
     * @param log                   The log file stream
     */
    Solver(IloEnv env, FlightVector &ConflictedFlightList, int iProcessingLevel, vdList &Mi, vdList &Pi,
           double **penalCost, viList &viConstraintList, std::ofstream &log, bool display) {
        Solver::env = env;
        IloModel model_temp(env);
        Solver::model = model_temp;
        IloCplex solver_temp(model);
        Solver::solver = solver_temp;
        configureSolver(log, display);
        initDecisionVariables((int) ConflictedFlightList.size());
        initConstraints(viConstraintList, (int) ConflictedFlightList.size(), Mi, Pi, penalCost);
        initFunctionObjective(iProcessingLevel, ConflictedFlightList);
    }

    /**
     * Destructor.
     */
    virtual ~Solver() {
        functionObjective.end();
        decisionVariables.end();
        solver.end();
        model.end();
    }

    /**
     * solve method, call explicitly to solve the ILP model.
     */
    void solve() {
//        std::cout << "\t\tSolving..." << std::flush;
        IloNumArray values(env);
//        solver.exportModel("model.lp");
        if (solver.solve()) {
            solver.getValues(values, decisionVariables);
            decisionVariablesValues = values;
            dFunctionObjectiveValue = solver.getObjValue();

//            std::cout << "Ok" << std::endl;
        } else {

//            std::cout << "Failed" << std::endl;
            abort();
        }
    }

    /**
     * Get the decision variables value after solving the model.
     * @return an array of decision variables value
     */
    IloNumArray getDecisionVariablesValues() const {
        return decisionVariablesValues;
    }

    /**
     * Get the function objective value after solving the model.
     * @return the function objective value
     */
    double getFunctionObjectiveValue() const {
        return dFunctionObjectiveValue;
    }
};

#endif //SOLVER_H
