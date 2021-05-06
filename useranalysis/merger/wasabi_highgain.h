//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun May  2 17:27:34 2021 by ROOT version 6.22/08
// from TTree tree/tree
// found on file: rootfiles/highgain_run0230.root
//////////////////////////////////////////////////////////

#ifndef wasabi_highgain_h
#define wasabi_highgain_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class wasabi_highgain {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           evt;
   Long64_t        dgtz_ts;
   Int_t           dgtz_b;
   Double_t        dgtz_e[64];
   Double_t        dgtz_bl[64];
   Double_t        dgtz_t[64];
   Int_t           dgtz_ch[64];
   UShort_t        dgtz_nsample[64];
   UShort_t        dgtz_waveform[64][90];
   UShort_t        dgtz_sample[64][90];
   Double_t        x;
   Double_t        y;
   Int_t           z;
   Double_t        ex;
   Double_t        ey;
   Long64_t        dssd_ts_x;
   Long64_t        dssd_ts_y;
   UShort_t        dssd_ex[32];
   Double_t        dssd_exbl[32];
   Double_t        dssd_tx[32];
   Double_t        dssd_ey[32];
   Double_t        dssd_eybl[32];
   Double_t        dssd_ty[32];
   Int_t           dssd_mutly;

   // List of branches
   TBranch        *b_evt;   //!
   TBranch        *b_dgtz_ts;   //!
   TBranch        *b_dgtz_b;   //!
   TBranch        *b_dgtz_e;   //!
   TBranch        *b_dgtz_bl;   //!
   TBranch        *b_dgtz_t;   //!
   TBranch        *b_dgtz_ch;   //!
   TBranch        *b_dgtz_nsample;   //!
   TBranch        *b_dgtz_waveform;   //!
   TBranch        *b_dgtz_sample;   //!
   TBranch        *b_x;   //!
   TBranch        *b_y;   //!
   TBranch        *b_z;   //!
   TBranch        *b_ex;   //!
   TBranch        *b_ey;   //!
   TBranch        *b_dssd_ts_x;   //!
   TBranch        *b_dssd_ts_y;   //!
   TBranch        *b_dssd_ex;   //!
   TBranch        *b_dssd_exbl;   //!
   TBranch        *b_dssd_tx;   //!
   TBranch        *b_dssd_ey;   //!
   TBranch        *b_dssd_eybl;   //!
   TBranch        *b_dssd_ty;   //!
   TBranch        *b_dssd_mutly;   //!

   wasabi_highgain(char* infile, TTree *tree=0);
   virtual ~wasabi_highgain();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t GetEntries(){return fChain->GetEntries();}
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

wasabi_highgain::wasabi_highgain(char* infile, TTree *tree) : fChain(0)
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

wasabi_highgain::~wasabi_highgain()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t wasabi_highgain::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t wasabi_highgain::LoadTree(Long64_t entry)
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

void wasabi_highgain::Init(TTree *tree)
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

   fChain->SetBranchAddress("evt", &evt, &b_evt);
   fChain->SetBranchAddress("dgtz_ts", &dgtz_ts, &b_dgtz_ts);
   fChain->SetBranchAddress("dgtz_b", &dgtz_b, &b_dgtz_b);
   fChain->SetBranchAddress("dgtz_e", dgtz_e, &b_dgtz_e);
   fChain->SetBranchAddress("dgtz_bl", dgtz_bl, &b_dgtz_bl);
   fChain->SetBranchAddress("dgtz_t", dgtz_t, &b_dgtz_t);
   fChain->SetBranchAddress("dgtz_ch", dgtz_ch, &b_dgtz_ch);
   fChain->SetBranchAddress("dgtz_nsample", dgtz_nsample, &b_dgtz_nsample);
   fChain->SetBranchAddress("dgtz_waveform", dgtz_waveform, &b_dgtz_waveform);
   fChain->SetBranchAddress("dgtz_sample", dgtz_sample, &b_dgtz_sample);
   fChain->SetBranchAddress("x", &x, &b_x);
   fChain->SetBranchAddress("y", &y, &b_y);
   fChain->SetBranchAddress("z", &z, &b_z);
   fChain->SetBranchAddress("ex", &ex, &b_ex);
   fChain->SetBranchAddress("ey", &ey, &b_ey);
   fChain->SetBranchAddress("dssd_ts_x", &dssd_ts_x, &b_dssd_ts_x);
   fChain->SetBranchAddress("dssd_ts_y", &dssd_ts_y, &b_dssd_ts_y);
   fChain->SetBranchAddress("dssd_ex", dssd_ex, &b_dssd_ex);
   fChain->SetBranchAddress("dssd_exbl", dssd_exbl, &b_dssd_exbl);
   fChain->SetBranchAddress("dssd_tx", dssd_tx, &b_dssd_tx);
   fChain->SetBranchAddress("dssd_ey", dssd_ey, &b_dssd_ey);
   fChain->SetBranchAddress("dssd_eybl", dssd_eybl, &b_dssd_eybl);
   fChain->SetBranchAddress("dssd_ty", dssd_ty, &b_dssd_ty);
   fChain->SetBranchAddress("dssd_mutly", &dssd_mutly, &b_dssd_mutly);
   Notify();
}

Bool_t wasabi_highgain::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void wasabi_highgain::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t wasabi_highgain::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
