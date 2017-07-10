//
// Created by chenghaowang on 30/06/17.
//

#ifndef SOLUTION_H
#define SOLUTION_H

#include "ProcessClock.h"
#include "Solver.h"

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
const double COEFPI = 5;
const double COEFPIJ = 1;//0.7
const double EPSILON = 0.05;

double UniformDistribution(double dLowerBound, double dUpperBound){
    uni_dist UniformDist(dLowerBound, dUpperBound);
    return  UniformDist(generator);
}

double GaussianDistribution1(double t) {
    normal_dist NormalDist((Flight::BETA - Flight::ALPHA)/2, (Flight::ALPHA + Flight::BETA) / (2 * sqrt(2) * boost::math::erf_inv(Flight::GAMMA)));
    return t+NormalDist(generator);
}

double GaussianDistribution2(double t) {
    normal_dist NormalDist(0, (Flight::ALPHA + Flight::BETA) / (2 * sqrt(2) * boost::math::erf_inv(Flight::GAMMA)));
    return t+NormalDist(generator);
}

double GaussianDistribution3(double t){
    double p = UniformDistribution(0, 1);
    if (p > Flight::RATIO){
        normal_dist NormalDist(Flight::mu_1_1, Flight::sigma_1_1);
        return t+fabs(NormalDist(generator));
    }else{
        normal_dist NormalDist(Flight::mu_1_2, Flight::sigma_1_2);
        return t+fabs(NormalDist(generator));
    }
}

void InitTable(double** probabilityConflict, double** delayWithoutConflict, double** penalCost, int iSize){
    for (int i=0;i<iSize;i++){
        for (int j=0;j<iSize;j++){
            delayWithoutConflict[i][j]=0;
            probabilityConflict[i][j]=0;
            penalCost[i][j]=0;
        }
    }
}



void DestroyTable(double** probabilityConflict, double** delayWithoutConflict, double** penalCost, int iSize){
    for (int i=0;i<iSize;i++){
        delete[] delayWithoutConflict[i];
        delete[] probabilityConflict[i];
        delete[] penalCost[i];
    }
    delete[] delayWithoutConflict;
    delete[] probabilityConflict;
    delete[] penalCost;
}

void InitConflictedList(FlightVector &vpCandidateFlightList, FlightVector *vpConflictFlightList, bool bGeometricMethod, int iModeRandom){
    double *pdDelay = new double();
    bool *pbWhoWait = new bool();
    for(auto && fi: vpCandidateFlightList){
        for(auto&& fj: vpCandidateFlightList){
            if (*fi == *fj) {
                continue;
            }
            double prob = fi->getProbabilityConflict(fj, pdDelay, pbWhoWait, bGeometricMethod, iModeRandom);
            if (prob > 0) {
                vpConflictFlightList->push_back(fi);
                break;
            }
        }
    }
    delete pdDelay;
    delete pbWhoWait;
}

void CalculateP_ijAndMi(double **probabilityConflict, double **delayWithoutConflict, double **penalCost,
                        int iConflictedFlightSize, DoubleVector *pvdMi){
    double *pdDelay = new double();
    bool *pbWhoWait = new bool();
    for (int i = 0 ; i < iConflictedFlightSize; i++) {
        double sumPenalCost = 0.0;
        for (int j = 0 ; j < iConflictedFlightSize; j++) {
            if (probabilityConflict[i][j] > Flight::MIN_PROBA){
                penalCost[i][j] = COEFPIJ*delayWithoutConflict[i][j];
                sumPenalCost += penalCost[i][j];
            }
        }
        pvdMi->push_back(sumPenalCost);
    }
    delete pdDelay;
    delete pbWhoWait;
}

void CalculatePi(FlightVector &vpConflictFlightList, DoubleVector *pvdPi){
    for(auto &&fi: vpConflictFlightList){
        double hour = ( fi->getArrivingTime() - fi->getDepartureTime()) / 60.0;
        if (hour <= 1.0) {
            pvdPi->push_back(COEFPI);
        }
        else {
            pvdPi->push_back(COEFPI + (hour - 1.0)*0.1); // c=0.1
        }
    }
}

void CalculateConflictProbability(FlightVector &vpConflictFlightList, double** probabilityConflict, double** delayWithoutConflict, bool bGeometricMethod, int iModeRandom){
    int size = (int)vpConflictFlightList.size();
    bool *pbWhoWait = new bool();
    double *pdDelay = new double(0);
    for (int i = 0; i < size - 1; i++) {
        Flight* fi = vpConflictFlightList[i];
        for (int j = i + 1; j < size; j++) {
            Flight * fj = vpConflictFlightList[j];
            probabilityConflict[i][j] = probabilityConflict[j][i] = fi->getProbabilityConflict(fj, pdDelay, pbWhoWait, bGeometricMethod, iModeRandom);
            if (*pbWhoWait){
                delayWithoutConflict[i][j] = *pdDelay;
                delayWithoutConflict[j][i] = 0;
            }else{
                delayWithoutConflict[i][j] = 0;
                delayWithoutConflict[j][i] = *pdDelay;
            }
        }
    }
    delete pdDelay;
    delete pbWhoWait;
}

int MinIndexArgs0(IntVector &viSearchList, int iConflictedFlightSize, double **probabilityConflict, double **delayWithoutConflict, DoubleVector &vdPi, bool bGeometricMethod){

    int iMinIndex = -1;
    double dMaxValue = std::numeric_limits<double>::max();
    if (bGeometricMethod) {
        for(auto && index: viSearchList){
            double dSumPenalCost=0;
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
    }
    if (iMinIndex == -1){
        std::cerr<<"[ERROR] the selected index invalid !" << std::endl;
    }
    return iMinIndex;
}

int MinIndexArgs1(IntVector &viSearchList, int iConflictedFlightSize, double **probabilityConflict,
                  double **delayWithoutConflict, DoubleVector &vdPi,
                  IloNumArray &decisionVariables, bool bGeometricMethod){
    int iMinIndex = -1;
    double dMaxValue=std::numeric_limits<double>::max();
    if (bGeometricMethod){
        for(auto && index: viSearchList){
            double dSumPenalCost=0;
            for(int j =0; j< iConflictedFlightSize; j++){
                dSumPenalCost += delayWithoutConflict[index][j]*(probabilityConflict[index][j])*decisionVariables[j]/2;
            }
            //i : min Pi-E[sum{p_ij}]
            dSumPenalCost = vdPi[index] - dSumPenalCost;
            if (dSumPenalCost < dMaxValue) {
                dMaxValue = dSumPenalCost;
                iMinIndex = index;
            }
        }
    }else{

    }
    if (iMinIndex == -1){
        std::cerr<<"[ERROR] the selected index invalid !" << std::endl;
    }
    return iMinIndex;
}

bool FeasibilityHoeffding(double dEpsilon, IloNumArray &decisionVariables, DoubleVector &vdPi, double **delayWithoutConflict, int iConflictedFlightSize, double **probabilityConflict) {
    for (int i = 0; i <iConflictedFlightSize; i++) {
        double temp = 0;
        if (decisionVariables[i] == 1) {
            for (int j = 0; j < iConflictedFlightSize; j++) {
                if (probabilityConflict[i][j] > 0) {
                    temp +=probabilityConflict[i][j] * delayWithoutConflict[i][j] * decisionVariables[j] / 2;
                }
            }
            temp = vdPi[i] - temp;
            temp = 2 * pow(temp, 2);
            double temp2 = 0;
            for (int j = 0; j < iConflictedFlightSize; j++) {
                if (probabilityConflict[i][j] > 0) {
                    temp2 +=pow(delayWithoutConflict[i][j] ,2) * decisionVariables[j];
                }
            }
            temp /= temp2;
            temp = exp(-temp);
            if (temp > dEpsilon) {
                return false;
            }
        }
    }
    return true;
}

bool FeasibilityMonteCarlo(double dEpsilon, IloNumArray &decisionVariables, FlightVector &vpConflictedFlightList, int *piIndex, IntVector &viSearchList, DoubleVector &vdPi,  bool bGeometricMethod, int iModeRandom){
    int iConflictedFlightsSize = (int)vpConflictedFlightList.size();
    int iConflictedCounter=0;
    int iMaxConflict = -1;
    int iMaxIndex = -1;

    double** delayWithoutConflict=new double *[iConflictedFlightsSize];
    double** probabilityConflict=new double *[iConflictedFlightsSize];
    double** penalCost= new double*[iConflictedFlightsSize];

    for (int i=0;i<iConflictedFlightsSize;i++){
        delayWithoutConflict[i]=new double[iConflictedFlightsSize];
        probabilityConflict[i]=new double[iConflictedFlightsSize];
        penalCost[i]=new double[iConflictedFlightsSize];
    }
    *piIndex=-1;
    int *table=new int[iConflictedFlightsSize];
    for (int i=0;i<iConflictedFlightsSize;i++){
        table[i]=0;
    }
    std::cout << "[INFO] Generate a new scenario." << std::endl;
    for (int a=0;a< 1000;a++){
        InitTable(probabilityConflict, delayWithoutConflict, penalCost, iConflictedFlightsSize);
        bool test=true;
        FlightVector ListFlight;
        // to generate a new scenario from current flight list.
        for(auto && j: viSearchList){
            if (decisionVariables[j] == 1){
                Flight *fj = vpConflictedFlightList[j];
                Time iOldDepartureTime = fj->getDepartureTime();
                Time iNewDepartureTime = 0;
                switch (iModeRandom) {
                    case 0:
                        iNewDepartureTime = GaussianDistribution1(iOldDepartureTime);
                        break;
                    case 1:
                        iNewDepartureTime = GaussianDistribution2(iOldDepartureTime);
                        break;
                    default:
                        iNewDepartureTime = GaussianDistribution3(iOldDepartureTime);
                        break;
                }
                ListFlight.push_back(fj->GenerateNewFlight(iNewDepartureTime, bGeometricMethod));
            }
        }
        if (ListFlight.size()==0){
            continue;
        }
        CalculateConflictProbability(ListFlight, probabilityConflict, delayWithoutConflict, bGeometricMethod, iModeRandom);
        for (int i=0;i<iConflictedFlightsSize;i++){
            double sum=0;
            for (int j=0;j<iConflictedFlightsSize;j++){
                sum +=penalCost[i][j];
            }
            if (sum > vdPi[i]){
                test = false;
                table[i]=table[i]+1;
                break;
            }
        }
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
    DestroyTable(probabilityConflict, delayWithoutConflict, penalCost, iConflictedFlightsSize);
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
        const LevelVector& levels = flight->getFeasibleLevelsList();
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
                std::cout<<"[INFO] change the level from " << flight->getDefaultLevel() << " to " << iProcessingLevel << std::endl;
            (*flightAssignmentMap)[flight] = true;
        }
    }
}

void GetMaxConflictCount(double **probability, int iConflictedFlightsSize, double *maxConflictCount){
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

void ApproximateFLA(Network *pNetwork, double *somme_objectif, double *maxConflictCount, bool bGeometricMethod, int iModeRandom, int iModeMethod){
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
            std::cout << "OK" << std::endl <<"\t\tCandidate flights size: " <<  CandidateFlightList.size() <<std::endl;
            std::cout << "\tInitialize the Conflicted Flight list..." << std::flush;
            InitConflictedList(CandidateFlightList, &ConflictFlightList, bGeometricMethod, iModeRandom);
            std::cout << "OK" << std::endl << "\t\tConflicted flights size: " << ConflictFlightList.size() << std::endl;
            levelEx[iProcessingLevel] = true;
            std:: cout << "\tAssign the flights which have no conflict with the other flights in current level..." << std::endl;
            InitAssignedFlights(iProcessingLevel, CandidateFlightList, ConflictFlightList, somme_objectif, &flightAssignmentMap);
            std::cout << "OK" << std::endl << "\t\tNon-Conflicted flights size: " << CandidateFlightList.size()-ConflictFlightList.size() << std::endl;
            if (ConflictFlightList.size() == 0) {
                std::cout << "[INFO] All flights in current level can be assigned without any conflict." << std::endl;
                continue;
            }
            std:: cout << "\tCreate and Initialize the conflict table..." << std::flush;
            double **probability=new double *[(int)ConflictFlightList.size()];
            double **delai=new double *[(int)ConflictFlightList.size()];
            double **penalCost=new double *[(int)ConflictFlightList.size()];

            for (int i=0;i<(int)ConflictFlightList.size();i++){
                probability[i]=new double[(int)ConflictFlightList.size()];
                delai[i]=new double[(int)ConflictFlightList.size()];
                penalCost[i]=new double[(int)ConflictFlightList.size()];
            }
            InitTable(probability, delai, penalCost, (int)ConflictFlightList.size());
            std:: cout << "OK" << std::endl;
            std:: cout << "\tCalculate the conflict probability and delay time to avoid the conflict..." << std::flush;
            CalculateConflictProbability(ConflictFlightList, probability, delai, bGeometricMethod, iModeRandom);
            std:: cout << "OK" << std::endl;
            std:: cout << "\tCalculate the p_ij and Mi vector..." << std::flush;
            CalculateP_ijAndMi(probability, delai, penalCost, (int) ConflictFlightList.size(), &Mi);
            std:: cout << "OK" << std::endl;
            std:: cout << "\tCalculate Pi vector..." << std::flush;
            CalculatePi(ConflictFlightList, &Pi);
            std:: cout << "OK" << std::endl;
            bool bIsAssignedPreferred = false;
            for(auto &&flight: ConflictFlightList){
                if (flight->getDefaultLevel() == iProcessingLevel) {
                    bIsAssignedPreferred = true;
                }
            }
            if (!bIsAssignedPreferred) {
                std::cout << "[INFO] No flight in current will be assigned as its preferred flight level." << std::endl;
            }
            GetMaxConflictCount(probability, (int)ConflictFlightList.size(), maxConflictCount);
            //step one///////////////////////////////////////////////////////////////////////
            // to find the index of aircraft that has minimal sum of Pij.
            std::cout << "\tGo to step one:" << std::endl;
            IntVector viSearchList;
            for(int i = 0; i < (int)ConflictFlightList.size(); i++) {
                viSearchList.push_back(i);
            }
            int index = MinIndexArgs0(viSearchList, (int)ConflictFlightList.size(), probability, delai, Pi, bGeometricMethod);
            std::cout << "\t\tMin index:" << index <<std::endl;
            IntVector viConstraintList;
            viConstraintList.push_back(index);
            viSearchList.erase(std::remove(viSearchList.begin(), viSearchList.end(), index), viSearchList.end());
            Solver *solver = new Solver(env, ConflictFlightList, iProcessingLevel, Mi, Pi, penalCost, viConstraintList, cplexLogFile);
            solver->solve();
            double dFunctionObjectiveValue = solver->getFunctionObjectiveValue();
            IloNumArray decisionVariablesValues = solver->getDecisionVariablesValues();
            delete solver;
            //finish step one////////////////////////////////////////////////////////////////
            int * ind = new int();
            /*Test de feasibility of each constraint*/
            switch (iModeMethod) {
                case 0:
                    /*Hoeffding*/
                    std::cout<< "[INFO] Using Hoeffding method" << std::endl;
                    while (!FeasibilityHoeffding(EPSILON, decisionVariablesValues, Pi, delai, (int)ConflictFlightList.size(), probability) && viSearchList.size() > 0) {
                        std::cout << "\tGo to step two:" << std::endl;
                        index = MinIndexArgs1(viSearchList, (int)ConflictFlightList.size(), probability, delai, Pi,
                                              decisionVariablesValues, bGeometricMethod);
                        std::cout << "\t\tMin index:" << index <<std::endl;
                        viConstraintList.push_back(index);
                        viSearchList.erase(std::remove(viSearchList.begin(), viSearchList.end(), index), viSearchList.end());
                        solver = new Solver(env, ConflictFlightList, iProcessingLevel, Mi, Pi, penalCost, viConstraintList, cplexLogFile);

                        solver->solve();
                        dFunctionObjectiveValue = solver->getFunctionObjectiveValue();
                        decisionVariablesValues.end();
                        decisionVariablesValues = solver->getDecisionVariablesValues();
                        delete solver;
                    }
                    break;
                case 1:/*MonteCarlo*/
                    std::cout<< "[INFO] Using Monte-Carlo method" << std::endl;
                    while (!FeasibilityMonteCarlo(EPSILON, decisionVariablesValues, ConflictFlightList, ind, viConstraintList, Pi, bGeometricMethod, iModeRandom) && viSearchList.size() > 0) {
                        //Si on ne trouve pas une bonne index dans la test, on utilise la formule
                        std::cout << "\tGo to step two:" << std::endl;
                        if (*ind == -1) {
                            index = MinIndexArgs1(viSearchList, (int)ConflictFlightList.size(), probability, delai, Pi,
                                                  decisionVariablesValues, bGeometricMethod);
                        }
                        else {
                            index = *ind;
                        }
                        std::cout << "\t\tMin index:" << index <<std::endl;
                        viConstraintList.push_back(index);
                        viSearchList.erase(std::remove(viSearchList.begin(), viSearchList.end(), index), viSearchList.end());
                        solver = new Solver(env, ConflictFlightList, iProcessingLevel, Mi, Pi, penalCost, viConstraintList, cplexLogFile);
                        solver->solve();
                        dFunctionObjectiveValue = solver->getFunctionObjectiveValue();
                        decisionVariablesValues.end();
                        decisionVariablesValues = solver->getDecisionVariablesValues();
                        delete solver;
                    }
                    break;
            }
            delete ind;
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
            *somme_objectif += dFunctionObjectiveValue;
            DestroyTable(probability, delai, penalCost, (int)ConflictFlightList.size());
            std::cout<< "Ok" << std::endl;
        }// end loop of level list
        int compteur = 0;
        int compteur2 = 0;
        for (FlightVector::const_iterator itf = vpFlightList.begin(); itf != vpFlightList.end(); itf++) {
            Flight *fi = (*itf);
            if (fi->getCurrentLevel() != fi->getDefaultLevel()) {
                compteur++;
            }
            if (flightAssignmentMap[fi] == false) {
                compteur2++;
            }
        }
        std::cout << "OK" <<std::endl;
        std::cout << "Nombre de flight change level:" << compteur << std::endl;
        std::cout << "Nombre de flight non affecte:" << compteur2 << std::endl;
        std::cout << "La somme des valeurs d'objectifs:" << *somme_objectif << std::endl;
        std::cout << "Max conflict:" << *maxConflictCount << std::endl;
        cplexLogFile.close();
        env.end();
        processClock.end();
        std::cout << "elapsed time = " << processClock.getCpuTime() <<std::endl;
    }
    catch (IloException& e) {std::cerr<<e.getMessage()<< std::endl;}
    catch (...) {std::cerr<<"error" << std::endl; }
}
#endif //SOLUTION_H
