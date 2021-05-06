#!/bin/bash
lowgainexedir=/home/daq/bRIKEN21/offline/offline_wasabi_lowgain
lowgainexe=/home/daq/bRIKEN21/offline/offline_wasabi_lowgain/offline
datadir=/home/daq/bRIKEN21/rawdata/beam
outputdir=/home/daq/bRIKEN21/offline/merger/rootfiles
listdir=/home/daq/bRIKEN21/offline/merger/list
ls -tr $datadir/daq2_beamrun$(printf "%04d" $1)-*.evt >$listdir/list_lowgain_$(printf "%04d" $1).txt
cd $lowgainexedir
$lowgainexe $listdir/list_lowgain_$(printf "%04d" $1).txt $outputdir/lowgain_run$(printf "%04d" $1).root list
cd -
