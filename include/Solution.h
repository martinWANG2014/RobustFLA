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

void
CalculateConflictProbability(FlightVector &vpConflictFlightList, double **ppdConflictProbability, double **ppdDiffTime,
                             double **ppdWaitingTimeMax, double **ppdDelayWait,
                             bool deterministicRule, bool deterministic);

/**
 * Get the index of most infeasible constraint.
 * @param vpConflictFlightList      The conflict flights list
 * @param viSearchList              The search list
 * @param vdPi                      The Admissible cost list
 * @param ppdConflictProbability    The pointer of conflict probability matrix
 * @param ppdDiffTime              The pointer of delay time matrix
 * @param bGeoMethod                A flag whether use the geometric method
 * @return the index of most infeasible constraint.
 */
int MinIndexArgs0(FlightVector &vpConflictFlightList, const vdList &vdPi, double **ppdConflictProbability,
                  double **ppdDiffTime, double **ppdWaitingTimeMax, double **ppdWait);

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
bool FeasibilityHoeffding(const vdList &vdPi, const IloNumArray &decisionVariables, double **ppdConflictProbability,
                          double **ppdWaitingTimeMax, double **ppdWait, double dEpsilon, int iConflictedFlightSize,
                          int *piIndex, bool modeDisplay);

/**
 * * Check the constraint feasibility
 * @param vpConflictedFlightList    The conflict flight list
 * @param viSearchList              The search list
 * @param vdPi                      The admissible cost list
 * @param decisionVariables         The decision variable value of last solution
 * @param ppdConflictProbability    The pointer of the conflict probability matrix
 * @param ppdDiffTime              The pointer of the delay time matrix
 * @param dEpsilon                  The robust parameter
 * @param piIndexMostInfeasible     The index of most infeasible constraint
 * @return Whether the solution has a high confidence of feasibility.
 */
bool FeasibilityMixtureGaussian(FlightVector &vpConflictedFlightList, const vdList &vdPi,
                                const IloNumArray &decisionVariables, double **ppdConflictProbability,
                                double **ppdDiffTime, double **ppdWaitingTimeMax, double **ppdWait,
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
                           const IloNumArray &decisionVariables, double dEpsilon,
                           int *piIndex, bool modeDisplay, int nbIteration, bool deterministicRule);


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
             LevelVector &viLevelsList, ProcessClock &processClock, double epsilon, double *dSumBenefits,
             double minAdmissibleCost, double maxAdmissibleCost,
             int *iMaxNbConflict, int iModeMethod, bool modeDisplay, int nbIterations, bool deterministicRule);

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
SolvingFLAByLevel(FlightVector &vpFlightList, FlightsLevelMap &infeasibleFlightMap,
                  FlightVector &vpPreviousFlightList,
                  const IloEnv &env, LevelExamine &levelEx,
                  FlightLevelAssignmentMap &flightLevelAssignmentMap, double epsilon, double minAdmissibleCost,
                  double maxAdmissibleCost,
                  int *iMaxNbConflict,
                  Level iProcessingLevel, int iModeMethod,
                  bool modeDisplay, int nbIterations, bool deterministicRule);

void ApproximateFLA(const Network *pNetwork, String dataFilename,
                    double dEpsilon, double dCoefPi, double minAdmissibleCost, double maxAdmissibleCost,
                    int feasibleSize,
                    int iModeMethod, int percentileSup, bool modeDisplay, bool deterministicRule, int nbIterations = 1000);

void writeJsonSolution(String dataFilename, double epsilon, double coefPi, double SumBenefits, double ElapsedTime,
                       double minAdmissibleCost, double maxAdmissibleCost,
                       int feasibleSize, int method, int percentileSup,
                       int nbFlightsChangeLevel, int nbMaxConflict,
                       int nbMaxDiverseLevels, int nbIterations,
                       bool deterministicRule);

double MixtureGaussianDistributionWithFourComponents();

double getFoldedProbability(double mu_left, double mu_right, double sigma_2, double dBound, bool bLB);

double getProbabilityGaussian(double mu_left, double mu_right, double variance1, double variance2, double variance3,
                              double variance4, double dBound, bool bLB = true);

#endif //SOLUTION_H
