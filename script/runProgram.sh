#!/usr/bin/env bash

# data directory path
data_dir="../data"

# the solution directory path
solution_dir="../solution"

# the original flight txt data file path
flight_txt=${data_dir}/flight.txt

# the flight json data file path
flight_json=${data_dir}/flight.json

# the original airport txt data file path
airport_txt=${data_dir}/airport.txt

# the airport json data file path
airport_json=${data_dir}/airport.json

# the original beacon txt data file path
beacon_txt=${data_dir}/beacon.txt

# the beacon json data file path
beacon_json=${data_dir}/beacon.json

# the executable program path
executable_target="../build/bin/RobustFLA"

# the program usage message.
function Usage {
    echo -e "[USAGE] RobustFLA method_mode period_length epsilon coefficient_Pi [random_mode alpha beta gamma [w1 w2 p]]"
    echo -e "method_mode 0 \t\t\t Enumeration Method, the parameters: method_mode period_length epsilon coefficient_Pi required. "
    echo -e "method_mode 1 \t\t\t Hoeffding Inequalities Method, the parameters: method_mode period_length epsilon coefficient_Pi required. "
    echo -e "method_mode 2 \t\t\t Monte-Carlo Simulation Method, the parameters: method_mode period_length epsilon coefficient_Pi random_mode alpha beta gamma required. if random_mode is 2, then the parameters: w1 w2 p required."
    echo -e "method_mode 3 \t\t\t Gaussian Method, the parameters: method_mode period_length epsilon coefficient_Pi random_mode alpha beta gamma required. if random_mode is 2, then the parameters: w1 w2 p required."
    echo -e "period_length \t\t\t An integer, the length of period that consider en-route conflict occurs at a waypoint."
    echo -e "epsilon \t\t\t An integer in [0, 100],  the tolerance of infeasibility of separated constraints in FLA problem."
    echo -e "coefficient_Pi \t\t\t An integer in [0, 100], the percentage of aircraft's flight time to induce an admissible penal cost"
    echo -e "random_mode 0 \t\t\t the generated departure time follows a normal distribution, where the mean is t+(beta-alpha)/2, and that the generated departure time has a confident of gamma at an interval [t-alpha, t+beta]."
    echo -e "random_mode 1 \t\t\t the generated departure time follows a normal distribution, where the mean is t, and that the generated departure time has a confident of gamma at an interval [t-alpha, t+beta]."
    echo -e "random_mode 2 \t\t\t the generated departure time follows a normal distribution, where the mean is t-w1*alpha in left part Pr(x<t)=p and is t+w12*beta in right part Pr(x>=t)=1-p, and that the generated departure time has a confident of gamma at an interval [t-alpha, t+beta]."
    echo -e "alpha \t\t\t An integer, the lower bound for the interval [t-alpha, t+beta]"
    echo -e "beta \t\t\t An integer, the upper bound for the interval [t-alpha, t+beta]"
    echo -e "gamma \t\t\t An integer in [0, 100], Pr(t-alpha<=x<=t+beta)>=gamma%, the confident that generated departure time fall in an interval [t-alpha, t+beta]"
    echo -e "w1 \t\t\t An integer in [0, 100], the mean u=t-w1*alpha of the left part x<t"
    echo -e "w2 \t\t\t An integer in [0, 100], the mean u=t+w2*beta of the right part x>=t"
    echo -e "p \t\t\t An integer in [0, 100], the probability that Pr(x<t)=p%"
}

# method to generate the result filename.
function getResultFileName {
    local filename="res"
    for i in $@; do
        filename=${filename}"_"${i}
    done
    filename=${filename}".txt"
    echo ${filename}
}

# check the data directory.
if [[ ! -e ${data_dir} ]]; then
    echo ${data_dir} " not exists! Please check the data path."
    exit
fi

#  check the solution directory, if not exists, then create it!
if [[ ! -e ${solution_dir} ]]; then
    mkdir -p ${solution_dir}
fi

# check the airport data file, if not exists, then convert it from txt data file.
if [[ ! -e ${airport_json} ]]; then
    # check the airport data txt file.
    if [[ ! -e ${airport_txt} ]]; then
        echo ${airport_txt} " not exists! Please check the airport data."
        exit
    else
        ./convertTXT2JSON.sh ${airport_txt}
    fi
fi

# check the beacon data file, if not exists, then convert it from txt data file.
if [[ ! -e ${beacon_json} ]]; then
    # check the beacon data txt file.
    if [[ ! -e ${beacon_txt} ]]; then
        echo ${beacon_txt} " not exists! Please check the waypoint data."
        exit
    else
        ./convertTXT2JSON.sh ${beacon_txt}
    fi
fi

# check the flight data file, if not exists, then convert it from txt data file.
if [[ ! -e ${flight_json} ]]; then
    # check the flight data txt file.
    if [[ ! -e ${flight_txt} ]]; then
        echo ${flight_txt} " not exists! Please check the flight data."
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
case $1 in
    0)
        echo "[INFO] Call Enumeration Method"
        if [[ $# -ne 4 ]]; then
            echo "[ERROR] invalid parameters list."
            Usage
            exit
        fi
    ;;
    1)
        echo "[INFO] Call Hoeffding Inequalities Method"
        if [[ $# -ne 4 ]]; then
            echo "[ERROR] invalid parameters list."
            Usage
            exit
        fi
    ;;
    2)
        echo "[INFO] Call Monte-Carlo Simulation Method"
        if [[ $# -lt 8 ]]; then
            echo "[ERROR] invalid parameters list."
            Usage
            exit
        else
            if [[ $5 -eq 2 ]]; then
                if [[ $# -ne 10 ]]; then
                    echo "[ERROR] invalid parameters list."
                    Usage
                    exit
                fi
            else
                if [[ $# -ne 8 ]]; then
                    echo "[ERROR] invalid parameters list."
                    Usage
                    exit
                fi
            fi
        fi
    ;;
    3)
        echo "[INFO] Call Gaussian Method"
        if [[ $# -lt 8 ]]; then
            echo "[ERROR] invalid parameters list."
            Usage
            exit
        else
            if [[ $5 -eq 2 ]]; then
                if [[ $# -ne 10 ]]; then
                    echo "[ERROR] invalid parameters list."
                    Usage
                    exit
                fi
            else
                if [[ $# -ne 8 ]]; then
                    echo "[ERROR] invalid parameters list."
                    Usage
                    exit
                fi
            fi
        fi
    ;;
    *)
        echo "[ERROR] Not support such method"
        Usage
        exit
esac

# call the program.
echo  " ${executable_target} ${airport_json} ${beacon_json} ${flight_json} $@"
${executable_target} ${airport_json} ${beacon_json} ${flight_json} $@ > ${solution_dir}/$(getResultFileName $@)