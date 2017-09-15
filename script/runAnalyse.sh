#!/usr/bin/env bash
if [[ $# -ne 2 ]];then 
	echo "Usage: runAnalyse.sh target_dir result_file.csv"
	exit
fi

echo "Type;period_length;epsilon;feasibleSize;coef_Pi;s_d;display;random_mode;alpha;beta;gamma;w1;w2;p;nbChange;maxNbConflict;SumBenifit;ElapsedTime"  > $2

for f in $(find $1 -name 'res*.txt')
 do
  element=$(echo $(basename ${f}) | tr '_' '\n' | tr '.' '\n')
  array=$(tail -n 4 ${f} | awk -F':' '{print $2}')
	#echo ${element[@]:3:${#element}-7} ${array}	
  echo ${element[@]:3:${#element}-7} ${array} | awk -F' ' 'BEGIN{OFS=";"} {for(i=1;i<8;i++) outArray=outArray $i OFS; if(NF==11){for(i=8;i<15;i++) outArray=outArray OFS;for(i=8;i<12;i++) outArray=outArray $i OFS } else if (NF==15){for(i=8;i<12;i++) outArray=outArray $i OFS;for(i=12;i<15;i++) outArray=outArray OFS;for(i=12;i<17;i++) outArray=outArray $i OFS } else {for(i=8;i<19;i++) outArray=outArray $i OFS} ; print outArray }' >> $2 
done
