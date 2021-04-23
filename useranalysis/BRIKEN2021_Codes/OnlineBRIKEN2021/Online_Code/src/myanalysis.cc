
/* ************************ DO NOT REMOVE  ****************************** */
#include <iostream>
#include <TStyle.h>
#include <pmonitor/pmonitor.h>
#include "myanalysis.h"
#include <time.h>
#include <ctime>

#include <libDataStruct.h>
#include <bitset>
#include <stdint.h>
#include <vector>
#include <map>

#include <TSystem.h>
#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TTree.h>
#include <TRandom.h>
#include <TString.h>
#include <TCanvas.h>

#include <iostream>
#include <fstream>
#include <sstream>


#include <correlation.h>
#include "TThread.h"

//#define SLOW_ONLINE 100000
#define RATE_CAL_REFESH_SECONDS 10 // NUMBER OF SECONDS PER CANVAS REFRESH

#define MAX_N_BOARD 20
#define V1730_MAX_N_CH 16
#define V1740_N_MAX_CH 64
//! parameters for V1740
#define NSBL 8
#define N_MAX_WF_LENGTH 90
UShort_t trig_pos = N_MAX_WF_LENGTH*30/100;//unit of sample
UShort_t sampling_interval = 16*8;//unit of ns ASK PHONG IF 16*16

TFile* file0;
NIGIRI* data;

int nevt = 0;

// MAX VERSION---------------------------------------------------------------------------------

// General histograms
TH2F* H2D_Energy;
TH1F* HEnergy1D_IndividualDetector[160];
TH1F* HRate1D_IndividualDetector[160];

// Neutron histograms
TH1F* HRate_3He_Ring[7];
TH1F* HEnergy3He_Ring[7];
TH1F* HRate_3HeTotal;
TH1F* HEnergy_3HeTotal;

// Gamma histograms
TH1F* HEnergy_GammaTotal;
TH1F* HRate_GammaTotal;

//Correlation histograms
TH1F* HCorrelationNeutronNeutron;
TH1F* HCorrelationF11Neutron;
TH1F* HCorrelationNeutronGamma;
TH1F* HCorrelationF11Gamma;

// Rate distribution histograms
TH1F* HRateNeutronBottomRight;
TH1F* HRateNeutronBottomLeft;
TH1F* HRateNeutronTopRight;
TH1F* HRateNeutronTopLeft;



// Dummies
int nevt_pre = -1;
int FlagRepetition = 0;
int FlagStart=0;
double TimeStampReference=0;

// Map for correlations
std::multimap<double,double> MapNeutron;
std::multimap<double,double> MapF11_Right;
std::multimap<double,double> MapGamma;
int MapNeutronMaxBufferSize = 2000; // Set the size of the correlation buffer. Increasing this value slow down the online


// Canvas General
TCanvas *Energy2D_Canvas;
TCanvas *EnergyDetectorCanvas;
TCanvas *RateDetectorCanvas;

// Canvas Neutron
TCanvas *Neutron_RingRate_Canvas;
TCanvas *Neutron_RingEnergy_Canvas;
//--------------------

// Canvas Gamma
TCanvas *Gamma_Rate_Canvas;
TCanvas *Gamma_Energy_Canvas;
//--------------------

// Canvas Ancillary
TCanvas *Ancillary_Rate_Canvas;
TCanvas *Ancillary_Energy_Canvas;
//--------------------

// Canvas Correlation
TCanvas *Correlation_Canvas;
//--------------------

// Canvas Neutron distribution
TCanvas *NeutronDistribution_Canvas;
//--------------------


// Configuration file parameters
std::vector<string> vName;
std::vector<uint16_t> vId;
std::vector<uint16_t> vCrate;
std::vector<uint16_t> vChannel;
std::vector<uint16_t> vInputType;
std::vector<uint16_t> vIndex1;
std::vector<uint16_t> vIndex2;
std::vector<double> vEOffset;
std::vector<double> vEFactor;
std::vector<double> vThreshold;
int IdChannel_Trigger=0;


int vCenter[16] = {2,6,8,12,24,14,18,20,28,48,40,60,83,96,113,126};
int vBottomRight[31] = {7,10,22,19,49,50,51,55,56,57,61,62,64,75,76,79,80,114,115,119,120,127,128,129,130,132,136,137,138,139,140};
int vBottomLeft[31] = {1,4,13,16,45,46,47,52,53,54,58,59,63,73,74,77,78,111,112,116,117,118,122,123,124,125,131,132,133,134,135};
int vTopRight[31] = {9,11,21,23,31,35,36,37,29,30,44,41,42,67,68,71,72,84,85,89,90,91,97,98,99,100,106,107,108,109,110};
int vTopLeft[31] = {3,5,15,17,32,33,34,38,25,26,27,43,39,65,66,69,70,81,82,86,87,88,92,93,94,95,101,102,103,104,105};

//------------------------------------------------------------

// Function to read the configuration file and fill vectors to use them later.
void ConfigurationFile_Reader(){

  cout << "Reading configuration file" << endl;

  vName.clear();
  vId.clear();
  vCrate.clear();
  vChannel.clear();
  vInputType.clear();
  vIndex1.clear();
  vIndex2.clear();
  vEOffset.clear();
  vEFactor.clear();
  vThreshold.clear();

   string FileName="OfflineConf.csv";
   string line;
   ifstream Fd(FileName.data());

   if( !Fd){
     std::cerr << "ERROR -1!!!!!!!" << std::endl;
     std::cerr << "CONFIGURATION FILE OfflineConf.csv NOT FOUND IN THIS DIRECTORY" << std::endl;
     exit(-1);
   }

   while ( Fd.good() ) {
     getline (Fd,line);
     if(line.size()>5){
       std::string Name,Module,Type,Parameter;
       uint16_t Id=0, Crate=0, Channel=0, InputType=0, Index1=0, Index2=0;
       double EOffset=0, EFactor=0, Threshold=0;
       istringstream iss(line,istringstream::in);
       iss>>Name;
       iss>>Id;
       iss>>Crate;
       iss>>Channel;
       iss>>InputType;
       iss>>Index1;
       iss>>Index2;
       iss>>EOffset;
       iss>>EFactor;
       iss>>Threshold;


       vName.push_back(Name);
       vId.push_back(Id);
       vCrate.push_back(Crate);
       vChannel.push_back(Channel);
       vInputType.push_back(InputType);
       vIndex1.push_back(Index1);
       vIndex2.push_back(Index2);
       vEOffset.push_back(EOffset);
       vEFactor.push_back(EFactor);
       vThreshold.push_back(Threshold);
     }
   }
}

void OpenMainCanvas(){
  if (!Energy2D_Canvas || !Neutron_RingEnergy_Canvas || !Neutron_RingRate_Canvas) cout << "Create Main Canvas" << endl;

  if (Energy2D_Canvas)Energy2D_Canvas->Close();
  gStyle->SetOptStat("e");

  Energy2D_Canvas = new TCanvas("Energy2D_Canvas","Energy 2D Canvas",900,900);
  Energy2D_Canvas->SetFillColor(10);
  Energy2D_Canvas->Divide(1,1);
  Energy2D_Canvas->Draw();
  Energy2D_Canvas->cd();
  H2D_Energy->Draw("colz");


// Neutron canvas----------------------------------------------------------
  if (Neutron_RingEnergy_Canvas)Neutron_RingEnergy_Canvas->Close();
  Neutron_RingEnergy_Canvas = new TCanvas("Neutron_RingEnergy_Canvas","Neutron Energy Ring",900,900);
  Neutron_RingEnergy_Canvas->SetFillColor(10);
  Neutron_RingEnergy_Canvas->Divide(4,2);
  Neutron_RingEnergy_Canvas->Draw();
  Neutron_RingEnergy_Canvas->cd(1);
  HEnergy_3HeTotal->Draw("histo");
  for (int imn=0; imn<7; imn++){
    Neutron_RingEnergy_Canvas->cd(imn+2);
    HEnergy3He_Ring[imn]->Draw("histo");
  }

  if (Neutron_RingRate_Canvas)Neutron_RingRate_Canvas->Close();
  Neutron_RingRate_Canvas = new TCanvas("Neutron_RingRate_Canvas","Neutron Rate Ring",900,900);
  Neutron_RingRate_Canvas->SetFillColor(10);
  Neutron_RingRate_Canvas->Divide(4,2);
  Neutron_RingRate_Canvas->Draw();
  Neutron_RingRate_Canvas->cd(1);
  HRate_3HeTotal->Draw("histo");
  for (int imn=0; imn<7; imn++){
    Neutron_RingRate_Canvas->cd(imn+2);
    HRate_3He_Ring[imn]->Draw("histo");
  }
//----------------------------------------------------------------------

// Gamma canvas----------------------------------------------------------
  if (Gamma_Energy_Canvas)Gamma_Energy_Canvas->Close();
  Gamma_Energy_Canvas = new TCanvas("Gamma_Energy_Canvas","Gamma Energy",900,900);
  Gamma_Energy_Canvas->SetFillColor(10);
  Gamma_Energy_Canvas->Divide(4,2);
  Gamma_Energy_Canvas->Draw();
  Gamma_Energy_Canvas->cd(1);
  HEnergy_GammaTotal->Draw("histo");
  Gamma_Energy_Canvas->cd(2);
  HEnergy1D_IndividualDetector[140]->Draw("histo");
  for (int imn=2; imn<8; imn++){
    Gamma_Energy_Canvas->cd(imn+1);
    HEnergy1D_IndividualDetector[imn+140]->Draw("histo");
  }

  if (Gamma_Rate_Canvas)Gamma_Rate_Canvas->Close();
  Gamma_Rate_Canvas = new TCanvas("Gamma_Rate_Canvas","Gamma Rate",900,900);
  Gamma_Rate_Canvas->SetFillColor(10);
  Gamma_Rate_Canvas->Divide(4,2);
  Gamma_Rate_Canvas->Draw();
  Gamma_Rate_Canvas->cd(1);
  HRate_GammaTotal->Draw("histo");
  Gamma_Rate_Canvas->cd(2);
  HRate1D_IndividualDetector[140]->Draw("histo");
  for (int imn=2; imn<8; imn++){
    Gamma_Rate_Canvas->cd(imn+1);
    HRate1D_IndividualDetector[imn+140]->Draw("histo");
  }
//----------------------------------------------------------------------
// Ancillary canvas----------------------------------------------------------
  if (Ancillary_Energy_Canvas)Ancillary_Energy_Canvas->Close();
  Ancillary_Energy_Canvas = new TCanvas("Ancillary_Energy_Canvas","Ancillary Energy",900,900);
  Ancillary_Energy_Canvas->SetFillColor(10);
  Ancillary_Energy_Canvas->Divide(2,2);
  Ancillary_Energy_Canvas->Draw();
  for (int imn=0; imn<8; imn++){
    if (HEnergy1D_IndividualDetector[imn+148]){
      Ancillary_Energy_Canvas->cd(imn+1);
      HEnergy1D_IndividualDetector[imn+148]->Draw("histo");
    }
  }

  if (Ancillary_Rate_Canvas)Ancillary_Rate_Canvas->Close();
  Ancillary_Rate_Canvas = new TCanvas("Ancillary_Rate_Canvas","Ancillary Rate",900,900);
  Ancillary_Rate_Canvas->SetFillColor(10);
  Ancillary_Rate_Canvas->Divide(2,2);
  Ancillary_Rate_Canvas->Draw();
  for (int imn=0; imn<8; imn++){
    if (HRate1D_IndividualDetector[imn+148]){
      Ancillary_Rate_Canvas->cd(imn+1);
      HRate1D_IndividualDetector[imn+148]->Draw("histo");
    }
  }
//----------------------------------------------------------------------
// Correlation canvas----------------------------------------------------------
  if (Correlation_Canvas)Correlation_Canvas->Close();
  Correlation_Canvas = new TCanvas("Correlation_Canvas","Correlation Canvas",900,900);
  Correlation_Canvas->SetFillColor(10);
  Correlation_Canvas->Divide(2,2);
  Correlation_Canvas->Draw();
  Correlation_Canvas->cd(1);
  HCorrelationNeutronNeutron->Draw("histo");
  Correlation_Canvas->cd(2);
  HCorrelationF11Neutron->Draw("histo");
  Correlation_Canvas->cd(3);
  HCorrelationNeutronGamma->Draw("histo");
  Correlation_Canvas->cd(4);
  HCorrelationF11Gamma->Draw("histo");

  //Neutron distribution
  NeutronDistribution_Canvas = new TCanvas("NeutronDistribution_Canvas","Neutron ditribution Canvas",900,900);
  NeutronDistribution_Canvas->SetFillColor(10);
  NeutronDistribution_Canvas->Divide(2,2);
  NeutronDistribution_Canvas->Draw();
  NeutronDistribution_Canvas->cd(4);
  HRateNeutronBottomRight->Draw("histo");
  NeutronDistribution_Canvas->cd(3);
  HRateNeutronBottomLeft->Draw("histo");
  NeutronDistribution_Canvas->cd(2);
  HRateNeutronTopRight->Draw("histo");
  NeutronDistribution_Canvas->cd(1);
  HRateNeutronTopLeft->Draw("histo");
//----------------------------------------------------------------------

}// End function


void Init(){

    ConfigurationFile_Reader();
    FlagStart = 0;

    // Energy histograms--------------------------------------------------------------------------------
    // TH2
    H2D_Energy = new TH2F("H2D_Energy"," Id vs Energy ; Id ; Energy(keV) ",64*3,0,64*3,5000,0,5000);
    HEnergy_3HeTotal = new TH1F("HEnergy_3HeTotal","Energy 3He Total ; Time(s) ; Counts",600,0,3000);
    // TH1 He3 rings
    for (int imn=0; imn<7; imn++){
      HEnergy3He_Ring[imn] = new TH1F(Form("HEnergy3He_Ring%d",imn+1),Form("Energy 3He Ring%d ; Energy(keV) ; Counts",imn+1),600,0,3000);
    }
    // TH1 Detectors
    for (int imk=1; imk<vName.size(); imk++){
      string HistoNameEnergy = " Energy detector "+vName[imk]+"; Energy(keV) ; Counts";
      if (vInputType[imk]==1)HEnergy1D_IndividualDetector[imk-1] = new TH1F(Form("HEnergy1D_Id%d",imk),HistoNameEnergy.c_str(),600,0,3000);
      if (vInputType[imk]==2)HEnergy1D_IndividualDetector[imk-1] = new TH1F(Form("HEnergy1D_Id%d",imk),HistoNameEnergy.c_str(),6000,0,6000);
      if (vInputType[imk]==3)HEnergy1D_IndividualDetector[imk-1] = new TH1F(Form("HEnergy1D_Id%d",imk),HistoNameEnergy.c_str(),6000,0,6000);
      //for (int inr=0; inr<(HEnergy1D_IndividualDetector[imk-1]->GetNbinsX()); inr++){
        //HEnergy1D_IndividualDetector[imk-1]->SetBinContent(inr,0.0001);
      //}
    }

    // Gamma Total
    HEnergy_GammaTotal = new TH1F("HEnergy_GammaTotal","Energy Gamma Total ; Energy(keV) ; Counts",6000,0,6000);

    //--------------------------------------------------------------------------------------------------

    // Rate histograms--------------------------------------------------------------------------------
    // He3 Total
    HRate_3HeTotal = new TH1F("HRate_3HeTotal","Rate 3He Total ; Time(s) ; Counts",400,0,4000);
    // He3 Ring
    for (int imn=0; imn<7; imn++){
      HRate_3He_Ring[imn] = new TH1F(Form("HRate_3He_Ring%d",imn+1),Form("Rate 3He Ring%d ; Time(s) ; Counts",imn+1),400,0,4000);
    }
    // TH1 Detectors
    for (int imk=1; imk<vName.size(); imk++){
      string HistoNameRate = " Rate detector "+vName[imk]+"; Time(s) ; Counts";
      HRate1D_IndividualDetector[imk-1] = new TH1F(Form("HRate1D_Id%d",imk),HistoNameRate.c_str(),400,0,4000);
      //for (int inr=0; inr<(HRate1D_IndividualDetector[imk-1]->GetNbinsX()); inr++){
        //HRate1D_IndividualDetector[imk-1]->SetBinContent(inr,0.00001);
      //}
    }
    // Gamma Total
    HRate_GammaTotal = new TH1F("HRate_GammaTotal","Rate Gamma Total ; Time(s) ; Counts",400,0,4000);
    //----------------------------------------------------------
    // Correlation histograms histograms--------------------------------------------------------------------------------
    // Neutron
    HCorrelationNeutronNeutron = new TH1F("HCorrelationNeutronNeutron","Neutron-Neutron Correlation ; Time(ms) ; Counts",800,-2,2);
    HCorrelationF11Neutron = new TH1F("HCorrelationF11Neutron","F11-Neutron Correlation ; Time(ms) ; Counts",800,-2,2);
    HCorrelationNeutronGamma = new TH1F("HCorrelationNeutronGamma","Neutron-Gamma Correlation ; Time(ms) ; Counts",800,-2,2);
    HCorrelationF11Gamma = new TH1F("HCorrelationF11Gamma","F11-Gamma Correlation ; Time(ms) ; Counts",200,-0.1,0.1);
    //----------------------------------------------------------

    // Neutron distribution-------------------------------------

    HRateNeutronBottomRight = new TH1F("HRateNeutronBottomRight","Bottom Right 3He Rate ; Time(s) ; Counts",400,0,4000);
    HRateNeutronBottomLeft = new TH1F("HRateNeutronBottomLeft","Bottom Left 3He Rate ; Time(s) ; Counts",400,0,4000);
    HRateNeutronTopRight = new TH1F("HRateNeutronTopRight","Top Right 3He Rate ; Time(s) ; Counts",400,0,4000);
    HRateNeutronTopLeft = new TH1F("HRateNeutronTopLeft","Top Left 3He Rate ; Time(s) ; Counts",400,0,4000);

    //--------------------------------------------------------




    OpenMainCanvas();
}


void DoUpdate(){ // Refresh function

    if ( nevt_pre!=nevt){//Refresh condition
      FlagRepetition=0;
      //cout << "********************" << endl;
      //cout << "Refresh time!!!" << endl;
      //cout << "Event:  " << nevt << endl;
      nevt_pre = nevt;

      Energy2D_Canvas->cd();
      gPad->Modified();
      Energy2D_Canvas->Update();

// Neutron refresh----------------------------------
      for (int imn=0; imn<8; imn++){
        Neutron_RingEnergy_Canvas->cd(imn+1);
        gPad->Modified();
        Neutron_RingEnergy_Canvas->Update();
      }
      for (int imn=0; imn<8; imn++){
        Neutron_RingRate_Canvas->cd(imn+1);
        gPad->Modified();
        Neutron_RingRate_Canvas->Update();
      }

// Gamma refresh----------------------------------
      for (int imn=0; imn<8; imn++){
        Gamma_Energy_Canvas->cd(imn+1);
        gPad->Modified();
        Gamma_Energy_Canvas->Update();
      }
      for (int imn=0; imn<8; imn++){
        Gamma_Rate_Canvas->cd(imn+1);
        gPad->Modified();
        Gamma_Rate_Canvas->Update();
      }

// Ancillary refresh----------------------------------
      for (int imn=0; imn<4; imn++){
        Ancillary_Energy_Canvas->cd(imn+1);
        gPad->Modified();
        Ancillary_Energy_Canvas->Update();
      }
      for (int imn=0; imn<4; imn++){
        Ancillary_Rate_Canvas->cd(imn+1);
        gPad->Modified();
        Ancillary_Rate_Canvas->Update();
      }

//Correlation refresh------------------------------------
      for (int imn=0; imn<4; imn++){
        Correlation_Canvas->cd(imn+1);
        gPad->Modified();
        Correlation_Canvas->Update();
      }

//Neutron distribution refresh------------------------------------
      for (int imn=0; imn<4; imn++){
        NeutronDistribution_Canvas->cd(imn+1);
        gPad->Modified();
        NeutronDistribution_Canvas->Update();
      }



    }// End refresh condition


    if ( nevt_pre==nevt && FlagRepetition==0){//Report when no events
      cout << "Not reading new events"  << endl;
      FlagRepetition=1;
    }


}

void ProcessEvent(NIGIRI* data_now){

    if (data_now->b<0){
        //data_now->Print();
    }


// Maybe should we convinient to change this condition but I NEED first the FINAL configuration
    if (data_now->b==8||data_now->b==9||data_now->b==10){
      for (Int_t i=0;i<V1740_N_MAX_CH;i++){// Loop per all the 64 channels from 1740 modules
        NIGIRIHit* hit=data_now->GetHit(i);
        if (hit->clong>50){//Avoid noisy channels

          if (FlagStart==0){// If it is the first entry read the conf file and set a time reference.
            ConfigurationFile_Reader();
            TimeStampReference = (hit->ts*1e-9)-50;//First timestamp used as reference to plot rate histograms
            FlagStart=1;
          }


        // Assign each signal a channel and detector
          IdChannel_Trigger = 10;
          int Conf_Flag1 = 0;
          for (int yun=1; yun<vCrate.size(); yun++){
            if (vCrate[yun] == data_now->b && vChannel[yun]==i){
              IdChannel_Trigger = yun;
              Conf_Flag1 = 1;
              break;
            }
          }
          if (Conf_Flag1==0){
            cout << "Module or channel number missing in configuration file. Assigning arbitrary Id=10" << endl;
            cout << "Module missing:  " << data_now->b << endl;
            cout << "Channel missing:  " << i << endl;
            cout << "hit->clong:  " << hit->clong << endl;
          }
        //----------------------------------------------------

        //Filling histograms
        if (hit->clong*vEFactor[IdChannel_Trigger]+vEOffset[IdChannel_Trigger]>vThreshold[IdChannel_Trigger]){// Set threshold in keV.

          // Energy histograms------------------------
          H2D_Energy->Fill(IdChannel_Trigger,hit->clong*vEFactor[IdChannel_Trigger]+vEOffset[IdChannel_Trigger]);
          HEnergy_3HeTotal->Fill(hit->clong*vEFactor[IdChannel_Trigger]+vEOffset[IdChannel_Trigger],1./(HEnergy_3HeTotal->GetBinWidth(10)) );
          HEnergy1D_IndividualDetector[IdChannel_Trigger-1]->Fill(hit->clong*vEFactor[IdChannel_Trigger]+vEOffset[IdChannel_Trigger], 1./(HEnergy1D_IndividualDetector[IdChannel_Trigger-1]->GetBinWidth(10)));
          if (vInputType[IdChannel_Trigger]==1)HEnergy3He_Ring[vIndex1[IdChannel_Trigger]-1]->Fill(hit->clong*vEFactor[IdChannel_Trigger]+vEOffset[IdChannel_Trigger], 1./(HEnergy3He_Ring[vIndex1[IdChannel_Trigger]-1]->GetBinWidth(10)));
          //------------------------------------------
          //Rate histograms
          if (vInputType[IdChannel_Trigger]==1)HRate_3HeTotal->Fill(hit->ts*1e-9-TimeStampReference, 1./(HRate_3HeTotal->GetBinWidth(10)) );
          HRate1D_IndividualDetector[IdChannel_Trigger-1]->Fill(hit->ts*1e-9-TimeStampReference, 1./(HRate1D_IndividualDetector[IdChannel_Trigger-1]->GetBinWidth(10)) );
          if (vInputType[IdChannel_Trigger]==1)HRate_3He_Ring[vIndex1[IdChannel_Trigger]-1]->Fill(hit->ts*1e-9-TimeStampReference, 1./(HRate_3He_Ring[vIndex1[IdChannel_Trigger]-1]->GetBinWidth(10)) );


          if (vInputType[IdChannel_Trigger]==1)MapNeutron.emplace(hit->ts,hit->ts);


          for (int tny=0; tny<31; tny++){
            if (vBottomRight[tny]==IdChannel_Trigger)HRateNeutronBottomRight->Fill(hit->ts*1e-9-TimeStampReference, 1./(HRateNeutronBottomRight->GetBinWidth(10)) );
          }
          for (int tny=0; tny<31; tny++){
            if (vBottomLeft[tny]==IdChannel_Trigger)HRateNeutronBottomLeft->Fill(hit->ts*1e-9-TimeStampReference, 1./(HRateNeutronBottomLeft->GetBinWidth(10)) );
          }
          for (int tny=0; tny<31; tny++){
            if (vTopRight[tny]==IdChannel_Trigger)HRateNeutronTopRight->Fill(hit->ts*1e-9-TimeStampReference, 1./(HRateNeutronTopRight->GetBinWidth(10)) );
          }
          for (int tny=0; tny<31; tny++){
            if (vTopLeft[tny]==IdChannel_Trigger)HRateNeutronTopLeft->Fill(hit->ts*1e-9-TimeStampReference, 1./(HRateNeutronTopLeft->GetBinWidth(10)) );
          }

        }
      }// End condition clong<100
    }// End loop all 64 channels V1740

    if (MapNeutron.size()>MapNeutronMaxBufferSize){// Correlation plot neutron
      for (auto it=MapNeutron.begin(); it!=MapNeutron.end(); it++){
        for (auto itk=MapNeutron.begin(); itk!=MapNeutron.end(); itk++){ // Correlation plot neutron neutron
          HCorrelationNeutronNeutron->Fill((itk->second-it->second)*1e-6, HCorrelationNeutronNeutron->GetBinWidth(10));
        }
        for (auto itk=MapF11_Right.begin(); itk!=MapF11_Right.end(); itk++){ // Correlation plot F11 neutron
          HCorrelationF11Neutron->Fill((it->second-itk->second)*1e-6, HCorrelationF11Neutron->GetBinWidth(10));
        }
        for (auto itk=MapGamma.begin(); itk!=MapGamma.end(); itk++){ // Correlation plot F11 neutron
          HCorrelationNeutronGamma->Fill((itk->second-it->second)*1e-6, HCorrelationNeutronGamma->GetBinWidth(10));
        }
      }

      for (auto it=MapF11_Right.begin(); it!=MapF11_Right.end(); it++){
        for (auto itk=MapGamma.begin(); itk!=MapGamma.end(); itk++){ // Correlation plot neutron neutron
          HCorrelationF11Gamma->Fill((itk->second-it->second)*1e-6, HCorrelationF11Gamma->GetBinWidth(10));
        }
      }


      MapNeutron.clear();
      MapF11_Right.clear();
      MapGamma.clear();
    }

  }// End condition board selection
}// End function


void OpenFile(const char* filename){
    file0 = new TFile(filename,"recreate");
    cout << "Open" << endl;
}

int SaveHistograms(){

  time_t now = time(0);
  tm *ltm = localtime(&now);

  string Year=Form("%d",1900+ltm->tm_year);
  string Month=Form("%d",1+ltm->tm_mon);
  string Day=Form("%d",ltm->tm_mday);
  string Hour=Form("%d",ltm->tm_hour);
  string Minute=Form("%d",ltm->tm_min);
  string Time = "Online"+Year+Month+Day+"_"+Hour+Minute+".root";

  string OldConfFile="cp  OfflineConf.csv OfflineConf_Online"+Year+Month+Day+"_"+Hour+Minute+".csv";
  gSystem->Exec(OldConfFile.c_str());

  file0 = new TFile(Time.c_str(),"recreate");

  if (file0){
    cout << "Save histograms and canvas" << endl;

    if (H2D_Energy)H2D_Energy->Write();
    if (HEnergy_3HeTotal)HEnergy_3HeTotal->Write();
    // TH1 He3 rings
    for (int imn=0; imn<7; imn++){
      if (HEnergy3He_Ring[imn])HEnergy3He_Ring[imn]->Write();
    }
    // TH1 Detectors
    for (int imk=0; imk<160; imk++){
      if (HEnergy1D_IndividualDetector[imk]) HEnergy1D_IndividualDetector[imk]->Write();
    }
    //--------------------------------------------------------------------------------------------------
    // Rate histograms--------------------------------------------------------------------------------
    // He3 Total
    HRate_3HeTotal->Write();
    // He3 Ring
    for (int imn=0; imn<7; imn++){
      if (HRate_3He_Ring[imn])HRate_3He_Ring[imn]->Write();
    }
    // TH1 Detectors
    for (int imk=0; imk<160; imk++){
      if (HRate1D_IndividualDetector[imk]) HRate1D_IndividualDetector[imk]->Write();
    }
    // Correlation histograms--------------------------------------------------------------------------------
    if (HCorrelationNeutronNeutron) HCorrelationNeutronNeutron->Write();
    if (HCorrelationF11Neutron) HCorrelationF11Neutron->Write();
    if (HCorrelationNeutronGamma) HCorrelationNeutronGamma->Write();
    if (HCorrelationF11Gamma) HCorrelationF11Gamma->Write();

    // Neutron distribution histograms
    if (HRateNeutronBottomRight) HRateNeutronBottomRight->Write();
    if (HRateNeutronBottomLeft) HRateNeutronBottomLeft->Write();
    if (HRateNeutronTopRight) HRateNeutronTopRight->Write();
    if (HRateNeutronTopLeft) HRateNeutronTopLeft->Write();


    // Canvas General
    if (Energy2D_Canvas) Energy2D_Canvas->Write();
    if (EnergyDetectorCanvas) EnergyDetectorCanvas->Write();
    if (RateDetectorCanvas) RateDetectorCanvas->Write();
    //--------------------

    // Canvas Neutron
    if (Neutron_RingRate_Canvas) Neutron_RingRate_Canvas->Write();
    if (Neutron_RingEnergy_Canvas) Neutron_RingEnergy_Canvas->Write();
    //--------------------

    // Canvas Gamma
    if (Gamma_Rate_Canvas) Gamma_Rate_Canvas->Write();
    if (Gamma_Energy_Canvas) Gamma_Energy_Canvas->Write();
    //--------------------

    // Canvas Ancillary
    if (Ancillary_Rate_Canvas) Ancillary_Rate_Canvas->Write();
    if (Ancillary_Energy_Canvas) Ancillary_Energy_Canvas->Write();
    //--------------------

    // Canvas Correlation
    if (Correlation_Canvas) Correlation_Canvas->Write();
    //--------------------

    // Canvas Neutron Distribution
    if (NeutronDistribution_Canvas) NeutronDistribution_Canvas->Write();
    //--------------------

  }
  return 0;
}

int CloseMe(){

    cout<< "Number events: " << nevt <<endl;

    SaveHistograms();

    cout << "Close" << endl;

    return 0;
}

//!**************************************************
//! Data decoding
//! *************************************************

//! parameters for decoding V1740 zerosuppression
#define V1740_HDR 6

//! packet map

typedef enum{
    NONE = 0,
    LUPO = 1,
    V1740ZSP = 2,
    V1740RAW = 3,
    V1730DPPPHA = 4,
}pmap_decode;


//*****************************************************************************************************************************************************************************************
// PACKETMAP ZONE. This maps indicate the structure of digitaizer used in the measure.
// If these vectors are not well set the program won't work.

//#define N_PACKETMAP 14
//const int packetmap[]={50,51,52,53,54,55,56,57,58,59,60,100,101,102,103};
//const pmap_decode packetdecode[]={V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1730DPPPHA,V1730DPPPHA,V1730DPPPHA,V1730DPPPHA};

#define N_PACKETMAP 14
const int packetmap[]={49,50,51,52,53,54,55,56,57,58,59,60,61,100};
const pmap_decode packetdecode[]={NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1730DPPPHA};

//#define N_PACKETMAP 14
//const int packetmap[]={49,50,51,52,53,54,55,56,57,58,59,60,61,100};
//const pmap_decode packetdecode[]={LUPO,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1730DPPPHA};

//#define N_PACKETMAP 16
//const int packetmap[]={49,50,51,52,53,54,55,56,57,58,59,60,100,101,102,103};
//const pmap_decode packetdecode[]={LUPO,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1730DPPPHA,V1730DPPPHA,V1730DPPPHA,V1730DPPPHA};
//*****************************************************************************************************************************************************************************************

UShort_t ledthr[MAX_N_BOARD][V1740_N_MAX_CH];
NIGIRI* data_prev[MAX_N_BOARD];

int init_done = 0;


struct refesh_thread_argument
{
  unsigned int refreshinterval;
};
static TThread *refesh_thread = 0;

void rateupdate(void * ptr){
    refesh_thread_argument* ta = (refesh_thread_argument *) ptr;
    unsigned int my_refreshinterval = ta->refreshinterval;
    time_t last_time = time(0);  //force an update on start
    //cout<<"refesh thread initialized, current time since January 1, 1970"<<last_time<<endl;
    while (1)
    {
        time_t x = time(0);
        if ( x - last_time > my_refreshinterval)
      {
        last_time = x;
        DoUpdate();
       }
    }
}

int pinit()
{
  if (init_done) return 1;
  init_done = 1;
#ifdef RATE_CAL_REFESH_SECONDS
  refesh_thread_argument* ta = new refesh_thread_argument;
  ta->refreshinterval = RATE_CAL_REFESH_SECONDS;
  refesh_thread = new TThread(rateupdate,ta);
  refesh_thread->Run();
#endif

  gROOT->ProcessLine(".L libDataStruct.so");
  data=new NIGIRI;
  for (Int_t i=0;i<MAX_N_BOARD;i++){
      data_prev[i]=new NIGIRI;
      for (Int_t j=0;j<V1740_N_MAX_CH;j++){
          ledthr[i][j]=850;
      }
  }


  Init();
  return 0;
}

void decodeV1740raw(Packet* p1740raw){
    //!**************************************************
    //! v1740Raw data packet
    //! *************************************************
    int* tmp;
    int* words;
    words=(int*) p1740raw->getIntArray(tmp);
    int nevt=p1740raw->getPadding();
    int ipos=0;
    for (int evt=0;evt<nevt;evt++){
        //NIGIRI* data=new NIGIRI;
        data->Clear();
        data->DecodeHeaderRaw(words,ipos,p1740raw->getHitFormat());
        ipos+=data->event_size;
        ProcessEvent(data);
    }//end of event loop
}

#define TSCORR 6
double thr_abs = 150;

void decodeV1740zsp(Packet* p1740zsp){
    //!**************************************************
    //! v1740 with zerosuppresion data packet
    //! *************************************************
    int* temp;
    int* gg;
    gg=(int*) p1740zsp->getIntArray(temp);
    //! header
    //NIGIRI* data=new NIGIRI;
    int nEvents = p1740zsp->getPadding();
    //cout<<"\n********** "<<nEvents<<" ***********\n"<<endl;
    int k=0;

    for (Int_t i=0;i<nEvents;i++){
        data->Clear();
        int headaddr = k;
        data->DecodeHeaderZsp(&gg[k],p1740zsp->getHitFormat());
        k+=V1740_HDR+V1740_N_MAX_CH;
        //! get number of channels from channel mask
        double min_finets = 99999;
        int ich_min_finets = -1;
        for (int i=0;i<V1740_N_MAX_CH;i++){
            int chgrp = i/8;
            if (((data->channel_mask>>chgrp)&0x1)==0) continue;
            //! header
            NIGIRIHit* chdata=new NIGIRIHit;
            chdata->ch = i;//for sorter
            int nsample = gg[headaddr+V1740_HDR+i];
            if (nsample>NSBL&&nsample<N_MAX_WF_LENGTH){
                data->board_fail_flag = 1;
            }
            chdata->nsample = nsample;
            UShort_t WaveLine[nsample];
            int ispl = 0;
            for (int j=0;j<nsample/2+nsample%2;j++){
                if (ispl<nsample) {
                    WaveLine[ispl]=gg[k]&0xFFFF;
                    chdata->pulse.push_back(gg[k]&0xFFFF);

                }
                ispl++;
                if (ispl<nsample) {
                    WaveLine[ispl]=(gg[k]>>16)&0xFFFF;
                    chdata->pulse.push_back((gg[k]>>16)&0xFFFF);
                }
                ispl++;
                k++;
            }
            //!--------------------
            if (nsample>NSBL){
                chdata->processPulseV1740(data->ts,NSBL,ledthr[data->b][chdata->ch],trig_pos,sampling_interval);
            }

            if (chdata->finets<min_finets&&chdata->finets>=0){
                min_finets =chdata->finets;
                ich_min_finets = i;
            }
            data->AddHit(chdata);
        }//loop all channels
        data->trig_ch = ich_min_finets;

        if (data->board_fail_flag==1){
            data_prev[data->b]->MergePulse(data,data_prev[data->b]->ts,NSBL,ledthr[data->b],trig_pos,sampling_interval,N_MAX_WF_LENGTH);
        }
        //ProcessEvent(data);
        //! process data
        if (data_prev[data->b]->b>=0){
            if (data_prev[data->b]->board_fail_flag!=1)
                ProcessEvent(data_prev[data->b]);
            data_prev[data->b]->Clear();
        }
        data->Copy(*data_prev[data->b]);
        //data_prev[data->b] = (NIGIRI*) data->Clone();
    }
}

void decodeV1730dpppha(Packet* p1730dpppha){
    //!**************************************************
    //! v1730DPPPHA packet
    //! *************************************************
    int* tmp;
    int* words;
    words=(int*) p1730dpppha->getIntArray(tmp);
    int totalsize = p1730dpppha->getPadding();
    int pos=0;
    boardaggr_t boardaggr;
    while (pos<totalsize){
        //! board aggr
        //! check if we have valid datum
        if ((words[pos]&0xF0000000)!=0xA0000000) cout<<"Error decoding V1730DPPPHA!"<<endl;
        boardaggr.size = (words[pos]&0xFFFFFFF);
        boardaggr.board_id = (words[pos+1]&0xF8000000)>>27;
        boardaggr.board_fail_flag = (bool)((words[pos+1]&0x4000000)>>26);
        boardaggr.pattern = (words[pos+1]&0x7FFF00)>>8;
        boardaggr.dual_ch_mask = (words[pos+1]&0xFF);
        boardaggr.counter = (words[pos+2]&0x7FFFFF);
        boardaggr.timetag = words[pos+3];
        //boardaggr.print();
        pos+=4;
        for (int i=0;i<V1730_MAX_N_CH/2;i++){
            if (((boardaggr.dual_ch_mask>>i)&0x1)==0) continue;
            //! read channel aggr
            channelaggr_t channelaggr;
            channelaggr.groupid=i;
            channelaggr.formatinfo=(words[pos]&0x80000000)>>31;
            channelaggr.size=words[pos]&0x7FFFFFFF;
            channelaggr.dual_trace_flag=(words[pos+1]&0x80000000)>>31;
            channelaggr.energy_enable_flag=(words[pos+1]&0x40000000)>>30;
            channelaggr.trigger_time_stamp_enable_flag=(words[pos+1]&0x20000000)>>29;
            channelaggr.extra2_enable_flag=(words[pos+1]&0x10000000)>>28;
            channelaggr.waveformsample_enable_flag=(words[pos+1]&0x8000000)>>27;
            channelaggr.extra_option_enable_flag=(words[pos+1]&0x7000000)>>24;
            channelaggr.analogprobe1_selection=(words[pos+1]&0xC00000)>>22;
            channelaggr.analogprobe2_selection=(words[pos+1]&0x300000)>>20;
            channelaggr.digitalprobe_selection=(words[pos+1]&0xF0000)>>16;
            int nsampleto8=(words[pos+1]&0xFFFF);
            channelaggr.n_samples=nsampleto8*8;
            //channelaggr.print();
            pos+=2;

            //! read dual channel data
            //! check if this has two channel data
            int nword_samples=channelaggr.n_samples/2+channelaggr.n_samples%2;
            int nevents=(channelaggr.size-2)/(nword_samples+3);
            for (int i=0;i<nevents;i++){
                channel_t channeldata;
                // read header
                int odd_even_flag=(words[pos]&0x80000000)>>31;
                if (odd_even_flag==0) channeldata.ch=channelaggr.groupid*2;//even
                else channeldata.ch=channelaggr.groupid*2+1;
                channeldata.trigger_time_tag=(words[pos]&0x7FFFFFFF);
                channeldata.n_samples=channelaggr.n_samples;
                pos++;
                // read data samples
                for (int n=0;n<channeldata.n_samples/2+channeldata.n_samples%2;n++){
                    //! read channel samples
                    int ap1= (words[pos]&0x3FFF);
                    int ap2= (words[pos]&0x3FFF0000)>>16;
                    if (channelaggr.dual_trace_flag==1){//dual trace
                        channeldata.ap1_sample.push_back(ap1);//even sample of ap1
                        channeldata.ap2_sample.push_back(ap2);//even sample of ap2
                    }else{//single trace
                        channeldata.ap1_sample.push_back(ap1);//even sample of ap1
                        channeldata.ap1_sample.push_back(ap2);//odd sample of ap1
                        //cout<<"single trace"<<endl;
                    }
                    channeldata.dp_sample.push_back((words[pos]&0x4000)>>14);//even sample
                    channeldata.dp_sample.push_back((words[pos]&0x40000000)>>30);//odd sample
                    channeldata.trg_sample.push_back((words[pos]&0x8000)>>15);//even sample
                    channeldata.trg_sample.push_back((words[pos]&0x80000000)>>31);//odd sample
                    pos++;
                }
                // read extras
                channeldata.extras2 = words[pos];
                channeldata.extras = (words[pos+1]&0x1F0000)>>16;
                channeldata.pileup_rollover_flag = (words[pos+1]&0x8000)>>15;
                channeldata.energy = (words[pos+1]&0x7FFF);
                pos+=2;
                //channeldata.print();
                //! process channel data
                //NIGIRI* data=new NIGIRI;
                data->Clear();
                data->b = p1730dpppha->getHitFormat();
                data->evt = boardaggr.counter;
                data->event_size = channelaggr.size;
                UInt_t tslsb = (UInt_t) (channeldata.trigger_time_tag&0x7FFFFFFF);
                UInt_t tsmsb = (UInt_t) ((channeldata.extras2>>16)&0xFFFF);
                Double_t tpz_baseline = ((Double_t)(channeldata.extras2&0xFFFF))/4.;
                data->ts = (((ULong64_t)tsmsb<<31)&0x7FFF80000000)|(ULong64_t)tslsb;
                data->ts = data->ts*V1730_DGTZ_CLK_RES;



                if (channeldata.energy>0){

                  if (FlagStart==0){// If it is the first entry read the conf file and set a time reference.
                    ConfigurationFile_Reader();
                    TimeStampReference = (data->ts*1e-9)-50;//First timestamp used as reference to plot rate histograms
                    FlagStart=1;
                  }


                // Assign each signal a channel and detector
                  IdChannel_Trigger = 10;
                  int Conf_Flag1 = 0;
                  for (int yun=1; yun<vCrate.size(); yun++){
                    if (vCrate[yun] == data->b && vChannel[yun]==channeldata.ch){
                      IdChannel_Trigger = yun;
                      Conf_Flag1 = 1;
                      break;
                    }
                  }
                  if (Conf_Flag1==0){
                    cout << "Module or channel number missing in configuration file. Assigning arbitrary Id=10" << endl;
                    cout << "Module missing:  " << data->b << endl;
                    cout << "Channel missing:  " << channeldata.ch << endl;
                  }
                  //----------------------------------------------------
                  //Filling histograms
                  if (channeldata.energy*vEFactor[IdChannel_Trigger]+vEOffset[IdChannel_Trigger]>vThreshold[IdChannel_Trigger]){// Set threshold in keV.

                    // Energy histograms------------------------
                    H2D_Energy->Fill(IdChannel_Trigger,channeldata.energy*vEFactor[IdChannel_Trigger]+vEOffset[IdChannel_Trigger]);
                    if (vInputType[IdChannel_Trigger]==2)HEnergy_GammaTotal->Fill(channeldata.energy*vEFactor[IdChannel_Trigger]+vEOffset[IdChannel_Trigger], 1./(HEnergy_GammaTotal->GetBinWidth(10)) );
                    HEnergy1D_IndividualDetector[IdChannel_Trigger-1]->Fill(channeldata.energy*vEFactor[IdChannel_Trigger]+vEOffset[IdChannel_Trigger], 1./(HEnergy1D_IndividualDetector[IdChannel_Trigger-1]->GetBinWidth(10)));
                    //------------------------------------------
                    //Rate histograms
                    HRate1D_IndividualDetector[IdChannel_Trigger-1]->Fill(data->ts*1e-9-TimeStampReference, 1./(HRate1D_IndividualDetector[IdChannel_Trigger-1]->GetBinWidth(10)) );
                    if (vInputType[IdChannel_Trigger]==2)HRate_GammaTotal->Fill(data->ts*1e-9-TimeStampReference, 1./(HRate_GammaTotal->GetBinWidth(10)) );


                    if (vId[IdChannel_Trigger]==149)MapF11_Right.emplace(data->ts,data->ts);
                    if (vInputType[IdChannel_Trigger]==2)MapGamma.emplace(data->ts,data->ts);

                  }
                }

            }//loop on channel data
            //pos+=channelaggr.size;
        }//loop through all  dual channels data
    }//end loop on all words
    //std::cout<<"---"<<std::endl;
}

void decodelupo(Packet* pLUPO){
    int* tmp;
    int* gg;
    gg=(int*) pLUPO->getIntArray(tmp);
    //NIGIRI* data=new NIGIRI;
    data->Clear();
    data->b = pLUPO->getHitFormat();
    UInt_t tslsb = (UInt_t)gg[3];
    UInt_t tsmsb = (UInt_t)gg[2];
    data->ts = (((ULong64_t)tsmsb<<32)&0xFFFF00000000)|(ULong64_t)tslsb;//resolution is 10 ns!
    data->ts = data->ts*LUPO_CLK_RES;
    data->pattern= gg[1];//daq counter
    data->evt = gg[0];
    ProcessEvent(data);
}

int process_event (Event * e)
{

    nevt++;
//    time_t tevt =e->getTime();
//    cout<<"***************** Eventpacket number  = "<<e->getEvtSequence()<<" / record date time"<<asctime(gmtime(&tevt))<<"***************\n"<<endl;
    Packet *pmap[N_PACKETMAP];
    for (Int_t i=0;i<N_PACKETMAP;i++){
        pmap[i]=e->getPacket(packetmap[i]);
        if (pmap[i]){
            if (packetdecode[i]==LUPO){
                decodelupo(pmap[i]);
            }else if (packetdecode[i]==V1740ZSP){
                decodeV1740zsp(pmap[i]);
            }else if (packetdecode[i]==V1740RAW){
                decodeV1740raw(pmap[i]);
            }else if (packetdecode[i]==V1730DPPPHA){
                decodeV1730dpppha(pmap[i]);
            }else{
                //cout<<"out of definition!"<<endl;
            }
            delete pmap[i];
#ifdef SLOW_ONLINE
    usleep(SLOW_ONLINE);
#endif
        }
    }
    return 0;
}
int pclose(){
    cout<<"Closing"<<endl;
    CloseMe();
    return 0;
}

int phsave (const char *filename){
    OpenFile(filename);
    return 0;
}

int main (int argc, char* argv[])
{
    if (argc!=3&&argc!=4&&argc!=5){
        cout<<"Usage. ./offline input output"<<endl;
        cout<<"Usage. ./offline input_list_file output anything"<<endl;
        return 0;
    }
    if (argc==3) pfileopen(argv[1]);
    else pfileopen(argv[1]);
    phsave(argv[2]);

    cout << argv[3] << endl;
    cout << argv[4] << endl;
    prun();
    //phelp();
    //pidentify(0);
    return 0;
}
