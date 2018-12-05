#!/bin/bash

counter=1

while [ $counter -le 8 ]
do
	mpiexec -np $counter a.out
	((counter++))
done

echo "finished"
