#!/usr/bin/env bash
if [[ $# -ne 2 ]];then 
	echo "Usage: runAnalyse.sh target_dir result_file.csv"
	exit
fi

echo ";;;;;;;;;;;Deterministic;;;Hoeffding;;;Monte-Carlo;;;Gaussian;"  > $2
echo "periodeLen;epsilon;feasSize;coef_Pi;alpha;beta;gamma;w1;w2;p;pa;nbChange;maxLevelChange;ElapsedTime;nbChange;maxLevelChange;ElapsedTime;nbChange;maxLevelChange;ElapsedTime;nbChange;maxLevelChange;ElapsedTime;"  >> $2

for f in $(find $1 -name 'res*.txt')
do
  element=$(echo $(basename ${f}) | tr '_' ';' | tr '.' '\n')
  echo ${f}
  res=$(cat ${f} | awk '/^Result/ {getline;print;getline;getline;getline;print;getline;getline;print;}' | cut -d ':' -f 2 | tr '\n' ';' )
  echo $(echo ${element[@]:6:${#element}-13} ${res} | tr -d '[:space:]')  >> $2 
done
