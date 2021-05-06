//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu May  6 00:19:02 2021 by ROOT version 6.22/08
// from TTree BRIKENTree/BRIKENTree
// found on file: briken/daq3_run0021.root
//////////////////////////////////////////////////////////

#ifndef BRIKENTree_h
#define BRIKENTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <string>
#include <vector>
// Header file for the classes stored in the TTree if any.

class BRIKENTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.
   static constexpr Int_t kMaxNeutrons = 1;
   static constexpr Int_t kMaxGamma = 1;
   static constexpr Int_t kMaxAncillary = 1;

   // Declaration of leaf types
 //BrikenTreeData  *Neutrons_;
   Double_t        Neutrons_E;
   ULong_t         Neutrons_T;
   UShort_t        Neutrons_Id;
   UShort_t        Neutrons_type;
   UShort_t        Neutrons_Index1;
   UShort_t        Neutrons_Index2;
   UShort_t        Neutrons_InfoFlag;
   string          Neutrons_Name;
   vector<unsigned short> Neutrons_Samples;
 //BrikenTreeData  *Gamma_;
   Double_t        Gamma_E;
   ULong_t         Gamma_T;
   UShort_t        Gamma_Id;
   UShort_t        Gamma_type;
   UShort_t        Gamma_Index1;
   UShort_t        Gamma_Index2;
   UShort_t        Gamma_InfoFlag;
   string          Gamma_Name;
   vector<unsigned short> Gamma_Samples;
 //BrikenTreeData  *Ancillary_;
   Double_t        Ancillary_E;
   ULong_t         Ancillary_T;
   UShort_t        Ancillary_Id;
   UShort_t        Ancillary_type;
   UShort_t        Ancillary_Index1;
   UShort_t        Ancillary_Index2;
   UShort_t        Ancillary_InfoFlag;
   string          Ancillary_Name;
   vector<unsigned short> Ancillary_Samples;

   // List of branches
   TBranch        *b_Neutrons_E;   //!
   TBranch        *b_Neutrons_T;   //!
   TBranch        *b_Neutrons_Id;   //!
   TBranch        *b_Neutrons_type;   //!
   TBranch        *b_Neutrons_Index1;   //!
   TBranch        *b_Neutrons_Index2;   //!
   TBranch        *b_Neutrons_InfoFlag;   //!
   TBranch        *b_Neutrons_Name;   //!
   TBranch        *b_Neutrons_Samples;   //!
   TBranch        *b_Gamma_E;   //!
   TBranch        *b_Gamma_T;   //!
   TBranch        *b_Gamma_Id;   //!
   TBranch        *b_Gamma_type;   //!
   TBranch        *b_Gamma_Index1;   //!
   TBranch        *b_Gamma_Index2;   //!
   TBranch        *b_Gamma_InfoFlag;   //!
   TBranch        *b_Gamma_Name;   //!
   TBranch        *b_Gamma_Samples;   //!
   TBranch        *b_Ancillary_E;   //!
   TBranch        *b_Ancillary_T;   //!
   TBranch        *b_Ancillary_Id;   //!
   TBranch        *b_Ancillary_type;   //!
   TBranch        *b_Ancillary_Index1;   //!
   TBranch        *b_Ancillary_Index2;   //!
   TBranch        *b_Ancillary_InfoFlag;   //!
   TBranch        *b_Ancillary_Name;   //!
   TBranch        *b_Ancillary_Samples;   //!

   BRIKENTree(char* infile, TTree *tree=0);
   virtual ~BRIKENTree();
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

BRIKENTree::BRIKENTree(char* infile, TTree *tree) : fChain(0)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(infile);
      if (!f || !f->IsOpen()) {
         f = new TFile(infile);
      }
      f->GetObject("BRIKENTree",tree);
   }
   Init(tree);
}

BRIKENTree::~BRIKENTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t BRIKENTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t BRIKENTree::LoadTree(Long64_t entry)
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

void BRIKENTree::Init(TTree *tree)
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

   fChain->SetBranchAddress("Neutrons.E", &Neutrons_E, &b_Neutrons_E);
   fChain->SetBranchAddress("Neutrons.T", &Neutrons_T, &b_Neutrons_T);
   fChain->SetBranchAddress("Neutrons.Id", &Neutrons_Id, &b_Neutrons_Id);
   fChain->SetBranchAddress("Neutrons.type", &Neutrons_type, &b_Neutrons_type);
   fChain->SetBranchAddress("Neutrons.Index1", &Neutrons_Index1, &b_Neutrons_Index1);
   fChain->SetBranchAddress("Neutrons.Index2", &Neutrons_Index2, &b_Neutrons_Index2);
   fChain->SetBranchAddress("Neutrons.InfoFlag", &Neutrons_InfoFlag, &b_Neutrons_InfoFlag);
   fChain->SetBranchAddress("Neutrons.Name", &Neutrons_Name, &b_Neutrons_Name);
   fChain->SetBranchAddress("Neutrons.Samples", &Neutrons_Samples, &b_Neutrons_Samples);
   fChain->SetBranchAddress("Gamma.E", &Gamma_E, &b_Gamma_E);
   fChain->SetBranchAddress("Gamma.T", &Gamma_T, &b_Gamma_T);
   fChain->SetBranchAddress("Gamma.Id", &Gamma_Id, &b_Gamma_Id);
   fChain->SetBranchAddress("Gamma.type", &Gamma_type, &b_Gamma_type);
   fChain->SetBranchAddress("Gamma.Index1", &Gamma_Index1, &b_Gamma_Index1);
   fChain->SetBranchAddress("Gamma.Index2", &Gamma_Index2, &b_Gamma_Index2);
   fChain->SetBranchAddress("Gamma.InfoFlag", &Gamma_InfoFlag, &b_Gamma_InfoFlag);
   fChain->SetBranchAddress("Gamma.Name", &Gamma_Name, &b_Gamma_Name);
   fChain->SetBranchAddress("Gamma.Samples", &Gamma_Samples, &b_Gamma_Samples);
   fChain->SetBranchAddress("Ancillary.E", &Ancillary_E, &b_Ancillary_E);
   fChain->SetBranchAddress("Ancillary.T", &Ancillary_T, &b_Ancillary_T);
   fChain->SetBranchAddress("Ancillary.Id", &Ancillary_Id, &b_Ancillary_Id);
   fChain->SetBranchAddress("Ancillary.type", &Ancillary_type, &b_Ancillary_type);
   fChain->SetBranchAddress("Ancillary.Index1", &Ancillary_Index1, &b_Ancillary_Index1);
   fChain->SetBranchAddress("Ancillary.Index2", &Ancillary_Index2, &b_Ancillary_Index2);
   fChain->SetBranchAddress("Ancillary.InfoFlag", &Ancillary_InfoFlag, &b_Ancillary_InfoFlag);
   fChain->SetBranchAddress("Ancillary.Name", &Ancillary_Name, &b_Ancillary_Name);
   fChain->SetBranchAddress("Ancillary.Samples", &Ancillary_Samples, &b_Ancillary_Samples);
   Notify();
}

Bool_t BRIKENTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void BRIKENTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t BRIKENTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
