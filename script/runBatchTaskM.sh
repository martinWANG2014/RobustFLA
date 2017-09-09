#!/usr/bin/env bash
#for periodLength in 30 60; do
#    for feasibleSize in 3 4 5; do
#        for coefPi in 5 10 15 20 25; do
#            for epsilon in 5 10 15 20 25; do
#                 ./runProgram.sh 0 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0
#                 ./runProgram.sh 1 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0
#                for a in 15; do
#                   for b in 45; do
#                        ./runProgram.sh 3 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 0 ${a} ${b} 95
##                        ./runProgram.sh 2 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 0 ${a} ${b} 95
##                        ./runProgram.sh 4 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 0 ${a} ${b} 95
##                        ./runProgram.sh 5 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 0 ${a} ${b} 95
#                        for w1 in  25 ;  do
#                            for w2 in  25 ; do
#                                for p in 40 50 60; do
##                                    ./runProgram.sh 5 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 2 ${a} ${b} 95 ${w1} ${w2} ${p}
##                                    ./runProgram.sh 4 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 2 ${a} ${b} 95 ${w1} ${w2} ${p}
#                                    ./runProgram.sh 3 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 2 ${a} ${b} 95 ${w1} ${w2} ${p}
##                                    ./runProgram.sh 2 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 2 ${a} ${b} 95 ${w1} ${w2} ${p}
#                                done
#                            done
#                        done
#                    done
#                done
#            done
#        done
#    done
#done

#for periodLength in 30 60; do
#    for feasibleSize in 3 4 5; do
#        for coefPi in 5 10 15 20 25; do
#            for epsilon in 5 10 15 20 25; do
##                 ./runProgram.sh 0 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0
#                 ./runProgram.sh 1 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0
##                for a in 15; do
##                   for b in 45; do
##                        ./runProgram.sh 3 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 0 ${a} ${b} 95
###                        ./runProgram.sh 2 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 0 ${a} ${b} 95
###                        ./runProgram.sh 4 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 0 ${a} ${b} 95
###                        ./runProgram.sh 5 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 0 ${a} ${b} 95
##                        for w1 in  25 ;  do
##                            for w2 in  25 ; do
##                                for p in 40 50 60; do
###                                    ./runProgram.sh 5 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 2 ${a} ${b} 95 ${w1} ${w2} ${p}
###                                    ./runProgram.sh 4 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 2 ${a} ${b} 95 ${w1} ${w2} ${p}
##                                    ./runProgram.sh 3 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 2 ${a} ${b} 95 ${w1} ${w2} ${p}
###                                    ./runProgram.sh 2 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 2 ${a} ${b} 95 ${w1} ${w2} ${p}
##                                done
##                            done
##                        done
##                    done
##                done
#            done
#        done
#    done
#done

#for periodLength in 30 60; do
#    for feasibleSize in 3 4 5; do
#        for coefPi in 5 10 15 20 25; do
#            for epsilon in 5 10 15 20 25; do
#                 ./runProgram.sh 0 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0
#                 ./runProgram.sh 1 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0
#                for a in 15; do
#                   for b in 45; do
#                        ./runProgram.sh 3 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 0 ${a} ${b} 95
#                        ./runProgram.sh 2 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 0 ${a} ${b} 95
#                        ./runProgram.sh 4 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 0 ${a} ${b} 95
#                        ./runProgram.sh 5 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 0 ${a} ${b} 95
#                        for w1 in  25 ;  do
#                            for w2 in  25 ; do
#                                for p in 40 50 60; do
#                                    ./runProgram.sh 5 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 2 ${a} ${b} 95 ${w1} ${w2} ${p}
#                                    ./runProgram.sh 4 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 2 ${a} ${b} 95 ${w1} ${w2} ${p}
#                                    ./runProgram.sh 3 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 2 ${a} ${b} 95 ${w1} ${w2} ${p}
#                                    ./runProgram.sh 2 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 2 ${a} ${b} 95 ${w1} ${w2} ${p}
#                                done
#                            done
#                        done
#                    done
#                done
#            done
#        done
#    done
#one

for periodLength in 30 ; do
    for feasibleSize in 3 ; do
        for coefPi in 5 10 15 20 25; do
            for epsilon in 5 10 15 20 25; do
#                 ./runProgram.sh 0 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0
#                 ./runProgram.sh 1 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0
                for a in 15; do
                   for b in 45; do
#                        ./runProgram.sh 3 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 0 ${a} ${b} 95
#                        ./runProgram.sh 2 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 0 ${a} ${b} 95
                        ./runProgram.sh 4 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 0 ${a} ${b} 95
#                        ./runProgram.sh 5 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 0 ${a} ${b} 95
                        for w1 in  25 ;  do
                            for w2 in  25 ; do
                                for p in 40 45 50 55 60; do
#                                    ./runProgram.sh 5 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 2 ${a} ${b} 95 ${w1} ${w2} ${p}
                                    ./runProgram.sh 4 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 2 ${a} ${b} 95 ${w1} ${w2} ${p}
#                                    ./runProgram.sh 3 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 2 ${a} ${b} 95 ${w1} ${w2} ${p}
#                                    ./runProgram.sh 2 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} 0 2 ${a} ${b} 95 ${w1} ${w2} ${p}
                                done
                            done
                        done
                    done
                done
            done
        done
    done
done
