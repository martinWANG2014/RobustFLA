#!/usr/bin/env bash

# data-ok3 directory path
data_dir="../data-ok3"

# the log-ok3 directory path
log_dir="../log-ok3"

# the solution-ok3 directory path
solution_dir="../solution-ok3"

# the original flight txt data-ok3 file path
flight_txt=${data_dir}/flight.txt

# the flight json data-ok3 file path
flight_json=${data_dir}/flight.json

# the original airport txt data-ok3 file path
airport_txt=${data_dir}/airport.txt

# the airport json data-ok3 file path
airport_json=${data_dir}/airport.json

# the original beacon txt data-ok3 file path
beacon_txt=${data_dir}/beacon.txt

# the beacon json data-ok3 file path
beacon_json=${data_dir}/beacon.json

# the executable program path
executable_target="../build/bin/RobustFLA"

# the program usage message.
function Usage {
    echo -e "[USAGE] runProgram.sh alpha beta gamma w1 w2 p pa sigmaMode generateFlight period_length feasible_list_size method_mode epsilon coefficient_Pi nbIterations deterministicRule displayMode"
    echo -e "method_mode 0 \t\t\t Deterministic Method"
    echo -e "method_mode 1 \t\t\t Hoeffding Inequalities Method"
    echo -e "method_mode 2 \t\t\t Monte-Carlo Simulation Method"
    echo -e "method_mode 3 \t\t\t Gaussian Method"
    echo -e "period_length \t\t\t An integer, the length of period that consider en-route conflict occurs at a wayPoint."
    echo -e "epsilon \t\t\t An integer in [0, 100],  the tolerance of infeasibility of separated constraints in FLA problem."
    echo -e "feasible_list_size \t\t\t An integer, the size of feasible flight level list."
    echo -e "coefficient_Pi \t\t\t An integer in [0, 100], the percentage of aircraft's flight time to induce an admissible penal cost"
    echo -e "alpha \t\t\t An integer, the lower bound for the interval [t-alpha, t+beta]"
    echo -e "beta \t\t\t An integer, the upper bound for the interval [t-alpha, t+beta]"
    echo -e "gamma \t\t\t An integer in [0, 10000], Pr(t-alpha<=x<=t+beta)>=gamma/10000*100%, the confident that generated departure time fall in an interval [t-alpha, t+beta]"
    echo -e "w1 \t\t\t An integer in [0, 100], the mean u=t-w1*alpha of the left part x<t"
    echo -e "w2 \t\t\t An integer in [0, 100], the mean u=t+w2*beta of the right part x>=t"
    echo -e "p \t\t\t An integer in [0, 100], the probability that Pr(x<t)=p%"
    echo -e "pa \t\t\t An integer in [0, 100], the percentage of additional flights"
    echo -e "modeSigma \t\t\t A boolean, 1, set the dynamic sigma value;  0, set the statistic sigma value."
    echo -e "modeDisplay \t\t\t A boolean, 1, show the details;  0, don't display the details."

}

# method to generate the result filename.
function getLogFileName {
    local filename="res"
    for i in $@; do
        filename=${filename}"_"${i}
    done
    filename=${filename}".txt"
    echo ${filename}
}

# check the data-ok3 directory.
if [[ ! -e ${data_dir} ]]; then
    echo ${data_dir} " not exists! Please check the data-ok3 path."
    exit
fi

#  check the log-ok3 directory, if not exists, then create it!
if [[ ! -e ${log_dir} ]]; then
    mkdir -p ${log_dir}
fi

#  check the solution-ok3 directory, if not exists, then create it!
if [[ ! -e ${solution_dir} ]]; then
    mkdir -p ${solution_dir}
fi

# check the airport data-ok3 file, if not exists, then convert it from txt data-ok3 file.
if [[ ! -e ${airport_json} ]]; then
    # check the airport data-ok3 txt file.
    if [[ ! -e ${airport_txt} ]]; then
        echo ${airport_txt} " not exists! Please check the airport data-ok3."
        exit
    else
        ./convertTXT2JSON.sh ${airport_txt}
    fi
fi

# check the beacon data-ok3 file, if not exists, then convert it from txt data-ok3 file.
if [[ ! -e ${beacon_json} ]]; then
    # check the beacon data-ok3 txt file.
    if [[ ! -e ${beacon_txt} ]]; then
        echo ${beacon_txt} " not exists! Please check the waypoint data-ok3."
        exit
    else
        ./convertTXT2JSON.sh ${beacon_txt}
    fi
fi

# check the flight data-ok3 file, if not exists, then convert it from txt data-ok3 file.
if [[ ! -e ${flight_json} ]]; then
    # check the flight data-ok3 txt file.
    if [[ ! -e ${flight_txt} ]]; then
        echo ${flight_txt} " not exists! Please check the flight data-ok3."
        exit
    else
        ./convertTXT2JSON.sh ${flight_txt}
    fi
fi

# check the executable program.
if [[ ! -e ${executable_target} ]]; then
    echo ${executable_target} " not exists, please compiler the program first"
    exit
fi

# check the parameters list
if [[ $# -ne 17 ]]; then
    echo "[ERROR] invalid parameters list."
    Usage
    exit
fi


# call the program.
echo  " ${executable_target} ${airport_json} ${beacon_json} ${flight_json} $@"
${executable_target} ${airport_json} ${beacon_json} ${flight_json} $@ > ${log_dir}/$(getLogFileName $@)
