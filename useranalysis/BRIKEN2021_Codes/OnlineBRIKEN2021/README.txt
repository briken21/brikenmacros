README ONLINE analysis BRIKEN detector.
Max Pallas: max.pallas@upc.edu
*********************************************

myanalysis.cc
This code is the Online analysis for the BRIKEN detector.
Input: Data stream from the DAQ. It also can read evt files
IMPORTANT: Check the PACKETMAP ZONE defined in this code and define the correct map for each set of files.
Output: Histograms for several data*. This histograms can be saved as root files

These histograms are:

Energy(calibrated):
TH2 Energy vs Id for all detectors.
TH1 for each detector (140 3He, 8 clovers, X ancillary).
TH1 for each ring of 3He detectors.

Rate:
TH1 Total rate adding 3He all detectors.
TH1 for each detector (140 3He, 8 clovers, X ancillary).
TH1 for each ring of 3He detectors.

Correlation:
TH1 correlation neutron neutron.
TH1 correlation F11 neutron.


To run this code, REPLACE the code myanalysis.cc from the src/ directory. Only one file that does the analysis can be placed in the directory.
In that directory, the cmake looks for the .cc files. For that reason, this script can be named whatever you want if the extension is .cc.



The configuration files in this directory:

OfflineConf.csv:
This file contains the channel configuration.
The line structure must be preserved (First 140 3He, then 8 Clover, then X Ancillary).
It is possible to add more parameters but then myanalysis.cc must be also changed.
Some of the values of the parameter are wrong (especially for the clovers and ancillary). 
So once the final setup is defined it must be set in this configuration file.


