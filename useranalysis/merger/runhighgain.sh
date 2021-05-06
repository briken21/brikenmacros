#!/bin/bash
highgainexedir=/home/daq/bRIKEN21/offline/offline_wasabi_highgain
highgainexe=/home/daq/bRIKEN21/offline/offline_wasabi_highgain/offline
datadir=/home/daq/bRIKEN21/rawdata/beam
outputdir=/home/daq/bRIKEN21/offline/merger/rootfiles
listdir=/home/daq/bRIKEN21/offline/merger/list
ls -tr $datadir/daq0_beamrun$(printf "%04d" $1)-*.evt >$listdir/list_highgain0_$(printf "%04d" $1).txt
cd $highgainexedir
$highgainexe $listdir/list_highgain0_$(printf "%04d" $1).txt $outputdir/highgain0_run$(printf "%04d" $1).root list

ls -tr $datadir/daq1_beamrun$(printf "%04d" $1)-*.evt >$listdir/list_highgain1_$(printf "%04d" $1).txt
cd $highgainexedir
$highgainexe $listdir/list_highgain1_$(printf "%04d" $1).txt $outputdir/highgain1_run$(printf "%04d" $1).root list
hadd -f $outputdir/highgain_run$(printf "%04d" $1).root $outputdir/highgain0_run$(printf "%04d" $1).root $outputdir/highgain1_run$(printf "%04d" $1).root
cd -
