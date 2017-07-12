#!/usr/bin/env bash
for i in {5..50..5}; do
    for j in {5..50..5}; do
        ./RobustFLA ./airport.json ./beacon.json ./flight.json ${i} 0 ${j} > logH_${i}_${j}.txt
        ./RobustFLA ./airport.json ./beacon.json ./flight.json ${i} 2 ${j} > logG_${i}_${j}.txt
    done
done