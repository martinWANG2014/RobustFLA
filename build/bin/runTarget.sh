#!/usr/bin/env bash
for i in {5..50..5}; do
    for j in {5..50..5}; do
#            ./RobustFLA ./airport.json ./beacon.json ./flight.json 1 0 ${i} ${j} 100 15 60 0.95 0.33 0.33 0.5 > logM_${i}_${j}.txt

        ./RobustFLA ./airport.json ./beacon.json ./flight.json 0 0 ${i} ${j} 100 15 60 0.95 0.33 0.33 0.5 > logH_${i}_${j}.txt
        ./RobustFLA ./airport.json ./beacon.json ./flight.json 2 0 ${i} ${j} 100 15 60 0.95 0.33 0.33 0.5 > logG_${i}_${j}.txt
#        ./RobustFLA ./airport.json ./beacon.json ./flight.json ${i} 0 ${j} > logH_${i}_${j}.txt
#        ./RobustFLA ./airport.json ./beacon.json ./flight.json ${i} 2 ${j} > logG_${i}_${j}.txt
    done
done
