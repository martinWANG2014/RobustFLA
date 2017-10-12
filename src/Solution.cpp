//
// Created by chenghaowang on 24/07/17.
//
#include "../include/Solution.h"

void writeJsonSolution(String SolutionDir, String dataFilename, vdList parameterList, double epsilon, double coefPi,
                       int feasibleSize, bool modeSigma, double percentAdditionalFlights, int method,
                       int nbFlightsChangeLevel, int nbMaxConflict,
                       int nbMaxDiverseLevels, double SumBenefits, double ElapsedTime, int nbIterations) {
    //check the solutionDir


    using boost::property_tree::ptree;
    using boost::property_tree::write_json;
    ptree root;
    ptree rootSolution;
    ptree rootConfig;

    //write the configuration of solution
    rootConfig.put("dataSource", dataFilename);
    rootConfig.put("minSeparatedDistance", MIN_SEPARATION_DISTANCE);
    rootConfig.put("minAngleCos", MIN_ANGLE);
    rootConfig.put("PeriodLength", Network::PERIOD_LENGTH);
    rootConfig.put("FeasibleListSize", feasibleSize);
    rootConfig.put("alpha", -parameterList[0]);
    rootConfig.put("beta", parameterList[1]);
    rootConfig.put("gamma", doubleToString4(parameterList[2]));
    rootConfig.put("muLeft", doubleToString2(-parameterList[0] * parameterList[3]));
    rootConfig.put("muRight", doubleToString2(parameterList[1] * parameterList[4]));
    rootConfig.put("densityProbaLeft", doubleToString2(parameterList[5]));
    if (method == 0) {
        rootConfig.put("epsilon", "");
    } else {
        rootConfig.put("epsilon", doubleToString2(epsilon));
    }
    rootConfig.put("coefPi", doubleToString2(coefPi));
    rootConfig.put("modeGauss", modeSigma ? "dynamic" : "static");
    switch (method) {
        case 0 :
            rootConfig.put("method", "Deterministic");
            break;
        case 1 :
            rootConfig.put("method", "Hoeffding");
            break;
        case 2 :
            rootConfig.put("method", "MonteCarlo");
            break;
        case 3 :
            rootConfig.put("method", "Gaussian");
            break;
        default:
            rootConfig.put("method", "Unknown");
            break;
    }
    rootConfig.put("percentageAdditionalFlights", percentAdditionalFlights);
    if (method == 2) {
        rootConfig.put("nbIterations", nbIterations);
    }
    root.push_back(std::make_pair("config", rootConfig));

    //write the solution
    rootSolution.put("NbLevelsChange", nbFlightsChangeLevel);
    rootSolution.put("NbMaxConflict", nbMaxConflict);
    rootSolution.put("NbMaxDiverseLevels", nbMaxDiverseLevels);
    rootSolution.put("SumBenefits", doubleToString2(SumBenefits));
    rootSolution.put("ElapsedTime", doubleToString2(ElapsedTime));
    root.push_back(std::make_pair("solution", rootSolution));


    // get the target filename.
    std::stringstream outFilename;
    if (method == 2) {
        outFilename << SolutionDir << "/res_" << method << "_" << Network::PERIOD_LENGTH << "_" << epsilon << "_"
                    << feasibleSize << "_" << coefPi << "_" << parameterList[0] << "_" << parameterList[1] << "_"
                    << parameterList[2] << "_" << parameterList[3] << "_" << parameterList[4] << "_" << parameterList[5]
                    << "_" << (modeSigma ? 1 : 0) << "_" << percentAdditionalFlights << "_" << nbIterations << ".json";
    } else {
        outFilename << SolutionDir << "/res_" << method << "_" << Network::PERIOD_LENGTH << "_" << epsilon << "_"
                    << feasibleSize << "_" << coefPi << "_" << parameterList[0] << "_" << parameterList[1] << "_"
                    << parameterList[2] << "_" << parameterList[3] << "_" << parameterList[4] << "_" << parameterList[5]
                    << "_" << (modeSigma ? 1 : 0) << "_" << percentAdditionalFlights << "_-" << ".json";
    }

    // write to the target file.
    write_json(outFilename.str(), root);

}

void ApproximateFLA(const Network *pNetwork, const vdList &vdParameter, String solutionDir, String dataFilename,
                    double dEpsilon, double dCoefPi,
                    int feasibleSize, double percentAdditionalFlights, double *dSumBenefits, int *iMaxNbConflict,
                    int iModeMethod, bool sigmaMode, bool modeDisplay, int nbIterations) {
//    int iNbFlightNotAssigned = 0;
    int iNbFlightChangeLevel = 0;
    IloEnv env;
    FlightLevelAssignmentMap flightLevelAssignmentMap;
    std::ofstream cplexLogFile("cplexLog.txt", std::ios::out | std::ios::app);


    FlightVector vpFlightList = pNetwork->getFlightsList();
    LevelVector viLevelsList = pNetwork->getFlightLevelsList();

    std::cout << "[INFO] Starting ApproximateFLA method..." << std::endl;
    ProcessClock processClock;
    //Start the process clock.
    processClock.start();
    int iNbFlightNotAssigned = 0;
    int iMaxDiverseLevel = 0;
    try {

        // Try to assign the flights.
        iNbFlightNotAssigned = SolveFLA(vpFlightList, flightLevelAssignmentMap, env, vdParameter, viLevelsList,
                                        processClock, dEpsilon,
                                        dCoefPi,
                                        dSumBenefits, iMaxNbConflict, iModeMethod, modeDisplay, nbIterations);
        // Get the number of flights that change its most preferred flight level.
        iNbFlightChangeLevel = getNbFlightsChangeLevel(vpFlightList);
        iMaxDiverseLevel = getMaxDiverseLevel(vpFlightList);

    } catch (IloException &e) { std::cerr << e.getMessage() << std::endl; }
    catch (...) { std::cerr << "error" << std::endl; }
    cplexLogFile.close();
    env.end();
    processClock.end();
    switch (iModeMethod) {
        case 0:
            std::cout << "Result Det" << std::endl;
            break;
        case 1:
            std::cout << "Result Hoef" << std::endl;
            break;
        case 2:
            std::cout << "Result MC" << std::endl;
            break;
        case 3:
            std::cout << "Result Gaus" << std::endl;
            break;
        default:
            std::cout << "Result nDef" << std::endl;
            break;
    }
    double elapsedTime = processClock.getCpuTime();
    std::cout << "\tNb of flights change level: " << iNbFlightChangeLevel << std::endl;
    std::cout << "\tNb of flights not be assigned: " << iNbFlightNotAssigned << std::endl;
    std::cout << "\tMax conflict: " << *iMaxNbConflict << std::endl;
    std::cout << "\tMax diverse levels: " << iMaxDiverseLevel << std::endl;
    std::cout << "\tSum benefits: " << *dSumBenefits << std::endl;
    std::cout << "\tElapsed time: " << elapsedTime << std::endl << std::endl;
    writeJsonSolution(solutionDir, dataFilename, vdParameter, dEpsilon, dCoefPi, feasibleSize, sigmaMode,
                      percentAdditionalFlights, iModeMethod, iNbFlightChangeLevel, *iMaxNbConflict,
                      iMaxDiverseLevel, *dSumBenefits, elapsedTime, nbIterations);
}

int getNbFlightsChangeLevel(FlightVector &flightVector) {
    int iNbFlightChangeLevel = 0;
    for (auto &&fi : flightVector) {
        if (fi->getCurrentLevel() != fi->getDefaultLevel()) {
            iNbFlightChangeLevel++;
        }
    }
    return iNbFlightChangeLevel;
}

int getMaxDiverseLevel(FlightVector &flightVector) {
    int iMaxDiverseLevel = 0;
    for (auto &&fi : flightVector) {
        int offset = fi->getCurrentLevel() - fi->getDefaultLevel();
        if (offset > iMaxDiverseLevel) {
            iMaxDiverseLevel = offset;
        }
    }
    return iMaxDiverseLevel;
}

bool
SolvingFLAByLevelPP(FlightVector &vpFlightList, FlightsLevelMap &infeasibleFlightMap,
                    FlightVector &vpPreviousFlightList,
                    const IloEnv &env, const vdList &vdParameter, LevelExamine &levelEx,
                    FlightLevelAssignmentMap &flightLevelAssignmentMap, double epsilon,
                    int *iMaxNbConflict,
                    Level iProcessingLevel, int iModeMethod,
                    bool modeDisplay, int nbIterations) {
    //Process the assignment problem for each flight level
    if (modeDisplay) {
        std::cout << "[INFO] Level: " << iProcessingLevel << std::endl;
    }
    FlightVector CandidateFlightList;
    FlightVector ConflictFlightList;
    vdList Mi, Pi;
    double dDelayTime = 0;
    double dDelayTimeMax = 0;
    bool bWait = true;

    //Initialize the candidate flight list in current processing flight level.
    for (auto &&flight: vpFlightList) {
        const LevelVector &levels = flight->getFeasibleLevelList();
        Level iPreferredLevel = flight->getDefaultLevel();

        if (!flightLevelAssignmentMap[flight].first) {
            if (iPreferredLevel == iProcessingLevel) {
                CandidateFlightList.push_back(flight);
            } else {
                auto position = std::find(levels.begin(), levels.end(), iProcessingLevel);
                if (position != levels.end()) {
                    bool valid = true;
                    auto iter = levels.begin();
                    while (valid && iter < position) {
                        valid = levelEx[*iter];
                        iter++;
                    }
                    if (valid) {
                        CandidateFlightList.push_back(flight);
                    }
                }
            }
        } else {
            if (flightLevelAssignmentMap[flight].second == iProcessingLevel) {
                CandidateFlightList.push_back(flight);
            }
        }
    }

    //Remove infeasible flights
    for (auto &&flight:infeasibleFlightMap[iProcessingLevel]) {
        auto position = std::find(CandidateFlightList.begin(), CandidateFlightList.end(), flight);
        if (position != CandidateFlightList.end()) {
            CandidateFlightList.erase(position);
            if (modeDisplay) {
                std::cout << "\tRemove: " << flight->getCode() << std::endl;
            }
        }
    }

    //Initialize the conflicted flight list.
    for (auto &&fi: CandidateFlightList) {
        for (auto &&fj: CandidateFlightList) {
            if (*fi == *fj) {
                continue;
            }
            double prob = fi->CalculateProbabilityConflictAndDelayForFlight(fj, &dDelayTime, &dDelayTimeMax, &bWait);
            if (prob > 0) {
                ConflictFlightList.push_back(fi);
                break;
            }
        }
    }

    levelEx[iProcessingLevel] = true;
    if (modeDisplay) {
        std::cout << "\tCandidateFlights: " << CandidateFlightList.size() << std::endl;
        std::cout << "\tConflictFlights: " << ConflictFlightList.size() << std::endl;
    }

    for (auto &&flight:ConflictFlightList) {
        if (!contains(vpPreviousFlightList, flight)) {
            bool conflict = false;
            for (auto &&fj : ConflictFlightList) {
                if (*flight == *fj) {
                    continue;
                }
                double prob = flight->CalculateProbabilityConflictAndDelayForFlight(fj, &dDelayTime, &dDelayTimeMax,
                                                                                    &bWait);
                if (prob > 0) {
                    conflict = true;
                }
            }
            if (conflict) {
                flightLevelAssignmentMap[flight].first = false;
                flightLevelAssignmentMap[flight].second = -1;
            }
        }
    }

    vpPreviousFlightList.clear();
    for (auto &&flight:ConflictFlightList) {
        vpPreviousFlightList.push_back(flight);
    }

    //Assign all conflict-free flight.
    for (auto &&flight: CandidateFlightList) {
        if (!contains(ConflictFlightList, flight)) {
            flight->setCurrentLevel(iProcessingLevel);
            flightLevelAssignmentMap[flight].first = true;
            flightLevelAssignmentMap[flight].second = iProcessingLevel;
        }
    }

    //If the conflict flight list size equal 0, then go to process the next flight level.
    int iConflictFlightsSize = (int) ConflictFlightList.size();
    if (iConflictFlightsSize == 0 && modeDisplay) {
        std::cout << "\tNo conflict flights" << std::endl;
        return true;
    }

    double **ppdConflictProbability = CreateTable(iConflictFlightsSize);
    double **ppdDelayTime = CreateTable(iConflictFlightsSize);
    double **ppdDelayTimeMax = CreateTable(iConflictFlightsSize);
    InitTable(ppdConflictProbability, iConflictFlightsSize);
    InitTable(ppdDelayTime, iConflictFlightsSize);
    InitTable(ppdDelayTimeMax, iConflictFlightsSize);
    CalculateConflictProbability(ConflictFlightList, ppdConflictProbability, ppdDelayTime, ppdDelayTimeMax,
                                 iModeMethod == 0);

    //Calculate Mi list and maximal conflict count.
    for (int i = 0; i < iConflictFlightsSize; i++) {
        double sumPenalCost = 0.0;
        int iConflictCount = 0;
        for (int j = 0; j < iConflictFlightsSize; j++) {
            if (i != j && ppdConflictProbability[i][j] > 0) {
                sumPenalCost += std::max(0.0, ppdDelayTime[i][j]);
                iConflictCount++;
            }
        }
        Mi.push_back(sumPenalCost);
        if (iConflictCount > *iMaxNbConflict) {
            *iMaxNbConflict = iConflictCount;
        }
    }
    //Calculate Pi list.
    for (auto &&fi: ConflictFlightList) {
        Pi.push_back(std::max(60.0, fi->getDuration()) * fi->getCoefPi());
    }

    viList viConstraintList;
    //Relax the most infeasible constraint.
    int iMinIndexArgs = MinIndexArgs0(ConflictFlightList, Pi, ppdConflictProbability, ppdDelayTime,
                                      modeDisplay);
    if (iMinIndexArgs == -1) {
        DestroyTable(ppdConflictProbability, iConflictFlightsSize);
        DestroyTable(ppdDelayTime, iConflictFlightsSize);
        return true;
    }
    if (modeDisplay) {
        std::cout << "\t\t\tStart" << std::endl;
        std::cout << "\t\t[INFO] Index: " << iMinIndexArgs << std::endl;
    }
    IloNumArray decisionVariablesValues;
    Solver *solver;
    if (iModeMethod > 0) {
        viConstraintList.push_back(iMinIndexArgs);
        solver = new Solver(env);
        solver->initDecisionVariables(iConflictFlightsSize);
        solver->initFunctionObjective(ConflictFlightList, iProcessingLevel);
        solver->initConstraint(viConstraintList, Mi, Pi, ppdConflictProbability, ppdDelayTime, iConflictFlightsSize);
        solver->prefixAssignedFlight(ConflictFlightList, flightLevelAssignmentMap, iProcessingLevel);
        solver->solve();
        decisionVariablesValues = solver->getDecisionVariablesValues();
        delete solver;
    }
    int iMinIndexArgsFromFeaCheck = -1;
    bool feasible;
    //Test de feasibility of each constraint.
    //Relax the most infeasible constraint, until the solutionC has a high expected confidence.
    switch (iModeMethod) {
        case 0:
            /**
             * Deterministic Method
             */
            viConstraintList.clear();
            for (int i = 0; i < (int) ConflictFlightList.size(); i++) {
                viConstraintList.push_back(i);
            }
            solver = new Solver(env);
            solver->initDecisionVariables(iConflictFlightsSize);
            solver->initFunctionObjective(ConflictFlightList, iProcessingLevel);
            solver->initConstraint(viConstraintList, Mi, Pi, ppdConflictProbability, ppdDelayTimeMax,
                                   iConflictFlightsSize);
            solver->solve();
            decisionVariablesValues = solver->getDecisionVariablesValues();
            delete solver;
            break;
        case 1:/*Hoeffding*/
            while (true) {
                feasible = FeasibilityHoeffding(Pi, decisionVariablesValues, vdParameter,
                                                ppdConflictProbability, ppdDelayTime,
                                                ppdDelayTimeMax,
                                                epsilon, iConflictFlightsSize, &iMinIndexArgsFromFeaCheck, modeDisplay);
                if (feasible) {
                    break;
                } else {
                    if (contains(viConstraintList, iMinIndexArgsFromFeaCheck) ||
                        (int) viConstraintList.size() == iConflictFlightsSize) {
                        infeasibleFlightMap[iProcessingLevel].push_back(ConflictFlightList[iMinIndexArgsFromFeaCheck]);
                        DestroyTable(ppdConflictProbability, iConflictFlightsSize);
                        DestroyTable(ppdDelayTime, iConflictFlightsSize);
                        return false;
                    }
                }
                if (modeDisplay) {
                    std::cout << "\t\t[INFO] Index: " << iMinIndexArgsFromFeaCheck << std::endl;
                }
                viConstraintList.push_back(iMinIndexArgsFromFeaCheck);
                //ReInitialize the model, then resolve it.
                solver = new Solver(env);
                solver->initDecisionVariables(iConflictFlightsSize);
                solver->initFunctionObjective(ConflictFlightList, iProcessingLevel);
                solver->initConstraint(viConstraintList, Mi, Pi, ppdConflictProbability, ppdDelayTime,
                                       iConflictFlightsSize);
                solver->prefixAssignedFlight(ConflictFlightList, flightLevelAssignmentMap, iProcessingLevel);
                solver->solve();
                decisionVariablesValues = solver->getDecisionVariablesValues();
                delete solver;
            }
            break;
        case 3:/*Gaussian*/
            while (true) {
                feasible = FeasibilityGaussian(ConflictFlightList, Pi, decisionVariablesValues,
                                               ppdConflictProbability, ppdDelayTime,
                                               epsilon, &iMinIndexArgsFromFeaCheck, modeDisplay);
                if (feasible) {
                    break;
                } else {
                    if (contains(viConstraintList, iMinIndexArgsFromFeaCheck) ||
                        (int) viConstraintList.size() == iConflictFlightsSize) {
                        infeasibleFlightMap[iProcessingLevel].push_back(ConflictFlightList[iMinIndexArgsFromFeaCheck]);
                        DestroyTable(ppdConflictProbability, iConflictFlightsSize);
                        DestroyTable(ppdDelayTime, iConflictFlightsSize);
                        return false;
                    }
                }
                if (modeDisplay) {
                    std::cout << "\t\t[INFO] Index: " << iMinIndexArgsFromFeaCheck << std::endl;
                }
                viConstraintList.push_back(iMinIndexArgsFromFeaCheck);
                //ReInitialize the model, then resolve it.
                solver = new Solver(env);
                solver->initDecisionVariables(iConflictFlightsSize);
                solver->initFunctionObjective(ConflictFlightList, iProcessingLevel);
                solver->initConstraint(viConstraintList, Mi, Pi, ppdConflictProbability, ppdDelayTime,
                                       iConflictFlightsSize);
                solver->prefixAssignedFlight(ConflictFlightList, flightLevelAssignmentMap, iProcessingLevel);
                solver->solve();
                decisionVariablesValues = solver->getDecisionVariablesValues();
                delete solver;
            }
            break;
        case 2:/*MonteCarlo with departure time*/
            while (true) {
                feasible = FeasibilityMonteCarlo(ConflictFlightList, viConstraintList, Pi, decisionVariablesValues,
                                                 vdParameter, epsilon, &iMinIndexArgsFromFeaCheck, modeDisplay,
                                                 nbIterations);
                if (feasible) {
                    break;
                } else {
                    if (contains(viConstraintList, iMinIndexArgsFromFeaCheck) ||
                        (int) viConstraintList.size() == iConflictFlightsSize) {
                        infeasibleFlightMap[iProcessingLevel].push_back(ConflictFlightList[iMinIndexArgsFromFeaCheck]);
                        DestroyTable(ppdConflictProbability, iConflictFlightsSize);
                        DestroyTable(ppdDelayTime, iConflictFlightsSize);
                        return false;
                    }
                }
                if (modeDisplay) {
                    std::cout << "\t\t[INFO] Index: " << iMinIndexArgsFromFeaCheck << std::endl;
                }
                viConstraintList.push_back(iMinIndexArgsFromFeaCheck);
                //ReInitialize the model, then resolve it.
                solver = new Solver(env);
                solver->initDecisionVariables(iConflictFlightsSize);
                solver->initFunctionObjective(ConflictFlightList, iProcessingLevel);
                solver->initConstraint(viConstraintList, Mi, Pi, ppdConflictProbability, ppdDelayTime,
                                       iConflictFlightsSize);
                solver->prefixAssignedFlight(ConflictFlightList, flightLevelAssignmentMap, iProcessingLevel);
                solver->solve();
                decisionVariablesValues = solver->getDecisionVariablesValues();
                delete solver;
            }
            break;
        default:
            std::cerr << "Not support ! " << std::endl;
            abort();
            break;
    }

    DestroyTable(ppdConflictProbability, iConflictFlightsSize);
    DestroyTable(ppdDelayTime, iConflictFlightsSize);
    DestroyTable(ppdDelayTimeMax, iConflictFlightsSize);
    for (int i = 0; i < iConflictFlightsSize; i++) {
        if (decisionVariablesValues[i] == 1) {
            Flight *fi = ConflictFlightList[i];
            fi->setCurrentLevel(iProcessingLevel);
            flightLevelAssignmentMap[fi].first = true;
            flightLevelAssignmentMap[fi].second = iProcessingLevel;
        }
    }
    return true;
}

int SolveFLA(FlightVector &vpFlightList, FlightLevelAssignmentMap &flightLevelAssignmentMap, const IloEnv &env,
             const vdList &vdParameter,
             LevelVector &viLevelsList, ProcessClock &processClock, double epsilon, double dCoefPi,
             double *dSumBenefits, int *iMaxNbConflict,
             int iModeMethod, bool modeDisplay, int nbIterations) {
    LevelExamine levelEx;
    FlightsLevelMap infeasibleFlightMap;
    LevelQueue queueLevel;
    FlightVector vpPreviousFlightList;
    *dSumBenefits = 0;
    *iMaxNbConflict = 0;

    //Initialize the flight assignment map.
    for (auto &&flight: vpFlightList) {
        flightLevelAssignmentMap[flight] = std::make_pair(false, -1);
    }

    //Initialize the examine flight levels list.
    for (auto &&level: viLevelsList) {
        levelEx[level] = false;
    }

    //Initialize current flight level for each flight as its most preferred one.
    for (auto &&fi: vpFlightList) {
        fi->resetLevel();
    }

    //Initialize the infeasible flight level map
    for (auto &&level: viLevelsList) {
        FlightVector infeasibleFlight;
        infeasibleFlightMap.insert(std::make_pair(level, infeasibleFlight));
    }

    int iNbFlightNotAssigned = 1;
    int iNbFlightChangeLevel;
    int count = 0;
    while (iNbFlightNotAssigned > 0) {
        //Initialize the flight level queue.
        for (auto &&processingLevel : viLevelsList) {
            queueLevel.push_back(processingLevel);
        }
        infeasibleFlightMap.clear();
        while (!queueLevel.empty()) {
            Level processingLevel = queueLevel.front();
            queueLevel.pop_front();
            bool solved = false;
            while (!solved) {
                solved = SolvingFLAByLevelPP(vpFlightList, infeasibleFlightMap, vpPreviousFlightList,
                                             env, vdParameter,
                                             levelEx, flightLevelAssignmentMap, epsilon, iMaxNbConflict,
                                             processingLevel, iModeMethod, modeDisplay, nbIterations);
            }
        }
        FlightVector flightNotAssigned;
        for (auto &&fi: flightLevelAssignmentMap) {
            if (!fi.second.first) {
                flightNotAssigned.push_back(fi.first);
                fi.first->setCoefPi(std::max(fi.first->getCoefPi() + 0.01, 0.20));
            }
        }
        // Get the number of flights that change its most preferred flight level.
        iNbFlightChangeLevel = getNbFlightsChangeLevel(vpFlightList);
        // Get the number of unassigned flights.
        if ((int) flightNotAssigned.size() == iNbFlightNotAssigned) {
            count++;
            if (count > 1) {
                for (auto &&fi : flightNotAssigned) {
                    Level newLevel = findNextFeasibleLevel(fi->getDefaultLevel(), fi->getLastFeasibleLevel());
                    if (newLevel != -1) {
                        fi->addNewFeasibleLevel(newLevel);
                        if (!contains(viLevelsList, newLevel)) {
                            viLevelsList.push_back(newLevel);
                        }
                    }
                }
                std::cout << "enlarge level" << std::endl;
                count = 0;
            }
        } else {
            count = 0;
        }
        iNbFlightNotAssigned = (int) flightNotAssigned.size();
        std::cout << "Solve the problem P':" << std::endl;
        std::cout << "\tNb of flights change level: " << iNbFlightChangeLevel << std::endl;
        std::cout << "\tNb of flights not be assigned: " << iNbFlightNotAssigned << std::endl;
        processClock.end();
        std::cout << "\tElapsed time: " << processClock.getCpuTime() << std::endl;
        if (iModeMethod == 2) {
            std::cout << "NbIteration: " << nbIterations;
        }
    }
    for (auto &&fi: flightLevelAssignmentMap) {
        auto index = std::find(fi.first->getFeasibleLevelList().begin(), fi.first->getFeasibleLevelList().end(),
                               fi.second.second);
        if (index != fi.first->getFeasibleLevelList().end()) {
            long lIndex = index - fi.first->getFeasibleLevelList().begin();
            *dSumBenefits += exp(3 - lIndex);
        }
    }
    return iNbFlightNotAssigned;
}

bool FeasibilityMonteCarlo(FlightVector &vpConflictedFlightList, const viList &viConstraintList, const vdList &vdPi,
                           const IloNumArray &decisionVariables, const vdList &vdParameter, double dEpsilon,
                           int *piIndex, bool modeDisplay, int nbIteration) {
    int iConflictedFlightsSize = (int) vpConflictedFlightList.size();
    int iConflictedCounter = 0;
//    int iMaxConflict = -1;
    bool feasible = true;
    *piIndex = -1;
    double **ppdConflictProbability = CreateTable(iConflictedFlightsSize);
    double **ppdDelayTime = CreateTable(iConflictedFlightsSize);
    double **ppdDelayTimeMax = CreateTable(iConflictedFlightsSize);
    viList table;
    for (int i = 0; i < iConflictedFlightsSize; i++) {
        table.push_back(0);
    }
    //Simulate 1000 scenarios
    for (int a = 0; a < nbIteration; a++) {
        InitTable(ppdDelayTime, iConflictedFlightsSize);
        InitTable(ppdDelayTimeMax, iConflictedFlightsSize);
        InitTable(ppdConflictProbability, iConflictedFlightsSize);
        bool test = true;
        //Generate a new scenario from current flight list.
        for (auto fj : vpConflictedFlightList) {
//        for (int i = 0; i < iConflictedFlightsSize; i++) {
//            if (decisionVariables[i] == 1 && !contains(viConstraintList, i)) {
//                Flight *fj = vpConflictedFlightList[i];
                Time iOldDepartureTime = fj->getDepartureTime();
                Time iDelta = 0;
                iDelta = GaussianDistribution3(vdParameter, fj->getSigmaPrime());
                fj->GenerateNewFlight(iOldDepartureTime + iDelta);
//            }
        }
        CalculateConflictProbability(vpConflictedFlightList, ppdConflictProbability, ppdDelayTime, ppdDelayTimeMax,
                                     false);

        for (int i = 0; i < iConflictedFlightsSize; i++) {
            if (decisionVariables[i] == 1) {
//                std::cout << "assign i : "<< i << std::endl;
                double sum = 0;
                for (int j = 0; j < iConflictedFlightsSize; j++) {
                    if (j != i && ppdConflictProbability[i][j] > 0 && decisionVariables[j] == 1) {
                        sum += std::max(0.0, ppdDelayTime[i][j]);
//                        std::cout << ppdConflictProbability[i][j] << "\t" << ppdDelayTime[i][j]<<std::endl;
                    }
                }
                if (sum > vdPi[i]) {
                    test = false;
                    table[i] = table[i] + 1;
                    break;
                }
            }
        }

        if (!test) {
            iConflictedCounter++;
        }
        for (auto &&fj: vpConflictedFlightList) {
            fj->resetRouteTimeList();
        }
        if (dEpsilon == 0.05) {
            if ((nbIteration - iConflictedCounter) / (nbIteration * 1.0) < 0.961) {
                feasible = false;
            }
        }
        if (dEpsilon == 0.10) {
            if ((nbIteration - iConflictedCounter) / (nbIteration * 1.0) < 0.915) {
                feasible = false;
            }
        }
        if (dEpsilon == 0.15) {
            if ((nbIteration - iConflictedCounter) / (nbIteration * 1.0) < 0.868) {
                feasible = false;
            }
        }
        if (dEpsilon == 0.20) {
            if ((nbIteration - iConflictedCounter) / (nbIteration * 1.0) < 0.821) {
                feasible = false;
            }
        }
        if (dEpsilon == 0.25) {
            if ((nbIteration - iConflictedCounter) / (nbIteration * 1.0) < 0.772) {
                feasible = false;
            }
        }
        if (!feasible) {
            break;
        }
    }
    for (int item = 0; item < iConflictedFlightsSize; item++) {
        if (decisionVariables[item] == 0) {
            table[item] = 0;
        }
//        if (decisionVariables[item] == 1 && table[item] > iMaxConflict) {
//            iMaxConflict = table[item];
//            *piIndex = item;
//        }
    }
    auto pos_max = std::max_element(table.begin(), table.end());
    *piIndex = int(pos_max - table.begin());
    if (modeDisplay) {
        std::cout << "i: " << *piIndex << std::endl;
    }
    table.clear();
    DestroyTable(ppdDelayTime, iConflictedFlightsSize);
    DestroyTable(ppdDelayTimeMax, iConflictedFlightsSize);
    DestroyTable(ppdConflictProbability, iConflictedFlightsSize);
    if (modeDisplay) {
        std::cout << "\t\t\tCount: " << (nbIteration - iConflictedCounter) / (nbIteration * 1.0) << std::endl;
    }
//    if (dEpsilon == 0.05) {
//        if (1000 - iConflictedCounter < 961) {
//            return false;
//        }
//    }
//    if (dEpsilon == 0.1) {
//        if (1000 - iConflictedCounter < 915) {
//            return false;
//        }
//    }
//    if (dEpsilon == 0.15) {
//        if (1000 - iConflictedCounter < 868) {
//            return false;
//        }
//    }
//    if (dEpsilon == 0.2) {
//        if (1000 - iConflictedCounter < 821) {
//            return false;
//        }
//    }
//    if (dEpsilon == 0.25) {
//        if (1000 - iConflictedCounter < 772) {
//            return false;
//        }
//    }

    if (modeDisplay && feasible) {
        std::cout << "\t\t\tFeasibility ==> ok!" << std::endl;
    }
    return feasible;
}

bool FeasibilityGaussian(FlightVector &vpConflictedFlightList, const vdList &vdPi,
                         const IloNumArray &decisionVariables, double **ppdConflictProbability, double **ppdDelayTime,
                         double dEpsilon, int *piIndex, bool modeDisplay) {
    int iSize = (int) vpConflictedFlightList.size();
    *piIndex = -1;
//    double dMin = 1;
//    bool feasible = true;
    for (int i = 0; i < iSize; i++) {
        if (int(decisionVariables[i]) == 1) {
//            std::cout << "assign i : "<< i << std::endl;
            double exp_mu = 0.0;
            double exp_sigma_2 = 0.0;
            int iCounter = 0;
            double dLeftConstrainedFeasibility = 1;
            double mu;
            double sigma_2;
            for (int j = 0; j < iSize; j++) {
                if (ppdConflictProbability[i][j] > 0 && decisionVariables[j] == 1 && ppdDelayTime[i][j] > 0) {
                    mu = ppdDelayTime[i][j];
                    sigma_2 = (pow(vpConflictedFlightList[i]->getSigma(), 2) +
                               pow(vpConflictedFlightList[j]->getSigma(), 2));
                    dLeftConstrainedFeasibility *= 0.5 * (boost::math::erf((vdPi[i] - mu) / (sqrt(2 * sigma_2))) + 1);
                    exp_mu += mu;
                    exp_sigma_2 += sigma_2;
                    iCounter++;
                }
            }
            double dFeasibility = 0.5 * (boost::math::erf((vdPi[i] - exp_mu) / (sqrt(2 * exp_sigma_2))) + 1);
            if (iCounter > 1) {
                dFeasibility *= dLeftConstrainedFeasibility;
            }
            if (modeDisplay) {
                std::cout << "\t\t\ti: " << i << "==>Feas: " << dFeasibility << "===> Eps: " << 1 - dEpsilon
                          << std::endl;
            }
            if (dFeasibility < 1.0 - dEpsilon) {
//                if (dFeasibility < dMin) {
//                    *piIndex = i;
//                }
//                if (dFeasibility < 0.5) {
//                    return false;
//                }
//                feasible = false;
                *piIndex = i;
                return false;
            }
        }
    }
//    if (!feasible)
//        return false;
    if (modeDisplay) {
        std::cout << "\t\t\tFeasibility ==> ok!" << std::endl;
    }
    return true;
}

bool FeasibilityHoeffding(const vdList &vdPi,
                          const IloNumArray &decisionVariables, const vdList &vdParameter,
                          double **ppdConflictProbability, double **ppdDelayTime, double **ppdDelayTimeMax,
                          double dEpsilon,
                          int iConflictedFlightSize, int *piIndex, bool modeDisplay) {
    *piIndex = -1;
    for (int i = 0; i < iConflictedFlightSize; i++) {
        double dInFeasibility = 0;
        double temp2 = 0;
        if (int(decisionVariables[i]) == 1) {
//            std::cout << "assign i : "<< i << std::endl;
            for (int j = 0; j < iConflictedFlightSize; j++) {
                if (ppdConflictProbability[i][j] > 0) {
                    double maxPenalCost = std::max(0.0, ppdDelayTimeMax[i][j]) * decisionVariables[j] -
                                          std::max(ppdDelayTime[i][j] - vdParameter[1] - vdParameter[2], 0.0) *
                                          decisionVariables[j];
                    dInFeasibility += ppdConflictProbability[i][j] * maxPenalCost / 2;
                    temp2 += pow(maxPenalCost, 2);
                }
            }
            dInFeasibility = exp(-(2 * pow(vdPi[i] - dInFeasibility, 2)) / temp2);
            if (modeDisplay) {
                std::cout << "\t\t\ti: " << i << "==>Feas: " << 1 - dInFeasibility << "===> Eps: " << 1 - dEpsilon
                          << std::endl;
            }
            if (dInFeasibility > dEpsilon) {
                *piIndex = i;
                return false;
            }
        }
    }
    if (modeDisplay) {
        std::cout << "\t\t\tFeasibility ==> ok!" << std::endl;
    }
    return true;
}

bool FeasibilityEnumeration(const vdList &vdPi, const IloNumArray &decisionVariables,
                            double **ppdConflictProbability, double **ppdDelayTime, double dEpsilon,
                            int iConflictedFlightsSize, int *indexMostInfeasible, bool modeDisplay) {
    *indexMostInfeasible = -1;

    //Iterate all conflict flights in current flight level
    for (int i = 0; i < iConflictedFlightsSize; i++) {
        //For each assigned flight(x[i] = 1) in current level of last solutionC
        if (int(decisionVariables[i]) == 1) {
//            std::cout << "assign i : "<< i << std::endl;
            int iConflictFlightsOfI = 0;
            viList viCandidateConflictFlightOfI;
            double dInFeasibility = 0;
            double dSumPenalCost = 0;
            for (int j = 0; j < iConflictedFlightsSize; j++) {
                if (j != i && ppdConflictProbability[i][j] > 0) {
                    dSumPenalCost += std::max(0.0, ppdDelayTime[i][j]) * decisionVariables[j];
                    viCandidateConflictFlightOfI.push_back(j);
                    iConflictFlightsOfI++;
                }
            }
            bool bNotFinish = true;
            int k = iConflictFlightsOfI;
            //If the constraint is infeasible, then test all its combinations
            // util all the combination with k items will be feasible.
            if (dSumPenalCost > vdPi[i]) {
                while (bNotFinish && k > 0) {
                    qviList qviCombinationList = Combination(viCandidateConflictFlightOfI, k);
                    bool bValid = false;
                    //For each case of combination, test the occurrence probability.
                    for (auto subCombination: qviCombinationList) {
                        double dOccurrenceProbability = 1;
                        double dSumPenalCostP = 0.0;
                        viList viComplementList = getComplement((int) viCandidateConflictFlightOfI.size(),
                                                                subCombination);
                        for (auto indexJ: subCombination) {
                            int j = viCandidateConflictFlightOfI[indexJ];
                            dOccurrenceProbability *= ppdConflictProbability[i][j];
                            dSumPenalCostP += std::max(0.0, ppdDelayTime[i][j]);
                        }
                        for (auto indexJ: viComplementList) {
                            int j = viCandidateConflictFlightOfI[indexJ];
                            dOccurrenceProbability *= 1 - ppdConflictProbability[i][j];
                        }
                        if (dSumPenalCostP > vdPi[i]) {
                            bValid = true;
                            dInFeasibility += dOccurrenceProbability;
                        }
                    }
                    k--;
                    bNotFinish = bValid;
                }
                if (modeDisplay) {
                    std::cout << "\t\t\ti: " << i << "==>Feas: " << 1 - dInFeasibility << "===> Eps: " << 1 - dEpsilon
                              << std::endl;
                }
                if (dInFeasibility > dEpsilon) {
                    *indexMostInfeasible = i;
                    return false;
                }
            }
        }
    }
    if (modeDisplay) {
        std::cout << "\t\t\tFeasibility ==> ok!" << std::endl;
    }
    return true;
}

int MinIndexArgs1(FlightVector &vpConflictFlightList, const viList &contraintList, const vdList &vdPi,
                  const IloNumArray &decisionVariables, double **ppdConflictProbability, double **ppdDelayTime,
                  bool bMethod) {
    int iMinIndex = -1;
    int iConflictedFlightSize = (int) vpConflictFlightList.size();
    double dMaxValue = std::numeric_limits<double>::max();
    if (bMethod) {
        for (int index = 0; index < (int) vpConflictFlightList.size(); index++) {
//        for (auto &&index: viSearchList) {
            double dSumPenalCost = 0;

            for (int j = 0; j < iConflictedFlightSize; j++) {
                if (ppdConflictProbability[index][j] > 0) {
                    dSumPenalCost += std::max(0.0, ppdDelayTime[index][j]) * ppdConflictProbability[index][j] *
                                     decisionVariables[j] / 2;
                }
            }

            //i : min Pi-E[sum{p_ij}]
            dSumPenalCost = vdPi[index] - dSumPenalCost;
            if (dSumPenalCost < dMaxValue && !contains(contraintList, index)) {
                dMaxValue = dSumPenalCost;
                iMinIndex = index;
            }
        }
    } else {
        for (int index = 0; index < (int) vpConflictFlightList.size(); index++) {
//        for (auto &&index: viSearchList) {
            double dSumPenalCost = 0;

            for (int j = 0; j < iConflictedFlightSize; j++) {
                if (ppdConflictProbability[index][j] > 0) {
                    dSumPenalCost += std::max(0.0, ppdDelayTime[index][j]) *
                                     decisionVariables[j];
                }
            }

            //i : min Pi-E[sum{p_ij}]
            dSumPenalCost = vdPi[index] - dSumPenalCost;
            if (dSumPenalCost < dMaxValue && !contains(contraintList, index)) {
                dMaxValue = dSumPenalCost;
                iMinIndex = index;
            }
        }
    }
//    if (iMinIndex == -1) {
//        std::cerr << "[ERROR] the selected index invalid !" << std::endl;
//    }
    return iMinIndex;
}

int MinIndexArgs0(FlightVector &vpConflictFlightList, const vdList &vdPi,
                  double **ppdConflictProbability, double **ppdDelayTime, bool modeDisplay) {
    int iMinIndex = -1;
    int iConflictedFlightSize = (int) vpConflictFlightList.size();
    double dMaxValue = std::numeric_limits<double>::max();

    for (int index = 0; index < (int) vpConflictFlightList.size(); index++) {
        double dSumPenalCost = 0;

        for (int j = 0; j < iConflictedFlightSize; j++) {
            if (ppdConflictProbability[index][j] > 0) {
                dSumPenalCost += std::max(0.0, ppdDelayTime[index][j]);
            }
        }

        //i : min Pi-E[sum{p_ij}]
        dSumPenalCost = vdPi[index] - dSumPenalCost;
        if (dSumPenalCost < dMaxValue) {
            dMaxValue = dSumPenalCost;
            iMinIndex = index;
        }
    }
    return iMinIndex;
}

void CalculateConflictProbability(FlightVector &vpConflictFlightList, double **ppdConflictProbability,
                                  double **ppdDelayTime, double **ppdDelayTimeMax, bool deterministic) {
    int iSize = (int) vpConflictFlightList.size();
    bool bWait = true;
    double dDelay = 0;
    double dDelayMax = 0;
    for (int i = 0; i < iSize - 1; i++) {
        Flight *fi = vpConflictFlightList[i];
        for (int j = i + 1; j < iSize; j++) {
            Flight *fj = vpConflictFlightList[j];
            ppdConflictProbability[i][j] = ppdConflictProbability[j][i] = fi->CalculateProbabilityConflictAndDelayForFlight(
                    fj, &dDelay, &dDelayMax, &bWait, deterministic);
            if (bWait) {
                ppdDelayTime[i][j] = dDelay;
                ppdDelayTime[j][i] = 0.0;//std::min(-dDelay, 0.0);
                ppdDelayTimeMax[i][j] = dDelayMax;
                ppdDelayTimeMax[j][i] = 0.0;//std::min(-dDelayMax, 0.0);
            } else {
                ppdDelayTime[i][j] = 0.0;//std::min(-dDelay, 0.0);
                ppdDelayTime[j][i] = dDelay;
                ppdDelayTimeMax[i][j] = 0.0;// std::min(-dDelayMax, 0.0);
                ppdDelayTimeMax[j][i] = dDelayMax;
            }
        }
    }
}

void DestroyTable(double **ppdTable, int iSize) {
    for (int i = 0; i < iSize; i++) {
        delete[] ppdTable[i];
    }
    delete[] ppdTable;
}

void InitTable(double **ppdTable, int iSize) {
    for (int i = 0; i < iSize; i++) {
        for (int j = 0; j < iSize; j++) {
            ppdTable[i][j] = 0;
        }
    }
}

double **CreateTable(int iSize) {
    double **ppdDouble = new double *[iSize];
    for (int i = 0; i < iSize; i++) {
        ppdDouble[i] = new double[iSize];
    }
    return ppdDouble;
}

double GaussianDistribution3(const vdList &vdParameter, double dSigma) {
//    std::default_random_engine generator;
    std::random_device rd;
    std::mt19937 generator(rd());
    uni_dist UniformDist(0, 1);
    double p = UniformDist(generator);
    double delta = 0.0;
    if (p < vdParameter[5]) {
        normal_dist NormalDist(vdParameter[0] * vdParameter[3],
                               dSigma);
        delta = -fabs(NormalDist(generator));
    } else {
        normal_dist NormalDist(vdParameter[1] * vdParameter[4],
                               dSigma);
        delta = fabs(NormalDist(generator));
    }
    return delta;
}

double GaussianDistribution2(const vdList &vdParameter, double dSigma) {
    std::random_device rd;
    std::mt19937 generator(rd());
    normal_dist NormalDist(0, dSigma);
    return NormalDist(generator);
}

double GaussianDistribution1(const vdList &vdParameter, double dSigma) {
    std::random_device rd;
    std::mt19937 generator(rd());
    normal_dist NormalDist((vdParameter[1] - vdParameter[0]) / 2,
                           dSigma);
    return NormalDist(generator);
}

viList getComplement(int iSize, viList &candidateList) {
    viList resultList;
    for (int i = 0; i < iSize; i++) {
        auto index = std::find(candidateList.begin(), candidateList.end(), i);
        if (index == candidateList.end()) {
            resultList.push_back(i);
        }
    }
    return resultList;
}

qviList Combination(const viList &viConstraintList, int k) {
    qviList qviCombinationList;
    //Initialize the deque with one item.
    for (int i = 0; i < (int) viConstraintList.size() - k + 1; i++) {
        viList candidateList;
        candidateList.push_back(i);
        qviCombinationList.push_back(candidateList);
    }
    //Do a combination for each item in deque until all items have a size of k.
    while ((int) qviCombinationList.front().size() < k) {
        int iValue = qviCombinationList.front()[qviCombinationList.front().size() - 1];
        for (int i = iValue + 1;
             i < (int) viConstraintList.size() - k + 1 + (int) qviCombinationList.front().size(); i++) {
            viList candidateList = viList((viList &&) qviCombinationList.front());
            candidateList.push_back(i);
            qviCombinationList.push_back(candidateList);
        }
        qviCombinationList.pop_front();
    }
    return qviCombinationList;
}
