#!/usr/bin/env bash
if [[ $# -ne 2 ]]; then
    echo "error, should indicate the number of parallel threads, and name of task file"
    exit
fi

taskInstanceFile='./tasks_'$2
NbThreads=$1
if [[ -e ${taskInstanceFile} ]] ; then
    rm ${taskInstanceFile}
fi
for minAdmissibleCost in 0;do
    for maxAdmissibleCost in 30 ;do
        for method in 0 1 2; do
            for pa in 0 15 ; do
                for periodLength in 30 ; do
                    for feasibleSize in 3 ; do
                        for epsilon in  25 20 15 10 5; do
                            for coefPi in  15 10 5; do
                                echo ${pa} ${periodLength} ${feasibleSize} ${method} ${epsilon} ${coefPi} 1000 ${minAdmissibleCost} ${maxAdmissibleCost} 10 0 1 >> ${taskInstanceFile}
                            done
                        done
                    done
                done
            done
        done
    done
done
#for minAdmissibleCost in 0 5;do
#    for maxAdmissibleCost in 30 ;do
#        for method in 0 1 2; do
#            for pa in 0 15 ; do
#                for periodLength in 30 ; do
#                    for feasibleSize in 3 ; do
#                        for epsilon in  25 20 15 10 5; do
#                            for coefPi in  15 10 5; do
#                                echo ${pa} ${periodLength} ${feasibleSize} ${method} ${epsilon} ${coefPi} 1000 ${minAdmissibleCost} ${maxAdmissibleCost}  10 1 0 >> ${taskInstanceFile}
#                            done
#                        done
#                    done
#                done
#            done
#        done
#    done
#done


cat ${taskInstanceFile} | xargs -n 12 -P ${NbThreads} ./runProgram.sh
