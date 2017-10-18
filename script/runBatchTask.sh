#!/usr/bin/env bash
# start generate one data
./runProgram.sh 15 45 9957 33 20 5 0 1 1 30 3 0 10 10 1000 0
./runProgram.sh 15 45 9957 33 20 5 15 1 1 30 3 0 10 10 1000 0
for a in 15 ; do
    for b in 45; do
        for p in 9957; do
            for w1 in 33;  do
                for w2 in 20; do
                    for p in 5 ; do
                        for pa in 0 15; do
			    #./runProgram.sh ${a} ${b} 9957 ${w1} ${w2} ${p} ${pa} 1 1 30 3 0 10 10 1000 0
                            for periodLength in 30 ; do
                                for feasibleSize in 3 ; do
                                    for method in 0; do
                                        for epsilon in 10 15 20; do
                                            for coefPi in 8; do
                                                ./runProgram.sh ${a} ${b} 9957 ${w1} ${w2} ${p} ${pa} 1 0 ${periodLength} ${feasibleSize} ${method} ${epsilon} ${coefPi} 1000 0
                                            done
                                        done
                                    done
                                    for method in 1 3 2; do
                                        for epsilon in 10 15 20; do
                                            for coefPi in 8; do
                                                ./runProgram.sh ${a} ${b} 9957 ${w1} ${w2} ${p} ${pa} 1 0 ${periodLength} ${feasibleSize} ${method} ${epsilon} ${coefPi} 1000 0
                                            done
                                        done
                                    done
                                    for method in 2; do
                                        for epsilon in 10; do
                                            for coefPi in 8; do
                                                for nbiterations in 500 5000 10000;do
                                                    ./runProgram.sh ${a} ${b} 9957 ${w1} ${w2} ${p} ${pa} 1 0 ${periodLength} ${feasibleSize} ${method} ${epsilon} ${coefPi} ${nbiterations} 0
                                                done
                                            done
                                        done
                                    done
                                done
                            done
                        done
                    done
                done
            done
        done
    done
done
