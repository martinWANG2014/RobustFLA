//
// Created by chenghaowang on 25/05/17.
//

#include "../include/MathFunction.h"

double randDepartureTimeNormalDist(double dDepartureTime) {
    normal_dist NormalDistribution1(dDepartureTime, 3/3);
    normal_dist NormalDistribution2(dDepartureTime, 2/3);
    double dOffset;
    return dDepartureTime + (rand_100(generator) > 50)
           ? (dOffset = NormalDistribution2(generator), dOffset < 0 ? -dOffset : dOffset)
           : (dOffset = NormalDistribution1(generator), dOffset > 0 ? -dOffset : dOffset);
}

double randDepartureTimeInterval(double dDepartureTime) {
    double dSeed;
    double dSeed_2;
    return dDepartureTime + (dSeed = rand_100(generator), dSeed_2 = rand_100(generator), dSeed < 83)
            ? (dSeed < 63
                // n1~[0, 19)
                ? (dSeed < 19
                    ? (dSeed_2 < 30 ? (rand_3(generator) + (dSeed_2 < 10
                        // n2~[0, 10)
                        ? -15
                        // n2~[10, 30)
                        : -12))
                    : (dSeed_2 < 60
                        // n2~[30, 60)                                                                                       // n2~(30, 60]
                        ? (rand_2(generator) - 9)
                        // n2~[60, 100)
                        : (rand_3(generator) - 7)))
                // n1~[19, 63)
                : (rand_3(generator) + (dSeed_2 < 75
                    ? (dSeed_2 < 25
                        // n2~[0, 25)
                        ? -4
                        // n2~[25, 75)
                        : -1)
                    // n2~[75, 100)
                    : 2)))
            // n1~[63, 83)
            : (dSeed_2 < 70
                ? (dSeed_2 < 40
                    // n2~[0, 40)
                    ? (rand_3(generator) + 5)
                    // n2~[40, 70)
                    : (rand_2(generator) + 8))
                : (rand_3(generator) + (dSeed_2 > 90
                    // n2~[70, 90)
                    ? 10
                    // n2~[90, 100)
                    : 13))))
        // n1~[83, 93)
        : (rand_5(generator) + (dSeed < 93
            ? (dSeed_2 < 80
                ? (dSeed_2 < 50
                    // n2~[0, 50)
                    ? 16
                    // n2~[50, 80)
                    : 21)
                // n2~[80, 100)
                : 26)
            // n1~[93, 100)
            : (dSeed_2 < 75
                ? (dSeed_2 < 55
                    ? (dSeed_2 < 30
                        // n2~[0, 30)
                        ? 31
                        // n2~[30, 55)
                        : 36)
                    // n2~[55, 75)
                    : 41)
                : (dSeed_2 < 95
                    ? (dSeed_2 < 85
                        // n2~[75, 85)
                        ? 46
                        // n2~[85, 95)
                        : 51)
                    // n2~[95, 100)
                    : 56)
            )));
}
