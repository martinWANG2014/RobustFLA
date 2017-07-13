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
#include <map>
#include <set>
/******************************************************************************/
typedef std::vector<int> IntVector;
typedef std::vector<double> DoubleVector;
typedef int Level;
typedef std::map<Level, bool> LevelExamine;
typedef std::uniform_real_distribution<double> uni_dist;
std::default_random_engine generator;
//const double COEFPI = 0.15;
const double COEFPIJ = 1;//0.7
const double min_sigma = 0.1;
double UniformDistribution(double dLowerBound, double dUpperBound){
    uni_dist UniformDist(dLowerBound, dUpperBound);
    return  UniformDist(generator);
}

double GaussianDistribution1(const DoubleVector &vdParameter) {
    normal_dist NormalDist((vdParameter[1] - vdParameter[0]) / 2,
                           getSigma1(vdParameter[0], vdParameter[1], vdParameter[2]));
    return NormalDist(generator);
}

double GaussianDistribution2(const DoubleVector &vdParameter) {
    normal_dist NormalDist(0, getSigma2(vdParameter[0], vdParameter[1], vdParameter[2]));
    return NormalDist(generator);
}

double GaussianDistribution3(const DoubleVector &vdParameter) {
    double p = UniformDistribution(0, 1);
    if (p < vdParameter[5]) {
        normal_dist NormalDist(vdParameter[0] * vdParameter[3],
                               getSigma(vdParameter[0], vdParameter[2], vdParameter[3]));
        return -fabs(NormalDist(generator));
    }else{
        normal_dist NormalDist(vdParameter[1] * vdParameter[4],
                               getSigma(vdParameter[1], vdParameter[2], vdParameter[4]));
        return fabs(NormalDist(generator));
    }
}

double **CreateTable(int iSize) {
    double **ppdDouble = new double *[iSize];
    for (int i = 0; i < iSize; i++) {
        ppdDouble[i] = new double[iSize];
    }
    return ppdDouble;
}

void InitTable(double **ppdTable, int iSize) {
    for (int i=0;i<iSize;i++){
        for (int j=0;j<iSize;j++){
            ppdTable[i][j] = 0;
        }
    }
}

void DestroyTable(double **ppdTable, int iSize) {
    for (int i=0;i<iSize;i++){
        delete[] ppdTable[i];
    }
    delete[] ppdTable;
}

void InitConflictedList(const FlightVector &vpCandidateFlightList, FlightVector *vpConflictFlightList, bool bMethod) {
    double dDelay = 0;
    bool bWait = true;
    for (auto &&fi: vpCandidateFlightList) {
        for (auto &&fj: vpCandidateFlightList) {
            if (*fi == *fj) {
                continue;
            }
            double prob = fi->getProbabilityConflictAndDelay(fj, &dDelay, &bWait, bMethod);
            if (prob > 0) {
                vpConflictFlightList->push_back(fi);
                break;
            }
        }
    }
}

void CalculateMi(double **probabilityConflict, double **delayWithoutConflict,
                 int iConflictedFlightSize, DoubleVector *pvdMi) {
    for (int i = 0 ; i < iConflictedFlightSize; i++) {
        double sumPenalCost = 0.0;
        for (int j = 0 ; j < iConflictedFlightSize; j++) {
            if (probabilityConflict[i][j] > 0) {
                sumPenalCost += delayWithoutConflict[i][j];
            }
        }
        pvdMi->push_back(sumPenalCost);
    }
}

void CalculatePi(FlightVector &vpConflictFlightList, DoubleVector *pvdPi, double dCoefPi) {
    for(auto &&fi: vpConflictFlightList){
        double time = (fi->getArrivingTime() - fi->getDepartureTime());
        pvdPi->push_back(time * dCoefPi);
    }
}

void CalculateConflictProbability(FlightVector &vpConflictFlightList, double **probabilityConflict,
                                  double **delayWithoutConflict, double dCoefPij, bool bMethod) {
    int iSize = (int) vpConflictFlightList.size();
    bool bWait = true;
    double dDelay = 0;
    for (int i = 0; i < iSize - 1; i++) {
        Flight *fi = vpConflictFlightList[i];
        for (int j = i + 1; j < iSize; j++) {
            Flight *fj = vpConflictFlightList[j];
            probabilityConflict[i][j] = probabilityConflict[j][i] = fi->getProbabilityConflictAndDelay(fj, &dDelay,
                                                                                                       &bWait, bMethod);
            if (bWait) {
                delayWithoutConflict[i][j] = dCoefPij * dDelay;
                delayWithoutConflict[j][i] = 0;
            } else {
                delayWithoutConflict[i][j] = 0;
                delayWithoutConflict[j][i] = dCoefPij * dDelay;
            }
        }
    }
}

int MinIndexArgs0(IntVector &viSearchList, int iConflictedFlightSize, double **probabilityConflict,
                  double **delayWithoutConflict, DoubleVector &vdPi, bool bMethod) {
    int iMinIndex = -1;
    double dMaxValue = std::numeric_limits<double>::max();
    if (bMethod) {
        for (auto &&index: viSearchList) {
            double dSumPenalCost = 0;
            for (int j = 0; j < iConflictedFlightSize; j++) {
                dSumPenalCost += delayWithoutConflict[index][j] * probabilityConflict[index][j] / 2;
            }
            //i : min Pi-E[sum{p_ij}]
            dSumPenalCost = vdPi[index] - dSumPenalCost;
            if (dSumPenalCost < dMaxValue) {
                dMaxValue = dSumPenalCost;
                iMinIndex = index;
            }
        }
    }else{
        for (auto &&index: viSearchList) {
            double dSumPenalCost = 0;
            for (int j = 0; j < iConflictedFlightSize; j++) {
                dSumPenalCost += delayWithoutConflict[index][j] * delayWithoutConflict[index][j];
            }
            //i : min Pi-E[sum{p_ij}]
            dSumPenalCost = vdPi[index] - dSumPenalCost;
            if (dSumPenalCost < dMaxValue) {
                dMaxValue = dSumPenalCost;
                iMinIndex = index;
            }
        }
    }

    if (iMinIndex == -1){
        std::cerr<<"[ERROR] the selected index invalid !" << std::endl;
    }
    return iMinIndex;
}
int MinIndexArgs1(IntVector &viSearchList, int iConflictedFlightSize, double **probabilityConflict,
                  double **delayWithoutConflict, DoubleVector &vdPi,
                  IloNumArray &decisionVariables, bool bMethod) {
    int iMinIndex = -1;
    double dMaxValue=std::numeric_limits<double>::max();
    if (bMethod) {
        for (auto &&index: viSearchList) {
            double dSumPenalCost = 0;
            for (int j = 0; j < iConflictedFlightSize; j++) {
                dSumPenalCost +=
                        delayWithoutConflict[index][j] * (probabilityConflict[index][j]) * decisionVariables[j] / 2;
            }
            //i : min Pi-E[sum{p_ij}]
            dSumPenalCost = vdPi[index] - dSumPenalCost;
            if (dSumPenalCost < dMaxValue) {
                dMaxValue = dSumPenalCost;
                iMinIndex = index;
            }
        }
    }else{
        for (auto &&index: viSearchList) {
            double dSumPenalCost = 0;
            for (int j = 0; j < iConflictedFlightSize; j++) {
                dSumPenalCost +=
                        delayWithoutConflict[index][j] * (probabilityConflict[index][j]) * decisionVariables[j];
            }
            //i : min Pi-E[sum{p_ij}]
            dSumPenalCost = vdPi[index] - dSumPenalCost;
            if (dSumPenalCost < dMaxValue) {
                dMaxValue = dSumPenalCost;
                iMinIndex = index;
            }
        }
    }
    if (iMinIndex == -1){
        std::cerr<<"[ERROR] the selected index invalid !" << std::endl;
    }
    return iMinIndex;
}
bool FeasibilityHoeffding(double dEpsilon, IloNumArray &decisionVariables, DoubleVector &vdPi, double **delayWithoutConflict, int iConflictedFlightSize, double **probabilityConflict) {
    for (int i = 0; i <iConflictedFlightSize; i++) {
        double temp = 0;
        double temp2 = 0;
        if (decisionVariables[i] == 1) {
            for (int j = 0; j < iConflictedFlightSize; j++) {
                if (j != i) {
                    temp +=probabilityConflict[i][j] * delayWithoutConflict[i][j] * decisionVariables[j] / 2;
                    temp2 +=pow(delayWithoutConflict[i][j] ,2) * decisionVariables[j];
                }
            }
            temp = 2 * pow(vdPi[i] - temp, 2);
            temp = exp(-temp / temp2);
            if (temp > dEpsilon) {
                return false;
            }
        }
    }
    return true;
}

bool FeasibilityGaussian(double dEpsilon, IloNumArray &decisionVariables, DoubleVector &vdPi,
                         FlightVector vpConflictedFlightList, double **ppdPenalCost) {
    int iSize = (int) vpConflictedFlightList.size();
    for (int i = 0; i < iSize; i++) {
        if (decisionVariables[i] == 1) {
            double exp_mu = 0.0;
            double exp_sigma_2 = 0.0;
            for (int j = 0; j < iSize; j++) {
                if (j != i) {
                    exp_mu += ppdPenalCost[i][j] * decisionVariables[j];
                    exp_sigma_2 += (pow(vpConflictedFlightList[i]->getSigma(), 2) +
                                    pow(vpConflictedFlightList[j]->getSigma(), 2)) * pow(decisionVariables[j], 2);
                }
            }
            std::cout << (vdPi[i] - exp_mu) / (sqrt(2 * exp_sigma_2)) << std::endl;
            double prob = 0.5 * (boost::math::erf((vdPi[i] - exp_mu) / (sqrt(2 * exp_sigma_2))) + 1);

            std::cout << "exp_sigma : " << exp_sigma_2 << std::endl;
            std::cout << "proba: " << prob << std::endl;
            if (prob < 1 - dEpsilon) {
                return false;
            }
        }
    }
    return true;
}

bool FeasibilityMonteCarlo(double dEpsilon, IloNumArray &decisionVariables, FlightVector &vpConflictedFlightList,
                           int *piIndex, IntVector &viSearchList, DoubleVector &vdPi, int iModeRandom, double dCoefPij,
                           DoubleVector &vdParameter) {
    int iConflictedFlightsSize = (int)vpConflictedFlightList.size();
    int iConflictedCounter=0;
    int iMaxConflict = -1;
    int iMaxIndex = -1;

    double **delayWithoutConflict = CreateTable(iConflictedFlightsSize);
    double **probabilityConflict = CreateTable(iConflictedFlightsSize);
    *piIndex=-1;
    int *table=new int[iConflictedFlightsSize];
    for (int i=0;i<iConflictedFlightsSize;i++){
        table[i]=0;
    }
    std::cout << "[INFO] Generate a new scenario." << std::endl;
    for (int a=0;a< 1000;a++){
        InitTable(probabilityConflict, iConflictedFlightsSize);
        InitTable(delayWithoutConflict, iConflictedFlightsSize);
        bool test=true;
        FlightVector ListFlight;
        IntVector IndexFlight;
        // to generate a new scenario from current flight list.
        for (int k = 0; k < (int) viSearchList.size(); k++) {
            int j = viSearchList[k];
            if (decisionVariables[j] == 1){
                Flight *fj = vpConflictedFlightList[j];
                Time iOldDepartureTime = fj->getDepartureTime();
                Time iDelta = 0;
                switch (iModeRandom) {
                    case 0:
                        iDelta = GaussianDistribution1(vdParameter);
                        break;
                    case 1:
                        iDelta = GaussianDistribution2(vdParameter);
                        break;
                    default:
                        iDelta = GaussianDistribution3(vdParameter);
                        break;
                }
                Flight *flight = fj->GenerateNewFlight(iOldDepartureTime + iDelta, true);
                ListFlight.push_back(flight);
                IndexFlight.push_back(j);
            }
        }
        if (ListFlight.size()==0){
            continue;
        }
        CalculateConflictProbability(ListFlight, probabilityConflict, delayWithoutConflict, dCoefPij, true);
        for (auto &&fl: ListFlight) {
            delete fl;
        }
        ListFlight.clear();
        for (int i = 0; i < (int) IndexFlight.size(); i++) {
            double sum=0;
            for (int j = 0; j < (int) IndexFlight.size(); j++) {
                sum += delayWithoutConflict[i][j];
            }
            if (sum > vdPi[IndexFlight[i]]) {
                test = false;
                table[i]=table[i]+1;
                break;
            }
        }
        IndexFlight.clear();
        if (!test){
            iConflictedCounter ++;
        }
    }
    for(auto && item : viSearchList){
        if (decisionVariables[item] ==1 && table[item]> iMaxConflict){
            iMaxConflict = table[item];
            iMaxIndex = item;
        }
    }
    *piIndex=iMaxIndex;
    delete table;
    DestroyTable(probabilityConflict, iConflictedFlightsSize);
    DestroyTable(delayWithoutConflict, iConflictedFlightsSize);
//    std::cout << 1000-iConflictedCounter << std::endl;
    if (dEpsilon==0.1){
        if (1000-iConflictedCounter < 915){
            return false;
        }
    }
    if (dEpsilon==0.05){
        if (1000-iConflictedCounter < 961)
        {
            return false;
        }
    }
    if (dEpsilon==0.15){
        if (1000-iConflictedCounter <868){
            return false;
        }
    }
    if (dEpsilon==0.2){
        if (1000-iConflictedCounter < 821){
            return false;
        }
    }
    if (dEpsilon==0.25){
        if (1000-iConflictedCounter <772){
            return false;
        }
    }
    return true;
}

void InitFlightAssignmentMap(const FlightVector &vpFlightList, FlightAssignmentMap *pmpFlightAssignmentMap){
    for(auto &&flight: vpFlightList){
        (*pmpFlightAssignmentMap)[flight] = false;
    }
}

void InitExamineLevel(IntVector &viLevelsList, LevelExamine *pvpLevelEx){
    for(auto &&level: viLevelsList){
        (*pvpLevelEx)[level] = false;
    }
}

void InitCandidateFlights(const FlightVector &vpFlightList, FlightAssignmentMap &flightAssignmentMap,
                          LevelExamine &levelEx, int iProcessingLevel, FlightVector *CandidateFlightList){
    for(auto &&flight: vpFlightList){
        const LevelVector &levels = flight->getFeasibleLevelList();
        Level iPreferredLevel = flight->getDefaultLevel();
        if (iPreferredLevel == iProcessingLevel) {
            CandidateFlightList->push_back(flight);
        }
        else {
            for(auto &&level : levels){
                if (level == iProcessingLevel && flightAssignmentMap[flight] == false && levelEx[iPreferredLevel] == true) {
                    CandidateFlightList->push_back(flight);
                    break;
                }
            }
        }
    }
}

void InitAssignedFlights(int iProcessingLevel, FlightVector &CandidateFlightList, FlightVector &ConflictFlightList,
                         double *somme_objectif, FlightAssignmentMap *flightAssignmentMap){
    for(auto &&flight: CandidateFlightList){
        // if the flight is not in the actual candidate list.
        if (!contains(ConflictFlightList, flight)) {
            if (flight->getDefaultLevel() == iProcessingLevel) {
                *somme_objectif +=3;
            }
            else {
                *somme_objectif +=1;
            }
            flight->setCurrentLevel(iProcessingLevel);
            if (flight->getDefaultLevel() != iProcessingLevel)
                std::cout << "[INFO] flight: " << flight->getCode() << " change the level from "
                          << flight->getDefaultLevel() << " to " << iProcessingLevel << std::endl;
            (*flightAssignmentMap)[flight] = true;
        }
    }
}

void GetMaxConflictCount(double **probability, int iConflictedFlightsSize, int *maxConflictCount) {
    for (int i = 0; i < iConflictedFlightsSize; i++) {
        int temp = 0;
        for (int j = 0; j < iConflictedFlightsSize; j++) {
            if (probability[i][j]>0) {
                temp++;
            }
        }
        if (temp > *maxConflictCount) {
            *maxConflictCount = temp;
        }
    }
}

void ApproximateFLA(Network *pNetwork, double *dSumBenefits, int *iMaxNbConflict, int iModeMethod, int iModeRandom,
                    double epsilon, double dCoefPI, double dCoefPij, DoubleVector vdParameter) {
    std::ofstream cplexLogFile("cplexLog.txt", std::ios::out|std::ios::app);
    LevelExamine levelEx;
    ProcessClock processClock;
    IloEnv env;
    processClock.start();
    const FlightVector &vpFlightList = pNetwork->getFlightsList();
    LevelVector viLevelsList = pNetwork->getFlightLevelsList();
    FlightAssignmentMap flightAssignmentMap;
    InitFlightAssignmentMap(vpFlightList, &flightAssignmentMap);
    InitExamineLevel(viLevelsList, &levelEx);
    std:: cout << "[INFO] Starting ApproximateFLA method..." << std::endl;
    try {
        for (auto itA = viLevelsList.begin(); itA != viLevelsList.end(); itA++) {
            int iProcessingLevel = (*itA);
            std::cout << "[INFO] Processing Level: " << iProcessingLevel << std::endl;
            FlightVector CandidateFlightList;
            FlightVector ConflictFlightList;
            DoubleVector Mi, Pi;
            std::cout << "\tInitialize the Candidate Flight list..." << std::flush;
            InitCandidateFlights(vpFlightList, flightAssignmentMap, levelEx, iProcessingLevel, &CandidateFlightList);
            std::cout << "OK" << std::endl << "\t\tCandidate flights size: " << CandidateFlightList.size() << std::endl;

            std::cout << "\tInitialize the Conflicted Flight list..." << std::flush;
            InitConflictedList(CandidateFlightList, &ConflictFlightList, iModeMethod < 2);
            std::cout << "OK" << std::endl << "\t\tConflicted flights size: " << ConflictFlightList.size() << std::endl;
            levelEx[iProcessingLevel] = true;
            std::cout << "\tAssign the flights which have no conflict with the other flights in current level..."
                      << std::endl;
            InitAssignedFlights(iProcessingLevel, CandidateFlightList, ConflictFlightList, dSumBenefits,
                                &flightAssignmentMap);
            std::cout << "OK" << std::endl << "\t\tNon-Conflicted flights size: "
                      << CandidateFlightList.size() - ConflictFlightList.size() << std::endl;
            if (ConflictFlightList.size() == 0) {
                std::cout << "[INFO] All flights in current level can be assigned without any conflict." << std::endl;
                continue;
            }
            std::cout << "\tCreate and Initialize the conflict table..." << std::flush;
            double **probability = CreateTable((int) ConflictFlightList.size());
            double **delai = CreateTable((int) ConflictFlightList.size());
            InitTable(probability, (int) ConflictFlightList.size());
            InitTable(delai, (int) ConflictFlightList.size());
            std::cout << "OK" << std::endl;
            std::cout << "\tCalculate the conflict probability and delay time to avoid the conflict..." << std::flush;

            CalculateConflictProbability(ConflictFlightList, probability, delai, dCoefPij, iModeMethod < 2);
            std::cout << "OK" << std::endl;
            std::cout << "\tCalculate the p_ij and Mi vector..." << std::flush;
            CalculateMi(probability, delai, (int) ConflictFlightList.size(), &Mi);
            std::cout << "OK" << std::endl;
            std::cout << "\tCalculate Pi vector..." << std::flush;
            CalculatePi(ConflictFlightList, &Pi, dCoefPI);
            std::cout << "OK" << std::endl;
            bool bIsAssignedPreferred = false;
            for (auto &&flight: ConflictFlightList) {
                if (flight->getDefaultLevel() == iProcessingLevel) {
                    bIsAssignedPreferred = true;
                }
            }
            if (!bIsAssignedPreferred) {
                std::cout << "[INFO] No flight in current will be assigned as its preferred flight level." << std::endl;
            }
            GetMaxConflictCount(probability, (int) ConflictFlightList.size(), iMaxNbConflict);
            //step one///////////////////////////////////////////////////////////////////////
            // to find the index of aircraft that has minimal sum of Pij.
            std::cout << "\tGo to step one:" << std::endl;
            IntVector viSearchList;
            for (int i = 0; i < (int) ConflictFlightList.size(); i++) {
                viSearchList.push_back(i);
            }
            int index;
            index = MinIndexArgs0(viSearchList, (int) ConflictFlightList.size(), probability, delai, Pi,
                                  iModeMethod < 2);
            std::cout << "\t\tMin index:" << index << std::endl;
            IntVector viConstraintList;
            viConstraintList.push_back(index);
            viSearchList.erase(std::remove(viSearchList.begin(), viSearchList.end(), index), viSearchList.end());
            Solver *solver = new Solver(env, ConflictFlightList, iProcessingLevel, Mi, Pi, delai, viConstraintList,
                                        cplexLogFile);
            solver->solve();
            double dFunctionObjectiveValue = solver->getFunctionObjectiveValue();
            IloNumArray decisionVariablesValues = solver->getDecisionVariablesValues();
            delete solver;
            //finish step one////////////////////////////////////////////////////////////////
            int iIndexFromSimulation;
            /*Test de feasibility of each constraint*/
            switch (iModeMethod) {
                case 0:
                    /*Hoeffding*/
                    std::cout << "[INFO] Using Hoeffding method" << std::endl;
                    while (!FeasibilityHoeffding(epsilon, decisionVariablesValues, Pi, delai,
                                                 (int) ConflictFlightList.size(), probability) &&
                           viSearchList.size() > 0) {
                        std::cout << "\tGo to step two:" << std::endl;
                        index = MinIndexArgs1(viSearchList, (int) ConflictFlightList.size(), probability, delai, Pi,
                                              decisionVariablesValues, true);
                        std::cout << "\t\tMin index:" << index << std::endl;
                        viConstraintList.push_back(index);
                        viSearchList.erase(std::remove(viSearchList.begin(), viSearchList.end(), index),
                                           viSearchList.end());
                        solver = new Solver(env, ConflictFlightList, iProcessingLevel, Mi, Pi, delai,
                                            viConstraintList, cplexLogFile);

                        solver->solve();
                        dFunctionObjectiveValue = solver->getFunctionObjectiveValue();
                        decisionVariablesValues.end();
                        decisionVariablesValues = solver->getDecisionVariablesValues();
                        delete solver;
                    }
                    break;
                case 1:/*MonteCarlo*/
                    std::cout << "[INFO] Using Monte-Carlo method" << std::endl;
                    while (!FeasibilityMonteCarlo(epsilon, decisionVariablesValues, ConflictFlightList,
                                                  &iIndexFromSimulation,
                                                  viSearchList, Pi, iModeRandom, dCoefPij, vdParameter) &&
                           viSearchList.size() > 0) {
                        //Si on ne trouve pas une bonne index dans la test, on utilise la formule
                        std::cout << "\tGo to step two:" << std::endl;
                        if (iIndexFromSimulation == -1) {
                            index = MinIndexArgs1(viSearchList, (int) ConflictFlightList.size(), probability, delai, Pi,
                                                  decisionVariablesValues, true);
                        } else {
                            index = iIndexFromSimulation;
                        }
                        std::cout << "\t\tMin index:" << index << std::endl;
                        viConstraintList.push_back(index);
                        viSearchList.erase(std::remove(viSearchList.begin(), viSearchList.end(), index),
                                           viSearchList.end());
                        solver = new Solver(env, ConflictFlightList, iProcessingLevel, Mi, Pi, delai,
                                            viConstraintList, cplexLogFile);
                        solver->solve();
                        dFunctionObjectiveValue = solver->getFunctionObjectiveValue();
                        decisionVariablesValues.end();
                        decisionVariablesValues = solver->getDecisionVariablesValues();
                        delete solver;
                    }
                    break;
                case 2:/*Gaussian*/
                    std::cout << "[INFO] Using Gaussian method" << std::endl;
                    while (!FeasibilityGaussian(epsilon, decisionVariablesValues, Pi, ConflictFlightList, delai) &&
                           viSearchList.size() > 0) {
                        //Si on ne trouve pas une bonne index dans la test, on utilise la formule
                        std::cout << "\tGo to step two:" << std::endl;
                        index = MinIndexArgs1(viSearchList, (int) ConflictFlightList.size(), probability, delai, Pi,
                                              decisionVariablesValues, true);
                        std::cout << "\t\tMin index:" << index << std::endl;
                        viConstraintList.push_back(index);
                        viSearchList.erase(std::remove(viSearchList.begin(), viSearchList.end(), index),
                                           viSearchList.end());
                        solver = new Solver(env, ConflictFlightList, iProcessingLevel, Mi, Pi, delai,
                                            viConstraintList, cplexLogFile);
                        solver->solve();
                        dFunctionObjectiveValue = solver->getFunctionObjectiveValue();
                        decisionVariablesValues.end();
                        decisionVariablesValues = solver->getDecisionVariablesValues();
                        delete solver;
                    }
                    break;
            }
            if (viSearchList.size() == 0) {
                std::cout << "No solution feasible for this level" << std::endl;
            }
            for (int i = 0; i < (int)ConflictFlightList.size(); i++) {
                if ((decisionVariablesValues)[i] == 1) {
                    Flight *fi = ConflictFlightList[i];
                    fi->setCurrentLevel(iProcessingLevel);
                    if (fi->getDefaultLevel() != iProcessingLevel)
                        std::cout<<"[INFO] change the level from " << fi->getDefaultLevel() << "to " << iProcessingLevel << std::endl;
                    flightAssignmentMap[fi] = true;
                }
            }
            *dSumBenefits += dFunctionObjectiveValue;
            DestroyTable(probability, (int) ConflictFlightList.size());
            DestroyTable(delai, (int) ConflictFlightList.size());
            std::cout<< "Ok" << std::endl;
        }// end loop of level list
        int iNbFlightChangeLevel = 0;
        int iNbFlightNotAssigned = 0;
        for (FlightVector::const_iterator itf = vpFlightList.begin(); itf != vpFlightList.end(); itf++) {
            Flight *fi = (*itf);
            if (fi->getCurrentLevel() != fi->getDefaultLevel()) {
                iNbFlightChangeLevel++;
            }
            if (flightAssignmentMap[fi] == false) {
                iNbFlightNotAssigned++;
            }
        }
        std::cout << "OK" <<std::endl;
        std::cout << "Nb of flights change level:" << iNbFlightChangeLevel << std::endl;
        std::cout << "Nb of flights not be assigned:" << iNbFlightNotAssigned << std::endl;
        std::cout << "Sum of benefits :" << *dSumBenefits << std::endl;
        std::cout << "Max conflict:" << *iMaxNbConflict << std::endl;
        cplexLogFile.close();
        env.end();
        processClock.end();
        std::cout << "Elapsed time:" << processClock.getCpuTime() << std::endl;
    }
    catch (IloException& e) {std::cerr<<e.getMessage()<< std::endl;}
    catch (...) {std::cerr<<"error" << std::endl; }
}
#endif //SOLUTION_H
