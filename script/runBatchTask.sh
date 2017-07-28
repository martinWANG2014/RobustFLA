#!/usr/bin/env bash
for k in 15 30 45 60; do
    for i in  1 2 3 4 5 10 15; do
        for m in 5 4 3; do
            for j in  20 15 10 8 5 2; do
                 ./runProgram.sh 0 ${k} ${i} ${m} ${j} 1
                 ./runProgram.sh 1 ${k} ${i} ${m} ${j} 1
                for a in 5 10 15 20; do
                   for b in 30 40 50 60; do
                        ./runProgram.sh 2 ${k} ${i} ${m} ${j} 1 0 ${a} ${b} 95
                        ./runProgram.sh 3 ${k} ${i} ${m} ${j} 1 0 ${a} ${b} 95
                        ./runProgram.sh 4 ${k} ${i} ${m} ${j} 1 0 ${a} ${b} 95
                        ./runProgram.sh 5 ${k} ${i} ${m} ${j} 1 0 ${a} ${b} 95
                        ./runProgram.sh 2 ${k} ${i} ${m} ${j} 1 1 ${a} ${b} 95
                        ./runProgram.sh 3 ${k} ${i} ${m} ${j} 1 1 ${a} ${b} 95
                        ./runProgram.sh 4 ${k} ${i} ${m} ${j} 1 1 ${a} ${b} 95
                        ./runProgram.sh 5 ${k} ${i} ${m} ${j} 1 1 ${a} ${b} 95
                        for w1 in 10 20 30;  do
                            for w2 in 10 20 30; do
                                for p in 20 30 40 50 60 70 80; do
                                    ./runProgram.sh 5 ${k} ${i} ${m} ${j} 1 2 ${a} ${b} 95 ${w1} ${w2} ${p}
                                    ./runProgram.sh 4 ${k} ${i} ${m} ${j} 1 2 ${a} ${b} 95 ${w1} ${w2} ${p}
                                    ./runProgram.sh 3 ${k} ${i} ${m} ${j} 1 2 ${a} ${b} 95 ${w1} ${w2} ${p}
                                    ./runProgram.sh 2 ${k} ${i} ${m} ${j} 1 2 ${a} ${b} 95 ${w1} ${w2} ${p}
                                done
                            done
                        done
                    done
                done
            done
        done
    done
done
