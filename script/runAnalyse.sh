#!/usr/bin/env bash
echo "Type;period_length;epsilon;coef_Pi;random_mode;alpha;beta;gamma;w1;w2;p;nbChange;nbNotAssigned;total;maxNbConflict;time"  > $1

for f in $(find . -name 'log*.txt')
 do
  element=$(echo $(basename ${f}) | tr '_' '\n' | tr '.' '\n')
  array=$(tail -n 5 ${f} | awk -F':' '{print $2}')
  echo ${element[@]:1} ${array} | awk -F'' 'BEGIN{OFS=";"}{print $0}'>> $1
done