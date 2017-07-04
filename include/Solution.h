//
// Created by chenghaowang on 30/06/17.
//

#ifndef SOLUTION_H
#define SOLUTION_H

#include "ProcessClock.h"
#include "Network.h"
#include "Solver.h"
int MinIndexArgs0(int iConflictedFlightSize, double **probabilityConflict, double **delayWithoutConflict, DoubleVector &vdPi){
    int iIndex = -1;
    double dMaxValue=std::numeric_limits<double>::max();
    for (int i = 0;i < iConflictedFlightSize;i++){
        double dSumPenalCost=0;
        for (int j =0;j< iConflictedFlightSize;j++ )
        {
            dSumPenalCost += delayWithoutConflict[i][j]*probabilityConflict[i][j]/2;
        }
        dSumPenalCost = vdPi[i] - dSumPenalCost;
        if (dSumPenalCost < dMaxValue){
            dMaxValue = dSumPenalCost;
            iIndex = i;
        }
    }
    return iIndex;
}

int MinIndexArgument(int iConflictedFlightSize, double **probabilityConflict, IntVector &viConstraintsList, double **delayWithoutConflict, DoubleVector &vdPi,
                     IloNumArray &decisionVariables){
    int iIndex = -1;
    double dMaxValue=std::numeric_limits<double>::max();
    for (int i=0;i < iConflictedFlightSize;i++){
        if (contains(viConstraintsList, i)){
            continue;
        }
        double dSumPenalCost=0;
        for (int j=0;j< iConflictedFlightSize;j++ )
        {
            if (probabilityConflict[i][j] > 0){
                dSumPenalCost += delayWithoutConflict[i][j]*(probabilityConflict[i][j])*decisionVariables[j]/2;
            }
        }
        dSumPenalCost=vdPi[i] - dSumPenalCost;
        if (dSumPenalCost < dMaxValue){
            dMaxValue = dSumPenalCost;
            iIndex = i;
        }
    }
    return iIndex;
}

void CalculateConflictProbability(FlightVector &vpConflictFlightList, double** probabilityConflict, double** delayWithoutConflict, int modeConflict){
    int size = (int)vpConflictFlightList.size();
    int *IOrJ = new int();
    double *delay = new double();
    for (int i = 0; i < size - 1; i++) {
        Flight* fi = vpConflictFlightList[i];
        for (int j = i + 1; j < size; j++) {
            Flight * fj = vpConflictFlightList[j];
            if (*fi == *fj) {
                continue;
            }
            probabilityConflict[i][j] = probabilityConflict[j][i] = fi->getProbabilityConflict(*fj, delay, IOrJ, modeConflict);
            if (*IOrJ == 1){
                delayWithoutConflict[i][j] = *delay;
                delayWithoutConflict[j][i] = 0;
            }else{
                delayWithoutConflict[i][j] = 0;
                delayWithoutConflict[j][i] = *delay;
            }
        }
    }
    delete delay;
    delete IOrJ;
}


bool FeasibilityHoeffding(double dEpsilon, IloNumArray &decisionVariables, DoubleVector &vdPi, double **delayWithoutConflict, int iConflictedFlightSize, double **probabilityConflict) {
    for (int i=0;i<iConflictedFlightSize;i++){
        double temp=0;
        if (decisionVariables[i]==1){
            for (int j=0;j<iConflictedFlightSize;j++){
                if (probabilityConflict[i][j] > 0){
                    temp += probabilityConflict[i][j]*delayWithoutConflict[i][j]*decisionVariables[j]/2;
                }
            }
            double temp2=0;
            for (int j=0;j<iConflictedFlightSize;j++){
                if (probabilityConflict[i][j] > 0){
                    temp2 += delayWithoutConflict[i][j]*delayWithoutConflict[i][j]*decisionVariables[j];
                }
            }
            temp=exp(-((2* pow(vdPi[i] - temp, 2))/temp2));
            if (temp > dEpsilon){
                return false;
            }
        }
    }
    return true;
}

bool FeasibilityMonteCarlo(double dEpsilon, IloNumArray &decisionVariables, FlightVector &vpConflictedFlightList, int *piIndex, IntVector &viConstraintsList, DoubleVector &vdPi, int mode, double dSigma1, double dSigma2, double dW1, double dW2, double dRatio, int ModeConflict){
    int iConflictedFlightsSize = (int)vpConflictedFlightList.size();
    int iConflictedCounter=0;
    int iMaxConflict = -1;
    int iMaxIndex = -1;
    double** delayWithoutConflict=new double *[iConflictedFlightsSize];
    double** probabilityConflict=new double *[iConflictedFlightsSize];

    for (int i=0;i<iConflictedFlightsSize;i++){
        delayWithoutConflict[i]=new double[iConflictedFlightsSize];
        probabilityConflict[i]=new double[iConflictedFlightsSize];
    }
    *piIndex=-1;
    int *table=new int[iConflictedFlightsSize];
    for (int i=0;i<iConflictedFlightsSize;i++){
        table[i]=0;
    }
    for (int a=0;a< ITERATIONCOUNT;a++){
        for (int i=0;i<iConflictedFlightsSize;i++){
            for (int j=0;j<iConflictedFlightsSize;j++){
                delayWithoutConflict[i][j]=0;
                probabilityConflict[i][j]=0;
            }
        }
        bool test=true;
        FlightVector ListFlight;
        // to generate a new scenario from current flight list.
        for (int j=0; j< iConflictedFlightsSize; j++){
            //fot the aircraft who is assigned in LP model, and not be contained in Constraint List.
            if (decisionVariables[j]==1 && !contains(viConstraintsList, j)){
                Flight * fj=vpConflictedFlightList[j];
                Flight *flight = fj->clone();
                double dNewDepartureTime;
                switch(mode){
                    case 1:
                        dNewDepartureTime = randPiecewiseData(flight->getDepartureTime());
                        break;
                    default:
                        dNewDepartureTime = flight->getDepartureTime();
                        std::cerr << "[Fatal Error]: the mode of generate a new scenario doesn't support!" << std::endl;
                        break;
                }
                Route *pRoute = flight->getRoute()->update(dNewDepartureTime);
                flight->setRoute(pRoute);
                ListFlight.push_back(flight);
            }
        }
        CalculateConflictProbability(ListFlight, probabilityConflict, delayWithoutConflict, ModeConflict);
        for (int i=0;i<iConflictedFlightsSize;i++){
            double sum=0;
            for (int j=0;j<iConflictedFlightsSize;j++){
                sum +=delayWithoutConflict[i][j];
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
    for (int i=0;i<iConflictedFlightsSize;i++){
        if (decisionVariables[i]==1){
            if (table[i]> iMaxConflict&&!contains(viConstraintsList, i)){
                iMaxConflict = table[i];
                iMaxIndex = i;
            }
        }
    }
    *piIndex=iMaxIndex;
    delete table;
    for (int i=0;i<iConflictedFlightsSize;i++){
        delete[] delayWithoutConflict[i];
        delete[] probabilityConflict[i];
    }
    delete[] delayWithoutConflict;
    delete[] probabilityConflict;
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


void InitConflictedListAndCalculateMi(FlightVector &vpCandidateFlightList, FlightVector *vpConflictFlightList,
                                      DoubleVector *pvdMi, int ModeConflict){
    double *delay = new double();
    int *IOrJ = new int();
    for(int i = 0; i < (int)vpCandidateFlightList.size(); i++){
        Flight *fi = vpCandidateFlightList[i];
        double SumConffi = 0.0;
        bool test2 = false;
        for(int j = 0; j < (int)vpCandidateFlightList.size(); j++){
            Flight *fj= vpCandidateFlightList[j];
            if (*fi == *fj) {
                continue;
            }
            double prob = fi->getProbabilityConflict(*fj, delay, IOrJ, ModeConflict);
            if (prob > 0) {
                test2 = true;
                SumConffi += COEFPIJ;
            }
        }
        if (test2) {
            vpConflictFlightList->push_back(fi);
            pvdMi->push_back(SumConffi);
        }
    }
    delete delay;
    delete IOrJ;
}

void CalculatePi(FlightVector &vpConflictFlightList, DoubleVector* pvdPi){
    for (int i = 0; i < (int)vpConflictFlightList.size(); i++)
    {
        Flight* fi = vpConflictFlightList[i];
        double hour = ( fi->getArrivingTime() - fi->getDepartureTime()) / 60.0;
        if (hour <= 1.0) {
            pvdPi->push_back(COEFPI);
        }
        else {
            pvdPi->push_back(COEFPI + (hour - 1.0)*0.1); // c=0.1
        }
    }
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
        const LevelSet& levels = flight->getLevelSet();
        Level iPreferredLevel = flight->getDefaultLevel();
        if (iPreferredLevel == iProcessingLevel) {
            CandidateFlightList->push_back(flight);
        }
        else {
            for(auto &level : levels){
                if (level == iProcessingLevel && flightAssignmentMap[flight] == false && levelEx[iPreferredLevel] == true)
                {
                    CandidateFlightList->push_back(flight);
                    continue;
                }
            }
        }
    }
}

void InitAssignedFlights(int iProcessingLevel, FlightVector &CandidateFlightList, FlightVector &ConflictFlightList,
                         double *somme_objectif, FlightAssignmentMap *flightAssignmentMap){
    for(auto &&flight: CandidateFlightList){
        // if the aircraft is not in the actual candidate list.
        if (!contains(ConflictFlightList, flight)) {
            if (flight->getDefaultLevel() == iProcessingLevel) {
                *somme_objectif +=3;
            }
            else {
                *somme_objectif +=1;
            }
            flight->setCurrentLevel(iProcessingLevel);
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

void ApproximateFLA(Network *pNetwork, double *somme_objectif, double *maxConflictCount, int mode, double dSigma1, double dSigma2, double dW1, double dW2, double dRatio, int ModeConflict){
    LevelExamine levelEx;
    ProcessClock processClock;
    processClock.start();
    IloEnv env;
    try {
        const FlightVector &vpFlightList = pNetwork->getFlightList();
        IntVector viLevelsList = pNetwork->getLevelList();
        FlightAssignmentMap flightAssignmentMap;
        InitFlightAssignmentMap(vpFlightList, &flightAssignmentMap);
        InitExamineLevel(viLevelsList, &levelEx);
        for (IntVector::iterator itA = viLevelsList.begin(); itA != viLevelsList.end(); itA++) {
            int iProcessingLevel = (*itA);
            FlightVector CandidateFlightList;
            FlightVector ConflictFlightList;
            DoubleVector Mi, Pi;
            InitCandidateFlights(vpFlightList, flightAssignmentMap, levelEx, iProcessingLevel, &CandidateFlightList);
            InitConflictedListAndCalculateMi(CandidateFlightList, &ConflictFlightList, &Mi, ModeConflict);
            levelEx[iProcessingLevel] = true;
            InitAssignedFlights(iProcessingLevel, CandidateFlightList, ConflictFlightList, somme_objectif,
                                &flightAssignmentMap);
            int size = (int)ConflictFlightList.size();
            double** probability = new double *[size];
            double** delai = new double *[size];
            for (int i = 0; i < size; i++) {
                probability[i] = new double[size];
                delai[i] = new double[size];
            }
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    probability[i][j] = 0;
                    delai[i][j] = 0;
                }
            }
            CalculateConflictProbability(ConflictFlightList, probability, delai, ModeConflict);
            CalculatePi(ConflictFlightList, &Pi);
            // verify the conflicted flight list iConflictedFlightsSize
            // if the iConflictedFlightsSize = 0, then the flight hasn't any conflict with another aircraft in the same level.
            if (ConflictFlightList.size() == 0) {
                std::cout << "no conflict" << std::endl;
                continue;
            }
            // if all others conflicted flights don't assign in their preferred level.
            bool test = false;
            for(auto &&flight: ConflictFlightList){
                if (flight->getDefaultLevel() == iProcessingLevel) {
                    test = true;
                }
            }
            if (!test) {
                std::cout << " Tous les vols n'a pas cette niveau commme le niveau prefere" << std::endl;
                //continue;
            }
            GetMaxConflictCount(probability, (int)ConflictFlightList.size(), maxConflictCount);
            // to find the index of aircraft that has minimal sum of Pij.
            int index = MinIndexArgs0(size, probability, delai, Pi);
            IntVector viConstraintList;
            viConstraintList.push_back(index);
            Solver *solver = new Solver(env, ConflictFlightList, iProcessingLevel, Mi, Pi, probability, delai, viConstraintList);
            solver->solve();
            double dFunctionObjectiveValue = solver->getFunctionObjectiveValue();
            IloNumArray *decisionVariablesValues = solver->getDecisionVariablesValues();
            delete solver;
            int * ind = new int();
            /*Test de feasibility of each constraint*/
            switch (mode) {
                case 0:
                    /*Hoeffding*/
                    while (!FeasibilityHoeffding(EPSILON, *decisionVariablesValues, Pi, delai, size, probability) && (int)viConstraintList.size() != size) {
                        index = MinIndexArgument(size, probability, viConstraintList, delai, Pi,
                                                 *decisionVariablesValues);
                        viConstraintList.push_back(index);
                        solver = new Solver(env, ConflictFlightList, iProcessingLevel, Mi, Pi, probability, delai, viConstraintList);
                        solver->solve();
                        dFunctionObjectiveValue = solver->getFunctionObjectiveValue();
                        decisionVariablesValues->end();
                        decisionVariablesValues = solver->getDecisionVariablesValues();
                        delete solver;
                    }
                    break;
                case 2:/*MonteCarlo*/
                    while (!FeasibilityMonteCarlo(EPSILON, *decisionVariablesValues, ConflictFlightList, ind, viConstraintList, Pi, mode, dSigma1, dW1, dSigma2, dW2, dRatio, ModeConflict) && (int)viConstraintList.size() != size) {
                        //Si on ne trouve pas une bonne index dans la test, on utilise la formule
                        if (*ind == -1) {
                            index = MinIndexArgument(size, probability, viConstraintList, delai, Pi,
                                                     *decisionVariablesValues);
                        }
                        else {
                            index = *ind;
                        }
                        viConstraintList.push_back(index);
                        solver = new Solver(env, ConflictFlightList, iProcessingLevel, Mi, Pi, probability, delai, viConstraintList);
                        solver->solve();
                        dFunctionObjectiveValue = solver->getFunctionObjectiveValue();
                        decisionVariablesValues->end();
                        decisionVariablesValues = solver->getDecisionVariablesValues();
                        delete solver;
                    }
                    break;
            }
            delete ind;
            if ((int)viConstraintList.size() == size) {
                std::cout << "No solution feasible for this level" << std::endl;
            }
            for (int i = 0; i < size; i++) {
                if ((*decisionVariablesValues)[i] == 1) {
                    Flight *fi = ConflictFlightList[i];
                    fi->setCurrentLevel(iProcessingLevel);
                    flightAssignmentMap[fi] = true;
                }
            }
            *somme_objectif += dFunctionObjectiveValue;
            for (int i = 0; i < size; i++) {
                delete[] probability[i];
                delete[] delai[i];
            }
            delete[] probability;
            delete[] delai;
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
        std::cout << "Nombre de flight change level:" << compteur << std::endl;
        std::cout << "Nombre de flight non affecte:" << compteur2 << std::endl;
        std::cout << "La somme des valeurs d'objectifs:" << somme_objectif << std::endl;
        std::cout << "Max conflict:" << maxConflictCount << std::endl;
    }
    catch (IloException& ex) {
        std::cerr << "Error: " << ex << std::endl;
    }
    catch (...) {
        std::cerr << "Error: " << std::endl;
    }
    env.end();
    processClock.end();
}
#endif //SOLUTION_H
