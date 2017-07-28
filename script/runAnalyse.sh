#!/usr/bin/env bash
echo "Type;period_length;epsilon;feasibleSize;coef_Pi;fastmode;random_mode;alpha;beta;gamma;w1;w2;p;nbChange;nbNotAssigned;total;maxNbConflict;time"  > $2

for f in $(find $1 -name 'res*.txt')
 do
  element=$(echo $(basename ${f}) | tr '_' '\n' | tr '.' '\n')
  array=$(tail -n 5 ${f} | awk -F':' '{print $2}')
  echo ${element[@]:3:${#element}-7} ${array} | awk -F' ' 'BEGIN{OFS=";"} {for(i=1;i<7;i++) outArray=outArray $i OFS; if(NF==11){for(i=7;i<14;i++) outArray=outArray OFS;for(i=7;i<12;i++) outArray=outArray $i OFS } else if (NF==15){for(i=7;i<11;i++) outArray=outArray $i OFS;for(i=11;i<14;i++) outArray=outArray OFS;for(i=11;i<16;i++) outArray=outArray $i OFS } else {for(i=7;i<19;i++) outArray=outArray $i OFS} ; print outArray }' >> $2 
done
