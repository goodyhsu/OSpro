#!/bin/bash


sudo dmesg -c
sudo dmesg -c
sudo ./a.out < TIME_MEASUREMENT.txt > TIME_MEASUREMENT_stdout.txt
dmesg | grep Project1 > TIME_MEASUREMENT_dmesg.txt


names="FIFO PSJF SJF RR"
for  name in ${names} 
do
	for(( j=1; j<=5; j=j+1 ))
	do
		sudo dmesg -c
		sudo dmesg -c
		n=${name}"_"${j}
		input=${n}".txt"
		output=${n}"_stdout.txt"
		dm=${n}"_dmesg.txt"
		#echo $input

		sudo ./a.out < ${input} > ${output}
		dmesg | grep Project1 > ${dm} 
	done

done