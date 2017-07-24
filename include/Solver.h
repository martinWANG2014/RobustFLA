//
// Created by chenghaowang on 01/06/17.
//

#ifndef SOLVER_H
#define SOLVER_H
#undef IL_STD
#define  IL_STD

#include <ilcplex/ilocplex.h>
#include "Flight.h"

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


public:
    /**
     * Constructor with parameters.
     * @param env                   Cplex environment
     * @param log                   The log file stream
     */
    Solver(IloEnv env, std::ofstream &log);

    /**
     * Constructor with parameters.
     * @param env                   Cplex environment
     */
    Solver(IloEnv env);

    /**
     * Destructor.
     */
    virtual ~Solver();

    /**
     * solve method, call explicitly to solve the ILP model.
     */
    void solve();

    /**
     * Add new constraint into model
     * @param Mi                        The Mi list
     * @param Pi                        The Pi list
     * @param ppdDelayTime                 The pointer of delay time matrix
     * @param i                         The index of new constraint
     * @param iNbConflictedFlights      The number of conflict flights
     */
    void addNewConstraint(const vdList &Mi, const vdList &Pi, double **ppdDelayTime, int i, int iNbConflictedFlights);

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
    void initFunctionObjective(const FlightVector &ConflictedFlightList, int iProcessingLevel);


    /**
     * Get the decision variables value after solving the model.
     * @return an array of decision variables value
     */
    IloNumArray getDecisionVariablesValues() const;

    /**
     * Get the function objective value after solving the model.
     * @return the function objective value
     */
    double getFunctionObjectiveValue() const;
};

#endif //SOLVER_H
