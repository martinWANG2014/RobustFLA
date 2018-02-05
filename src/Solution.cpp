//
// Created by chenghaowang on 24/07/17.
//
#include "../include/Solution.h"

void writeJsonSolution(String dataFilename, double epsilon, double coefPi, double SumBenefits, double ElapsedTime,
                       double minAdmissibleCost, double maxAdmissibleCost, int feasibleSize, int method,
                       int percentileSup, int nbFlightsChangeLevel, int nbFlightsUnassigned, int nbMaxConflict,
                       int nbMaxDiverseLevels,
                       int nbIterations, const double timeout, const int maxcount, const bool tryAllAssign) {
    using boost::property_tree::ptree;
    using boost::property_tree::write_json;
    ptree root;
    ptree rootSolution;
    ptree rootConfig;

    //write the configuration of solution
    rootConfig.put("dataSource", dataFilename);
    rootConfig.put<double>("minSeparatedDistance", MIN_SEPARATION_DISTANCE);
    rootConfig.put<double>("minAngleCos", MIN_ANGLE);
    rootConfig.put<double>("PeriodLength", Network::PERIOD_LENGTH);
    rootConfig.put<int>("FeasibleListSize", feasibleSize);
    rootConfig.put<double>("SolutionTimeOut", timeout);
    rootConfig.put<int>("MaxCountForUnImprovedSol", maxcount);
    if (tryAllAssign) {
        rootConfig.put("TryAllAssign", "yes");
    } else {
        rootConfig.put("TryAllAssign", "no");
    }
    if (method == 0) {
        rootConfig.put("epsilon", "-");
    } else {
        rootConfig.put<double>("epsilon", epsilon);
    }
    rootConfig.put<double>("coefPi", coefPi);
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
            rootConfig.put("method", "GaussianLike");
            break;
        case 5 :
            rootConfig.put("method", "GaussianLike2");
            break;
        case 6 :
            rootConfig.put("method", "Sampling");
            break;
        case 4 :
            rootConfig.put("method", "EnhancedHoeffding");
            break;
        default:
            rootConfig.put("method", "Unknown");
            break;
    }
    rootConfig.put<double>("percentageAdditionalFlights", percentileSup);
    if (method == 2) {
        rootConfig.put<int>("nbIterations", nbIterations);
    }
    root.push_back(std::make_pair("config", rootConfig));

    //write the solution
    rootSolution.put<int>("NbLevelsChange", nbFlightsChangeLevel);
    rootSolution.put<int>("NbFlightUnassigned", nbFlightsUnassigned);
    rootSolution.put<int>("NbMaxConflict", nbMaxConflict);
    rootSolution.put<int>("NbMaxDiverseLevels", nbMaxDiverseLevels);
    rootSolution.put<double>("SumBenefits", SumBenefits);
    rootSolution.put<double>("ElapsedTime", ElapsedTime);
    rootSolution.put<double>("MinAdmissibleCost", minAdmissibleCost);
    rootSolution.put<double>("MaxAdmissibleCost", maxAdmissibleCost);
    root.push_back(std::make_pair("solution", rootSolution));


    // get the target filename.
    std::stringstream outFilename;
    if (method == 2) {
        outFilename << "../solution/res_" << method << "_" << Network::PERIOD_LENGTH << "_" << epsilon << "_"
                    << feasibleSize << "_" << coefPi << "_" << minAdmissibleCost << "_" << maxAdmissibleCost << "_"
                    << percentileSup << "_" << nbIterations << ".json";
    } else {
        outFilename << "../solution/res_" << method << "_" << Network::PERIOD_LENGTH << "_" << epsilon << "_"
                    << feasibleSize << "_" << coefPi << "_" << minAdmissibleCost << "_" << maxAdmissibleCost << "_"
                    << percentileSup << "_-" << ".json";
    }

    // write to the target file.
    write_json(outFilename.str(), root);

}

void
ApproximateFLA(const Network *pNetwork, String dataFilename, double dEpsilon, double dCoefPi, double minAdmissibleCost,
               double maxAdmissibleCost, int feasibleSize, int iModeMethod, int percentileSup, bool modeDisplay,
               int nbIterations, const double timeout, const int maxcount, const bool tryAllAssign) {
    int iNbFlightChangeLevel = 0;
    IloEnv env;
    FlightLevelAssignmentMap flightLevelAssignmentMap;
    ConflictMap conflictMap;
    MiPiMap miPiMap;
    std::ofstream cplexLogFile("cplexLog.txt", std::ios::out | std::ios::app);


    FlightVector vpFlightList = pNetwork->getFlightsList();
    LevelVector viLevelsList = pNetwork->getFlightLevelsList();

    std::cout << "[INFO] Starting ApproximateFLA method..." << std::endl;
    ProcessClock processClock{};
    //Start the process clock.
    processClock.start();
    int iNbFlightNotAssigned = 0;
    int iMaxDiverseLevel = 0;
    int iMaxNbConflict = 0;
    double dSumBenefits = 0.0;
    try {
        // Try to assign the flights.
        SolveFLA(vpFlightList, flightLevelAssignmentMap, conflictMap, miPiMap, env, viLevelsList, processClock,
                 dEpsilon, &dSumBenefits, minAdmissibleCost, maxAdmissibleCost, &iMaxNbConflict,
                 iModeMethod, modeDisplay, nbIterations, timeout, maxcount, tryAllAssign);
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
        case 5:
            std::cout << "Result Gaus2" << std::endl;
            break;
        case 6:
            std::cout << "Result Samp" << std::endl;
            break;
        case 4:
            std::cout << "Result eHoef" << std::endl;
            break;
        default:
            std::cout << "Result NDef" << std::endl;
            break;
    }
    double elapsedTime = processClock.getCpuTime();
    FlightVector flightNotAssigned;
    flightNotAssigned.clear();
    // for each unassigned flight, increase its admissible cost
    for (auto &&fi: flightLevelAssignmentMap) {
        if (!fi.second.first) {
            flightNotAssigned.push_back(fi.first);
        }
    }
    iNbFlightNotAssigned = static_cast<int>(flightNotAssigned.size());
    std::cout << "\tNb of flights change level: " << iNbFlightChangeLevel << std::endl;
    std::cout << "\tNb of flights not be assigned: " << iNbFlightNotAssigned << std::endl;
    std::cout << "\tMax conflict: " << iMaxNbConflict << std::endl;
    std::cout << "\tMax diverse levels: " << iMaxDiverseLevel << std::endl;
    std::cout << "\tSum benefits: " << dSumBenefits << std::endl;
    std::cout << "\tElapsed time: " << elapsedTime << std::endl << std::endl;
    // write the solution into file.
    writeJsonSolution(dataFilename, dEpsilon, dCoefPi, dSumBenefits, elapsedTime, minAdmissibleCost, maxAdmissibleCost,
                      feasibleSize, iModeMethod, percentileSup, iNbFlightChangeLevel, iNbFlightNotAssigned,
                      iMaxNbConflict, iMaxDiverseLevel,
                      nbIterations, timeout, maxcount, tryAllAssign);
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
SolvingFLAByLevel(FlightVector &vpFlightList, FlightsLevelMap &infeasibleFlightMap, FlightVector &vpPreviousFlightList,
                  const IloEnv &env, LevelExamine &levelEx, FlightLevelAssignmentMap &flightLevelAssignmentMap,
                  ConflictMap &conflictMap, MiPiMap &miPiMap,
                  double epsilon, double minAdmissibleCost, double maxAdmissibleCost, int *iMaxNbConflict,
                  Level iProcessingLevel, int iModeMethod, bool modeDisplay, int nbIterations) {

    //Process the assignment problem for each flight level
    if (modeDisplay) {
        std::cout << "[INFO] Level: " << iProcessingLevel << std::endl;
    }
    FlightVector CandidateFlightList;
    FlightVector ConflictFlightList;

    //vdList Mi, Pi;
    double dDiffTime = 0;
    double dWaitingTimeMax = 0;

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
            auto keyConflict = std::make_pair(fi->getCode(), fj->getCode());
            double prob;
            if (conflictMap.find(keyConflict) == conflictMap.end()) {
                if (modeDisplay) {
                    std::cout << "\t\tRecord: " << keyConflict.first << "\t" << keyConflict.second << std::endl;
                }
                prob = fi->CalculateProbabilityConflictAndDelayForFlight(fj, &dDiffTime, &dWaitingTimeMax, false);
                conflictMap.insert(std::make_pair(keyConflict, new Conflict(dDiffTime, dWaitingTimeMax, prob)));
            } else {
                prob = conflictMap.at(keyConflict)->getConflictProbability();
            }
            if (prob > MIN_PROBA) {
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

    for (auto &&fi:ConflictFlightList) {
        if (!contains(vpPreviousFlightList, fi)) {
            bool conflict = false;
            for (auto &&fj : ConflictFlightList) {
                if (*fi == *fj) {
                    continue;
                }
                auto keyConflict = std::make_pair(fi->getCode(), fj->getCode());
                double prob;
                if (conflictMap.find(keyConflict) == conflictMap.end()) {
                    prob = fi->CalculateProbabilityConflictAndDelayForFlight(fj, &dDiffTime, &dWaitingTimeMax, false);
                    conflictMap.insert(std::make_pair(keyConflict, new Conflict(dDiffTime, dWaitingTimeMax, prob)));
                }
                prob = conflictMap.at(keyConflict)->getConflictProbability();
                if (prob > MIN_PROBA) {
                    conflict = true;
                }
            }
            if (conflict) {
                flightLevelAssignmentMap[fi].first = false;
                flightLevelAssignmentMap[fi].second = -1;
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
    auto iConflictFlightsSize = (int) ConflictFlightList.size();
    if (iConflictFlightsSize == 0 && modeDisplay) {
        std::cout << "\tNo conflict flights" << std::endl;
        return true;
    }

    for (auto &&fi:ConflictFlightList) {
        for (auto &&fj : ConflictFlightList) {
            if (*fi == *fj) {
                continue;
            }
            auto keyConflict = std::make_pair(fi->getCode(), fj->getCode());
            double prob;
            if (conflictMap.find(keyConflict) == conflictMap.end()) {
                prob = fi->CalculateProbabilityConflictAndDelayForFlight(fj, &dDiffTime, &dWaitingTimeMax, false);
                conflictMap.insert(std::make_pair(keyConflict, new Conflict(dDiffTime, dWaitingTimeMax, prob)));
            }
        }
    }

    //Calculate Mi, Pi list and maximal conflict count.
    for (int i = 0; i < iConflictFlightsSize; i++) {
        std::string keyMiPi = ConflictFlightList[i]->getCode();
        int iConflictCount = 0;
        if (miPiMap.find(keyMiPi) == miPiMap.end()) {
            double dMi = 0.0;
            for (int j = 0; j < iConflictFlightsSize; j++) {
                if (i != j) {
                    auto keyConflict = std::make_pair(ConflictFlightList[i]->getCode(),
                                                      ConflictFlightList[j]->getCode());
                    dMi += conflictMap.at(keyConflict)->getMaxWaitTime();
                }
            }
            double dPi = std::min(std::max(minAdmissibleCost, ConflictFlightList[i]->getDuration() *
                                                              std::min(ConflictFlightList[i]->getCoefPi(), 0.20)),
                                  maxAdmissibleCost);
            miPiMap.insert(std::make_pair(keyMiPi, std::make_pair(dMi, dPi)));
            if (modeDisplay) {
                std::cout << "\t\tRecord Mi Pi: " << keyMiPi << std::endl;
            }
        }
        for (int j = 0; j < iConflictFlightsSize; j++) {
            if (i != j) {
                auto keyConflict = std::make_pair(ConflictFlightList[i]->getCode(),
                                                  ConflictFlightList[j]->getCode());
                if (conflictMap.at(keyConflict)->getConflictProbability() > MIN_PROBA) {
                    iConflictCount++;
                }
            }
            if (iConflictCount > *iMaxNbConflict) {
                *iMaxNbConflict = iConflictCount;
            }
        }
    }

    viList viConstraintList;
    //Relax the most infeasible constraint.
    int iMinIndexArgs = MinIndexArgs0(ConflictFlightList, conflictMap, miPiMap);
    if (iMinIndexArgs == -1) {
        return true;
    }
    if (modeDisplay) {
        std::cout << "\t\t\tStart" << std::endl;
        std::cout << "\t\t[INFO] Index: " << iMinIndexArgs << std::endl;
    }
    IloNumArray decisionVariablesValues;
    Solver *solver;
    viConstraintList.push_back(iMinIndexArgs);
    solver = new Solver(env);
    solver->initDecisionVariables(iConflictFlightsSize);
    solver->initFunctionObjective(ConflictFlightList, iProcessingLevel);
    solver->initConstraint(viConstraintList, ConflictFlightList, conflictMap, miPiMap);
    solver->prefixAssignedFlight(ConflictFlightList, flightLevelAssignmentMap, iProcessingLevel);
    solver->solve();
    decisionVariablesValues = solver->getDecisionVariablesValues();
    delete solver;
    int iMinIndexArgsFromFeaCheck = -1;
    bool feasible;
    //Test de feasibility of each constraint.
    //Relax the most infeasible constraint, until the solutionC has a high expected confidence.
    switch (iModeMethod) {
        case 0:
            /**
             * Deterministic Method
             */
            while (true) {
                feasible = FeasibilityRobustDet(ConflictFlightList, decisionVariablesValues, conflictMap, miPiMap,
                                                epsilon, &iMinIndexArgsFromFeaCheck, modeDisplay);
                if (feasible) {
                    break;
                } else {
                    if (contains(viConstraintList, iMinIndexArgsFromFeaCheck) ||
                        (int) viConstraintList.size() == iConflictFlightsSize) {
                        infeasibleFlightMap[iProcessingLevel].push_back(ConflictFlightList[iMinIndexArgsFromFeaCheck]);
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
                solver->initConstraint(viConstraintList, ConflictFlightList, conflictMap, miPiMap);
                solver->prefixAssignedFlight(ConflictFlightList, flightLevelAssignmentMap, iProcessingLevel);
                solver->solve();
                decisionVariablesValues = solver->getDecisionVariablesValues();
                delete solver;
            }
            break;
        case 1:/*Hoeffding*/
            while (true) {
                feasible = FeasibilityHoeffding(ConflictFlightList, decisionVariablesValues, conflictMap, miPiMap,
                                                epsilon, &iMinIndexArgsFromFeaCheck, modeDisplay);
                if (feasible) {
                    break;
                } else {
                    if (contains(viConstraintList, iMinIndexArgsFromFeaCheck) ||
                        (int) viConstraintList.size() == iConflictFlightsSize) {
                        infeasibleFlightMap[iProcessingLevel].push_back(ConflictFlightList[iMinIndexArgsFromFeaCheck]);
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
                solver->initConstraint(viConstraintList, ConflictFlightList, conflictMap, miPiMap);
                solver->prefixAssignedFlight(ConflictFlightList, flightLevelAssignmentMap, iProcessingLevel);
                solver->solve();
                decisionVariablesValues = solver->getDecisionVariablesValues();
                delete solver;
            }
            break;
        case 2:/*MonteCarlo with departure time*/
            while (true) {
                feasible = FeasibilityMonteCarlo(ConflictFlightList, decisionVariablesValues, viConstraintList, miPiMap,
                                                 epsilon, &iMinIndexArgsFromFeaCheck, modeDisplay, nbIterations);
                if (feasible) {
                    break;
                } else {
                    if (contains(viConstraintList, iMinIndexArgsFromFeaCheck) ||
                        (int) viConstraintList.size() == iConflictFlightsSize) {
                        infeasibleFlightMap[iProcessingLevel].push_back(ConflictFlightList[iMinIndexArgsFromFeaCheck]);
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
                solver->initConstraint(viConstraintList, ConflictFlightList, conflictMap, miPiMap);
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

    for (int i = 0; i < iConflictFlightsSize; i++) {
        Flight *fi = ConflictFlightList[i];
        if (decisionVariablesValues[i] == 1) {
            if (miPiMap.find(fi->getCode()) != miPiMap.end()) {
                miPiMap.erase(fi->getCode());
            }
            for (int j = 0; j < iConflictFlightsSize; j++) {
                auto keyConflict = std::make_pair(fi->getCode(), ConflictFlightList[j]->getCode());
                if (conflictMap.find(keyConflict) != conflictMap.end()) {
                    conflictMap.erase(keyConflict);
                }
            }
            fi->setCurrentLevel(iProcessingLevel);
            flightLevelAssignmentMap[fi].first = true;
            flightLevelAssignmentMap[fi].second = iProcessingLevel;
        } else {
            flightLevelAssignmentMap[fi].first = false;
            flightLevelAssignmentMap[fi].second = -1;
        }
    }
    return true;
}

int SolveFLA(FlightVector &vpFlightList, FlightLevelAssignmentMap &flightLevelAssignmentMap, ConflictMap &conflictMap,
             MiPiMap &miPiMap, const IloEnv &env,
             LevelVector &viLevelsList, ProcessClock &processClock, double epsilon, double *dSumBenefits,
             double minAdmissibleCost, double maxAdmissibleCost, int *iMaxNbConflict, int iModeMethod, bool modeDisplay,
             int nbIterations, const double timeout, const int maxcount, const bool tryAllAssign) {
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
    int iNbFlightChangeLevel = -1;
    int count = 0;
    while (iNbFlightNotAssigned > 0) {
        //Initialize the flight level queue.
        for (auto &&processingLevel : viLevelsList) {
            queueLevel.push(processingLevel);
        }
        infeasibleFlightMap.clear();
        while (!queueLevel.empty()) {
            Level processingLevel = queueLevel.front();
            queueLevel.pop();
            bool solved = false;
            while (!solved) {
                solved = SolvingFLAByLevel(vpFlightList, infeasibleFlightMap, vpPreviousFlightList, env, levelEx,
                                           flightLevelAssignmentMap, conflictMap, miPiMap,
                                           epsilon, minAdmissibleCost, maxAdmissibleCost, iMaxNbConflict,
                                           processingLevel, iModeMethod, modeDisplay, nbIterations);
            }
        }
        FlightVector flightNotAssigned;
        flightNotAssigned.clear();
        // for each unassigned flight, increase its admissible cost
        for (auto &&fi: flightLevelAssignmentMap) {
            if (!fi.second.first) {
                flightNotAssigned.push_back(fi.first);
                fi.first->setCoefPi(fi.first->getCoefPi() + 0.01);
            }
        }

        // Get the number of unassigned flights.
        //std::cout << tryAllAssign << std::endl;
        if (tryAllAssign) {
            if ((int) flightNotAssigned.size() >= iNbFlightNotAssigned) {
                count++;
                if (count >= maxcount) {
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
            }
        } else {
            if ((int) flightNotAssigned.size() >= iNbFlightNotAssigned) {
                count++;
            } else {
                if (getNbFlightsChangeLevel(vpFlightList) < iNbFlightChangeLevel) {
                    count = 0;
                }
            }
        }
        // Get the number of flights that change its most preferred flight level.
        iNbFlightChangeLevel = getNbFlightsChangeLevel(vpFlightList);
        iNbFlightNotAssigned = (int) flightNotAssigned.size();
        std::cout << "Solve the problem P':" << std::endl;
        std::cout << "\tNb of flights change level: " << iNbFlightChangeLevel << std::endl;
        std::cout << "\tNb of flights not be assigned: " << (int) flightNotAssigned.size() << std::endl;
        processClock.end();
        std::cout << "\tElapsed time: " << processClock.getCpuTime() << std::endl;
        if (iModeMethod == 2) {
            std::cout << "NbIteration: " << nbIterations << std::endl;
        }
        if (processClock.getCpuTime() > timeout || count >= maxcount) {
            iNbFlightNotAssigned = 0;
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

bool FeasibilityMonteCarlo(const FlightVector &vpConflictedFlightList, const IloNumArray &decisionVariables,
                           const viList &viConstraintList, const MiPiMap &miPiMap, double dEpsilon, int *piIndex,
                           bool modeDisplay, int nbIteration) {
    auto iConflictedFlightsSize = (int) vpConflictedFlightList.size();
    int iConflictedCounter = 0;
    bool feasible = true;
    *piIndex = -1;
    double **ppdConflictProbability = CreateTable(iConflictedFlightsSize);
    double **ppdDiffTime = CreateTable(iConflictedFlightsSize);
    double **ppdWaitingTimeMax = CreateTable(iConflictedFlightsSize);
    viList table;
    for (int i = 0; i < iConflictedFlightsSize; i++) {
        table.push_back(0);
    }
    //Generate nbIteration's scenarios
    for (int a = 0; a < nbIteration; a++) {
        InitTable(ppdDiffTime, iConflictedFlightsSize);
        InitTable(ppdWaitingTimeMax, iConflictedFlightsSize);
        InitTable(ppdConflictProbability, iConflictedFlightsSize);
        bool test = true;
        //Generate a new scenario from current flight list.
        for (int i = 0; i < iConflictedFlightsSize; i++) {
            if (decisionVariables[i] == 1 && !contains(viConstraintList, i)) {
                Flight *fj = vpConflictedFlightList[i];
                Time iOldDepartureTime = fj->getDepartureTime();
                Time iDelta = MixtureGaussianDistributionWithFourComponents();
                fj->GenerateNewFlight(iOldDepartureTime + iDelta);
            }
        }
        // recalculate the conflict probability and its delay time for each flight, call the deterministic method.
        CalculateConflictProbability(vpConflictedFlightList, ppdConflictProbability, ppdDiffTime, ppdWaitingTimeMax,
                                     true);

        for (int i = 0; i < iConflictedFlightsSize; i++) {
            if (decisionVariables[i] == 1) {
                double sum = 0;
                for (int j = 0; j < iConflictedFlightsSize; j++) {
                    if (j != i && ppdConflictProbability[i][j] > MIN_PROBA && decisionVariables[j] == 1 &&
                        0 <= ppdDiffTime[i][j] && ppdDiffTime[i][j] <= ppdWaitingTimeMax[i][j]) {
                        sum += ppdWaitingTimeMax[i][j] - ppdDiffTime[i][j];
                    }
                }
                if (sum > miPiMap.at(vpConflictedFlightList[i]->getCode()).second) {
                    test = false;
                    table[i] += 1;
                    break;
                }
            }
        }
        if (!test) {
            iConflictedCounter++;
        }
        // recover the original scenario
        for (auto &&fj: vpConflictedFlightList) {
            fj->resetRouteTimeList();
        }
    }
    // test the threshold
    if (dEpsilon == 0.05) {
        if ((nbIteration - iConflictedCounter) / (nbIteration * 1.0) < 0.961) {
            feasible = false;
        }
    } else if (dEpsilon == 0.10) {
        if ((nbIteration - iConflictedCounter) / (nbIteration * 1.0) < 0.915) {
            feasible = false;
        }
    } else if (dEpsilon == 0.15) {
        if ((nbIteration - iConflictedCounter) / (nbIteration * 1.0) < 0.868) {
            feasible = false;
        }
    } else if (dEpsilon == 0.20) {
        if ((nbIteration - iConflictedCounter) / (nbIteration * 1.0) < 0.821) {
            feasible = false;
        }
    } else if (dEpsilon == 0.25) {
        if ((nbIteration - iConflictedCounter) / (nbIteration * 1.0) < 0.772) {
            feasible = false;
        }
    } else {
        abort();
        std::cout << "the parameter epsilon is not correct!" << std::endl;
    }
    auto pos_max = std::max_element(table.begin(), table.end());
    *piIndex = int(pos_max - table.begin());
    if (modeDisplay) {
        std::cout << "i: " << *piIndex << std::endl;
    }
    table.clear();
    DestroyTable(ppdDiffTime, iConflictedFlightsSize);
    DestroyTable(ppdWaitingTimeMax, iConflictedFlightsSize);
    DestroyTable(ppdConflictProbability, iConflictedFlightsSize);
    if (modeDisplay) {
        std::cout << "\t\t\tCount: " << (nbIteration - iConflictedCounter) / (nbIteration * 1.0) << std::endl;
    }
    if (modeDisplay && feasible) {
        std::cout << "\t\t\tFeasibility ==> ok!" << std::endl;
    }
    return feasible;
}


bool FeasibilityHoeffding(const FlightVector &vpConflictedFlightList, const IloNumArray &decisionVariables,
                          const ConflictMap &conflictMap, const MiPiMap &miPiMap, double dEpsilon, int *piIndex,
                          bool modeDisplay) {
    *piIndex = -1;
    for (int i = 0, iConflictedFlightSize = static_cast<int>(vpConflictedFlightList.size());
         i < iConflictedFlightSize; i++) {
        double dInFeasibility = 0;
        double dTemp2 = 0;
        double dTemp1 = 0;
        double dSumBound = 0;
        if (decisionVariables[i] == 1) {
            for (int j = 0; j < iConflictedFlightSize; j++) {
                auto keyConflict = std::make_pair(vpConflictedFlightList[i]->getCode(),
                                                  vpConflictedFlightList[j]->getCode());
                if (j != i && conflictMap.at(keyConflict)->getConflictProbability() > MIN_PROBA) {
                    dTemp2 += pow(conflictMap.at(keyConflict)->getMaxWaitTime(), 2) * decisionVariables[j];
                    dSumBound += conflictMap.at(keyConflict)->getMaxWaitTime() * decisionVariables[j];
                    dTemp1 += conflictMap.at(keyConflict)->getAverageWaitTime() * decisionVariables[j];
                }
            }
            double dPi = miPiMap.at(vpConflictedFlightList[i]->getCode()).second;
            // vdPi[i] - dTemp1 > 0 to make t should positive in hoeffding's inequality
            if (dPi - dTemp1 > 0) {
                // dTemp2 > 0 to make sure there at least one variable.
                // vdPi[i] < dSumBound to make sure that probability is less than 1.
                if (dTemp2 > 0 && dPi < dSumBound) {
                    dInFeasibility = exp(-(2 * pow(dPi - dTemp1, 2)) / dTemp2);
                } else {
                    dInFeasibility = 0;
                }
            } else {
                dInFeasibility = 1;
            }
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


bool FeasibilityRobustDet(const FlightVector &vpConflictedFlightList, const IloNumArray &decisionVariables,
                          const ConflictMap &conflictMap, const MiPiMap &miPiMap, double dEpsilon, int *piIndex,
                          bool modeDisplay) {
    *piIndex = -1;
    for (int i = 0, iConflictedFlightSize = static_cast<int>(vpConflictedFlightList.size());
         i < iConflictedFlightSize; i++) {
        double dSumBound = 0;
        if (decisionVariables[i] == 1) {
            for (int j = 0; j < iConflictedFlightSize; j++) {
                auto keyConflict = std::make_pair(vpConflictedFlightList[i]->getCode(),
                                                  vpConflictedFlightList[j]->getCode());
                if (j != i && conflictMap.at(keyConflict)->getConflictProbability() > MIN_PROBA) {
                    dSumBound += conflictMap.at(keyConflict)->getMaxWaitTime() * decisionVariables[j];
                }
            }
            if (modeDisplay) {
                std::cout << "\t\t\ti: " << i << "==>Feas: " << 1 << "===> Eps: " << 1 - dEpsilon
                          << std::endl;
            }
            if (miPiMap.at(vpConflictedFlightList[i]->getCode()).second < dSumBound) {
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

int MinIndexArgs0(const FlightVector &vpConflictFlightList, ConflictMap &conflictMap, const MiPiMap &miPiMap) {
    int iMinIndex = -1;
    auto iConflictedFlightSize = (int) vpConflictFlightList.size();
    double dMaxValue = std::numeric_limits<double>::max();

    for (int index = 0; index < (int) vpConflictFlightList.size(); index++) {
        double dSumPenalCost = 0;

        for (int j = 0; j < iConflictedFlightSize; j++) {
            if (j != index) {
                auto keyConflict = std::make_pair(vpConflictFlightList[index]->getCode(),
                                                  vpConflictFlightList[j]->getCode());
                if (conflictMap.at(keyConflict)->getConflictProbability() > MIN_PROBA) {
                    double dExpectValue = 0;
                    if (!conflictMap.at(keyConflict)->isAverageCalculated()) {
                        dExpectValue = getExpectedValue(conflictMap.at(keyConflict)->getMaxWaitTime(),
                                                        conflictMap.at(keyConflict)->getDiffTime());
                        conflictMap.at(keyConflict)->setAverageWaitTime(dExpectValue);
                    }
                    dSumPenalCost += dExpectValue;
                }
            }
        }

        //i : min Pi-E[sum{p_ij}]
        dSumPenalCost = miPiMap.at(vpConflictFlightList[index]->getCode()).second - dSumPenalCost;
        if (dSumPenalCost < dMaxValue) {
            dMaxValue = dSumPenalCost;
            iMinIndex = index;
        }
    }
    return iMinIndex;
}

void
CalculateConflictProbability(const FlightVector &vpConflictFlightList, double **ppdConflictProbability,
                             double **ppdDiffTime,
                             double **ppdWaitingTimeMax, bool deterministic) {
    auto iSize = (int) vpConflictFlightList.size();
    double dDiffTime = 0;
    double dWaitingTimeMax = 0;
    for (int i = 0; i < iSize - 1; i++) {
        Flight *fi = vpConflictFlightList[i];
        for (int j = i + 1; j < iSize; j++) {
            Flight *fj = vpConflictFlightList[j];
            ppdConflictProbability[i][j] = ppdConflictProbability[j][i] = fi->CalculateProbabilityConflictAndDelayForFlight(
                    fj, &dDiffTime, &dWaitingTimeMax, deterministic);
            ppdDiffTime[i][j] = dDiffTime;
            ppdDiffTime[j][i] = -dDiffTime;
            ppdWaitingTimeMax[i][j] = ppdWaitingTimeMax[j][i] = dWaitingTimeMax;
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
    auto **ppdDouble = new double *[iSize];
    for (int i = 0; i < iSize; i++) {
        ppdDouble[i] = new double[iSize];
    }
    return ppdDouble;
}

double MixtureGaussianDistributionWithFourComponents() {
    // the seed of random function
    std::random_device rd;
    std::mt19937 generator(rd());
    // define a uniform distribution, to get four parts of components.
    uni_dist UniformDist(0.0, 1.0);
    double p = UniformDist(generator);
    double delta;
    if (p < P_1) {
        normal_dist NormalDist(MU_1, sqrt(SIGMA_2_1));
        delta = NormalDist(generator);
    } else if (p < P_1 + P_2) {
        normal_dist NormalDist(MU_2, sqrt(SIGMA_2_2));
        delta = NormalDist(generator);
    } else if (p < P_1 + P_2 + P_3) {
        normal_dist NormalDist(MU_3, sqrt(SIGMA_2_3));
        delta = NormalDist(generator);
    } else {
        normal_dist NormalDist(MU_4, sqrt(SIGMA_2_4));
        delta = NormalDist(generator);
    }
    return delta;
}





