#!/usr/bin/env bash

# data directory path
data_dir="../data"

# the log directory path
log_dir="../log"

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
    echo -e "[USAGE] runProgram.sh pa period_length feasible_list_size method_mode epsilon coefficient_Pi nbIterations minAdmissibleCost displayMode"
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

# check the data directory.
if [[ ! -e ${data_dir} ]]; then
    echo ${data_dir} " not exists! Please check the data path."
    exit
fi

#  check the log directory, if not exists, then create it!
if [[ ! -e ${log_dir} ]]; then
    mkdir -p ${log_dir}
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
if [[ $# -ne 13 ]]; then
    echo $#
    echo "[ERROR] invalid parameters list."
    Usage
    exit
fi


# call the program.
echo  " ${executable_target} ${airport_json} ${beacon_json} ${flight_json} $@"
${executable_target} ${airport_json} ${beacon_json} ${flight_json} $@ > ${log_dir}/$(getLogFileName $@)
