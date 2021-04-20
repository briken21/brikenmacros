#include "stdio.h"
#include "string.h"
void daqmonitor() {
  //gROOT->ProcessLine(".L libmyanalysis.so");
  gROOT->ProcessLine(".L /home/maxpallas/data/briken2021/TestScript/online1_he3/build/libmyanalysis.so");
  gROOT->ProcessLine("gSystem->Load(\"libpmonitor\")");
  gROOT->ProcessLine("pfileopen(\"/home/maxpallas/data/briken2021/Data/prerun00002.evt\")");
  //gROOT->ProcessLine("rcdaqopen()");
  //gROOT->ProcessLine("pstart()");
}
