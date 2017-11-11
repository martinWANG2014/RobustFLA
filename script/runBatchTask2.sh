#!/usr/bin/env bash
for method in 2; do
	for a in 15 ; do
    	for b in 45; do
        	for w1 in 33;  do
        	    for w2 in  20; do
        	        for p in 5 ; do
        	            for pa in 15; do
        	                for periodLength in 30 ; do
        	                    for feasibleSize in 3 ; do
        	                    	for epsilon in 10; do
        	        					for coefPi in 8; do 
        	                                ./runProgram.sh ${a} ${b} 9957 ${w1} ${w2} ${p} ${pa} 1 0 ${periodLength} ${feasibleSize} ${method} ${epsilon} ${coefPi} 1000 0 0 
							 
                                        done
                                    done
                                done
                            done
                        done
                    done
                done
            done
        done
    done
done 
