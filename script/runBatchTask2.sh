
#!/usr/bin/env bash
for periodLength in 30 ; do
    for feasibleSize in 3 ; do
        for a in 15 ; do
            for b in 45 ; do
                for w1 in 33;  do
                    for w2 in 20; do
                        for p in 5 ; do
                            for coefPi in 8; do
                                for epsilon in 25 20 15 10 5; do
                                    for pa in 15  ; do
                                        ./runProgram.sh 5 ${periodLength} ${epsilon} ${feasibleSize} ${coefPi} ${a} ${b} 9957 ${w1} ${w2} ${p} ${pa} 1 0
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
