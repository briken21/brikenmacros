#include <iostream>
#include <map>

#include "TTree.h"
#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TCanvas.h"

class AidaTreeData {
public:
    AidaTreeData( )= default;;
    ~AidaTreeData()= default;;

    ULong64_t       T;
    ULong64_t       Tfast;
    Double_t        E;
    Double_t        EX;
    Double_t        EY;
    Double_t        x;
    Double_t        y;
    Double_t        z;
    int32_t         nx;
    int32_t         ny;
    int32_t         nz;
    UChar_t         ID;
    void clear(){
        T=0;
        Tfast=0;
        E=0;
        EX=0;
        EY=0;
        x=0;
        y=0;
        z=0;
        ID=0;

    }

};

typedef ULong64_t myTime_t;


int main(int argc, char **argv) {
    std::cout << "Hello, World! Let's get to merging WASABI data." << std::endl;

    std::string sourceOne;
    std::string sourceTwo;

    if( argc == 3 ){
        sourceOne = argv[1];
        sourceTwo = argv[2];
    }
    else {
        return -1;
    }



    //  Variables for source one
    auto * chainOne = new TChain("AIDA_hits");
    chainOne->Add(sourceOne.c_str());
    TBranch *b_aidaOne;
    ULong64_t nEntriesOne = chainOne->GetEntries();
    ULong64_t currentEntryOne = 0;
    auto * aidaOne = new AidaTreeData;
    chainOne->SetBranchAddress("aida_hit", &aidaOne->T, &b_aidaOne);
    bool entryInMapOne = true;

    // Variables for source two
    auto * chainTwo = new TChain("AIDA_hits");
    chainTwo->Add(sourceTwo.c_str());
    TBranch *b_aidaTwo;
    ULong64_t nEntriesTwo = chainTwo->GetEntries();
    ULong64_t currentEntryTwo = 0;
    auto * aidaTwo = new AidaTreeData;
    chainTwo->SetBranchAddress("aida_hit", &aidaTwo->T, &b_aidaTwo);
    bool entryInMapTwo = true;

    //Variables for output tree
    TFile * ofile = TFile::Open( "sourceOneTimeOrdered.root", "recreate");
    TTree * outputTTree = new TTree("AIDA_hits","");
    AidaTreeData outputEntry;
    outputTTree->Branch("aida_hit", &outputEntry,"T/l:Tfast/l:E/D:EX/D:EY/D:x/D:y/D:z/D:nx/I:ny/I:nz/I:ID/b");

    //--------Variables for map control-------------
    std::multimap<myTime_t, AidaTreeData> aidaMap;
    std::multimap<myTime_t, AidaTreeData>::iterator aidaMapIt;
    std::multimap<myTime_t, AidaTreeData>::iterator aidaMapLimitIt;
    const Long64_t maxMapTimeSpan = 60e9; //Holds max 60s
    const Long64_t mapClearSpan = 10e9; // Once holding 60s clear 10s of map
    ULong64_t maxTS;

    while( currentEntryOne<nEntriesOne || currentEntryTwo < nEntriesTwo){
        //Get values from map
        if(currentEntryOne<nEntriesOne && entryInMapOne) {
            chainOne->GetEntry(currentEntryOne);
            entryInMapOne = false;
        }
        if(currentEntryTwo<nEntriesTwo && entryInMapTwo){
            chainTwo->GetEntry(currentEntryTwo);
            entryInMapTwo = false;
        }

        //Entering values into map
        if(!entryInMapOne){
            if(aidaOne->T > aidaTwo->T || currentEntryTwo == nEntriesTwo){
                if(aidaOne->T - aidaTwo->T < 5e9 || currentEntryTwo == nEntriesTwo){
                    aidaMap.emplace(aidaOne->T, *aidaOne);
                    currentEntryOne++;
                    entryInMapOne = true;
                }
            }
            else{
                aidaMap.emplace(aidaOne->T, *aidaOne);
                currentEntryOne++;
                entryInMapOne = true;
            }
        }
        if(!entryInMapTwo){
            if(aidaTwo->T > aidaOne->T || currentEntryOne == nEntriesOne){
                if(aidaTwo->T - aidaOne->T > 5e9 || currentEntryOne == nEntriesOne){
                    aidaMap.emplace(aidaTwo->T, *aidaTwo);
                    currentEntryTwo++;
                    entryInMapTwo = true;
                }
            }
            else{
                aidaMap.emplace(aidaTwo->T, *aidaTwo);
                currentEntryTwo++;
                entryInMapTwo = true;
            }
        }

        //Check if max span held in map
        aidaMapIt = aidaMap.begin();
        if(aidaOne->T < aidaTwo->T){
            maxTS = aidaOne->T;
        }
        else{
            maxTS = aidaTwo->T;
        }
        if(maxTS - aidaMapIt->first >= maxMapTimeSpan){
            aidaMapLimitIt = aidaMap.lower_bound(aidaMapIt->first + mapClearSpan);

            for(aidaMapIt = aidaMap.begin(); aidaMapIt != aidaMapLimitIt; aidaMapIt++){
                outputEntry = aidaMapIt->second;
                outputTTree->Fill();
            }
            aidaMap.erase(aidaMap.begin(),aidaMapLimitIt);
        }

    }
    //Clear the remaining map
    for(aidaMapIt = aidaMap.begin(); aidaMapIt != aidaMap.end(); aidaMapIt++){
        outputEntry = aidaMapIt->second;
        outputTTree->Fill();
    }
    outputTTree->Write();
    ofile->Close();




    //Check for time ordering of AIDA file
    /*
    std::cout << "Defining his" <<std::endl;
    TFile * file = new TFile("HisFile.root","RECREATE");
    TH1D * timeDif = new TH1D("TimeDif","",1e6,0,1e6);
    std::cout << "Starting loop" <<std::endl;
    ULong64_t previousTimestamp = 0;
    ULong64_t maxTSShift = 0;
    while( currentEntryOne < nEntriesOne){
        chainOne->GetEntry(currentEntryOne);
        if(aidaOne->T < previousTimestamp){
            //std::cout << "Warning full file is not time ordered." << std::endl;
            //std::cout << "Timewarp size: " << previousTimestamp - aidaOne->T << std::endl;
            if( previousTimestamp - aidaOne->T > maxTSShift){
                maxTSShift = previousTimestamp - aidaOne->T;
                std::cout << aidaOne->z << " z" << std::endl;
            }
        }

        timeDif->Fill(aidaOne->T - previousTimestamp);
        previousTimestamp = aidaOne->T;
        currentEntryOne++;
    }
    TCanvas * c1 = new TCanvas("c1","c1",800,800);
    timeDif->Draw();
    std::cout << "Max Time warp " << maxTSShift << std::endl;
    file->Write();


    //------------End of time order check-----------*/
    std::cout << nEntriesOne+nEntriesTwo << " " << currentEntryOne+currentEntryTwo << " " <<std::endl;



    return 0;
}