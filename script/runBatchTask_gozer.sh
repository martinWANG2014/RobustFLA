#!/usr/bin/env bash
if [[ $# -ne 1 ]]; then
    echo "error, should indicate the number of parallel threads"
    exit
fi

taskInstanceFile='./tasks_gozer'
NbThreads=$1
if [[ -e ${taskInstanceFile} ]] ; then
    rm ${taskInstanceFile}
fi
for minAdmissibleCost in 5 15;do
    for maxAdmissibleCost in  60 ;do
        for method in 0 1 3 2; do
            for pa in 0 15 ; do
                for periodLength in 30 ; do
                    for feasibleSize in 3 ; do
                        for epsilon in 25 20 15 10 5; do
                            for coefPi in  15 10 8 5; do
                                echo ${pa} ${periodLength} ${feasibleSize} ${method} ${epsilon} ${coefPi} 1000 ${minAdmissibleCost} ${maxAdmissibleCost} 0 0 >> ${taskInstanceFile}
                            done
                        done
                    done
                done
            done
        done
    done
done

cat ${taskInstanceFile} | xargs -n 11 -P ${NbThreads} ./runProgram.sh