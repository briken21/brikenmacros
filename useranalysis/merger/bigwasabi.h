//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Apr 30 12:31:21 2021 by ROOT version 6.22/08
// from TTree tree/tree
// found on file: merged_files/merged_run0168.root
//////////////////////////////////////////////////////////

#ifndef bigwasabi_h
#define bigwasabi_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

#define N_DE 4
#define N_PL 4
#define N_DSSD 4
#define N_STRIP_X 32
#define N_V1740 5
#define V1740_N_MAX_CH 64
#define N_MAX_WF_LENGTH 360

class bigwasabi {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   ULong64_t       ts;
   ULong64_t       sts;
   Double_t        tof;
   Double_t        zet;
   Double_t        aoq;
   Double_t        f5x;
   Double_t        f11x;
   Double_t        f11y;
   Double_t        f11dt;
   Double_t        beta;
   Int_t           evt[N_V1740];
   Long64_t        dgtz_ts[N_V1740];
   Double_t        dgtz_e[N_V1740*V1740_N_MAX_CH];
   Double_t        dgtz_bl[N_V1740*V1740_N_MAX_CH];
   Double_t        dgtz_t[N_V1740*V1740_N_MAX_CH];
   Int_t           dgtz_ch[N_V1740*V1740_N_MAX_CH];
   UShort_t        dgtz_nsample[N_V1740*V1740_N_MAX_CH];
   UShort_t        dgtz_waveform[N_V1740*V1740_N_MAX_CH][N_MAX_WF_LENGTH];
   UShort_t        dgtz_sample[N_V1740*V1740_N_MAX_CH][N_MAX_WF_LENGTH];
   Int_t           evtd;
   Long64_t        evtts;
   Double_t        de_e[N_DE];
   Int_t           de_z[N_DE];
   Int_t           de_maxz;
   Double_t        pl_e[N_PL];
   Double_t        x[N_DSSD];
   Double_t        y[N_DSSD];
   Double_t        ex[N_DSSD];
   Double_t        ey[N_DSSD];
   Long64_t        dssd_ts_x[N_DSSD];
   Long64_t        dssd_ts_y[N_DSSD];
   Int_t           max_z;
   Int_t           max_z_x;
   Int_t           max_z_y;
   UShort_t        dssd_ex[N_DSSD][N_STRIP_X];
   Double_t        dssd_exbl[N_DSSD][N_STRIP_X];
   Double_t        dssd_tx[N_DSSD][N_STRIP_X];
   Double_t        dssd_mutlx[N_DSSD];
   Double_t        dssd_ey[N_DSSD][N_STRIP_X];
   Double_t        dssd_eybl[N_DSSD][N_STRIP_X];
   Double_t        dssd_ty[N_DSSD][N_STRIP_X];
   Double_t        dssd_mutly[N_DSSD];

   // List of branches
   TBranch        *b_ts;   //!
   TBranch        *b_sts;   //!
   TBranch        *b_tof;   //!
   TBranch        *b_zet;   //!
   TBranch        *b_aoq;   //!
   TBranch        *b_f5x;   //!
   TBranch        *b_f11x;   //!
   TBranch        *b_f11y;   //!
   TBranch        *b_f11dt;   //!
   TBranch        *b_beta;   //!
   TBranch        *b_evt;   //!
   TBranch        *b_dgtz_ts;   //!
   TBranch        *b_dgtz_e;   //!
   TBranch        *b_dgtz_bl;   //!
   TBranch        *b_dgtz_t;   //!
   TBranch        *b_dgtz_ch;   //!
   TBranch        *b_dgtz_nsample;   //!
   TBranch        *b_dgtz_waveform;   //!
   TBranch        *b_dgtz_sample;   //!
   TBranch        *b_evtd;   //!
   TBranch        *b_evtts;   //!
   TBranch        *b_de_e;   //!
   TBranch        *b_de_z;   //!
   TBranch        *b_de_maxz;   //!
   TBranch        *b_pl_e;   //!
   TBranch        *b_x;   //!
   TBranch        *b_y;   //!
   TBranch        *b_ex;   //!
   TBranch        *b_ey;   //!
   TBranch        *b_dssd_ts_x;   //!
   TBranch        *b_dssd_ts_y;   //!
   TBranch        *b_max_z;   //!
   TBranch        *b_max_z_x;   //!
   TBranch        *b_max_z_y;   //!
   TBranch        *b_dssd_ex;   //!
   TBranch        *b_dssd_exbl;   //!
   TBranch        *b_dssd_tx;   //!
   TBranch        *b_dssd_mutlx;   //!
   TBranch        *b_dssd_ey;   //!
   TBranch        *b_dssd_eybl;   //!
   TBranch        *b_dssd_ty;   //!
   TBranch        *b_dssd_mutly;   //!

   bigwasabi(char* infile, TTree *tree=0);
   virtual ~bigwasabi();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t GetEntries(){return fChain->GetEntries();}
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

bigwasabi::bigwasabi(char* infile, TTree *tree) : fChain(0)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(infile);
      if (!f || !f->IsOpen()) {
         f = new TFile(infile);
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

bigwasabi::~bigwasabi()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t bigwasabi::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t bigwasabi::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void bigwasabi::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("ts", &ts, &b_ts);
   fChain->SetBranchAddress("sts", &sts, &b_sts);
   fChain->SetBranchAddress("tof", &tof, &b_tof);
   fChain->SetBranchAddress("zet", &zet, &b_zet);
   fChain->SetBranchAddress("aoq", &aoq, &b_aoq);
   fChain->SetBranchAddress("f5x", &f5x, &b_f5x);
   fChain->SetBranchAddress("f11x", &f11x, &b_f11x);
   fChain->SetBranchAddress("f11y", &f11y, &b_f11y);
   fChain->SetBranchAddress("f11dt", &f11dt, &b_f11dt);
   fChain->SetBranchAddress("beta", &beta, &b_beta);
   fChain->SetBranchAddress("evt", evt, &b_evt);
   fChain->SetBranchAddress("dgtz_ts", dgtz_ts, &b_dgtz_ts);
   fChain->SetBranchAddress("dgtz_e", dgtz_e, &b_dgtz_e);
   fChain->SetBranchAddress("dgtz_bl", dgtz_bl, &b_dgtz_bl);
   fChain->SetBranchAddress("dgtz_t", dgtz_t, &b_dgtz_t);
   fChain->SetBranchAddress("dgtz_ch", dgtz_ch, &b_dgtz_ch);
   fChain->SetBranchAddress("dgtz_nsample", dgtz_nsample, &b_dgtz_nsample);
   fChain->SetBranchAddress("dgtz_waveform", dgtz_waveform, &b_dgtz_waveform);
   fChain->SetBranchAddress("dgtz_sample", dgtz_sample, &b_dgtz_sample);
   fChain->SetBranchAddress("evtd", &evtd, &b_evtd);
   fChain->SetBranchAddress("evtts", &evtts, &b_evtts);
   fChain->SetBranchAddress("de_e", de_e, &b_de_e);
   fChain->SetBranchAddress("de_z", de_z, &b_de_z);
   fChain->SetBranchAddress("de_maxz", &de_maxz, &b_de_maxz);
   fChain->SetBranchAddress("pl_e", pl_e, &b_pl_e);
   fChain->SetBranchAddress("x", x, &b_x);
   fChain->SetBranchAddress("y", y, &b_y);
   fChain->SetBranchAddress("ex", ex, &b_ex);
   fChain->SetBranchAddress("ey", ey, &b_ey);
   fChain->SetBranchAddress("dssd_ts_x", dssd_ts_x, &b_dssd_ts_x);
   fChain->SetBranchAddress("dssd_ts_y", dssd_ts_y, &b_dssd_ts_y);
   fChain->SetBranchAddress("max_z", &max_z, &b_max_z);
   fChain->SetBranchAddress("max_z_x", &max_z_x, &b_max_z_x);
   fChain->SetBranchAddress("max_z_y", &max_z_y, &b_max_z_y);
   fChain->SetBranchAddress("dssd_ex", dssd_ex, &b_dssd_ex);
   fChain->SetBranchAddress("dssd_exbl", dssd_exbl, &b_dssd_exbl);
   fChain->SetBranchAddress("dssd_tx", dssd_tx, &b_dssd_tx);
   fChain->SetBranchAddress("dssd_mutlx", dssd_mutlx, &b_dssd_mutlx);
   fChain->SetBranchAddress("dssd_ey", dssd_ey, &b_dssd_ey);
   fChain->SetBranchAddress("dssd_eybl", dssd_eybl, &b_dssd_eybl);
   fChain->SetBranchAddress("dssd_ty", dssd_ty, &b_dssd_ty);
   fChain->SetBranchAddress("dssd_mutly", dssd_mutly, &b_dssd_mutly);
   Notify();
}

Bool_t bigwasabi::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void bigwasabi::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t bigwasabi::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
