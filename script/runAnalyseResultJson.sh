#!/usr/bin/env bash
if [[ $# -ne 2 ]];then 
	echo "Usage: runAnalyse.sh target_dir result_file.csv"
	exit
fi

#echo ";;;;;;Deterministic;;;;;Hoeffding;;;;;Monte-Carlo;;;;;;Gaussian;;;;"  > $2
echo ";;;;;;Deterministic;;;;;Hoeffding;;;;;Monte-Carlo;;;;;;"  > $2
#echo "tryall;minWait;maxWait;epsilon;coef_Pi;pa;nbChange;nbUnassigned;nbConflictMax;maxLevelChange;ElapsedTime;nbChange;nbUnassigned;nbConflictMax;maxLevelChange;ElapsedTime;nbChange;nbUnassigned;nbConflictMax;maxLevelChange;ElapsedTime;nbIterations;nbChange;nbUnassigned;nbConflictMax;maxLevelChange;ElapsedTime"  >> $2
echo "tryall;minWait;maxWait;epsilon;coef_Pi;pa;nbChange;nbUnassigned;nbConflictMax;maxLevelChange;ElapsedTime;nbChange;nbUnassigned;nbConflictMax;maxLevelChange;ElapsedTime;nbChange;nbUnassigned;nbConflictMax;maxLevelChange;ElapsedTime;nbIterations;"  >> $2
for f in $(find $1 -name 'res_0*.json')
do
	fileHoeff=$(echo ${f} | sed -e 's/res_0/res_1/g')
	fileMC=$(echo ${f} | sed -e 's/res_0/res_2/g' | sed -e 's/-/1000/g')
	fileGauss=$(echo ${f} | sed -e 's/res_0/res_3/g')
	
	epsilon=$(echo ${f} | cut -d '_' -f 4)
	coefPi=$(echo ${f} | cut -d '_' -f 6)
	minWait=$(echo ${f} | cut -d '_' -f 7)
	maxWait=$(echo ${f} | cut -d '_' -f 8)
	pa=$(echo ${f} | cut -d '_' -f 9)
	tryall=$(echo ${f} | cut -d '_' -f 11 | cut -d '.' -f 1)
	
	resDet=$(awk '/"solution"/ {getline;getline;print;getline;print;getline;print;getline;print;getline;getline;print;}' ${f} | cut -d ':' -f 2 | tr '"' ' ' | tr ',' ' ' | tr '\n' ' ')
	if [[ -e ${fileHoeff} ]]; then
	resHoeff=$(awk '/"solution"/ {getline;getline;print;getline;print;getline;print;getline;print;getline;getline;print;}' ${fileHoeff} | cut -d ':' -f 2 | tr '"' ' ' | tr ',' ' ' | tr '\n' ' ')
	else
	resHoeff=";;;"
	fi
	if [[ -e ${fileMC} ]]; then
	resMC=$(awk '/"solution"/ {getline;getline;print;getline;print;getline;print;getline;print;getline;getline;print;}' ${fileMC} | cut -d ':' -f 2 | tr '"' ' ' | tr ',' ' ' | tr '\n' ' ')
	else
	resMC=";;;;"
	fi
#	if [[ -e ${fileGauss} ]]; then
#	resGauss=$(awk '/"solution"/ {getline;print;getline;print;getline;print;getline;print;getline;getline;print;}' ${fileGauss} | cut -d ':' -f 2 | tr '"' ' ' | tr ',' ' ' | tr '\n' ' ')
#	else
#	resGauss=";;;"
#	fi
	echo "processing ${epsilon} ${coefPi} ${pa}"
#	echo ${tryall} ${minWait} ${maxWait} ${epsilon} ${coefPi} ${pa} ${resDet} ${resHoeff} ${resMC} "1000" ${resGauss} | tr ' ' ';'  >> $2
	echo ${tryall} ${minWait} ${maxWait} ${epsilon} ${coefPi} ${pa} ${resDet} ${resHoeff} ${resMC} "1000" | tr ' ' ';'  >> $2
done 
