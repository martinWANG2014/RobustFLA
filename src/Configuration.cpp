//
// Created by chenghaowang on 14/07/17.
//

#include <fstream>
#include "../include/Configuration.h"
#include "../include/Network.h"


LevelVector findFeasibleLevels(Level iDefaultLevel, int iSize) {
    LevelVector feasibleList;
    feasibleList.push_back(iDefaultLevel);
    int iCount;
    bool notFinish;
    bool hasElement;
    auto position = LevelIFRB.end();
    switch (iDefaultLevel % 4) {
        // in IFR B group
        case 0:
            position = std::find(LevelIFRB.begin(), LevelIFRB.end(), iDefaultLevel);
            iCount = 1;
            notFinish = true;
            hasElement = true;
            while (notFinish) {
                if (position + iCount < LevelIFRB.end()) {
                    feasibleList.push_back(*(position + iCount));
                } else {
                    hasElement = false;
                }
                if ((int) feasibleList.size() == iSize) {
                    notFinish = false;
                }
                if (notFinish && (position - iCount > LevelIFRB.begin() - 1)) {
                    feasibleList.push_back(*(position - iCount));
                    hasElement = true;
                }
                if (!hasElement || (int) feasibleList.size() == iSize) {
                    notFinish = false;
                }
                iCount++;
            }
            break;
            // in VFR B group
        case 1:
            position = std::find(LevelVFRB.begin(), LevelVFRB.end(), iDefaultLevel);
            iCount = 1;
            notFinish = true;
            hasElement = true;
            while (notFinish) {
                if (position + iCount < LevelVFRB.end()) {
                    feasibleList.push_back(*(position + iCount));
                } else {
                    hasElement = false;
                }
                if ((int) feasibleList.size() == iSize) {
                    notFinish = false;
                }
                if (notFinish && (position - iCount > LevelVFRB.begin() - 1)) {
                    feasibleList.push_back(*(position - iCount));
                    hasElement = true;
                }
                if (!hasElement || (int) feasibleList.size() == iSize) {
                    notFinish = false;
                }
                iCount++;
            }
            break;
            // in IFR A group
        case 2:
            position = std::find(LevelIFRA.begin(), LevelIFRA.end(), iDefaultLevel);
            iCount = 1;
            notFinish = true;
            hasElement = true;
            while (notFinish) {
                if (position + iCount < LevelIFRA.end()) {
                    feasibleList.push_back(*(position + iCount));
                } else {
                    hasElement = false;
                }
                if ((int) feasibleList.size() == iSize) {
                    notFinish = false;
                }
                if (notFinish && (position - iCount > LevelIFRA.begin() - 1)) {
                    feasibleList.push_back(*(position - iCount));
                    hasElement = true;
                }
                if (!hasElement || (int) feasibleList.size() == iSize) {
                    notFinish = false;
                }
                iCount++;
            }
            break;
            // in VFR A group
        default:
            position = std::find(LevelVFRA.begin(), LevelVFRA.end(), iDefaultLevel);
            iCount = 1;
            notFinish = true;
            hasElement = true;
            while (notFinish) {
                if (position + iCount < LevelVFRA.end()) {
                    feasibleList.push_back(*(position + iCount));
                } else {
                    hasElement = false;
                }
                if ((int) feasibleList.size() == iSize) {
                    notFinish = false;
                }
                if (notFinish && (position - iCount > LevelVFRA.begin() - 1)) {
                    feasibleList.push_back(*(position - iCount));
                    hasElement = true;
                }
                if (!hasElement || (int) feasibleList.size() == iSize) {
                    notFinish = false;
                }
                iCount++;
            }
            break;
    }
    return feasibleList;
}

Level findNextFeasibleLevel(int iDefaultLevel, Level lastLevel) {
    int iOffset;
    auto position = LevelIFRB.end();
    auto postiionLast = LevelIFRB.end();
    switch (iDefaultLevel % 4) {
        // in IFR B group
        case 0:
            position = std::find(LevelIFRB.begin(), LevelIFRB.end(), iDefaultLevel);
            postiionLast = std::find(LevelIFRB.begin(), LevelIFRB.end(), lastLevel);
            iOffset = (int) (postiionLast - position);
            if (iOffset < 0) {
                if (position - iOffset + 1 < LevelIFRB.end()) {
                    return *(position - iOffset + 1);
                } else if (position + iOffset - 1 > LevelIFRB.begin() - 1) {
                    return *(position + iOffset - 1);
                } else {
                    return -1;
                }
            } else {
                if (position - iOffset > LevelIFRB.begin() - 1) {
                    return *(position - iOffset);
                } else if (position + iOffset + 1 < LevelIFRB.end()) {
                    return *(position + iOffset + 1);
                } else {
                    return -1;
                }
            }
            // in VFR B group
        case 1:
            position = std::find(LevelVFRB.begin(), LevelVFRB.end(), iDefaultLevel);
            postiionLast = std::find(LevelVFRB.begin(), LevelVFRB.end(), lastLevel);
            iOffset = (int) (postiionLast - position);
            if (iOffset < 0) {
                if (position - iOffset + 1 < LevelVFRB.end()) {
                    return *(position - iOffset + 1);
                } else if (position + iOffset - 1 > LevelVFRB.begin() - 1) {
                    return *(position + iOffset - 1);
                } else {
                    return -1;
                }
            } else {
                if (position - iOffset > LevelVFRB.begin() - 1) {
                    return *(position - iOffset);
                } else if (position + iOffset + 1 < LevelVFRB.end()) {
                    return *(position + iOffset + 1);
                } else {
                    return -1;
                }
            }
            // in IFR A group
        case 2:
            position = std::find(LevelIFRA.begin(), LevelIFRA.end(), iDefaultLevel);
            postiionLast = std::find(LevelIFRA.begin(), LevelIFRA.end(), lastLevel);
            iOffset = (int) (postiionLast - position);
            if (iOffset < 0) {
                if (position - iOffset + 1 < LevelIFRA.end()) {
                    return *(position - iOffset + 1);
                } else if (position + iOffset - 1 > LevelIFRA.begin() - 1) {
                    return *(position + iOffset - 1);
                } else {
                    return -1;
                }
            } else {
                if (position - iOffset > LevelIFRA.begin() - 1) {
                    return *(position - iOffset);
                } else if (position + iOffset + 1 < LevelIFRA.end()) {
                    return *(position + iOffset + 1);
                } else {
                    return -1;
                }
            }
            // in VFR A group
        default:
            position = std::find(LevelVFRA.begin(), LevelVFRA.end(), iDefaultLevel);
            postiionLast = std::find(LevelVFRA.begin(), LevelVFRA.end(), lastLevel);
            iOffset = (int) (postiionLast - position);
            if (iOffset < 0) {
                if (position - iOffset + 1 < LevelVFRA.end()) {
                    return *(position - iOffset + 1);
                } else if (position + iOffset - 1 > LevelVFRA.begin() - 1) {
                    return *(position + iOffset - 1);
                } else {
                    return -1;
                }
            } else {
                if (position - iOffset > LevelVFRA.begin() - 1) {
                    return *(position - iOffset);
                } else if (position + iOffset + 1 < LevelVFRA.end()) {
                    return *(position + iOffset + 1);
                } else {
                    return -1;
                }
            }
    }
}


bool exists(String filename) {
    std::ifstream file(filename);
    return file.good();
}


double getIntervalProbability(double mu, double sigma_2, double dLB, double dUB) {
    double dRight = (dUB - mu) / (sqrt(2.0 * sigma_2));
    double dLeft = (dLB - mu) / (sqrt(2.0 * sigma_2));
    return 0.5 * (boost::math::erf(dRight) - boost::math::erf(dLeft));
}

double getIntervalDensityProbability(double mu, double sigma_2, double dLB, double dUB) {
    double dRight = pow(dUB - mu, 2) / (2.0 * sigma_2);
    double dLeft = pow(dLB - mu, 2) / (2.0 * sigma_2);
    return 1.0 / sqrt(2 * M_PI * sigma_2) * (exp(-dRight) - exp(-dLeft));
}

double getExpectedValue(double maximalValue, double mu) {
    return P_1 * P_1 * ((SIGMA_2_1 + SIGMA_2_1) *
                        getIntervalDensityProbability(mu + MU_1 - MU_1, (SIGMA_2_1 + SIGMA_2_1), 0, maximalValue) +
                        (maximalValue - (mu + MU_1 - MU_1)) *
                        getIntervalProbability(mu + MU_1 - MU_1, (SIGMA_2_1 + SIGMA_2_1), 0, maximalValue)) +
           P_1 * P_2 * ((SIGMA_2_1 + SIGMA_2_2) *
                        getIntervalDensityProbability(mu + MU_1 - MU_2, (SIGMA_2_1 + SIGMA_2_2), 0, maximalValue) +
                        (maximalValue - (mu + MU_1 - MU_2)) *
                        getIntervalProbability(mu + MU_1 - MU_2, (SIGMA_2_1 + SIGMA_2_2), 0, maximalValue)) +
           P_1 * P_3 * ((SIGMA_2_1 + SIGMA_2_3) *
                        getIntervalDensityProbability(mu + MU_1 - MU_3, (SIGMA_2_1 + SIGMA_2_3), 0, maximalValue) +
                        maximalValue - ((mu + MU_1 - MU_3)) *
                                       getIntervalProbability(mu + MU_1 - MU_3, (SIGMA_2_1 + SIGMA_2_3), 0,
                                                              maximalValue)) +
           P_1 * P_4 * ((SIGMA_2_1 + SIGMA_2_4) *
                        getIntervalDensityProbability(mu + MU_1 - MU_4, (SIGMA_2_1 + SIGMA_2_4), 0, maximalValue) +
                        (maximalValue - (mu + MU_1 - MU_4)) *
                        getIntervalProbability(mu + MU_1 - MU_4, (SIGMA_2_1 + SIGMA_2_4), 0, maximalValue)) +
           P_2 * P_1 * ((SIGMA_2_2 + SIGMA_2_1) *
                        getIntervalDensityProbability(mu + MU_2 - MU_1, (SIGMA_2_2 + SIGMA_2_1), 0, maximalValue) +
                        (maximalValue - (mu + MU_2 - MU_1)) *
                        getIntervalProbability(mu + MU_2 - MU_1, (SIGMA_2_2 + SIGMA_2_1), 0, maximalValue)) +
           P_2 * P_2 * ((SIGMA_2_2 + SIGMA_2_2) *
                        getIntervalDensityProbability(mu + MU_2 - MU_2, (SIGMA_2_2 + SIGMA_2_2), 0, maximalValue) +
                        (maximalValue - (mu + MU_2 - MU_2)) *
                        getIntervalProbability(mu + MU_2 - MU_2, (SIGMA_2_2 + SIGMA_2_2), 0, maximalValue)) +
           P_2 * P_3 * ((SIGMA_2_2 + SIGMA_2_3) *
                        getIntervalDensityProbability(mu + MU_2 - MU_3, (SIGMA_2_2 + SIGMA_2_3), 0, maximalValue) +
                        (maximalValue - (mu + MU_2 - MU_3)) *
                        getIntervalProbability(mu + MU_2 - MU_3, (SIGMA_2_2 + SIGMA_2_3), 0, maximalValue)) +
           P_2 * P_4 * ((SIGMA_2_2 + SIGMA_2_4) *
                        getIntervalDensityProbability(mu + MU_2 - MU_4, (SIGMA_2_2 + SIGMA_2_4), 0, maximalValue) +
                        (maximalValue - (mu + MU_2 - MU_4)) *
                        getIntervalProbability(mu + MU_2 - MU_4, (SIGMA_2_2 + SIGMA_2_4), 0, maximalValue)) +
           P_3 * P_1 * ((SIGMA_2_3 + SIGMA_2_1) *
                        getIntervalDensityProbability(mu + MU_3 - MU_1, (SIGMA_2_3 + SIGMA_2_1), 0, maximalValue) +
                        (maximalValue - (mu + MU_3 - MU_1)) *
                        getIntervalProbability(mu + MU_3 - MU_1, (SIGMA_2_3 + SIGMA_2_1), 0, maximalValue)) +
           P_3 * P_2 * ((SIGMA_2_3 + SIGMA_2_2) *
                        getIntervalDensityProbability(mu + MU_3 - MU_2, (SIGMA_2_3 + SIGMA_2_2), 0, maximalValue) +
                        (maximalValue - (mu + MU_3 - MU_2)) *
                        getIntervalProbability(mu + MU_3 - MU_2, (SIGMA_2_3 + SIGMA_2_2), 0, maximalValue)) +
           P_3 * P_3 * ((SIGMA_2_3 + SIGMA_2_3) *
                        getIntervalDensityProbability(mu + MU_3 - MU_3, (SIGMA_2_3 + SIGMA_2_3), 0, maximalValue) +
                        (maximalValue - (mu + MU_3 - MU_3)) *
                        getIntervalProbability(mu + MU_3 - MU_3, (SIGMA_2_3 + SIGMA_2_3), 0, maximalValue)) +
           P_3 * P_4 * ((SIGMA_2_3 + SIGMA_2_4) *
                        getIntervalDensityProbability(mu + MU_3 - MU_4, (SIGMA_2_3 + SIGMA_2_4), 0, maximalValue) +
                        (maximalValue - (mu + MU_3 - MU_4)) *
                        getIntervalProbability(mu + MU_3 - MU_4, (SIGMA_2_3 + SIGMA_2_4), 0, maximalValue)) +
           P_4 * P_1 * ((SIGMA_2_4 + SIGMA_2_1) *
                        getIntervalDensityProbability(mu + MU_4 - MU_1, (SIGMA_2_4 + SIGMA_2_1), 0, maximalValue) +
                        (maximalValue - (mu + MU_4 - MU_1)) *
                        getIntervalProbability(mu + MU_4 - MU_1, (SIGMA_2_4 + SIGMA_2_1), 0, maximalValue)) +
           P_4 * P_2 * ((SIGMA_2_4 + SIGMA_2_2) *
                        getIntervalDensityProbability(mu + MU_4 - MU_2, (SIGMA_2_4 + SIGMA_2_2), 0, maximalValue) +
                        (maximalValue - (mu + MU_4 - MU_2)) *
                        getIntervalProbability(mu + MU_4 - MU_2, (SIGMA_2_4 + SIGMA_2_2), 0, maximalValue)) +
           P_4 * P_3 * ((SIGMA_2_4 + SIGMA_2_3) *
                        getIntervalDensityProbability(mu + MU_4 - MU_3, (SIGMA_2_4 + SIGMA_2_3), 0, maximalValue) +
                        (maximalValue - (mu + MU_4 - MU_3)) *
                        getIntervalProbability(mu + MU_4 - MU_3, (SIGMA_2_4 + SIGMA_2_3), 0, maximalValue)) +
           P_4 * P_4 * ((SIGMA_2_4 + SIGMA_2_4) *
                        getIntervalDensityProbability(mu + MU_4 - MU_4, 2 * (SIGMA_2_4 + SIGMA_2_4), 0, maximalValue) +
                        (maximalValue - (mu + MU_4 - MU_4)) *
                        getIntervalProbability(mu + MU_4 - MU_4, (SIGMA_2_4 + SIGMA_2_4), 0, maximalValue));
}


