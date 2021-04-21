README OFFLINE analysis BRIKEN detector.
Max Pallas: max.pallas@upc.edu
*********************************************

myanalysis.cc
This code is the Offline analysis for the BRIKEN detector.
Input: evt file from digitizers V1730 and V1740. It looks for a configuration file called OfflineConf.csv in the same directory this code is.
IMPORTANT: Check the PACKETMAP ZONE defined in this code and define the correct map for each set of files.
Output: Root file with a tree called BRIKENTree (TimeStamps,Energy,Id,...).

To run this code, REPLACE the code myanalysis.cc from the src/ directory. Only one file that does the analysis can be placed in the directory.
In that directory, the cmake looks for the .cc files. For that reason, this script can be named whatever you want if the extension is .cc.
Also it is necessary to REPLACE libDataStruct.h located in include/ directory. This changes is required to have the exact same Tree from 2017.


The configuration files in this directory:

OfflineConf.csv:
This file contains the channel configuration.
The line structure must be preserved (First 140 3He, then 8 Clover, then X Ancillary).
It is possible to add more parameters but then myanalysis.cc must be also changed.
Some of the values of the parameter are wrong (especially for the clovers and ancillary). 
So once the final setup is defined it must be set in this configuration file.




