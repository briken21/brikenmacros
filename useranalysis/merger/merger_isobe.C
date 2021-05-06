#define bigrips_cxx
#include "bigrips.h"
#include "wasabi.h"
#include <TH2.h>
#include <TStyle.h>
#include <TTree.h>
#include <TFile.h>
#include <TCanvas.h>

#include <iostream>
#include <map>


#define N_V1740 5
#define V1740_N_MAX_CH 64
#define N_MAX_WF_LENGTH 360
#define N_DSSD 4
#define N_PL 4
#define N_DE 3
#define N_STRIP_X 3


using namespace std;

std::multimap <Long64_t,Long64_t> datamap_hit1; //! sort by timestamp
std::multimap <Long64_t,Long64_t> datamap_hit2; //! sort by timestamp
std::multimap <Long64_t,Long64_t>::iterator it_datamap_hit1;
std::multimap <Long64_t,Long64_t>::iterator it_datamap_hit2;


void merger_isobe(char* bigripsfile,char* wasabifile, char* outfile)
{
    bigrips* bigripsin = new bigrips(bigripsfile);
    wasabi* wasabiin = new wasabi(wasabifile);
    Long64_t brnentries = bigripsin->GetEntries();
    Long64_t wanenrties = wasabiin->GetEntries();

    cout<<bigripsin->GetEntries()<<"\t"<<wasabiin->GetEntries()<<endl;

    TFile* fout = new TFile(outfile,"RECREATE");
    TTree* treedata = new TTree("tree","tree");


    //! bigrips
    treedata->Branch("ts",&bigripsin->ts,"ts/L");
    treedata->Branch("sts",&bigripsin->sts,"sts/L");
    treedata->Branch("tof",&bigripsin->tof,"tof/D");
    treedata->Branch("zet",&bigripsin->zet,"zet/D");
    treedata->Branch("aoq",&bigripsin->aoq,"aoq/D");
    treedata->Branch("f5x",&bigripsin->f5x,"tof/D");
    treedata->Branch("f11x",&bigripsin->f11x,"f11x/D");
    treedata->Branch("f11y",&bigripsin->f11y,"f11y/D");
    treedata->Branch("f11dt",&bigripsin->f11dt,"f11dt/D");

    treedata->Branch("evt",wasabiin->evt,Form("evt[%d]/I",N_V1740));
    treedata->Branch("dgtz_ts",wasabiin->dgtz_ts,Form("dgtz_ts[%d]/L",N_V1740));
    treedata->Branch("dgtz_e",wasabiin->dgtz_e,Form("dgtz_e[%d]/D",V1740_N_MAX_CH*N_V1740));
    treedata->Branch("dgtz_bl",wasabiin->dgtz_bl,Form("dgtz_bl[%d]/D",V1740_N_MAX_CH*N_V1740));
    treedata->Branch("dgtz_t",wasabiin->dgtz_t,Form("dgtz_t[%d]/D",V1740_N_MAX_CH*N_V1740));
    treedata->Branch("dgtz_ch",wasabiin->dgtz_ch,Form("dgtz_ch[%d]/I",V1740_N_MAX_CH*N_V1740));
    treedata->Branch("dgtz_nsample",wasabiin->dgtz_nsample,Form("dgtz_nsample[%d]/s",V1740_N_MAX_CH*N_V1740));
    treedata->Branch("dgtz_waveform",wasabiin->dgtz_waveform,Form("dgtz_waveform[%d][%d]/s",V1740_N_MAX_CH*N_V1740,N_MAX_WF_LENGTH));
    treedata->Branch("dgtz_sample",wasabiin->dgtz_sample,Form("dgtz_sample[%d][%d]/s",V1740_N_MAX_CH*N_V1740,N_MAX_WF_LENGTH));


    treedata->Branch("evtd",&wasabiin->evtd,"evtd/I");
    treedata->Branch("evtts",&wasabiin->evtts,"evtts/L");

    treedata->Branch("de_e",wasabiin->de_e,Form("de_e[%d]/D",N_DE));
    treedata->Branch("de_z",wasabiin->de_z,Form("de_z[%d]/I",N_DE));
    treedata->Branch("de_maxz",&wasabiin->de_maxz,"de_maxz/I");

    treedata->Branch("pl_e",wasabiin->pl_e,Form("pl_e[%d]/D",N_PL));

    treedata->Branch("x",wasabiin->x,Form("x[%d]/D",N_DSSD));
    treedata->Branch("y",wasabiin->y,Form("y[%d]/D",N_DSSD));
    treedata->Branch("ex",wasabiin->ex,Form("ex[%d]/D",N_DSSD));
    treedata->Branch("ey",wasabiin->ey,Form("ey[%d]/D",N_DSSD));

    treedata->Branch("dssd_ts_x",wasabiin->dssd_ts_x,Form("dssd_ts_x[%d]/L",N_DSSD));
    treedata->Branch("dssd_ts_y",wasabiin->dssd_ts_y,Form("dssd_ts_y[%d]/L",N_DSSD));

    treedata->Branch("max_z",&wasabiin->max_z,"max_z/I");
    treedata->Branch("max_z_x",&wasabiin->max_z_x,"max_z_x/I");
    treedata->Branch("max_z_y",&wasabiin->max_z_y,"max_z_y/I");

    treedata->Branch("dssd_ex",wasabiin->dssd_ex,Form("dssd_ex[%d][%d]/s",N_DSSD,N_STRIP_X));
    treedata->Branch("dssd_exbl",wasabiin->dssd_exbl,Form("dssd_exbl[%d][%d]/D",N_DSSD,N_STRIP_X));
    treedata->Branch("dssd_tx",wasabiin->dssd_tx,Form("dssd_tx[%d][%d]/D",N_DSSD,N_STRIP_X));
    treedata->Branch("dssd_mutlx",wasabiin->dssd_mutlx,Form("dssd_mutlx[%d]/D",N_DSSD));

    treedata->Branch("dssd_ey",wasabiin->dssd_ey,Form("dssd_ey[%d][%d]/D",N_DSSD,N_STRIP_X));
    treedata->Branch("dssd_eybl",wasabiin->dssd_eybl,Form("dssd_eybl[%d][%d]/D",N_DSSD,N_STRIP_X));
    treedata->Branch("dssd_ty",wasabiin->dssd_ty,Form("dssd_ty[%d][%d]/D",N_DSSD,N_STRIP_X));
    treedata->Branch("dssd_mutly",wasabiin->dssd_mutly,Form("dssd_mutly[%d]/D",N_DSSD));

    datamap_hit1.clear();
    datamap_hit2.clear();

    for (Long64_t jentry=0; jentry<brnentries;jentry++) {
        bigripsin->GetEntry(jentry);
        datamap_hit1.insert(make_pair((Long64_t)bigripsin->ts,jentry));
        if (jentry<10) cout<<bigripsin->ts<<endl;
        if (jentry>brnentries-10) cout<<bigripsin->ts<<endl;
    }

    for (Long64_t jentry=0; jentry<wanenrties;jentry++) {
        wasabiin->GetEntry(jentry);
        datamap_hit2.insert(make_pair((Long64_t)wasabiin->evtts,jentry));
        if (jentry<10) cout<<wasabiin->evtts<<endl;
        if (jentry>wanenrties-10) cout<<wasabiin->evtts<<endl;
    }

    cout<<datamap_hit1.size()<<"\t"<<datamap_hit2.size()<<endl;
    Long64_t nevt = 0;
    for (it_datamap_hit1=datamap_hit1.begin();it_datamap_hit1!=datamap_hit1.end();it_datamap_hit1++){
        Long64_t ts=(Long64_t)it_datamap_hit1->first;
        Long64_t entry=(Long64_t)it_datamap_hit1->second;//brips
        Long64_t corrts = 0;
        Long64_t correntry = 0;
        Long64_t ts1 = ts - 50000;
        Long64_t ts2 = ts + 50000;
        it_datamap_hit2 = datamap_hit2.lower_bound(ts1);
	corrts = (Long64_t) it_datamap_hit2->first;
	if(fabs(ts-corrts)<50000){
	  std::cout  << ts << " " << corrts << " " << ts - corrts << " ok" <<std::endl;
	}
	else{
	  std::cout  << ts << " " << corrts << " " << ts - corrts << std::endl;
	}
        if(it_datamap_hit2!=datamap_hit2.end() && it_datamap_hit2->first<ts2){
	  corrts = (Long64_t) it_datamap_hit2->first;
	  correntry=(Long64_t)it_datamap_hit2->second;//wasabi
	  wasabiin->GetEntry(correntry);
	  bigripsin->GetEntry(entry);
	  treedata->Fill();
	  nevt++;
        }
    }
    treedata->Write();
    fout->Close();
    std::cout  << "num of matched events: " << nevt << std::endl;

}
