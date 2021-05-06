//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Apr 26 23:50:18 2021 by ROOT version 6.22/08
// from TTree tree/tree
// found on file: bigrips/run3088.root
//////////////////////////////////////////////////////////

#ifndef bigrips_h
#define bigrips_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class bigrips {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
 //TreeData        *bigrips;
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

   // List of branches
   TBranch        *b_bigrips_ts;   //!
   TBranch        *b_bigrips_sts;   //!
   TBranch        *b_bigrips_tof;   //!
   TBranch        *b_bigrips_zet;   //!
   TBranch        *b_bigrips_aoq;   //!
   TBranch        *b_bigrips_f5x;   //!
   TBranch        *b_bigrips_f11x;   //!
   TBranch        *b_bigrips_f11y;   //!
   TBranch        *b_bigrips_f11dt;   //!
   TBranch        *b_bigrips_beta;   //!

   bigrips(char* infile,TTree *tree=0);
   virtual ~bigrips();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t GetEntries(){return fChain->GetEntries();}
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   //virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

bigrips::bigrips(char* infile, TTree *tree) : fChain(0)
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


bigrips::~bigrips()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t bigrips::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t bigrips::LoadTree(Long64_t entry)
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

void bigrips::Init(TTree *tree)
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

   fChain->SetBranchAddress("ts", &ts, &b_bigrips_ts);
   fChain->SetBranchAddress("sts", &sts, &b_bigrips_sts);
   fChain->SetBranchAddress("tof", &tof, &b_bigrips_tof);
   fChain->SetBranchAddress("zet", &zet, &b_bigrips_zet);
   fChain->SetBranchAddress("aoq", &aoq, &b_bigrips_aoq);
   fChain->SetBranchAddress("f5x", &f5x, &b_bigrips_f5x);
   fChain->SetBranchAddress("f11x", &f11x, &b_bigrips_f11x);
   fChain->SetBranchAddress("f11y", &f11y, &b_bigrips_f11y);
   fChain->SetBranchAddress("f11dt", &f11dt, &b_bigrips_f11dt);
   fChain->SetBranchAddress("beta", &beta, &b_bigrips_beta);
   Notify();
}

Bool_t bigrips::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void bigrips::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t bigrips::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
