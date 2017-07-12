#!/usr/bin/env bash
echo "Type; epsilon; coefPi;nbChange;nbNotAssigned;total; maxNbConflict; time"  > $1

for f in $(find . -name 'log*.txt')
 do
  element=$(echo $(basename ${f}) | tr '_' '\n' | tr '.' '\n')
  array=$(tail -n 5 ${f} | awk -F':' '{print $2}')
  echo ${element} ${array} | awk 'BEGIN{OFS=";"}{split($0, a," "); print a[1],a[2],a[3],a[5],a[6],a[7], a[8], a[9]}'>> $1
done