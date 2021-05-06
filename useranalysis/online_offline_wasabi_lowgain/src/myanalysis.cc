
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
#define N_MAX_WF_LENGTH 360
#define MAX_MAP_LENGTH 100

#define N_DSSD 4
#define N_STRIP_X 32

#define N_DE 4

#define N_PL 4

#define N_V1740 5

UShort_t trig_pos = N_MAX_WF_LENGTH*10/100;//unit of sample
UShort_t sampling_interval = 16;//unit of ns


NIGIRI* data;

std::multimap <ULong64_t,NIGIRI*> datamap_dgtz[N_V1740]; //! sort by timestamp
std::multimap<ULong64_t,NIGIRI*>::iterator it_datamap_dgtz[N_V1740];


//! trees
TFile* fout;
//!tredata
TTree* tree;
Int_t evtcnt;
Int_t evt[N_V1740];
Long64_t dgtz_ts[N_V1740];
UShort_t dgtz_nsample[V1740_N_MAX_CH*N_V1740];
Double_t dgtz_e[V1740_N_MAX_CH*N_V1740];
Double_t dgtz_bl[V1740_N_MAX_CH*N_V1740];
Double_t dgtz_t[V1740_N_MAX_CH*N_V1740];
Int_t dgtz_ch[V1740_N_MAX_CH*N_V1740];
UShort_t dgtz_sample[V1740_N_MAX_CH*N_V1740][N_MAX_WF_LENGTH];
UShort_t dgtz_waveform[V1740_N_MAX_CH*N_V1740][N_MAX_WF_LENGTH];

//! reconstructed tree data
TTree* treereco;
Int_t evtd;
Long64_t evtts;

Double_t x[N_DSSD];
Double_t y[N_DSSD];
Double_t ex[N_DSSD];
Double_t ey[N_DSSD];

Double_t de_e[N_DE];
Int_t de_z[N_DE];
Double_t pl_e[N_PL];
Int_t pl_ch[N_PL];
Int_t de_maxz;

Int_t max_z;
Int_t max_z_x;
Int_t max_z_y;
Double_t           dssd_ex[N_DSSD][N_STRIP_X];
Double_t           dssd_exbl[N_DSSD][N_STRIP_X];
Double_t           dssd_tx[N_DSSD][N_STRIP_X];
Int_t           dssd_mutlx[N_DSSD];

Long64_t           dssd_ts_x[N_DSSD];
Long64_t           dssd_ts_y[N_DSSD];

Double_t           dssd_ey[N_DSSD][N_STRIP_X];
Double_t           dssd_eybl[N_DSSD][N_STRIP_X];
Double_t           dssd_ty[N_DSSD][N_STRIP_X];
Int_t           dssd_mutly[N_DSSD];

Int_t ntrigger[N_V1740];
Int_t ncorrtrigger[N_V1740];

//! Online histograms
TH1F* h_max_z;
TH2F* h_xy[N_DSSD];
TH1F* h_dssdrate[N_DSSD];
Long64_t dssdrate_tstart[N_DSSD];
Bool_t is_dssdrate_tstart[N_DSSD];

TCanvas* c1;
TCanvas* c2;

#define TS_WIN_LOW 100
#define TS_WIN_HIGH 100

void Init(){
    fout = 0;
    //treereco = 0;
    tree = 0;

    evtcnt = 0;
    for (int i=0;i<N_V1740;i++){
        dgtz_ts[i]=0;
        evt[i]=0;
        ntrigger[i]=0;
        ncorrtrigger[i]=0;
    }
    for (int i=0;i<V1740_N_MAX_CH*N_V1740;i++){
        dgtz_nsample[i]=-9999;
        dgtz_e[i]=-9999;
        dgtz_bl[i]=-9999;
        dgtz_t[i] = 0;
        dgtz_ch[i]=-9999;
        for (int j=0;j<N_MAX_WF_LENGTH;j++){
            dgtz_sample[i][j]=-9999;
            dgtz_waveform[i][j]=-9999;
        }
    }

    for (Int_t i=0;i<N_DE;i++){
        de_e[i] = -9999;
        de_z[i] = i;
    }

    for (Int_t i=0;i<N_PL;i++){
        pl_e[i] = -9999;
    }

    de_maxz = -1;

    evtd = 0;
    evtts = 0;
    max_z = -1;
    max_z_y = -1;
    max_z_x = -1;
    for (int i=0;i<N_DSSD;i++){
        dssd_ts_x[i] = 0;
        dssd_ts_y[i] = 0;
        x[i] = -1;
        y[i] = -1;
        ex[i] = -1;
        ey[i] = -1;
        dssd_mutlx[i]=-9999;
        dssd_mutly[i]=-9999;
        for (int j=0;j<N_STRIP_X;j++){
            dssd_ex[i][j]=-9999;
            dssd_exbl[i][j]=-9999;
            dssd_tx[i][j]=-9999;

            dssd_ey[i][j]=-9999;
            dssd_eybl[i][j]=-9999;
            dssd_ty[i][j]=-9999;
        }
    }

    //! histograms
    h_max_z = new TH1F("h_max_z","h_max_z",9,-5,4);
    for (Int_t i=0;i<N_DSSD;i++){
        h_xy[i]=new TH2F(Form("h_xy%i",i),Form("h_xy_implant%i",i),32,0,32,32,0,32);
        h_dssdrate[i]=new TH1F(Form("h_dssdrate%i",i),Form("h_dssd_implant_rate%i",i),4000,0,4000);
        dssdrate_tstart[i] = 0;
        is_dssdrate_tstart[i]=false;
    }

    //! Canvas draw
    if (fout==0){//Online mode
        c1 = new TCanvas("c1","c1",900,700);
        c1->Divide(3,2);
        c1->cd(1);
        h_max_z->Draw();
        for (Int_t i=0;i<N_DSSD;i++){
            c1->cd(i+2);
            h_xy[i]->Draw("colz");
        }
        c2 = new TCanvas("c2","c2",900,700);
        c2->Divide(2,2);
        for (Int_t i=0;i<N_DSSD;i++){
            c2->cd(i+1);
            h_dssdrate[i]->Draw();
        }


        pupdate(c1,3);
        pupdate(c2,3);
    }

//    tree = new TTree("tree","tree");
//    tree->Branch("evt",&evt,Form("evt[%d]/I",N_V1740));
//    tree->Branch("dgtz_ts",dgtz_ts,Form("dgtz_ts[%d]/L",N_V1740));
//    tree->Branch("dgtz_e",dgtz_e,Form("dgtz_e[%d]/D",V1740_N_MAX_CH*N_V1740));
//    tree->Branch("dgtz_bl",dgtz_bl,Form("dgtz_bl[%d]/D",V1740_N_MAX_CH*N_V1740));
//    tree->Branch("dgtz_t",dgtz_t,Form("dgtz_t[%d]/D",V1740_N_MAX_CH*N_V1740));
//    tree->Branch("dgtz_ch",dgtz_ch,Form("dgtz_ch[%d]/I",V1740_N_MAX_CH*N_V1740));
//    tree->Branch("dgtz_nsample",dgtz_nsample,Form("dgtz_nsample[%d]/s",V1740_N_MAX_CH*N_V1740));
//    tree->Branch("dgtz_waveform",dgtz_waveform,Form("dgtz_waveform[%d][%d]/s",V1740_N_MAX_CH*N_V1740,N_MAX_WF_LENGTH));
//    tree->Branch("dgtz_sample",dgtz_sample,Form("dgtz_sample[%d][%d]/s",V1740_N_MAX_CH*N_V1740,N_MAX_WF_LENGTH));


//    tree->Branch("evtd",&evtd,"evtd/I");
//    tree->Branch("evtts",&evtts,"evtts/L");

//    tree->Branch("de_e",de_e,Form("de_e[%d]/D",N_DE));
//    tree->Branch("de_z",de_z,Form("de_z[%d]/I",N_DE));
//    tree->Branch("de_maxz",&de_maxz,"de_maxz/I");

//    tree->Branch("pl_e",pl_e,Form("pl_e[%d]/D",N_PL));

//    tree->Branch("x",x,Form("x[%d]/D",N_DSSD));
//    tree->Branch("y",y,Form("y[%d]/D",N_DSSD));
//    tree->Branch("ex",ex,Form("ex[%d]/D",N_DSSD));
//    tree->Branch("ey",ey,Form("ey[%d]/D",N_DSSD));

//    tree->Branch("dssd_ts_x",dssd_ts_x,Form("dssd_ts_x[%d]/L",N_DSSD));
//    tree->Branch("dssd_ts_y",dssd_ts_y,Form("dssd_ts_y[%d]/L",N_DSSD));

//    tree->Branch("max_z",&max_z,"max_z/I");
//    tree->Branch("max_z_x",&max_z_x,"max_z_x/I");
//    tree->Branch("max_z_y",&max_z_y,"max_z_y/I");

//    tree->Branch("dssd_ex",dssd_ex,Form("dssd_ex[%d][%d]/s",N_DSSD,N_STRIP_X));
//    tree->Branch("dssd_exbl",dssd_exbl,Form("dssd_exbl[%d][%d]/D",N_DSSD,N_STRIP_X));
//    tree->Branch("dssd_tx",dssd_tx,Form("dssd_tx[%d][%d]/D",N_DSSD,N_STRIP_X));
//    tree->Branch("dssd_mutlx",dssd_mutlx,Form("dssd_mutlx[%d]/D",N_DSSD));

//    tree->Branch("dssd_ey",dssd_ey,Form("dssd_ey[%d][%d]/D",N_DSSD,N_STRIP_X));
//    tree->Branch("dssd_eybl",dssd_eybl,Form("dssd_eybl[%d][%d]/D",N_DSSD,N_STRIP_X));
//    tree->Branch("dssd_ty",dssd_ty,Form("dssd_ty[%d][%d]/D",N_DSSD,N_STRIP_X));
//    tree->Branch("dssd_mutly",dssd_mutly,Form("dssd_mutly[%d]/D",N_DSSD));
}

void OpenFile(const char* filename){
    fout = new TFile(filename,"RECREATE");
    tree = new TTree("tree","tree");
    tree->Branch("evt",&evt,Form("evt[%d]/I",N_V1740));
    tree->Branch("dgtz_ts",dgtz_ts,Form("dgtz_ts[%d]/L",N_V1740));
    tree->Branch("dgtz_e",dgtz_e,Form("dgtz_e[%d]/D",V1740_N_MAX_CH*N_V1740));
    tree->Branch("dgtz_bl",dgtz_bl,Form("dgtz_bl[%d]/D",V1740_N_MAX_CH*N_V1740));
    tree->Branch("dgtz_t",dgtz_t,Form("dgtz_t[%d]/D",V1740_N_MAX_CH*N_V1740));
    tree->Branch("dgtz_ch",dgtz_ch,Form("dgtz_ch[%d]/I",V1740_N_MAX_CH*N_V1740));
    tree->Branch("dgtz_nsample",dgtz_nsample,Form("dgtz_nsample[%d]/s",V1740_N_MAX_CH*N_V1740));
    tree->Branch("dgtz_waveform",dgtz_waveform,Form("dgtz_waveform[%d][%d]/s",V1740_N_MAX_CH*N_V1740,N_MAX_WF_LENGTH));
    tree->Branch("dgtz_sample",dgtz_sample,Form("dgtz_sample[%d][%d]/s",V1740_N_MAX_CH*N_V1740,N_MAX_WF_LENGTH));


    tree->Branch("evtd",&evtd,"evtd/I");
    tree->Branch("evtts",&evtts,"evtts/L");

    tree->Branch("de_e",de_e,Form("de_e[%d]/D",N_DE));
    tree->Branch("de_z",de_z,Form("de_z[%d]/I",N_DE));
    tree->Branch("de_maxz",&de_maxz,"de_maxz/I");

    tree->Branch("pl_e",pl_e,Form("pl_e[%d]/D",N_PL));

    tree->Branch("x",x,Form("x[%d]/D",N_DSSD));
    tree->Branch("y",y,Form("y[%d]/D",N_DSSD));
    tree->Branch("ex",ex,Form("ex[%d]/D",N_DSSD));
    tree->Branch("ey",ey,Form("ey[%d]/D",N_DSSD));

    tree->Branch("dssd_ts_x",dssd_ts_x,Form("dssd_ts_x[%d]/L",N_DSSD));
    tree->Branch("dssd_ts_y",dssd_ts_y,Form("dssd_ts_y[%d]/L",N_DSSD));

    tree->Branch("max_z",&max_z,"max_z/I");
    tree->Branch("max_z_x",&max_z_x,"max_z_x/I");
    tree->Branch("max_z_y",&max_z_y,"max_z_y/I");

    tree->Branch("dssd_ex",dssd_ex,Form("dssd_ex[%d][%d]/s",N_DSSD,N_STRIP_X));
    tree->Branch("dssd_exbl",dssd_exbl,Form("dssd_exbl[%d][%d]/D",N_DSSD,N_STRIP_X));
    tree->Branch("dssd_tx",dssd_tx,Form("dssd_tx[%d][%d]/D",N_DSSD,N_STRIP_X));
    tree->Branch("dssd_mutlx",dssd_mutlx,Form("dssd_mutlx[%d]/D",N_DSSD));

    tree->Branch("dssd_ey",dssd_ey,Form("dssd_ey[%d][%d]/D",N_DSSD,N_STRIP_X));
    tree->Branch("dssd_eybl",dssd_eybl,Form("dssd_eybl[%d][%d]/D",N_DSSD,N_STRIP_X));
    tree->Branch("dssd_ty",dssd_ty,Form("dssd_ty[%d][%d]/D",N_DSSD,N_STRIP_X));
    tree->Branch("dssd_mutly",dssd_mutly,Form("dssd_mutly[%d]/D",N_DSSD));
}

void CloseEvent(){
    //! Reco step 1
    evtd++;
    evtts = dgtz_ts[0];
    Int_t de_map[]={282,279,280,281};

    max_z = -N_DE-1;
    for (int i=0;i<N_DE;i++){
        de_e[i]=dgtz_e[de_map[i]];
        if (de_e[i]>100) {
            de_maxz = i;
            max_z = i-N_DE;
        }
    }

    Int_t pl_map[]={284,285,286,287};
    for (int i=0;i<N_PL;i++){
        pl_e[i]=dgtz_e[pl_map[i]];
    }

    Double_t ts_now[4];
    for (int i=0;i<N_DSSD;i++){
        ts_now[i]=0;
        dssd_mutlx[i]=0;
        dssd_mutly[i]=0;
    }

    for (int i=0;i<V1740_N_MAX_CH*4;i++){
        if (i==221||i==136) continue; //ignore this two strip
        Int_t z = i/(N_STRIP_X*2);

        Int_t xy = i%(N_STRIP_X*2);
        if (xy<N_STRIP_X){//x strips
            dssd_ex[z][xy] = 360-dgtz_t[i];
            dssd_tx[z][xy] = dgtz_t[i];
            dssd_exbl[z][xy] = dgtz_e[i]+dgtz_bl[i];
            if (dssd_ex[z][xy]<355){
                dssd_mutlx[z]++;
            }
            //if (dssd_ex[z][xy]>ex[z]){
            if (dssd_ex[z][xy]>ex[z]&&dssd_ex[z][xy]<355&&dssd_ex[z][xy]>250){
            //if (dssd_ex[z][xy]>ex[z]&&dssd_ex[z][xy]<355){
                ex[z] = dssd_ex[z][xy];
                x[z] = xy;
                max_z = z;
                max_z_x = z;
                dssd_ts_x[z] = dgtz_ts[z];
                if (!is_dssdrate_tstart[z]){
                    dssdrate_tstart[z] = dgtz_ts[z];
                    is_dssdrate_tstart[z]=true;
                }else{
                    ts_now[z] = (Double_t)(dgtz_ts[z] - dssdrate_tstart[z])/1e9;
                }
            }
        }else{//y strips
            dssd_ey[z][xy-N_STRIP_X] = 360-dgtz_t[i];
            dssd_ty[z][xy-N_STRIP_X] = dgtz_t[i];
            dssd_eybl[z][xy-N_STRIP_X] = dgtz_e[i]+dgtz_bl[i];

            if (dssd_ex[z][xy]<355){
                dssd_mutly[z]++;
            }
            //if (dssd_ey[z][xy-N_STRIP_X]>ey[z]){
            if (dssd_ey[z][xy-N_STRIP_X]>ey[z]&&dssd_ey[z][xy-N_STRIP_X]<355&&dssd_ey[z][xy-N_STRIP_X]>250){
            //if (dssd_ey[z][xy-N_STRIP_X]>ey[z]&&dssd_ey[z][xy-N_STRIP_X]<355){
                ey[z] = dssd_ey[z][xy-N_STRIP_X];
                y[z] = xy-N_STRIP_X;
                max_z = z;
                max_z_y = z;
                dssd_ts_y[z] = dgtz_ts[z];
            }
        }
    }
    if (max_z>=0){
        h_xy[max_z]->Fill(x[max_z],y[max_z]);
        if (ts_now[max_z]>0)
            h_dssdrate[max_z]->Fill(ts_now[max_z]);
        h_max_z->Fill(max_z);
    }
//    if (max_z>=0){
//        if (max_z==3&&pl_e[2]<10){
//            h_xy[max_z]->Fill(x[max_z],y[max_z]);
//            if (ts_now[max_z]>0)
//                h_dssdrate[max_z]->Fill(ts_now[max_z]);
//            h_max_z->Fill(max_z);
//        }
//        if (max_z<3){
//            h_xy[max_z]->Fill(x[max_z],y[max_z]);
//            if (ts_now[max_z]>0)
//                h_dssdrate[max_z]->Fill(ts_now[max_z]);
//            h_max_z->Fill(max_z);
//        }
//    }else{
//        h_max_z->Fill(max_z);
//    }


    if (tree!=0) tree->Fill();
    //if (treereco!=0) treereco->Fill();
    //! reset default value
    max_z = -1;
    max_z_y = -1;
    max_z_x = -1;

    for (Int_t i=0;i<N_DE;i++){
        de_e[i] = -9999;
        de_z[i] = i;
    }

    for (Int_t i=0;i<N_PL;i++){
        pl_e[i] = -9999;
    }

    de_maxz = -1;

    for (int i=0;i<N_DSSD;i++){
        dssd_ts_x[i] = 0;
        dssd_ts_y[i] = 0;
        x[i] = -1;
        y[i] = -1;
        ex[i] = -1;
        ey[i] = -1;
        dssd_mutlx[i]=-9999;
        dssd_mutly[i]=-9999;
        for (int j=0;j<N_STRIP_X;j++){
            dssd_ex[i][j]=-9999;
            dssd_exbl[i][j]=-9999;
            dssd_tx[i][j]=-9999;

            dssd_ey[i][j]=-9999;
            dssd_eybl[i][j]=-9999;
            dssd_ty[i][j]=-9999;
        }
    }
    //! reset default value
    for (int i=0;i<N_V1740;i++){
        dgtz_ts[i]=0;
        evt[i]=0;
    }
    for (int i=0;i<V1740_N_MAX_CH*N_V1740;i++){
        dgtz_nsample[i]=-9999;
        dgtz_e[i]=-9999;
        dgtz_bl[i]=-9999;
        dgtz_ch[i]=-9999;
        for (int j=0;j<N_MAX_WF_LENGTH;j++){
            dgtz_sample[i][j]=-9999;
            dgtz_waveform[i][j]=-9999;
        }
    }
}

void DoSort(bool flagend,NIGIRI*data=0){
    if (datamap_dgtz[0].size()>MAX_MAP_LENGTH||flagend){
        for(it_datamap_dgtz[0]=datamap_dgtz[0].begin();it_datamap_dgtz[0]!=datamap_dgtz[0].end();it_datamap_dgtz[0]++){
            Long64_t ts=(Long64_t)it_datamap_dgtz[0]->first;
            NIGIRI* hit0=(NIGIRI*)it_datamap_dgtz[0]->second;
            Long64_t ts1 = ts - TS_WIN_LOW;
            Long64_t ts2 = ts + TS_WIN_HIGH;
            Long64_t corrts = 0;
            for (Int_t ii=1;ii<N_V1740;ii++){
                if (ii==4){
                    ts1 = ts + 0;
                    ts2 = ts + 10000;
                }
                it_datamap_dgtz[ii] = datamap_dgtz[ii].lower_bound(ts1);
                while(it_datamap_dgtz[ii]!=datamap_dgtz[ii].end()&&it_datamap_dgtz[ii]->first<ts2){
                    corrts = (Long64_t) it_datamap_dgtz[ii]->first;
                    NIGIRI* hit1=(NIGIRI*)it_datamap_dgtz[ii]->second;
                    //! fill data for dgtz 1-3 here
                    dgtz_ts[ii]=corrts;
                    evt[ii]=evtcnt;
                    ncorrtrigger[ii]++;
                    for (Int_t i=0;i<hit1->GetMult();i++){
                        NIGIRIHit* hittemp=hit1->GetHit(i);
                        int ch=hittemp->ch+V1740_N_MAX_CH*ii;
                        dgtz_nsample[ch]=hittemp->nsample;
                        dgtz_e[ch]=hittemp->clong;
                        dgtz_t[ch]=hittemp->cshort;
                        dgtz_bl[ch]=hittemp->baseline;
                        dgtz_ch[ch]=ch;
                        int intcnt=0;
                        for (std::vector<UShort_t>::iterator it = hittemp->pulse.begin() ; it != hittemp->pulse.end(); ++it){
                            if(intcnt<N_MAX_WF_LENGTH){
                                dgtz_waveform[ch][intcnt]=*it;
                                dgtz_sample[ch][intcnt]=intcnt;
                            }
                            intcnt++;
                        }
                    }

                    if (!flagend) {//clear event here
                        std::multimap<ULong64_t,NIGIRI*>::iterator it_datamap_dgtz1p;
                        it_datamap_dgtz1p=it_datamap_dgtz[ii];
                        it_datamap_dgtz1p++;
                        for (std::multimap<ULong64_t,NIGIRI*>::iterator it_datamaptmp=datamap_dgtz[ii].begin();it_datamaptmp!=it_datamap_dgtz1p;it_datamaptmp++){
                            NIGIRI* hittmp=it_datamaptmp->second;
                            hittmp->Clear();
                            delete hittmp;
                        }
                        datamap_dgtz[ii].erase(datamap_dgtz[ii].begin(),it_datamap_dgtz1p);
                    }
                    break; //only find first ts match
                }// end while loop
            }// end loop all digitizer
            //!fill data for digitizer 0 here
            dgtz_ts[0]=ts;
            evt[0] = evtcnt;
            ncorrtrigger[0]++;
            for (Int_t i=0;i<hit0->GetMult();i++){
                NIGIRIHit* hittemp=hit0->GetHit(i);
                int ch=hittemp->ch;
                dgtz_nsample[ch]=hittemp->nsample;
                dgtz_e[ch]=hittemp->clong;
                dgtz_t[ch]=hittemp->cshort;
                dgtz_bl[ch]=hittemp->baseline;
                dgtz_ch[ch]=ch;
                int intcnt=0;
                for (std::vector<UShort_t>::iterator it = hittemp->pulse.begin() ; it != hittemp->pulse.end(); ++it){
                    if(intcnt<N_MAX_WF_LENGTH){
                        dgtz_waveform[ch][intcnt]=*it;
                        dgtz_sample[ch][intcnt]=intcnt;
                    }
                    intcnt++;
                }
            }

            //! Close event here
            CloseEvent();
            //! clear digitizer 1 event
            if (!flagend) {
                hit0->Clear();
                delete hit0;
                datamap_dgtz[0].erase(datamap_dgtz[0].begin(),++it_datamap_dgtz[0]);
                break;
            }
            evtcnt++;
        }// for loop all events in digitizer 0
    }//! max map length reached

    //! Fill all data here
    if (!flagend){
        NIGIRI* datac=new NIGIRI;
        data->Copy(*datac);
        if (data->b<8){
            datamap_dgtz[data->b-4].insert(make_pair(data->ts,datac));
            ntrigger[data->b-4]++;
        }else{//dE
            datamap_dgtz[4].insert(make_pair(data->ts,datac));
            ntrigger[4]++;
        }
    }
}

void ProcessEvent(NIGIRI* data_now){
    if ((data_now->b<8&&data_now->b>=4)||data_now->b==11){
        DoSort(false,data);
    }
}

void CloseMe(){
    DoSort(true);

    if (fout==0){
        time_t t=time(0);
        struct tm * now = localtime(&t);
        char tbuff[100];
        strftime(tbuff,100,"%y%m%d_%H%M",now);
        fout = new TFile(Form("online_%s.root",tbuff),"recreate");
    }

    if (tree!=0) tree->Write();
    //if (treereco!=0) treereco->Write();

    for (Int_t i=0;i<N_DSSD;i++){
        h_dssdrate[i]->Write();
        h_xy[i]->Write();
    }
    h_max_z->Write();
    fout->Close();

    cout<<"Close me!"<<endl;
    for (int i=0;i<N_V1740;i++){
        cout<<"Total/Correlated = "<<ntrigger[i]<<" / "<<ncorrtrigger[i]<<endl;
    }
}

void DoUpdate(){
//    if (tree!=0)
//        pstatus();
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

//! full map
#define N_PACKETMAP 14
const int packetmap[]={49,50,51,52,53,54,55,56,57,58,59,60,61,100};
const pmap_decode packetdecode[]={NONE,NONE,NONE,NONE,NONE,V1740ZSP,V1740ZSP,V1740ZSP,V1740ZSP,NONE,NONE,NONE,V1740ZSP,NONE};

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

        ProcessEvent(data);
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
