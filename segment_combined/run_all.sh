#!/bin/bash
sigma="0.1"
for i in 1 2 3 4 5
do
	mkdir "$i"_results

	for sigma in $(seq 0.1 0.1 1)
	do	
		for k in 100 200 300 400 500 600 700 800 900 1000
		do
			for min in 10 20 30 40 50 60 70 80 90 100 
			do
				./segment $sigma $k $min $i.ppm "$i"_results/${i}_sig${sigma}_k${k}_min${min}.ppm
			done
		done
	done
done
