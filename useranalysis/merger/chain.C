#include "TChain.h"
#include "TLatex.h"

#include "TCanvas.h"
#include "TPad.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "TRandom.h"
#include "TString.h"
#include <string>
using namespace std;

void chain(char* listfile){
  char tempchar1[1000];
  sprintf(tempchar1,"tree");
  TChain* ch = new TChain(tempchar1);
  std::ifstream ifs(listfile);
  string filelist[1000];

  Int_t nfiles=0;
  while (!ifs.eof()){
      ifs>>filelist[nfiles];
      cout<<filelist[nfiles]<<endl;
      nfiles++;
  }
  nfiles=nfiles-1;
  cout<<"There are "<<nfiles<<" files in total!"<<endl;

  for (Int_t i=0;i<nfiles;i++){
      char tempchar2[1000];
      sprintf(tempchar2,"%s/tree",filelist[i].c_str());
      ch->Add(tempchar2);
  }
}
