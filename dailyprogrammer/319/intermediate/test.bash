#!/bin/bash

for i in `seq 1 100`; 
do 
	./worm_wars-cla < input | head | sed "s/^/first $i: /" | grep nan; 
	./worm_wars-clan < input | head | sed "s/^/second $i: /" | grep nan; 
done

