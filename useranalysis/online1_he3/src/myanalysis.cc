
/* ************************ DO NOT REMOVE  ****************************** */
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
#define N_MAX_WF_LENGTH 90
UShort_t trig_pos = N_MAX_WF_LENGTH*30/100;//unit of sample
UShort_t sampling_interval = 16*16;//unit of ns

TFile* file0 = 0;
TTree* tree = 0;
NIGIRI* data;

TH2F* hwf2d[V1740_N_MAX_CH*10];
TH1F* hrate;
TH1F* hdeadtime;
TH1F* hrateupdate;

TH2F* he2d;
TH2F* he2dpeak;

TH1F* he1d_clover[16];

int nevt = 0;

std::multimap <ULong64_t,UChar_t> datamap_hit1; //! sort by timestamp
std::multimap <ULong64_t,UChar_t> datamap_hit2; //! sort by timestamp
std::multimap<ULong64_t,UChar_t>::iterator it_datamap_hit1;
std::multimap<ULong64_t,UChar_t>::iterator it_datamap_hit2;

TH1F *hcorr;
TH1F *hcorrb;
TH1F *hcorrch;

TH2F* h2d_brikenhit;
TRandom rr;
void genRndCircle(Double_t &x,Double_t &y,Double_t a,Double_t b,Double_t xpos,Double_t ypos,Double_t R){
    if (b<a){
        Double_t temp=a;
        a=b;
        b=temp;
    }
    x=xpos+b*R*TMath::Cos(2*TMath::Pi()*a/b);
    y=ypos+b*R*TMath::Sin(2*TMath::Pi()*a/b);
}

Double_t fHe3Id2posX[140];
Double_t fHe3Id2posY[140];
Double_t fHe3Id2posZ[140];
Double_t fHe3Id2diameter[140];
Double_t fHe3Id2ring[140];
Double_t fHe3Id2length[140];
Int_t id_map[20][V1740_N_MAX_CH];
void ReadMapping(){
    for (Int_t i=0;i<MAX_N_BOARD;i++){
        for (Int_t j=0;j<V1740_N_MAX_CH;j++){
            id_map[i][j] = -1;
        }
    }
    std::ifstream inpf((char*)"He3_mapping.txt");
    if (inpf.fail()){
        cout<<"No BELEN Mapping file is given"<<endl;
        return;
    }
    cout<<"Start reading BELEN Mapping file: He3_mapping.txt"<<endl;

    Int_t id,index1,index2;
    UShort_t ring;
    Double_t x,y,z;
    Double_t d,length;
    Int_t mm=0;
    while (inpf.good()){
        inpf>>id>>index1>>index2>>d>>x>>y>>z>>ring>>length;
        if (id<=500){//for he3
            fHe3Id2posX[id]=x;
            fHe3Id2posY[id]=y;
            fHe3Id2posZ[id]=z;
            fHe3Id2diameter[id]=d;
            fHe3Id2ring[id]=ring;
            fHe3Id2length[id]=length;
            id_map[index1][index2] = id;
        }
        //cout<<He3id<<"-"<<daqId<<"-"<<d<<"-"<<x<<"-"<<y<<"-"<<z<<endl;
        mm++;
    }
    cout<<"Read "<<mm<<" line"<<endl;
    inpf.close();
}

void Init(){
    for (Int_t i=0;i<64*3;i++){
        hwf2d[i]=new TH2F(Form("hwf2d%d",i),Form("hwf2d%d",i),300,0,300,500,0,4000);

        if (i<16){
            he1d_clover[i]=new TH1F(Form("he1d_clover%d",i),Form("he1d_clover%d",i),1000,0,40000);
        }
    }

    h2d_brikenhit = new TH2F("h2d_brikenhit","h2d_brikenhit",400,-400,400,400,-400,400);
    ReadMapping();

    hdeadtime = new TH1F("hdeadtime","hdeadtime",64*3,0,64*3);
    hrate = new TH1F("hrate","hrate",64*3,0,64*3);
    hrateupdate = new TH1F("hrateupdate","hrateupdate",64*3,0,64*3);

    he2d = new TH2F("he2d","he2d",64*3,0,64*3,2000,0,4000);
    he2dpeak = new TH2F("he2dpeak","he2dpeak",64*3,0,64*3,2000,0,4000);

    hcorr = new TH1F("hcorr","hcorr",5000,-200000,200000);
    hcorrb = new TH1F("hcorrb","hcorrb",5000,-200000,200000);
    hcorrch = new TH1F("hcorrch","hcorrch",5000,-200000,200000);

}

void ProcessEvent(NIGIRI* data_now){
    if (data_now->b==9){
        //data_now->Print();

//        if (datamap_hit1.size()>3000){
//            for (it_datamap_hit1=datamap_hit1.begin();it_datamap_hit1!=datamap_hit1.end();it_datamap_hit1++){
//                Long64_t ts=(Long64_t)it_datamap_hit1->first;
//                Int_t b=(Int_t)it_datamap_hit1->second;
//                Long64_t corrts = 0;
//                Int_t corrb = 0;
//                Long64_t ts1 = ts - 200000;
//                ULong64_t ts2 = ts + 200000;
//                it_datamap_hit2 = datamap_hit2.lower_bound(ts1);
//                while(it_datamap_hit2!=datamap_hit2.end()&&it_datamap_hit2->first<ts2){
//                    corrts = (Long64_t) it_datamap_hit2->first;
//                    corrb=(Int_t)it_datamap_hit2->second;
//                    if (corrts!=ts) {
//                        hcorr->Fill(corrts-ts);
//                        if (corrb!=b) hcorrb->Fill(corrts-ts);
//                        else hcorrch->Fill(corrts-ts);
//                    }
//                    //break;
//                    it_datamap_hit2++;
//                }
//            }
//            //cout<<"Clear"<<endl;
//            datamap_hit1.clear();
//            datamap_hit2.clear();
//        }
//        datamap_hit1.insert(make_pair(data->ts,data_now->b));
//        datamap_hit2.insert(make_pair(data->ts,data_now->b));

        for (Int_t i=0;i<V1740_N_MAX_CH;i++){
            NIGIRIHit* hit=data_now->GetHit(i);
            Int_t ch = hit->ch+(data_now->b-9)*V1740_N_MAX_CH;
            Int_t itcnt= 0 ;
            for (std::vector<UShort_t>::iterator it =hit->pulse.begin() ; it != hit->pulse.end(); ++it){
                hwf2d[ch]->Fill(itcnt,*it);
                itcnt++;
            }
            if (hit->clong>200&&hit->clong<1600){
                if (hit->clong>100) hrateupdate->Fill(ch);
                he2d->Fill(ch,hit->clong);
                he2dpeak->Fill(ch,hit->clong+hit->baseline);

                //! hit distribution
                Int_t ID = id_map[data_now->b][i];
                if (ID>0){//draw 2d hist
                    Double_t fposX = fHe3Id2posX[ID];
                    Double_t fposY = fHe3Id2posY[ID];
                    Double_t fposZ = fHe3Id2posZ[ID];
                    //! pertubating
                    Double_t a,b,x,y,r;
                    r=fHe3Id2diameter[ID]/2;
                    a=rr.Rndm();
                    b=rr.Rndm();
                    genRndCircle(x,y,a,b,fposX,fposY,r);
                    fposX = x;
                    fposY = y;
                    fposZ = rr.Rndm()*fHe3Id2length[ID]+fposZ-fHe3Id2length[ID]/2;
                    if (hit->clong>100) h2d_brikenhit->Fill(x,y,TMath::Pi()*r*r);
                }
                if (datamap_hit1.size()>3000){
                    for (it_datamap_hit1=datamap_hit1.begin();it_datamap_hit1!=datamap_hit1.end();it_datamap_hit1++){
                        Long64_t ts=(Long64_t)it_datamap_hit1->first;
                        Int_t b=(Int_t)it_datamap_hit1->second;
                        Long64_t corrts = 0;
                        Int_t corrb = 0;
                        Long64_t ts1 = ts - 200000;
                        ULong64_t ts2 = ts + 200000;
                        it_datamap_hit2 = datamap_hit2.lower_bound(ts1);
                        while(it_datamap_hit2!=datamap_hit2.end()&&it_datamap_hit2->first<ts2){
                            corrts = (Long64_t) it_datamap_hit2->first;
                            corrb=(Int_t)it_datamap_hit2->second;
                            if (corrts!=ts) {
                                hcorr->Fill(corrts-ts);
                                if (corrb!=b) hcorrb->Fill(corrts-ts);
                                else hcorrch->Fill(corrts-ts);
                            }
                            //break;
                            it_datamap_hit2++;
                        }
                    }
                    //cout<<"Clear"<<endl;
                    datamap_hit1.clear();
                    datamap_hit2.clear();
                }
                datamap_hit1.insert(make_pair(hit->ts,data_now->b));
                datamap_hit2.insert(make_pair(hit->ts,data_now->b));




            }//if hit->clong>0;
        }
    }
}

void DoUpdate(){
    for (Int_t i=0;i<hrateupdate->GetNbinsX();i++){
        hrate->SetBinContent(i+1,hrateupdate->GetBinContent(i+1)/RATE_CAL_REFESH_SECONDS);
    }
    hrateupdate->Reset();
    //pstatus();
}

void OpenFile(const char* filename){
    file0 = new TFile(filename,"recreate");
    tree = new TTree("tree","tree");
}

void CloseMe(){
    if (file0) {
        if (tree) tree->Write();
        file0->Close();
    }
    file0=new TFile("output.root","recreate");
    cout<<nevt<<endl;
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


//! full map
//#define N_PACKETMAP 14
//const int packetmap[]={49,50,51,52,53,54,55,56,57,58,59,60,61,100};
//const pmap_decode packetdecode[]={LUPO,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,V1730DPPPHA};

#define N_PACKETMAP 14
const int packetmap[]={49,50,51,52,53,54,55,56,57,58,59,60,61,100};
const pmap_decode packetdecode[]={NONE,V1740ZSP,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,V1740ZSP,V1740ZSP,V1740ZSP,NONE};

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
                //cout<<i<<"bbb"<<nsample<<endl;
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
