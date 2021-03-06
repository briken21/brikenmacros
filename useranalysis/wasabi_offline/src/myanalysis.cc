
/*mrun ************************ DO NOT REMOVE  ****************************** */
#include <iostream>
#include <pmonitor/pmonitor.h>
#include "myanalysis.h"
#include <time.h>

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

#include <correlation.h>
#include "TThread.h"

//#define SLOW_ONLINE 100000
#define RATE_CAL_REFESH_SECONDS 10

#define MAX_N_BOARD 20
#define V1730_MAX_N_CH 16
#define V1740_N_MAX_CH 64
//! parameters for V1740
#define NSBL 8
int N_MAX_WF_LENGTH = 90;
UShort_t trig_pos = N_MAX_WF_LENGTH*30/100;//unit of sample
UShort_t sampling_interval = 8*16;//unit of ns

TFile* file0 = 0;
TTree* tree = 0;
TTree * aida_tree = 0;
NIGIRI* data;

TH2F* hwf2d[V1740_N_MAX_CH*10];
TH2F* he2d;

NIGIRI * treedata;
AidaTreeData aida_data;
multimap<ULong64_t, AidaTreeData> aida_data_map;
multimap<ULong64_t, AidaTreeData>::iterator aidaIt;
// Histograms for wasabi
TH1D * channelHitPattern;
TH2D * channelVsEnergy;
TH2D * detXYBeta[4];
TH2D * detXYImplant[4];
TH2D * detExEyBeta[4];
TH2D * detExEyImplant[4];

TH2D * dE0vdESum;
double dE0 = 0;
double dESum = 0;
int detNum;
int runNumInt;

int nevt = 0;

void Init(){
    treedata = new NIGIRI;
    for (Int_t i=0;i<64*3;i++){
        hwf2d[i]=new TH2F(Form("hwf2d%d",i),Form("hwf2d%d",i),300,0,300,500,0,4000);
    }
    he2d = new TH2F("he2d","he2d",64*3,0,64*3,500,0,4000);
}

void ProcessEvent(NIGIRI* data_now){
    treedata->Clear();
    data_now->Copy(*treedata);
    tree->Fill();

    if (data_now->b == 11) {
        //dE detector event
        dEFromNIGIRI dEEventBuilder(*data_now);
        aida_data_map = dEEventBuilder.GetdEEvents();
        dESum = 0;
        for( auto dEIt : aida_data_map){
            aida_data = dEIt.second;
            if (aida_data.z == 0){
                dE0 = aida_data.E;
            } else{
                dESum += aida_data.E;
            }
            aida_tree->Fill();
            //channelHitPattern->Fill(aida_data.z+520);
            //channelVsEnergy->Fill(aida_data.z+520,aida_data.E);
            //dE0vdESum->Fill(dE0, dESum);
        }
    }

    if (data_now->b >= 0 && data_now->b <= 7) {
        AIDAFromNIGIRI aidaEventBuilder(*data_now, runNumInt);
        //Hit pattern histograms
        for(auto hit : data_now->fhits){
            if (0 < hit->clong){
                channelHitPattern->Fill(data_now->b*64+hit->ch);
                channelVsEnergy->Fill(data_now->b*64+hit->ch, hit->clong);
            }
        }

        aida_data_map = aidaEventBuilder.GetAIDAEvents();
        for (aidaIt = aida_data_map.begin(); aidaIt != aida_data_map.end(); aidaIt++) {
            aida_data = aidaIt->second;
            aida_tree->Fill();
            //Matched event histograms
            if(aida_data.ID == 4){
                detNum = (int)aida_data.z-11;
                detXYImplant[detNum]->Fill(aida_data.x, aida_data.y);
                detExEyImplant[detNum]->Fill(aida_data.EX,aida_data.EY);
            } else {
                detNum = (int)aida_data.z-11;
                detXYBeta[detNum]->Fill(aida_data.x, aida_data.y);
                detExEyBeta[detNum]->Fill(aida_data.EX,aida_data.EY);
            }
        }
    }

}

void DoUpdate(){
    pstatus();
}

void OpenFile(const char* filename){
    file0 = new TFile(filename,"recreate");
    tree = new TTree("tree","tree");
    tree->Branch("data",&treedata);
    aida_tree = new TTree("AIDA_hits","AIDA_hits");
    aida_tree->Branch("aida_hit", &aida_data,"T/l:Tfast/l:E/D:Ex/D:Ey/D:x/D:y/D:z/D:nx/I:ny/I:nz/I:ID/b");
    //Define histograms
    channelHitPattern = new TH1D("ChannelHitPattern","",535,0,535);
    channelVsEnergy = new TH2D("ChannelVsEnergy","",535,0,535,500,0,5000);
    dE0vdESum = new TH2D("dE0VsdESum","",500,0,5e3,500,0,5e3);

    std::string hName;
    for(int i = 0; i < 4; i++){
        hName = "DSSD" + std::to_string(i) + "XYBeta";
        detXYBeta[i] = new TH2D(hName.c_str(),"",32,0,32,32,0,32);
        hName = "DSSD" + std::to_string(i) + "XYImplant";
        detXYImplant[i] = new TH2D(hName.c_str(),"",32,0,32,32,0,32);
        hName = "DSSD" + std::to_string(i) + "EXEYBeta";
        detExEyBeta[i] = new TH2D(hName.c_str(),"",500,0,5000,500,0,5000);
        hName = "DSSD" + std::to_string(i) + "EXEYImplant";
        detExEyImplant[i] = new TH2D(hName.c_str(),"",500,0,5000,500,0,5000);
    }
}

void CloseMe(){
    if (file0) {
        if (tree) tree->Write();
        if (aida_tree) aida_tree->Write();

        channelHitPattern->Write();
        channelVsEnergy->Write();
        dE0vdESum->Write();
        for (int i = 0 ;i <4; i++) {
            detXYBeta[i]->Write();
            detXYImplant[i]->Write();
            detExEyBeta[i]->Write();
            detExEyImplant[i]->Write();
        }

        file0->Close();
    }
    cout<<nevt<<endl;
}

//!**************************************************
//! Data decoding
//! *************************************************

//! parameters for decoding V1740 zerosuppression
#define V1740_HDR 6

//! packet map
#define N_PACKETMAP 16
typedef enum{
    NONE = 0,
    LUPO = 1,
    V1740ZSP = 2,
    V1740RAW = 3,
    V1730DPPPHA = 4,
}pmap_decode;

//! full map
//const int packetmap[]={50,51,52,53,54,55,56,57,58,59,60,100,101,102,103};
//const pmap_decode packetdecode[]={V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1730DPPPHA,V1730DPPPHA,V1730DPPPHA,V1730DPPPHA};

//! current map Used for the Bi data
//const int packetmap[]={49,50,51,52,53,54,55,56,57,58,59,60,100,101,102,103};
//const pmap_decode packetdecode[]={LUPO,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1730DPPPHA,V1730DPPPHA,V1730DPPPHA,V1730DPPPHA};

//Map from 210419 elog
#define N_PACKETMAP 14
const int packetmap[]={49,50,51,52,53,54,55,56,57,58,59,60,61,100};
const pmap_decode packetdecode[]={LUPO,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1730DPPPHA};
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
      data_prev[i]=0;
      for (Int_t j=0;j<V1740_N_MAX_CH;j++){
          ledthr[i][j]=1600;
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
        if(data->b==11){
          N_MAX_WF_LENGTH = 90;
        }
        else if( data->b >3 && data->b < 9 && runNumInt >26){
            N_MAX_WF_LENGTH = 360;
        }
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
            if (nsample>NSBL&&nsample<N_MAX_WF_LENGTH){//!
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
            data_prev[data->b]->MergePulse(data,data_prev[data->b]->ts,NSBL,ledthr[data->b],trig_pos,sampling_interval);
        }
        //! process data
        if (data_prev[data->b]!=0){
            if (data_prev[data->b]->board_fail_flag!=1)
                ProcessEvent(data_prev[data->b]);
            delete data_prev[data->b];
        }
        data_prev[data->b] = (NIGIRI*) data->Clone();
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
                //cout<<channelaggr.extra2_enable_flag<<"\t"<<channelaggr.extra_option_enable_flag<<"\t"<<data->ts<<endl;
                NIGIRIHit* hit = new NIGIRIHit;
                hit->ch = channeldata.ch;
                hit->clong = channeldata.energy;
                hit->baseline = tpz_baseline;
                //! assign analog and digital probe
                hit->pulse_ap1 =channeldata.ap1_sample;
                hit->pulse_ap2 =channeldata.ap1_sample;
                hit->pulse_dp1 =channeldata.dp_sample;;
                hit->pulse_dp2 =channeldata.trg_sample;
                data->AddHit(hit);
                ProcessEvent(data);
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
    else{
        if(std::string(argv[3]) != "NOIMPLANT"){
        runNumInt = std::atoi(argv[3]);
        if( runNumInt > 26){
            N_MAX_WF_LENGTH = 360;
            trig_pos = 360*30/100;//unit of sample
            sampling_interval = 16;//unit of ns
        }

        std::cout << "RunNum: " << runNumInt <<std::endl;
        }
        plistopen(argv[1]);
    
    }
    phsave(argv[2]);
    prun();
    return 0;
}
