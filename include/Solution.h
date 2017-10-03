//
// Created by chenghaowang on 30/06/17.
//

#ifndef SOLUTION_H
#define SOLUTION_H

#include "ProcessClock.h"
#include "Solver.h"
#include "Input.h"

#undef IL_STD
#define  IL_STD

#include <ilcplex/ilocplex.h>

/**
 * Combination C(n,k), use the deque to implement a recursive combination.
 * @param viConstraintList  The constraint list, contains all the items.
 * @param k                 The number of selecting items.
 * @return All combinations of k items from n items.
 */
qviList Combination(const viList &viConstraintList, int k);

/**
 * Get the complement list of a candidate list
 * @param iSize             The size of a whole list
 * @param candidateList     The candidate list
 * @return A complement list of the candidate list
 */
viList getComplement(int iSize, viList &candidateList);

/**
 * Get a random data from the first type gaussian distribution
 * @param vdParameter       The parameter list for the gaussian distribution
 * @return a random data.
 */
double GaussianDistribution1(const vdList &vdParameter, double dSigma);

/**
 * Get a random data from the second type gaussian distribution
 * @param vdParameter       The parameter list for the gaussian distribution
 * @return a random data.
 */
double GaussianDistribution2(const vdList &vdParameter, double dSigma);

/**
 * Get a random data from the third type gaussian distribution
 * @param vdParameter       The parameter list for the gaussian distribution
 * @return a random data.
 */
double GaussianDistribution3(const vdList &vdParameter, double dSigma);

/**
 * Create real symmetric matrix in size of iSize*iSize
 * @param iSize     The number of rows and cols
 * @return A pointer that points to the created matrix.
 */
double **CreateTable(int iSize);

/**
 * Initialize the matrix with value 0.
 * @param ppdTable      The pointer of matrix
 * @param iSize         The number of rows and cols
 */
void InitTable(double **ppdTable, int iSize);

/**
 * Destroy the matrix.
 * @param ppdTable      The pointer of matrix
 * @param iSize         The number of rows and cols
 */
void DestroyTable(double **ppdTable, int iSize);

void CalculateConflictProbability(FlightVector &vpConflictFlightList, double **ppdConflictProbability,
                                  double **ppdDelayTime, double **ppdDelayTimeMax);

/**
 * Get the index of most infeasible constraint.
 * @param vpConflictFlightList      The conflict flights list
 * @param viSearchList              The search list
 * @param vdPi                      The Admissible cost list
 * @param ppdConflictProbability    The pointer of conflict probability matrix
 * @param ppdDelayTime              The pointer of delay time matrix
 * @param bGeoMethod                A flag whether use the geometric method
 * @return the index of most infeasible constraint.
 */
int MinIndexArgs0(FlightVector &vpConflictFlightList, const vdList &vdPi,
                  double **ppdConflictProbability,
                  double **ppdDelayTime, bool modeDisplay);

/**
 * Get the index of most infeasible constraint for last solution.
 * @param vpConflictFlightList      The conflict flights list
 * @param viSearchList              The search list
 * @param vdPi                      The Admissible cost list
 * @param decisionVariables         The decision variables values of last solution
 * @param ppdConflictProbability    The pointer of conflict probability matrix
 * @param ppdDelayTime              The pointer of delay time matrix
 * @param bGeoMethod                A flag whether use the geometric method
 * @return the index of most infeasible constraint.
 */
int MinIndexArgs1(FlightVector &vpConflictFlightList, const viList &constraintList, const vdList &vdPi,
                  const IloNumArray &decisionVariables, double **ppdConflictProbability, double **ppdDelayTime,
                  bool bMethod);

/**
 * Check the constraint feasibility
 * @param viSearchList              The search list
 * @param vdPi                      The admissible cost list
 * @param decisionVariables         The decision variable value of last solution
 * @param ppdConflictProbability    The pointer of the conflict probability matrix
 * @param ppdDelayTime              The pointer of the delay time matrix
 * @param dEpsilon                  The robust parameter
 * @param iConflictedFlightsSize    The conflict flight number
 * @param piIndexMostInfeasible     The index of most infeasible constraint
 * @return Whether the solution has a high confidence of feasibility.
 */
bool FeasibilityEnumeration(const vdList &vdPi, const IloNumArray &decisionVariables,
                            double **ppdConflictProbability, double **ppdDelayTime,
                            double dEpsilon, int iConflictedFlightsSize, int *piIndexMostInfeasible, bool modeDisplay);

/**
 * Check the constraint feasibility
 * @param viSearchList              The search list
 * @param vdPi                      The admissible cost list
 * @param decisionVariables         The decision variable value of last solution
 * @param ppdConflictProbability    The pointer of the conflict probability matrix
 * @param ppdDelayTime              The pointer of the delay time matrix
 * @param dEpsilon                  The robust parameter
 * @param iConflictedFlightsSize    The conflict flight number
 * @param piIndexMostInfeasible     The index of most infeasible constraint
 * @return Whether the solution has a high confidence of feasibility.
 */
bool FeasibilityHoeffding(const vdList &vdPi,
                          const IloNumArray &decisionVariables, const vdList &vdParameter,
                          double **ppdConflictProbability, double **ppdDelayTime, double **ppdDelayTimeMax,
                          double dEpsilon, int iConflictedFlightSize, int *piIndex, bool modeDisplay);

/**
 * * Check the constraint feasibility
 * @param vpConflictedFlightList    The conflict flight list
 * @param viSearchList              The search list
 * @param vdPi                      The admissible cost list
 * @param decisionVariables         The decision variable value of last solution
 * @param ppdConflictProbability    The pointer of the conflict probability matrix
 * @param ppdDelayTime              The pointer of the delay time matrix
 * @param dEpsilon                  The robust parameter
 * @param piIndexMostInfeasible     The index of most infeasible constraint
 * @return Whether the solution has a high confidence of feasibility.
 */
bool FeasibilityGaussian(FlightVector &vpConflictedFlightList, const vdList &vdPi,
                         const IloNumArray &decisionVariables, double **ppdConflictProbability, double **ppdDelayTime,
                         double dEpsilon, int *piIndex, bool modeDisplay);

/**
 * * Check the constraint feasibility
 * @param vpConflictedFlightList    The conflict flight list
 * @param viSearchList              The search list
 * @param vdPi                      The admissible cost list
 * @param decisionVariables         The decision variable value of last solution
 * @param dEpsilon                  The robust parameter
 * @param piIndexMostInfeasible     The index of most infeasible constraint
 * @return Whether the solution has a high confidence of feasibility.
 */
bool FeasibilityMonteCarlo(FlightVector &vpConflictedFlightList, const viList &viConstraintList, const vdList &vdPi,
                           const IloNumArray &decisionVariables, const vdList &vdParameter, double dEpsilon,
                           int *piIndex, bool modeDisplay);


/**
 * Fast Robust FLA
 * @param vpFlightList      The flight list
 * @param env               The cplex environment
 * @param vdParameter       The distributon parameter list
 * @param viLevelsList      The level list
 * @param epsilon           The robust parameter
 * @param dSumBenefits      The sum of benefits
 * @param iMaxNbConflict    The max number of potential conflict
 * @param iModeMethod       The method mode
 * @param iModeRandom       The random mode
 * @param cplexLogFile      The cplex log file
 * @return  A unassigned flights list
 */
int SolveFLA(FlightVector &vpFlightList, FlightLevelAssignmentMap &flightLevelAssignmentMap, const IloEnv &env,
             const vdList &vdParameter,
             LevelVector &viLevelsList, ProcessClock &processClock, double epsilon, double dCoefPi,
             double *dSumBenefits, int *iMaxNbConflict, int iModeMethod, bool modeDisplay);

/**
 * Get the number of flights that change it flight level.
 * @param vpFlightList the conflict flight list
 * @return the number of flights that change it flight level.
 */
int getNbFlightsChangeLevel(FlightVector &flightVector);

int getMaxDiverseLevel(FlightVector &flightVector);

/**
 * Approximate FLA method by each level.
 * @param vpFlightList
 * @param env
 * @param vdParameter
 * @param levelEx
 * @param flightAssignmentMap
 * @param epsilon
 * @param dSumBenefits
 * @param iMaxNbConflict
 * @param iProcessingLevel
 * @param iModeMethod
 * @param iModeRandom
 * @param cplexLogFile
 * @return
 */
bool
SolvingFLAByLevelPP(FlightVector &vpFlightList, FlightsLevelMap &infeasibleFlightMap,
                    FlightVector &vpPreviousFlightList,
                    const IloEnv &env, const vdList &vdParameter, LevelExamine &levelEx,
                    FlightLevelAssignmentMap &flightLevelAssignmentMap, double epsilon,
                    int *iMaxNbConflict,
                    Level iProcessingLevel, int iModeMethod,
                    bool modeDisplay);

/**
 * Approximate FLA method
 * @param pNetwork          The pointer of network
 * @param vdParameter       The distribution parameter list
 * @param dEpsilon          The robust parameter
 * @param dCoefPi           The coefficient parameter for each flight
 * @param dSumBenefits      The sum of benefits
 * @param iMaxNbConflict    The maximal number of potential conflict
 * @param iModeMethod       The method mode
 * @param iFeasibleSize     The feasible flight size
 */
void ApproximateFLA(const Network *pNetwork, const vdList &vdParameter, double dEpsilon, double dCoefPi,
                    double *dSumBenefits, int *iMaxNbConflict, int iModeMethod, bool modeDisplay);

#endif //SOLUTION_H
