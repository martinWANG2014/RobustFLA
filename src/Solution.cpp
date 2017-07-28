//
// Created by chenghaowang on 24/07/17.
//
#include "../include/Solution.h"

void ApproximateFLA(Network *pNetwork, const vdList &vdParameter, double dEpsilon, double dCoefPi, double *dSumBenefits,
                    int *iMaxNbConflict, int iModeMethod, int iFeasibleSize, int iModeRandom) {
    int iNbFlightNotAssigned = 0;
    int iLastNbFlightNotAssigned = 0;
    int iNbFlightChangeLevel = 0;
    int iLastNbFlightChangeLevel = 0;
    int maxLoopTime = 5;
    int iLoopTime = 0;
    bool bNotFinish = true;
    IloEnv env;
    std::ofstream cplexLogFile("cplexLog.txt", std::ios::out | std::ios::app);

    // Initialize the coefficient Pi for all flight in the network.
    pNetwork->InitCoefPi(dCoefPi);
    // Initialize the feasible flight levels for all flights in the network.
    pNetwork->InitFeasibleList(iFeasibleSize);
    // Initialize  the flight levels list of the network.
    pNetwork->InitFlightLevelsList();
    // Initialize the sigma for the random departure time.
    if (iModeMethod > 1) {
        pNetwork->SetSigma(vdParameter, iModeRandom);
    }

    FlightVector vpFlightList = pNetwork->getFlightsList();
    LevelVector viLevelsList = pNetwork->getFlightLevelsList();

    std::cout << "[INFO] Starting ApproximateFLA method..." << std::endl;
    ProcessClock processClock;
    //Start the process clock.
    processClock.start();
    try {
        while (bNotFinish && iLoopTime < maxLoopTime) {
            // Try to assign the flights.
            FlightVector vpFlightNotAssigned = SolveFLA(vpFlightList, env, vdParameter, viLevelsList, dEpsilon,
                                                        dSumBenefits, iMaxNbConflict, iModeMethod,
                                                        iModeRandom, cplexLogFile);
            // Get the number of flights that change its most preferred flight level.
            iNbFlightChangeLevel = getNbFlightsChangeLevel(vpFlightList);
            // Get the number of unassigned flights.
            iNbFlightNotAssigned = (int) vpFlightNotAssigned.size();
            std::cout << "[Solved] Nb of flights not be assigned: " << iNbFlightNotAssigned << std::endl;
            std::cout << "[Solved] Nb of flights change level: " << iNbFlightChangeLevel << std::endl << std::endl;
            //If all flights have been assigned, then Stop.
            if (iNbFlightNotAssigned == 0) {
                bNotFinish = false;
            }
                //Else if the assignment solver don't reduce the number of unassigned flights,
                // then enlarge the feasible flight level list for each unassigned flight.
            else if (iNbFlightNotAssigned == iLastNbFlightNotAssigned) {
                //If solver does not find improve solutionC within maximal consecutive iterations.
                if (iLastNbFlightChangeLevel == iNbFlightChangeLevel) {
                    iLoopTime++;
                }
                for (auto &&fi :vpFlightNotAssigned) {
                    fi->addNewFeasibleLevel(findNextFeasibleLevel(fi->getDefaultLevel(), fi->getLastFeasibleLevel()));
                    Level level = fi->getLastFeasibleLevel();
                    if (!contains(viLevelsList, level)) {
                        viLevelsList.push_back(level);
                    }
                    fi->setCoefPi(dCoefPi);
                }
            }
                //Else, increase the admissible cost for each unassigned flight.
            else {
                for (auto &&fi :vpFlightNotAssigned) {
                    fi->setCoefPi(std::min(fi->getCoefPi() + 0.05, 0.50));
                }
                iLastNbFlightChangeLevel = iNbFlightChangeLevel;
                iLastNbFlightNotAssigned = iNbFlightNotAssigned;
                iLoopTime = 0;
            }
        }
    } catch (IloException &e) { std::cerr << e.getMessage() << std::endl; }
    catch (...) { std::cerr << "error" << std::endl; }
    cplexLogFile.close();
    env.end();
    processClock.end();
    std::cout << "Flight Code:Current Level(default)" << std::endl;
    if (iNbFlightNotAssigned == 0) {
        for (auto &&fi: vpFlightList) {
            std::cout << fi->getCode() << ":" << fi->getCurrentLevel() << "(" << fi->getDefaultLevel() << ")"
                      << std::endl;

        }
    } else {
        std::cout << "No Solution..." << std::endl;
    }
    std::cout << "Nb of flights change level: " << iNbFlightChangeLevel << std::endl;
    std::cout << "Nb of flights not be assigned: " << iNbFlightNotAssigned << std::endl;
    std::cout << "Max conflict: " << *iMaxNbConflict << std::endl;
    std::cout << "Sum benefits: " << *dSumBenefits << std::endl;
    std::cout << "Elapsed time:" << processClock.getCpuTime() << std::endl;
}

int getNbFlightsChangeLevel(const FlightVector &vpFlightList) {
    int iNbFlightChangeLevel = 0;
    for (auto &&fi : vpFlightList) {
        if (fi->getCurrentLevel() != fi->getDefaultLevel()) {
            iNbFlightChangeLevel++;
        }
    }
    return iNbFlightChangeLevel;
}

FlightVector SolveFLA(const FlightVector &vpFlightList, const IloEnv &env, const vdList &vdParameter,
                      const LevelVector &viLevelsList, double epsilon, double *dSumBenefits, int *iMaxNbConflict,
                      int iModeMethod, int iModeRandom, std::ofstream &cplexLogFile) {
    LevelExamine levelEx;
    FlightAssignmentMap flightAssignmentMap;
    *dSumBenefits = 0;
    *iMaxNbConflict = 0;

    //Initialize the flight assignment map.
    for (auto &&flight: vpFlightList) {
        flightAssignmentMap[flight] = false;
    }

    //Initialize the examine flight levels list.
    for (auto &&level: viLevelsList) {
        levelEx[level] = false;
    }

    //Initialize current flight level for each flight as its most preferred one.
    for (auto &&fi: vpFlightList) {
        fi->resetLevel();
    }

    //Process the assignment problem for each flight level
    for (auto itA = viLevelsList.begin(); itA != viLevelsList.end(); itA++) {
        int iProcessingLevel = (*itA);
//        std::cout << "[INFO] Level: " << iProcessingLevel << std::endl;
        FlightVector CandidateFlightList;
        FlightVector ConflictFlightList;
        vdList Mi, Pi;
        double dDelayTime = 0;
        bool bWait = true;

        //Initialize the candidate flight list in current processing flight level.
        for (auto &&flight: vpFlightList) {
            const LevelVector &levels = flight->getFeasibleLevelList();
            Level iPreferredLevel = flight->getDefaultLevel();
            if (iPreferredLevel == iProcessingLevel) {
                CandidateFlightList.push_back(flight);
            } else {
                for (auto &&level : levels) {
                    if (level == iProcessingLevel && flightAssignmentMap[flight] == false &&
                        levelEx[iPreferredLevel] == true) {
                        CandidateFlightList.push_back(flight);
                        break;
                    }
                }
            }
        }

        //Initialize the conflicted flight list.
        for (auto &&fi: CandidateFlightList) {
            for (auto &&fj: CandidateFlightList) {
                if (*fi == *fj) {
                    continue;
                }
                double prob = fi->CalculateProbabilityConflictAndDelayForFlight(fj, &dDelayTime, &bWait,
                                                                                iModeMethod < 3);
                if (prob > 0) {
                    ConflictFlightList.push_back(fi);
                    break;
                }
            }
        }

        //Assign all conflict-free flight.
        for (auto &&flight: CandidateFlightList) {
            if (!contains(ConflictFlightList, flight)) {
                auto index = std::find(flight->getFeasibleLevelList().begin(), flight->getFeasibleLevelList().end(),
                                       iProcessingLevel);
                if (index != flight->getFeasibleLevelList().end()) {
                    long lIndex = index - flight->getFeasibleLevelList().begin();
                    *dSumBenefits += exp(3 - lIndex);
                }
                flight->setCurrentLevel(iProcessingLevel);
                flightAssignmentMap[flight] = true;
            }
        }


        //Release unused resources
        CandidateFlightList.clear();

        //Mark current processing flight level has been examined.
        levelEx[iProcessingLevel] = true;

        //If the conflict flight list size equal 0, then go to treat next flight level.
        int iConflictFlightsSize = (int) ConflictFlightList.size();
        if (iConflictFlightsSize == 0) {
            continue;
        }

        double **ppdConflictProbability = CreateTable(iConflictFlightsSize);
        double **ppdDelayTime = CreateTable(iConflictFlightsSize);
        InitTable(ppdConflictProbability, iConflictFlightsSize);
        InitTable(ppdDelayTime, iConflictFlightsSize);
        CalculateConflictProbability(ConflictFlightList, ppdConflictProbability, ppdDelayTime, iModeMethod < 3);

        //Calculate Mi list and maximal conflict couunt.
        for (int i = 0; i < iConflictFlightsSize; i++) {
            double sumPenalCost = 0.0;
            int iConflictCount = 0;
            for (int j = 0; j < iConflictFlightsSize; j++) {
                if (ppdConflictProbability[i][j] > 0) {
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
            double time = (fi->getArrivingTime() - fi->getDepartureTime());
            Pi.push_back(std::max(60.0, time) * fi->getCoefPi());
        }

        viList viSearchList;
        viList viConstraintList;
        for (int i = 0; i < iConflictFlightsSize; i++) {
            viSearchList.push_back(i);
        }
        //Relax the most infeasible constraint.
        int iMinIndexArgs = MinIndexArgs0(ConflictFlightList, viSearchList, Pi, ppdConflictProbability, ppdDelayTime,
                                          iModeMethod < 3);
//        std::cout << "\t[INFO] Index: " << iMinIndexArgs << std::endl;
        viConstraintList.push_back(iMinIndexArgs);
        viSearchList.erase(std::remove(viSearchList.begin(), viSearchList.end(), iMinIndexArgs), viSearchList.end());
        Solver *solver = new Solver(env, cplexLogFile);
        solver->initDecisionVariables(iConflictFlightsSize);
        solver->initFunctionObjective(ConflictFlightList, iProcessingLevel);
        solver->initConstraint(viConstraintList, Mi, Pi, ppdDelayTime, iConflictFlightsSize);
        solver->solve();
        double dFunctionObjectiveValue = solver->getFunctionObjectiveValue();
        IloNumArray decisionVariablesValues = solver->getDecisionVariablesValues();
        delete solver;
        int iMinIndexArgsFromFeaCheck = -1;
        //Test de feasibility of each constraint.
        //Relax the most infeasible constraint, until the solutionC has a high expected confidence.
        switch (iModeMethod) {
            case 0:/*enumeration method*/
                while (!FeasibilityEnumeration(viSearchList, Pi, decisionVariablesValues, ppdConflictProbability,
                                               ppdDelayTime,
                                               epsilon, iConflictFlightsSize, &iMinIndexArgsFromFeaCheck) &&
                       viSearchList.size() > 0) {
                    if (iMinIndexArgsFromFeaCheck == -1) {
                        iMinIndexArgs = MinIndexArgs1(ConflictFlightList, viSearchList, Pi, decisionVariablesValues,
                                                      ppdConflictProbability,
                                                      ppdDelayTime, true);
                    } else {
                        iMinIndexArgs = iMinIndexArgsFromFeaCheck;
                    }
//                    std::cout << "\t[INFO] Index: " << iMinIndexArgs << std::endl;
                    viConstraintList.push_back(iMinIndexArgs);
                    //Remove the most infeasible constraint from search list.
                    viSearchList.erase(std::remove(viSearchList.begin(), viSearchList.end(), iMinIndexArgs),
                                       viSearchList.end());
                    //ReInitialize the model, then resolve it.
                    solver = new Solver(env, cplexLogFile);
                    solver->initDecisionVariables(iConflictFlightsSize);
                    solver->initFunctionObjective(ConflictFlightList, iProcessingLevel);
                    solver->initConstraint(viConstraintList, Mi, Pi, ppdDelayTime, iConflictFlightsSize);
                    solver->solve();
                    dFunctionObjectiveValue = solver->getFunctionObjectiveValue();
                    decisionVariablesValues = solver->getDecisionVariablesValues();
                    delete solver;
                }
                break;
            case 1:/*Hoeffding*/
                while (!FeasibilityHoeffding(viSearchList, Pi, decisionVariablesValues,
                                             ppdConflictProbability,
                                             ppdDelayTime,
                                             epsilon, iConflictFlightsSize, &iMinIndexArgsFromFeaCheck) &&
                       viSearchList.size() > 0) {
                    if (iMinIndexArgsFromFeaCheck == -1) {
                        iMinIndexArgs = MinIndexArgs1(ConflictFlightList, viSearchList, Pi, decisionVariablesValues,
                                                      ppdConflictProbability,
                                                      ppdDelayTime, true);
                    } else {
                        iMinIndexArgs = iMinIndexArgsFromFeaCheck;
                    }
//                    std::cout << "\t[INFO] Index: " << iMinIndexArgs << std::endl;
                    viConstraintList.push_back(iMinIndexArgs);
                    //Remove the most infeasible constraint from search list.
                    viSearchList.erase(std::remove(viSearchList.begin(), viSearchList.end(), iMinIndexArgs),
                                       viSearchList.end());
                    //ReInitialize the model, then resolve it.
                    solver = new Solver(env, cplexLogFile);
                    solver->initDecisionVariables(iConflictFlightsSize);
                    solver->initFunctionObjective(ConflictFlightList, iProcessingLevel);
                    solver->initConstraint(viConstraintList, Mi, Pi, ppdDelayTime, iConflictFlightsSize);
                    solver->solve();
                    dFunctionObjectiveValue = solver->getFunctionObjectiveValue();
                    decisionVariablesValues = solver->getDecisionVariablesValues();
                    delete solver;
                }
                break;
            case 2:/*MonteCarlo*/
                while (!FeasibilityMonteCarlo(ConflictFlightList, viSearchList, Pi, decisionVariablesValues,
                                              vdParameter,
                                              epsilon, &iMinIndexArgsFromFeaCheck, iModeRandom, true) &&
                       viSearchList.size() > 0) {
                    if (iMinIndexArgsFromFeaCheck == -1) {
                        iMinIndexArgs = MinIndexArgs1(ConflictFlightList, viSearchList, Pi, decisionVariablesValues,
                                                      ppdConflictProbability,
                                                      ppdDelayTime, true);
                    } else {
                        iMinIndexArgs = iMinIndexArgsFromFeaCheck;
                    }
//                    std::cout << "\t[INFO] Index: " << iMinIndexArgs << std::endl;
                    viConstraintList.push_back(iMinIndexArgs);
                    //Remove the most infeasible constraint from search list.
                    viSearchList.erase(std::remove(viSearchList.begin(), viSearchList.end(), iMinIndexArgs),
                                       viSearchList.end());
                    //ReInitialize the model, then resolve it.
                    solver = new Solver(env, cplexLogFile);
                    solver->initDecisionVariables(iConflictFlightsSize);
                    solver->initFunctionObjective(ConflictFlightList, iProcessingLevel);
                    solver->initConstraint(viConstraintList, Mi, Pi, ppdDelayTime, iConflictFlightsSize);
                    solver->solve();
                    dFunctionObjectiveValue = solver->getFunctionObjectiveValue();
                    decisionVariablesValues = solver->getDecisionVariablesValues();
                    delete solver;
                }
                break;
            case 3:/*Gaussian*/
                while (!FeasibilityGaussian(ConflictFlightList, viSearchList, Pi, decisionVariablesValues,
                                            ppdConflictProbability, ppdDelayTime,
                                            epsilon, &iMinIndexArgsFromFeaCheck) &&
                       viSearchList.size() > 0) {
                    if (iMinIndexArgsFromFeaCheck == -1) {
                        iMinIndexArgs = MinIndexArgs1(ConflictFlightList, viSearchList, Pi, decisionVariablesValues,
                                                      ppdConflictProbability,
                                                      ppdDelayTime, false);
                    } else {
                        iMinIndexArgs = iMinIndexArgsFromFeaCheck;
                    }
//                    std::cout << "\t[INFO] Index: " << iMinIndexArgs << std::endl;
                    viConstraintList.push_back(iMinIndexArgs);
                    //Remove the most infeasible constraint from search list.
                    viSearchList.erase(std::remove(viSearchList.begin(), viSearchList.end(), iMinIndexArgs),
                                       viSearchList.end());
                    //ReInitialize the model, then resolve it.
                    solver = new Solver(env, cplexLogFile);
                    solver->initDecisionVariables(iConflictFlightsSize);
                    solver->initFunctionObjective(ConflictFlightList, iProcessingLevel);
                    solver->initConstraint(viConstraintList, Mi, Pi, ppdDelayTime, iConflictFlightsSize);
                    solver->solve();
                    dFunctionObjectiveValue = solver->getFunctionObjectiveValue();
                    decisionVariablesValues = solver->getDecisionVariablesValues();
                    delete solver;
                }
                break;
            case 4:/*MonteCarlo with departure time*/
                while (!FeasibilityMonteCarlo(ConflictFlightList, viSearchList, Pi, decisionVariablesValues,
                                              vdParameter,
                                              epsilon, &iMinIndexArgsFromFeaCheck, iModeRandom, false) &&
                       viSearchList.size() > 0) {
                    if (iMinIndexArgsFromFeaCheck == -1) {
                        iMinIndexArgs = MinIndexArgs1(ConflictFlightList, viSearchList, Pi, decisionVariablesValues,
                                                      ppdConflictProbability,
                                                      ppdDelayTime, false);
                    } else {
                        iMinIndexArgs = iMinIndexArgsFromFeaCheck;
                    }
//                    std::cout << "\t[INFO] Index: " << iMinIndexArgs << std::endl;
                    viConstraintList.push_back(iMinIndexArgs);
                    //Remove the most infeasible constraint from search list.
                    viSearchList.erase(std::remove(viSearchList.begin(), viSearchList.end(), iMinIndexArgs),
                                       viSearchList.end());
                    //ReInitialize the model, then resolve it.
                    solver = new Solver(env, cplexLogFile);
                    solver->initDecisionVariables(iConflictFlightsSize);
                    solver->initFunctionObjective(ConflictFlightList, iProcessingLevel);
                    solver->initConstraint(viConstraintList, Mi, Pi, ppdDelayTime, iConflictFlightsSize);
                    solver->solve();
                    dFunctionObjectiveValue = solver->getFunctionObjectiveValue();
                    decisionVariablesValues = solver->getDecisionVariablesValues();
                    delete solver;
                }
                break;
            case 5:/*enumeration method with departure time*/
                while (!FeasibilityEnumeration(viSearchList, Pi, decisionVariablesValues, ppdConflictProbability,
                                               ppdDelayTime,
                                               epsilon, iConflictFlightsSize, &iMinIndexArgsFromFeaCheck) &&
                       viSearchList.size() > 0) {
                    if (iMinIndexArgsFromFeaCheck == -1) {
                        iMinIndexArgs = MinIndexArgs1(ConflictFlightList, viSearchList, Pi, decisionVariablesValues,
                                                      ppdConflictProbability,
                                                      ppdDelayTime, false);
                    } else {
                        iMinIndexArgs = iMinIndexArgsFromFeaCheck;
                    }
//                    std::cout << "\t[INFO] Index: " << iMinIndexArgs << std::endl;
                    viConstraintList.push_back(iMinIndexArgs);
                    //Remove the most infeasible constraint from search list.
                    viSearchList.erase(std::remove(viSearchList.begin(), viSearchList.end(), iMinIndexArgs),
                                       viSearchList.end());
                    //ReInitialize the model, then resolve it.
                    solver = new Solver(env, cplexLogFile);
                    solver->initDecisionVariables(iConflictFlightsSize);
                    solver->initFunctionObjective(ConflictFlightList, iProcessingLevel);
                    solver->initConstraint(viConstraintList, Mi, Pi, ppdDelayTime, iConflictFlightsSize);
                    solver->solve();
                    dFunctionObjectiveValue = solver->getFunctionObjectiveValue();
                    decisionVariablesValues = solver->getDecisionVariablesValues();
                    delete solver;
                }
                break;
            default:
                std::cerr << "Not support ! " << std::endl;
                abort();
                break;
        }
        *dSumBenefits += dFunctionObjectiveValue;
        for (int i = 0; i < iConflictFlightsSize; i++) {
            if ((decisionVariablesValues)[i] == 1) {
                Flight *fi = ConflictFlightList[i];
                fi->setCurrentLevel(iProcessingLevel);
                flightAssignmentMap[fi] = true;
            }
        }
        DestroyTable(ppdConflictProbability, iConflictFlightsSize);
        DestroyTable(ppdDelayTime, iConflictFlightsSize);
    }
    FlightVector vpFlightNotAssigned;
    for (auto &&fi: flightAssignmentMap) {
        if (!fi.second) {
            vpFlightNotAssigned.push_back(fi.first);
        }
    }
    return vpFlightNotAssigned;
}

bool FeasibilityMonteCarlo(const FlightVector &vpConflictedFlightList, const viList &viSearchList, const vdList &vdPi,
                           const IloNumArray &decisionVariables, const vdList &vdParameter, double dEpsilon,
                           int *piIndex, int iModeRandom, bool bGeoMethod) {
    int iConflictedFlightsSize = (int) vpConflictedFlightList.size();
    int iConflictedCounter = 0;
    int iMaxConflict = -1;
    *piIndex = -1;
    double **ppdConflictProbability = CreateTable(iConflictedFlightsSize);
    double **ppdDelayTime = CreateTable(iConflictedFlightsSize);
    int *table = new int[iConflictedFlightsSize];
    for (int i = 0; i < iConflictedFlightsSize; i++) {
        table[i] = 0;
    }
    //Simulate 1000 scenarios
    for (int a = 0; a < 1000; a++) {
        InitTable(ppdDelayTime, iConflictedFlightsSize);
        InitTable(ppdConflictProbability, iConflictedFlightsSize);
        bool test = true;
        //Generate a new scenario from current flight list.
        for (auto fj : vpConflictedFlightList) {
            Time iOldDepartureTime = fj->getDepartureTime();
            Time iDelta = 0;
            switch (iModeRandom) {
                case 0:
                    iDelta = GaussianDistribution1(vdParameter, fj->getSigma());
                    break;
                case 1:
                    iDelta = GaussianDistribution2(vdParameter, fj->getSigma());
                    break;
                default:
                    iDelta = GaussianDistribution3(vdParameter, fj->getSigma());
                    break;
            }
            fj->GenerateNewFlight(iOldDepartureTime + iDelta, bGeoMethod);
        }
        CalculateConflictProbability(vpConflictedFlightList, ppdConflictProbability, ppdDelayTime, bGeoMethod);
        for (int i = 0; i < iConflictedFlightsSize; i++) {
            if (decisionVariables[i] == 1) {
                double sum = 0;
                for (int j = 0; j < iConflictedFlightsSize; j++) {
                    if (ppdDelayTime[i][j] > 0 && decisionVariables[j] == 1) {
                        sum += std::max(0.0, ppdDelayTime[i][j]);
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
    }
    for (auto &&item : viSearchList) {
        if (decisionVariables[item] == 1 && table[item] > iMaxConflict) {
            iMaxConflict = table[item];
            *piIndex = item;
        }
    }
    delete table;
    DestroyTable(ppdDelayTime, iConflictedFlightsSize);
    DestroyTable(ppdConflictProbability, iConflictedFlightsSize);
    if (dEpsilon == 0.1) {
        if (1000 - iConflictedCounter < 915) {
            return false;
        }
    }
    if (dEpsilon == 0.05) {
        if (1000 - iConflictedCounter < 961) {
            return false;
        }
    }
    if (dEpsilon == 0.15) {
        if (1000 - iConflictedCounter < 868) {
            return false;
        }
    }
    if (dEpsilon == 0.2) {
        if (1000 - iConflictedCounter < 821) {
            return false;
        }
    }
    if (dEpsilon == 0.25) {
        if (1000 - iConflictedCounter < 772) {
            return false;
        }
    }
    return true;
}

bool FeasibilityGaussian(const FlightVector &vpConflictedFlightList, const viList &viSearchList, const vdList &vdPi,
                         const IloNumArray &decisionVariables, double **ppdConflictProbability, double **ppdDelayTime,
                         double dEpsilon, int *piIndex) {
    int iSize = (int) vpConflictedFlightList.size();
    double dMinFeasibility = std::numeric_limits<double>::max();
    *piIndex = -1;
    for (int i = 0; i < iSize; i++) {
        if (decisionVariables[i] == 1) {
            double exp_mu = 0.0;
            double exp_sigma_2 = 0.0;
            int iCounter = 0;
            double dLeftConstrainedFeasibility = 1;
            double mu;
            double sigma_2;
            for (int j = 0; j < iSize; j++) {
                if (ppdConflictProbability[i][j] > 0 && decisionVariables[j] == 1) {
                    mu = ppdDelayTime[i][j];
                    sigma_2 = pow(vpConflictedFlightList[i]->getSigma(), 2) +
                              pow(vpConflictedFlightList[j]->getSigma(), 2);
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
            if (dFeasibility < 1 - dEpsilon) {
                return false;
            }
            if (dFeasibility < dMinFeasibility && contains(viSearchList, i)) {
                dMinFeasibility = dFeasibility;
                *piIndex = i;
            }
        }
    }
    return true;
}

bool FeasibilityHoeffding(const viList &viSearchList, const vdList &vdPi,
                          const IloNumArray &decisionVariables,
                          double **ppdConflictProbability, double **ppdDelayTime, double dEpsilon,
                          int iConflictedFlightSize, int *piIndex) {
    double dMinFeasibility = std::numeric_limits<double>::max();
    *piIndex = -1;
    for (int i = 0; i < iConflictedFlightSize; i++) {
        double dFeasibility = 0;
        double temp2 = 0;
        if (decisionVariables[i] == 1) {
            for (int j = 0; j < iConflictedFlightSize; j++) {
                if (ppdConflictProbability[i][j] > 0 && decisionVariables[j] == 1) {
//                    double sigma_2 = pow(vpConflictedFlightList[i]->getSigma(), 2) +
//                                     pow(vpConflictedFlightList[j]->getSigma(), 2);
                    double maxPenalCost = std::max(0.0, ppdDelayTime[i][j]);
                    dFeasibility += ppdConflictProbability[i][j] * maxPenalCost / 2;
                    temp2 += pow(maxPenalCost, 2);
                }
            }
            dFeasibility = exp(-(2 * pow(vdPi[i] - dFeasibility, 2)) / temp2);
            if (dFeasibility > dEpsilon) {
                return false;
            }
            if (1 - dFeasibility < dMinFeasibility && contains(viSearchList, i)) {
                dMinFeasibility = dFeasibility;
                *piIndex = i;
            }
        }
    }
    return true;
}

bool FeasibilityEnumeration(const viList &viSearchList, const vdList &vdPi, const IloNumArray &decisionVariables,
                            double **ppdConflictProbability, double **ppdDelayTime, double dEpsilon,
                            int iConflictedFlightsSize, int *piIndexMostInfeasible) {
    *piIndexMostInfeasible = -1;
    double dMinFeasibility = std::numeric_limits<double>::max();
    //Iterate all conflict flights in current flight level
    for (int i = 0; i < iConflictedFlightsSize; i++) {
        //For each assigned flight(x[i] = 1) in current level of last solutionC
        if (decisionVariables[i] == 1) {
            int iConflictFlightsOfI = 0;
            viList viCandidateConflictFlightOfI;
            double dFeasibility = 0;
            double dSumPenalCost = 0;
            for (int j = 0; j < iConflictedFlightsSize; j++) {
                if (ppdConflictProbability[i][j] > 0) {
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
                            dFeasibility += dOccurrenceProbability;
                        }
                    }
                    k--;
                    bNotFinish = bValid;
                }
                if (dFeasibility > dEpsilon) {
                    return false;
                }

                if (1 - dFeasibility < dMinFeasibility && contains(viSearchList, i)) {
                    dMinFeasibility = dFeasibility;
                    *piIndexMostInfeasible = i;
                }
            }
        }
    }
    return true;
}

int MinIndexArgs1(const FlightVector &vpConflictFlightList, const viList &viSearchList, const vdList &vdPi,
                  const IloNumArray &decisionVariables, double **ppdConflictProbability, double **ppdDelayTime,
                  bool bMethod) {
    int iMinIndex = -1;
    int iConflictedFlightSize = (int) vpConflictFlightList.size();
    double dMaxValue = std::numeric_limits<double>::max();
    if (bMethod) {
        for (auto &&index: viSearchList) {
            double dSumPenalCost = 0;
            for (int j = 0; j < iConflictedFlightSize; j++) {
                if (ppdConflictProbability[index][j] > 0 && decisionVariables[j] == 1) {
                    dSumPenalCost += std::max(0.0, ppdDelayTime[index][j]) * ppdConflictProbability[index][j] / 2;
                }
            }
            //i : min Pi-E[sum{p_ij}]
            dSumPenalCost = vdPi[index] - dSumPenalCost;
            if (dSumPenalCost < dMaxValue) {
                dMaxValue = dSumPenalCost;
                iMinIndex = index;
            }
        }
    } else {
        for (auto &&index: viSearchList) {
            double dSumPenalCost = 0;
            for (int j = 0; j < iConflictedFlightSize; j++) {
                if (ppdConflictProbability[index][j] > 0 && decisionVariables[j] == 1) {
                    double sigma = vpConflictFlightList[j]->getSigma();
                    double mu = ppdDelayTime[index][j];
                    dSumPenalCost += (sigma / sqrt(2.0 * M_PI) * exp(-pow(mu, 2) / (2 * pow(sigma, 2))) +
                                      mu / 2.0 * (1 - boost::math::erf(-mu / (sqrt(2.0) * sigma))));

                }
            }
            //i : min Pi-E[sum{p_ij*x_j}]
            dSumPenalCost = vdPi[index] - dSumPenalCost;
            if (dSumPenalCost < dMaxValue) {
                dMaxValue = dSumPenalCost;
                iMinIndex = index;
            }
        }
    }
    if (iMinIndex == -1) {
        std::cerr << "[ERROR] the selected index invalid !" << std::endl;
    }
    return iMinIndex;
}

int MinIndexArgs0(const FlightVector &vpConflictFlightList, const viList &viSearchList, const vdList &vdPi,
                  double **ppdConflictProbability, double **ppdDelayTime, bool bGeoMethod) {
    int iMinIndex = -1;
    int iConflictedFlightSize = (int) vpConflictFlightList.size();
    double dMaxValue = std::numeric_limits<double>::max();
    if (bGeoMethod) {
        for (auto &&index: viSearchList) {
            double dSumPenalCost = 0;
            for (int j = 0; j < iConflictedFlightSize; j++) {
                if (ppdConflictProbability[index][j] > 0) {
                    dSumPenalCost += std::max(0.0, ppdDelayTime[index][j]) * ppdConflictProbability[index][j] / 2;
                }
            }
            //i : min Pi-E[sum{p_ij}]
            dSumPenalCost = vdPi[index] - dSumPenalCost;
            if (dSumPenalCost < dMaxValue) {
                dMaxValue = dSumPenalCost;
                iMinIndex = index;
            }
        }
    } else {
        for (auto &&index: viSearchList) {
            double dSumPenalCost = 0;
            for (int j = 0; j < iConflictedFlightSize; j++) {
                if (ppdConflictProbability[index][j] > 0) {
                    double sigma = vpConflictFlightList[j]->getSigma();
                    double mu = ppdDelayTime[index][j];
                    dSumPenalCost += sigma / sqrt(2.0 * M_PI) * exp(-pow(mu, 2) / (2 * pow(sigma, 2))) +
                                     mu / 2.0 * (1 - boost::math::erf(-mu / (sqrt(2.0) * sigma)));
                }
            }
            //i : min Pi-E[sum{p_ij}]
            dSumPenalCost = vdPi[index] - dSumPenalCost;
            if (dSumPenalCost < dMaxValue) {
                dMaxValue = dSumPenalCost;
                iMinIndex = index;
            }
        }
    }

    if (iMinIndex == -1) {
        std::cerr << "[ERROR] the selected index invalid !" << std::endl;
    }
    return iMinIndex;
}

void CalculateConflictProbability(const FlightVector &vpConflictFlightList, double **ppdConflictProbability,
                                  double **ppdDelayTime, bool bGeometricMethod) {
    int iSize = (int) vpConflictFlightList.size();
    bool bWait = true;
    double dDelay = 0;
    for (int i = 0; i < iSize - 1; i++) {
        Flight *fi = vpConflictFlightList[i];
        for (int j = i + 1; j < iSize; j++) {
            Flight *fj = vpConflictFlightList[j];
            ppdConflictProbability[i][j] = ppdConflictProbability[j][i] = fi->CalculateProbabilityConflictAndDelayForFlight(
                    fj, &dDelay,
                    &bWait, bGeometricMethod);
            if (bWait) {
                ppdDelayTime[i][j] = dDelay;
                ppdDelayTime[j][i] = 0;
            } else {
                ppdDelayTime[i][j] = 0;
                ppdDelayTime[j][i] = dDelay;
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
    std::default_random_engine generator;

    uni_dist UniformDist(0, 1);
    double p = UniformDist(generator);
    if (p < vdParameter[5]) {
        normal_dist NormalDist(vdParameter[0] * vdParameter[3],
                               dSigma);
        return -fabs(NormalDist(generator));
    } else {
        normal_dist NormalDist(vdParameter[1] * vdParameter[4],
                               dSigma);
        return fabs(NormalDist(generator));
    }
}

double GaussianDistribution2(const vdList &vdParameter, double dSigma) {
    std::default_random_engine generator;
    normal_dist NormalDist(0, dSigma);
    return NormalDist(generator);
}

double GaussianDistribution1(const vdList &vdParameter, double dSigma) {
    std::default_random_engine generator;
    normal_dist NormalDist((vdParameter[1] - vdParameter[0]) / 2,
                           dSigma);
    return NormalDist(generator);
}

viList getComplement(int iSize, const viList &candidateList) {
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

void ApproximateFLAEstimation(Network *pNetwork, const vdList &vdParameter, double dEpsilon, double dCoefPi,
                              double *dSumBenefits, int *iMaxNbConflict, int iModeMethod, int iFeasibleSize,
                              int iModeRandom) {
    int iNbFlightNotAssigned = 0;
//    int iLastNbFlightNotAssigned = 0;
    int iNbFlightChangeLevel = 0;
//    int iLastNbFlightChangeLevel = 0;
//    int maxLoopTime = 5;
//    int iLoopTime = 0;
//    bool bNotFinish = true;
    IloEnv env;
    std::ofstream cplexLogFile("cplexLog.txt", std::ios::out | std::ios::app);

    // Initialize the coefficient Pi for all flight in the network.
    pNetwork->InitCoefPi(dCoefPi);
    // Initialize the feasible flight levels for all flights in the network.
    pNetwork->InitFeasibleList(iFeasibleSize);
    // Initialize  the flight levels list of the network.
    pNetwork->InitFlightLevelsList();
    // Initialize the sigma for the random departure time.
    if (iModeMethod > 1) {
        pNetwork->SetSigma(vdParameter, iModeRandom);
    }

    FlightVector vpFlightList = pNetwork->getFlightsList();
    LevelVector viLevelsList = pNetwork->getFlightLevelsList();

    std::cout << "[INFO] Starting ApproximateFLA method..." << std::endl;
    ProcessClock processClock;
    //Start the process clock.
    processClock.start();
    try {
        FlightVector vpFlightNotAssigned = SolveFLA(vpFlightList, env, vdParameter, viLevelsList, dEpsilon,
                                                    dSumBenefits, iMaxNbConflict, iModeMethod,
                                                    iModeRandom, cplexLogFile);
        // Get the number of flights that change its most preferred flight level.
        iNbFlightChangeLevel = getNbFlightsChangeLevel(vpFlightList);
        // Get the number of unassigned flights.
        iNbFlightNotAssigned = (int) vpFlightNotAssigned.size();
    } catch (IloException &e) { std::cerr << e.getMessage() << std::endl; }
    catch (...) { std::cerr << "error" << std::endl; }
    cplexLogFile.close();
    env.end();
    processClock.end();
    std::cout << "Nb of flights change level: " << iNbFlightChangeLevel << std::endl;
    std::cout << "Nb of flights not be assigned: " << iNbFlightNotAssigned << std::endl;
    std::cout << "Max conflict: " << *iMaxNbConflict << std::endl;
    std::cout << "Sum benefits: " << *dSumBenefits << std::endl;
    std::cout << "Elapsed time:" << processClock.getCpuTime() << std::endl;
}
