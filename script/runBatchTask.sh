#!/usr/bin/env bash
for k in 30; do
    for i in 5 10; do
        for m in 5 4 3; do
            for j in 15 10 5 2; do
                for a in 15; do
                   for b in 60; do
                        ./runProgram.sh 0 ${k} ${i} ${m} ${j} 0 ${a} ${b} 95
                        ./runProgram.sh 1 ${k} ${i} ${m} ${j} 0 ${a} ${b} 95
                        ./runProgram.sh 3 ${k} ${i} ${m} ${j} 0 ${a} ${b} 95
                        ./runProgram.sh 0 ${k} ${i} ${m} ${j} 1 ${a} ${b} 95
                        ./runProgram.sh 1 ${k} ${i} ${m} ${j} 1 ${a} ${b} 95
                        ./runProgram.sh 3 ${k} ${i} ${m} ${j} 1 ${a} ${b} 95
                        ./runProgram.sh 2 ${k} ${i} ${m} ${j} 0 ${a} ${b} 95
                        ./runProgram.sh 2 ${k} ${i} ${m} ${j} 1 ${a} ${b} 95
                        for w1 in 25;  do
                            for w2 in 25; do
                                for p in 40 50 60; do
                                    ./runProgram.sh 0 ${k} ${i} ${m} ${j} 2 ${a} ${b} 95 ${w1} ${w2} ${p}
                                    ./runProgram.sh 1 ${k} ${i} ${m} ${j} 2 ${a} ${b} 95 ${w1} ${w2} ${p}
                                    ./runProgram.sh 3 ${k} ${i} ${m} ${j} 2 ${a} ${b} 95 ${w1} ${w2} ${p}
                                    ./runProgram.sh 2 ${k} ${i} ${m} ${j} 2 ${a} ${b} 95 ${w1} ${w2} ${p}
                                done
                            done
                        done
                    done
                done
            done
        done
    done
done