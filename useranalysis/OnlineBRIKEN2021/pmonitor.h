#ifndef __PMONITOR__
#define __PMONITOR__

#include <TVirtualPad.h>
#include <TThread.h>

#include <Event/Event.h>
#include <Event/EventTypes.h>

int pstatus ();

int ptestopen ();

//int petopen (const char * etname);
//int poncsetopen (const char * etname);

int poncsopen (const char * filename);
int rcdaqopen (const char * ip=0);
int pfileopen (const char * filename);
int plistopen (const char * filename);
int pstart (const int nevents);
void prun ();
void prun (const int nevents);
int pstart ();
int pstop ();
int pclose ();
//int pclear();
int pidentify (const int n);
int pidentify ();
int pclearidentify ();
int pcontrol ();
int pcontrol (const int seconds);
int pwait ();
int plock();
int prelease();
const char *pname();
int pexitstatus();
//int pgui ();
//int prmgui ();

int pinit ();
int process_event (Event * e);
int SaveHistograms(const char *filenameC);
int Draw_Histograms_Energy(int DetectorNumber, int Rebin, TString Option);
int Draw_Histograms_Rate(int DetectorNumber, int Rebin, TString Option);


void OpenMainCanvas();

int phsave (const char *filename ="histos.root");
int phDraw_Detector_Energy (int Detector, int RebinValue, TString Options);
int phDraw_Detector_Energy (int Detector, int RebinValue);
int phDraw_Detector_Energy (int Detector);
int phDraw_Detector_Rate (int Detector, int RebinValue, TString Options);
int phDraw_Detector_Rate (int Detector, int RebinValue);
int phDraw_Detector_Rate (int Detector);

int pOpenCanvas();


void phelp();

void pupdate(TVirtualPad * pad, const unsigned int refresh = 5);
//void start_update(TVirtualPad * pad);
void pend_update(TVirtualPad * pad = 0);

#endif /* __PMONITOR__ */
