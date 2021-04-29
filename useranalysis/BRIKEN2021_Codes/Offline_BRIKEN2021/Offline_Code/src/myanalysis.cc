// myanalysis_MP_V1730_V1740.cc
// This code is the Offline analysis for the BRIKEN detector.
// Input: evt file from digitizers V1730 and V1740.
// It looks for a configuration file called OfflineConf.csv in the same directory this code is.
// IMPORTANT: Check the PACKETMAP ZONE defined in this code and define the correct map for each set of files.
// Output: Root file with a tree called BRIKENTree (TimeStamps,Energy,Id,...).
/* ************************ DO NOT REMOVE  ****************************** */
#include <iostream>
#include <pmonitor/pmonitor.h>
#include "myanalysis.h"
#include <time.h>
#include <stdlib.h>

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
#define RATE_CAL_REFESH_SECONDS 10

#define MAX_N_BOARD 20
#define V1730_MAX_N_CH 16
#define V1740_N_MAX_CH 64
//! parameters for V1740
#define NSBL 8
#define N_MAX_WF_LENGTH 90
UShort_t trig_pos = N_MAX_WF_LENGTH*30/100;//unit of sample
UShort_t sampling_interval = 16*8;//unit of ns

TFile* file0 = 0;
TTree* tree = 0;
// MAX VERSION---------------------------------------------------------------------------------
TTree* BRIKENTree = 0;
BrikenTreeData data_BRIKEN;
//------------------------------------------------------------

NIGIRI* data;

TH2F* hwf2d[V1740_N_MAX_CH*10];
TH2F* he2d;

NIGIRI * treedata;

// MAX VERSION---------------------------------------------------------------------------------
BrikenTreeData    ToNeuFill;
BrikenTreeData    ToGammaFill;
BrikenTreeData    ToAncFill;
TBranch *NeutronsBranch;
TBranch *GammaBranch;
TBranch *AncillaryBranch;
//BrikenTreeData magnitudes
// E -> energy
// T -> TimeStamps
// Id -> Detector numeration
// type -> Group of detectors associated to each branch ( (1)Neutron, (2)Gamma, (3)Ancillary )
// Index1 -> Number of ring
// Index2 -> Dummy
// InfoFlag -> Dummy
// Name -> Name of the detector

std::multimap<uint64_t,BrikenTreeData> DataMapBRIKEN;

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
std::vector<double> vEFactorSecond;
std::vector<double> vThreshold;

double TimeOLD=0;

double Last_Timestamp=0;
double Last_Energy=0;
double Last_Module=0;
double Last_Channel=0;
int Count_RepeatTime=0;
int Count_RepeatTime_Neutron=0;
int IdChannel_Trigger=10;

int nevt_neutrons=0;
int NumberEvents=0;
int MultiplicitySignals=0;

string ConfFileName;
//------------------------------------------------------------

int nevt = 0;

double CalibrateEnergy(double eraw, double a0, double a1, double a2){
  double ene = eraw + rand()/double(RAND_MAX)-0.5;
  return a0+(a1 + a2*ene)*ene;
}



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
  vEFactorSecond.clear();
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
       double EOffset=0, EFactor=0, EFactorSecond=0, Threshold=0;
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
       iss>>EFactorSecond;
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
       vEFactorSecond.push_back(EFactorSecond);
       vThreshold.push_back(Threshold);
     }
   }

   Fd.close();
   std::string CopyFile ="cp ";

   std::size_t lastindex = ConfFileName.find_last_of(".");
   std::string SaveConfFile = ConfFileName.substr(0, lastindex);
   CopyFile +="OfflineConf.csv";

   CopyFile +=" ";
   CopyFile +=SaveConfFile;
   CopyFile +="_RunConf.csv";
   cout << "CopyFile:  " << CopyFile << endl;
   std::cout<<CopyFile.c_str()<<std::endl;
   system(CopyFile.c_str());

}

void ProcessEvent(NIGIRI* data_now){
    treedata->Clear();
    data_now->Copy(*treedata);
    tree->Fill();
}

// Function to sort the timestamps, the associated magnitudes and fill the BRIKENTree.
void ProcessEvent_BRIKENTree_Organized(std::multimap<uint64_t,BrikenTreeData> DataMap){

  cout << "BRIKENTree size:  " << DataMap.size() << endl;

  for(auto it=DataMap.begin(); it!=DataMap.end(); it++){

    ToNeuFill.Clear();
    ToGammaFill.Clear();
    ToAncFill.Clear();

    //if (it->second.T-TimeOLD==0)cout << "Repeated timestamp" << it->second.E << endl;
    if (it->second.T-TimeOLD<0)cout << "Timestamp sort error!!!" << "Number event:  " << nevt << endl;
    TimeOLD = it->second.T;

    if (it->second.type==1)ToNeuFill = it->second;
    if (it->second.type==2)ToGammaFill = it->second;
    if (it->second.type==3)ToAncFill = it->second;

    BRIKENTree->Fill();

  }
}

void Init(){
    treedata = new NIGIRI;
}



void DoUpdate(){
    pstatus();
}



// Open the root file, create the Tree and read the configuration file.
void OpenFile(const char* filename){

  file0 = new TFile(filename,"recreate");
  ConfFileName = filename;

  BRIKENTree = new TTree("BRIKENTree","BRIKENTree");
  NeutronsBranch = BRIKENTree->Branch("Neutrons.",&ToNeuFill);
  GammaBranch = BRIKENTree->Branch("Gamma.",&ToGammaFill);
  AncillaryBranch = BRIKENTree->Branch("Ancillary.",&ToAncFill);
  cout << "BRIKENTree Created!" << endl;

  ConfigurationFile_Reader();
  cout << "Configuration File Working!" << endl;

}

// End the program.  Fill the BRIKENTree and store it in a root file. Also print the proportion of counts with repeated time stamp.
void CloseMe(){
  ProcessEvent_BRIKENTree_Organized(DataMapBRIKEN);
  cout << "Count_RepeatTime/Total(%):  " << float(Count_RepeatTime)/float(NumberEvents)*100. << endl;
  cout << "Count_RepeatTime_NeutronRegion/Total_NeutronRegion(%):  " << float(Count_RepeatTime_Neutron)/float(nevt_neutrons)*100. << endl;

  if (file0) {
    if (tree) tree->Write();
    if (BRIKENTree) BRIKENTree->Write();
    file0->Close();
  }
  cout<< "Number of events: " << NumberEvents <<endl;
  cout<< "Number of events neutron region: " << nevt_neutrons <<endl;
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

void BRIKENTree_Construction(NIGIRI* treeModule, int BoardNumber){

  MultiplicitySignals=0;
  for (int irn=0; irn<64; irn++){
    if ( treeModule->fhits.at(irn)->clong > 0 ){

      data_BRIKEN.Clear();

      IdChannel_Trigger = 10;// THIS MUST BE CHANGED BUT WE DONT HAVE THE FINAL MODULE DISTRIBUTION
      int Conf_Flag1 = 0;
      for (int yun=1; yun<vCrate.size(); yun++){
        if (vCrate[yun] == BoardNumber && vChannel[yun]==irn){
          IdChannel_Trigger = yun;
          Conf_Flag1 = 1;
          break;
        }
      }
      if (Conf_Flag1==0){
        cout << "Module or channel number missing in configuration file. Assigning arbitrary Id=10" << endl;
        cout << "Module missing:  " << BoardNumber << endl;
        cout << "Channel missing:  " << irn << endl;
      }

      data_BRIKEN.T = treeModule->fhits.at(irn)->ts;
      //if (irn==30 && data_BRIKEN.T!=0)cout << "data_BRIKEN.T:  " << data_BRIKEN.T << endl;
      data_BRIKEN.E = (treeModule->fhits.at(irn)->clong+ rand()/double(RAND_MAX) - 0.5) * ( treeModule->fhits.at(irn)->clong * vEFactorSecond[IdChannel_Trigger] + vEFactor[IdChannel_Trigger]) + vEOffset[IdChannel_Trigger];
      //data_BRIKEN.E = CalibrateEnergy(treeModule->fhits.at(irn)->clong,vEOffset[IdChannel_Trigger],vEFactor[IdChannel_Trigger],vEFactorSecond[IdChannel_Trigger])
      data_BRIKEN.Id = IdChannel_Trigger;
      data_BRIKEN.type = vInputType[IdChannel_Trigger];
      data_BRIKEN.Index1 = vIndex1[IdChannel_Trigger];
      data_BRIKEN.Index2 = vIndex2[IdChannel_Trigger];
      data_BRIKEN.Name = vName[IdChannel_Trigger];


      DataMapBRIKEN.emplace(data_BRIKEN.T,data_BRIKEN);
      NumberEvents++;
      if (data_BRIKEN.E>150 && data_BRIKEN.E<820 && data_BRIKEN.type==1)nevt_neutrons++;

      if ( data_BRIKEN.T != Last_Timestamp )MultiplicitySignals=0;
      if ( data_BRIKEN.T == Last_Timestamp ){
        Count_RepeatTime++;
        MultiplicitySignals++;
        if (data_BRIKEN.E>150 && data_BRIKEN.E<820)Count_RepeatTime_Neutron++;
        data_BRIKEN.T = data_BRIKEN.T + MultiplicitySignals;
        //cout << "MultiplicitySignals:  " << MultiplicitySignals << endl;
      }
      Last_Timestamp = data_BRIKEN.T-MultiplicitySignals;
      Last_Energy = data_BRIKEN.E;
    }
  }
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
        if (data->b==12)cout << "data->b=" << data->b << endl;
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
            //if (data_prev[data->b]->fhits.at(30)->ts!=0)cout << "data_prev[data->b]->fhits.at(30)->ts:  " << data_prev[data->b]->fhits.at(30)->ts << endl;
                BRIKENTree_Construction(data_prev[data->b], data->b);
                //ProcessEvent(data_prev[data->b]);
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

                data_BRIKEN.Clear();

                if (channeldata.energy>0){

                  int IdChannel_Trigger = 10;// THIS MUST BE CHANGED BUT WE DONT HAVE THE FINAL MODULE DISTRIBUTION
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

                  data_BRIKEN.T = data->ts;

                  //double EnergyRaw = channeldata.energy  - 0.5;
                  //data_BRIKEN.E = EnergyRaw * ( EnergyRaw * vEFactorSecond[IdChannel_Trigger]+ vEFactor[IdChannel_Trigger] ) + vEOffset[IdChannel_Trigger];
                  data_BRIKEN.E = CalibrateEnergy(channeldata.energy,vEOffset[IdChannel_Trigger],vEFactor[IdChannel_Trigger],vEFactorSecond[IdChannel_Trigger]);
                  data_BRIKEN.Id = IdChannel_Trigger;
                  data_BRIKEN.type = vInputType[IdChannel_Trigger];
                  data_BRIKEN.Index1 = vIndex1[IdChannel_Trigger];
                  data_BRIKEN.Index2 = vIndex2[IdChannel_Trigger];
                  data_BRIKEN.Name = vName[IdChannel_Trigger];

                  DataMapBRIKEN.emplace(data_BRIKEN.T,data_BRIKEN);
                  NumberEvents++;
                  if (data_BRIKEN.E>150 && data_BRIKEN.E<820 && data_BRIKEN.type==1)nevt_neutrons++;

                  if ( data_BRIKEN.T == Last_Timestamp && (data_BRIKEN.E<900 || Last_Energy<900) ){
                    Count_RepeatTime++;
                    if (data_BRIKEN.E>150 && data_BRIKEN.E<820)Count_RepeatTime_Neutron++;
                  }
                  Last_Timestamp = data_BRIKEN.T;
                  Last_Energy = data_BRIKEN.E;

                }
            }//loop on channel data
            //pos+=channelaggr.size;
        }//loop through all  dual channels data
    }//end loop on all words
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
    if (argc!=3&&argc!=4){
        cout<<"Usage. ./offline input output"<<endl;
        cout<<"Usage. ./offline input_list_file output anything"<<endl;
        return 0;
    }
    if (argc==3) pfileopen(argv[1]);
    else plistopen(argv[1]);
    phsave(argv[2]);
    prun();
    return 0;
}
