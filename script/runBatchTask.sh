#!/usr/bin/env bash
for periodLength in 30 ; do
    for feasibleSize in 3 ; do
        for a in 15 ; do
            for b in 45 60; do
                for w1 in 33;  do
                    for w2 in 25; do
                        for p in 5 10; do
                            for coefPi in 5 8 10 15; do
                                ./runProgram.sh 0 ${periodLength} 0 ${feasibleSize} ${coefPi} ${a} ${b} 9957 ${w1} ${w2} ${p} 0 1
                                for epsilon in 25 20 15 10 5; do
                                    ./runProgram.sh 1 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} ${a} ${b} 9957 ${w1} ${w2} ${p} 0 1
                                    ./runProgram.sh 3 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} ${a} ${b} 9957 ${w1} ${w2} ${p} 0 1
                                    ./runProgram.sh 2 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} ${a} ${b} 9957 ${w1} ${w2} ${p} 0 1
                                done
                            done
                        done
                    done
                done
            done
        done
    done
done
