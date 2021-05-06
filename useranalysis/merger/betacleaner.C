#define bigrips_cxx
#include "bigwasabi.h"
#include "wasabi_highgain.h"
#include "BRIKENTree.h"
#include <TH1.h>
#include <TH2.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TStyle.h>
#include <TTree.h>
#include <TFile.h>
#include <TCanvas.h>

#include <iostream>
#include <map>

#define DSSD 0


#define LIGHTPART_TSWINDOW_UP 1000
#define LIGHTPART_TSWINDOW_LOW 1000

#define IMPLANT_NOISE_TSWINDOW_UP 500000
#define IMPLANT_NOISE_TSWINDOW_LOW 500000


#define BETAGAMMA_TS_WIN_LOW 20000
#define BETAGAMMA_TS_WIN_UP 20000

#define TS_WIN_LOW 30000000000
#define TS_WIN_UP 5000000000


using namespace std;

typedef struct wasabi_str{
    Long64_t jentry;
    Double_t x;
    Double_t y;
    Short_t z;
    Double_t Ex;
    Double_t Ey;
    Double_t zet,aoq,de0,de1,de2,de3,beta;
    Bool_t flag_goodevt;
}wasabi_t;


typedef struct clover_str{
    Long64_t jentry;
    Int_t ch;
    Double_t e;
}clover_t;

void ClearWasabiStr(wasabi_t* sin){
    sin->jentry=-9999;
    sin->x=-9999;
    sin->y=-9999;
    sin->z=-9999;
}


const Int_t kMaxGamma = 200;
typedef struct{
    ULong64_t evt, ts;
    Double_t t,x,y,z,ex,ey,ion_x,ion_y,ion_z,ion_ex,ion_ey,zet,aoq,beta,de0,de1,de2,de3;
    Int_t gc_hit;
    Double_t gc_e[kMaxGamma];
    Double_t gc_t[kMaxGamma];
    Int_t gc_ch[kMaxGamma];
} decaydata_t;


std::multimap <Long64_t,wasabi_t*> datamap_hit1; //! sort by timestamp
std::multimap <Long64_t,wasabi_t*> datamap_hit2; //! sort by timestamp
std::multimap<Long64_t,wasabi_t*>::iterator it_datamap_hit1;
std::multimap<Long64_t,wasabi_t*>::iterator it_datamap_hit2;

std::multimap <Long64_t,wasabi_t*> datamap_hit3; //! sort by timestamp
std::multimap<Long64_t,wasabi_t*>::iterator it_datamap_hit3;

std::multimap <Long64_t,wasabi_t*> datamap_hit4; //! sort by timestamp
std::multimap<Long64_t,wasabi_t*>::iterator it_datamap_hit4;

std::multimap <Long64_t,clover_t*> datamap_hit5; //! sort by timestamp
std::multimap<Long64_t,clover_t*>::iterator it_datamap_hit5;

void betacleaner(char* bigwasabifile, char* wasabi_highgainfile, char* brikenfile, char* outfile)
{
    bigwasabi* bigwasabiin = new bigwasabi(bigwasabifile);
    wasabi_highgain* wasabi_highgainin = new wasabi_highgain(wasabi_highgainfile);
    BRIKENTree* brikenin = new BRIKENTree(brikenfile);
    Long64_t wasabi_highgainentries = wasabi_highgainin->GetEntries();
    Long64_t bigwasbientries = bigwasabiin->GetEntries();
    Long64_t brikenentries = brikenin->GetEntries();

    TFile* fout = new TFile(outfile,"RECREATE");
    TTree* treedata = new TTree("tree","tree");
    decaydata_t decay;
    treedata->Branch("decay",&decay,"evt/l:ts/l:t/D:x/D:y/D:z/D:ion_x/D:ion_y/D:ion_z/D:ex/D:ey/D:ion_ex/D:ion_ey/D:zet/D:aoq/D:beta/D:de0/D:de1/D:de2/D:de3/D");
    treedata->Branch("gc_hit",&decay.gc_hit,"gc_hit/I");
    treedata->Branch("gc_e",&decay.gc_e,"gc_e[gc_hit]/D");
    treedata->Branch("gc_t",&decay.gc_t,"gc_t[gc_hit]/D");
    treedata->Branch("gc_ch",&decay.gc_ch,"gc_ch[gc_hit]/I");

    //! histograms
    TH1F* htcorr = new TH1F("htcorr","htcorr",5000,-TS_WIN_LOW,TS_WIN_UP);
    TH2F* htecorr = new TH2F("htecorr","htecorr",5000,-TS_WIN_LOW,TS_WIN_UP,500,0,4000);
    TH1F* hdx = new TH1F("hdx","hdx",64,-32,32);
    TH1F* hdy = new TH1F("hdy","hdy",64,-32,32);
    TH2F* hexey = new TH2F("hexey","hexey",500,0,4000,500,0,5000);
    TH2F* hexeyclean = new TH2F("hexeyclean","hexeyclean",500,0,4000,500,0,5000);
    TH2F* hxy = new TH2F("hxy","hxy",32,0,32,32,0,32);
    TH2F* hxyclean = new TH2F("hxyclean","hxyclean",32,0,32,32,0,32);
    TH2F* hxyimplant = new TH2F("hxyimplant","hxyimplant",32,0,32,32,0,32);


    cout<<"reading wasabi_highgain"<<endl;
    for (Long64_t jentry=0; jentry<wasabi_highgainentries;jentry++) {
        wasabi_highgainin->GetEntry(jentry);
        if (wasabi_highgainin->z==0&&1.13*wasabi_highgainin->ey-wasabi_highgainin->ex<200&&
                1.13*wasabi_highgainin->ey-wasabi_highgainin->ex>-200&&wasabi_highgainin->ex>100&&wasabi_highgainin->ey>100){
            wasabi_t* str=new wasabi_t;
            str->jentry = jentry;
            str->x = wasabi_highgainin->x;
            str->y = wasabi_highgainin->y;
            str->z = wasabi_highgainin->z;
            str->Ex = wasabi_highgainin->ex;
            str->Ey = wasabi_highgainin->ey;
            hexey->Fill(str->Ex,str->Ey);
            hxy->Fill(str->x,str->y);
            str->flag_goodevt = true;
            datamap_hit1.insert(make_pair((Long64_t)wasabi_highgainin->dgtz_ts,str));
        }

        if (wasabi_highgainin->z==2){
            if (wasabi_highgainin->ex>100&&wasabi_highgainin->ey>100){
                wasabi_t* str=new wasabi_t;
                str->jentry = jentry;
                str->x = wasabi_highgainin->x;
                str->y = wasabi_highgainin->y;
                str->z = wasabi_highgainin->z;
                str->Ex = wasabi_highgainin->ex;
                str->Ey = wasabi_highgainin->ey;
                str->flag_goodevt = true;
                datamap_hit2.insert(make_pair((Long64_t)wasabi_highgainin->dgtz_ts,str));
            }
        }
    }

    cout<<"reading bigwasabi"<<endl;
    for (Long64_t jentry=0; jentry<bigwasbientries;jentry++) {
        bigwasabiin->GetEntry(jentry);
        wasabi_t* str=new wasabi_t;
        str->jentry = jentry;
        str->x = bigwasabiin->x[DSSD];
        str->y = bigwasabiin->y[DSSD];
        str->z = DSSD;
        str->zet = bigwasabiin->zet;
        str->aoq = bigwasabiin->aoq;
        str->de0 = bigwasabiin->de_e[0];
        str->de1 = bigwasabiin->de_e[1];
        str->de2 = bigwasabiin->de_e[2];
        str->de3 = bigwasabiin->de_e[3];
        str->beta = bigwasabiin->beta;

        if (bigwasabiin->zet>46){
            str->flag_goodevt = true;
        }else{
            str->flag_goodevt = false;
        }
        datamap_hit3.insert(make_pair((Long64_t)bigwasabiin->evtts,str));

        if (bigwasabiin->max_z==0&&bigwasabiin->zet>46){
            hxyimplant->Fill(str->x,str->y);
            datamap_hit4.insert(make_pair((Long64_t)bigwasabiin->evtts,str));
        }
    }

    cout<<"reading briken"<<endl;
    for (Long64_t jentry=0; jentry<bigwasbientries;jentry++) {
        brikenin->GetEntry(jentry);
        if (brikenin->Gamma_T>0){
            clover_t* str=new clover_t;
            str->jentry = jentry;
            str->e=brikenin->Gamma_E;
            str->ch=brikenin->Gamma_Id;
            datamap_hit5.insert(make_pair((Long64_t)brikenin->Gamma_T,str));
        }

    }
    cout<<datamap_hit1.size()<<"\t"<<datamap_hit2.size()<<"\t"<<datamap_hit3.size()<<"\t"<<datamap_hit4.size()<<"\t"<<datamap_hit5.size()<<endl;

    Long64_t nevt = 0;
    for (it_datamap_hit1=datamap_hit1.begin();it_datamap_hit1!=datamap_hit1.end();it_datamap_hit1++){
        Long64_t ts=(Long64_t)it_datamap_hit1->first;
        wasabi_t* entry=(wasabi_t*)it_datamap_hit1->second;//brips
        Long64_t corrts = 0;
        Long64_t correntry = 0;
        Long64_t ts1 = ts - LIGHTPART_TSWINDOW_LOW;
        Long64_t ts2 = ts + LIGHTPART_TSWINDOW_UP;
        it_datamap_hit2 = datamap_hit2.lower_bound(ts1);
        while(it_datamap_hit2!=datamap_hit2.end()&&it_datamap_hit2->first<ts2){
            corrts = (Long64_t) it_datamap_hit2->first;
            wasabi_t* correntry=(wasabi_t*)it_datamap_hit2->second;//wasabi
            if (corrts==ts){
                it_datamap_hit2++;
                continue;
            }
            //htcorr->Fill(corrts-ts);
            entry->flag_goodevt = false;
            it_datamap_hit2++;
        }

        corrts = 0;
        correntry = 0;
        ts1 = ts - IMPLANT_NOISE_TSWINDOW_LOW;
        ts2 = ts + IMPLANT_NOISE_TSWINDOW_UP;
        it_datamap_hit3 = datamap_hit3.lower_bound(ts1);
        while(it_datamap_hit3!=datamap_hit3.end()&&it_datamap_hit3->first<ts2){
            corrts = (Long64_t) it_datamap_hit3->first;
            wasabi_t* correntry=(wasabi_t*)it_datamap_hit3->second;//wasabi
            if (corrts==ts){
                it_datamap_hit3++;
                continue;
            }
            //htcorr->Fill(corrts-ts);
            entry->flag_goodevt = false;
            it_datamap_hit3++;
        }
        nevt++;
    }


    for (it_datamap_hit1=datamap_hit1.begin();it_datamap_hit1!=datamap_hit1.end();it_datamap_hit1++){
        Long64_t ts=(Long64_t)it_datamap_hit1->first;
        wasabi_t* entry=(wasabi_t*)it_datamap_hit1->second;//brips
        if (entry->flag_goodevt){
            hexeyclean->Fill(entry->Ex,entry->Ey);
            hxyclean->Fill(entry->x,entry->y);
        }else{
            continue;
        }

        //! beta gamma
        Long64_t corrts = 0;
        Long64_t correntry = 0;
        Long64_t ts1 = ts - BETAGAMMA_TS_WIN_LOW;
        Long64_t ts2 = ts + BETAGAMMA_TS_WIN_UP;
        it_datamap_hit5= datamap_hit5.lower_bound(ts1);


        decay.gc_hit = 0;
        while(it_datamap_hit5!=datamap_hit5.end()&&it_datamap_hit5->first<ts2){
            corrts = (Long64_t) it_datamap_hit5->first;
            clover_t* correntry=(clover_t*)it_datamap_hit5->second;//wasabi
            if (corrts==ts){
                it_datamap_hit5++;
                continue;
            }
            decay.gc_e[decay.gc_hit] = correntry->e;
            decay.gc_ch[decay.gc_hit] = correntry->ch;
            decay.gc_t[decay.gc_hit] = (Double_t)(corrts - ts);// us
            decay.gc_hit++;
            it_datamap_hit5++;
        }


        //!beta ion
        corrts = 0;
        correntry = 0;
        ts1 = ts - TS_WIN_LOW;
        ts2 = ts + TS_WIN_UP;
        it_datamap_hit4 = datamap_hit4.lower_bound(ts1);
        while(it_datamap_hit4!=datamap_hit4.end()&&it_datamap_hit4->first<ts2){
            corrts = (Long64_t) it_datamap_hit4->first;
            wasabi_t* correntry=(wasabi_t*)it_datamap_hit4->second;//wasabi
            if (corrts==ts){
                it_datamap_hit4++;
                continue;
            }
            //if (!((correntry->x-entry->x)<=1&&(correntry->x-entry->x)>=-1&&(correntry->y-entry->y)<=1&&(correntry->y-entry->y)>=-1))
            if (!((correntry->x-entry->x)<=0.5&&(correntry->x-entry->x)>=-0.5&&(correntry->y-entry->y)<=0.5&&(correntry->y-entry->y)>=-0.5))
            {
                it_datamap_hit4++;
                continue;
            }
            //! fill data here
            decay.t = (Double_t) (ts-corrts);
            decay.x  = entry->x;
            decay.y  = entry->y;
            decay.z  = entry->z;
            decay.ex = entry->Ex;
            decay.ey = entry->Ey;
            decay.ion_ex = correntry->Ex;
            decay.ion_ey = correntry->Ey;
            decay.ion_x =correntry->x;
            decay.ion_y =correntry->y;
            decay.ion_z =correntry->z;
            decay.zet = correntry->zet;
            decay.aoq = correntry->aoq;
            decay.de0 = correntry->de0;
            decay.de1 = correntry->de1;
            decay.de2 = correntry->de2;
            decay.de3 = correntry->de3;
            decay.beta = correntry->beta;
            treedata->Fill();
            if (ts-corrts>10000000||ts-corrts<0){
                htcorr->Fill(ts-corrts);
                htecorr->Fill(ts-corrts,entry->Ex);
            }
            it_datamap_hit4++;
        }

    }
    htcorr->Write();
    htecorr->Write();
    hdy->Write();
    hdx->Write();
    hexey->Write();
    hexeyclean->Write();
    hxy->Write();
    hxyclean->Write();
    hxyimplant->Write();

    treedata->Write();
    fout->Close();
}
