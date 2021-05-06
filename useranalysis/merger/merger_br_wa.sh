#!/bin/bash
if [ $# == 2 ]; then
        #./runhighgain.sh $2
	#./runlowgain.sh $2
	runno=$(printf "%04d" $2)
	root -b -q 'merger.C("bigrips/run'$1'.root","rootfiles/lowgain_run'$runno'.root","merged_files/merged_run'$runno'.root")'
	root -b -q 'betacleaner.C("merged_files/merged_run'$runno'.root","rootfiles/highgain_run'$runno'.root","briken/daq3_run'$runno'.root","merged_files/mergedionbeta_run'$runno'.root")'
else
	echo "usage: ./merger_br_wa.sh bigripsrun_no wasabirun_no"
fi
