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
    Double_t        Ex;
    Double_t        Ey;
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
        Ex=0;
        Ey=0;
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
    std::string sourceThree;
    std::string outputFile;

    if( argc == 5 ){
        sourceOne = argv[1];
        sourceTwo = argv[2];
        sourceThree = argv[3];
        outputFile = argv[4];
    }
    else {
        std::cout << "WAS3ABI merger requires three source files and one output file." << std::endl;
        std::cout << "One for each of the DAQs." <<std::endl;
        std::cout << "Usage: ./wasabi_merge /path/sourceOne /path/sourceTwo /path/SourceThree /path/OutputFile" << std::endl;
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

    auto *chainTwo = new TChain("AIDA_hits");
    ULong64_t nEntriesTwo = 0;
    ULong64_t currentEntryTwo = 0;
    auto aidaTwo = new AidaTreeData;
    aidaTwo->clear();
    if (sourceTwo != "NULL") {
        chainTwo->Add(sourceTwo.c_str());
        TBranch *b_aidaTwo;
        nEntriesTwo = chainTwo->GetEntries();
        currentEntryTwo = 0;
        chainTwo->SetBranchAddress("aida_hit", &aidaTwo->T, &b_aidaTwo);
    }
    bool entryInMapTwo = true;

    // Variables for source three
    auto * chainThree = new TChain("AIDA_hits");
    ULong64_t nEntriesThree = 0;
    ULong64_t currentEntryThree = 0;
    auto aidaThree = new AidaTreeData;
    aidaThree->clear();
    if (sourceThree != "NULL") {
        chainThree->Add(sourceThree.c_str());
        TBranch *b_aidaThree;
        nEntriesThree = chainThree->GetEntries();
        currentEntryThree = 0;
        chainThree->SetBranchAddress("aida_hit", &aidaThree->T, &b_aidaThree);
    }
    bool entryInMapThree = true;

    //Variables for output tree
    TFile * ofile = TFile::Open( outputFile.c_str(), "recreate");
    TTree * outputTTree = new TTree("AIDA_hits","");
    AidaTreeData outputEntry;
    outputTTree->Branch("aida_hit", &outputEntry,"T/l:Tfast/l:E/D:EX/D:EY/D:x/D:y/D:z/D:nx/I:ny/I:nz/I:ID/b");

    //--------Variables for map control-------------
    std::multimap<myTime_t, AidaTreeData> aidaMap;
    std::multimap<myTime_t, AidaTreeData>::iterator aidaMapIt;
    std::multimap<myTime_t, AidaTreeData>::iterator aidaMapLimitIt;
    std::multimap<myTime_t, AidaTreeData> aidaEventMap;
    const Long64_t maxMapTimeSpan = 120e9; //Holds max 60s
    const Long64_t mapClearSpan = 10e9; // Once holding 60s clear 10s of map
    ULong64_t maxTS;
    ULong64_t mapSize = 0;
    ULong64_t lastImplantT = 0;
    double maxImpDSSD = 0;

    while( currentEntryOne<nEntriesOne || currentEntryTwo < nEntriesTwo || currentEntryThree < nEntriesThree){
        if(currentEntryOne % 1000000 == 0){
            std::cout << currentEntryOne << " " << currentEntryTwo << " " << currentEntryThree << std::endl;
        }
        //Get values from map
        if(currentEntryOne<nEntriesOne && entryInMapOne) {
            chainOne->GetEntry(currentEntryOne);
            entryInMapOne = false;
        }
        if(currentEntryTwo<nEntriesTwo && entryInMapTwo){
            chainTwo->GetEntry(currentEntryTwo);
            entryInMapTwo = false;
        }
        if(currentEntryThree < nEntriesThree && entryInMapThree){
            chainThree->GetEntry(currentEntryThree);
            entryInMapThree = false;
        }

        //Entering values into map
        if(!entryInMapOne){
            if((aidaOne->T > aidaTwo->T || currentEntryTwo == nEntriesTwo) &&
                (aidaOne->T > aidaThree->T || currentEntryThree == nEntriesThree)){
                if((aidaOne->T - aidaTwo->T < 5e9 || currentEntryTwo == nEntriesTwo) &&
                    (aidaOne->T - aidaThree->T < 5e9 || currentEntryThree == nEntriesThree)){
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
            if((aidaTwo->T > aidaOne->T || currentEntryOne == nEntriesOne) &&
               (aidaTwo->T > aidaThree->T || currentEntryThree == nEntriesThree)){
                if((aidaTwo->T - aidaOne->T < 5e9 || currentEntryOne == nEntriesOne) &&
                    (aidaTwo->T - aidaThree->T < 5e9 || currentEntryThree == nEntriesThree)){
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
        if(!entryInMapThree){
            if((aidaThree->T > aidaTwo->T || currentEntryTwo == nEntriesTwo) &&
               (aidaThree->T > aidaOne->T || currentEntryOne == nEntriesOne)){
                if((aidaThree->T - aidaTwo->T < 5e9 || currentEntryTwo == nEntriesTwo) &&
                    (aidaThree->T - aidaOne->T < 5e9 || currentEntryOne == nEntriesOne)){
                    aidaMap.emplace(aidaThree->T, *aidaThree);
                    currentEntryThree++;
                    entryInMapThree = true;
                }
            }
            else{
                aidaMap.emplace(aidaThree->T, *aidaThree);
                currentEntryThree++;
                entryInMapThree = true;
            }
        }

        if(mapSize == aidaMap.size()){
            std::cout << "Map hasn't changed size" << std::endl;
            std::cout << currentEntryOne << " " << currentEntryTwo << " " << currentEntryThree <<std::endl;
            std::cout << nEntriesOne << " " << nEntriesTwo << " " << nEntriesThree << std::endl;
            std::cout << aidaOne->T << " " << aidaTwo->T << " " << aidaThree->T << std::endl;
            std::cout << entryInMapOne << " " << entryInMapTwo << " " << entryInMapThree << std::endl;
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

            lastImplantT = 0;
            for(aidaMapIt = aidaMap.begin(); aidaMapIt != aidaMapLimitIt; aidaMapIt++){
                if (aidaMapIt->second.ID == 5 || (aidaMapIt->second.T - lastImplantT < 5000 && lastImplantT != 0)) {
                    //Within 5us of last implant
                    aidaEventMap.emplace(aidaMapIt->first, aidaMapIt->second);
                    if(aidaMapIt->second.z > maxImpDSSD){
                        maxImpDSSD = aidaMapIt->second.z;
                    }
                }
                else{
                    //Greater than 5us span since last implant.
                    //Through out all but dE and max
                    for( auto event : aidaEventMap){
                        if(event.second.ID == 5){
                            outputEntry = event.second;
                            outputTTree->Fill();
                        }
                        else if( event.second.z < 11 || event.second.z == maxImpDSSD) {
                            outputEntry = event.second;
                            outputTTree->Fill();
                        }
                    }
                    lastImplantT = 0;
                    maxImpDSSD = 0;
                    aidaEventMap.clear();

                }
            }
            aidaMap.erase(aidaMap.begin(),aidaMapLimitIt);
        }
        mapSize = aidaMap.size();
    }
    //Clear the remaining map
    for(aidaMapIt = aidaMap.begin(); aidaMapIt != aidaMap.end(); aidaMapIt++){
        if (aidaMapIt->second.ID == 5 || (aidaMapIt->second.T - lastImplantT < 5000 && lastImplantT != 0)) {
            //Within 5us of last implant
            aidaEventMap.emplace(aidaMapIt->first, aidaMapIt->second);
            if(aidaMapIt->second.z > maxImpDSSD){
                maxImpDSSD = aidaMapIt->second.z;
            }
        }
        else{
            //Greater than 5us span since last implant.
            //Through out all but dE and max
            for( auto event : aidaEventMap){
                if(event.second.ID == 5){
                    outputEntry = event.second;
                    outputTTree->Fill();
                }
                else if( event.second.z < 11 || event.second.z == maxImpDSSD){
                    outputEntry = event.second;
                    outputTTree->Fill();
                }
            }
            lastImplantT = 0;
            maxImpDSSD = 0;
            aidaEventMap.clear();

        }
    }
    //Clear the remaining event map
    for( auto event : aidaEventMap){
        if(event.second.ID == 5){
            outputEntry = event.second;
            outputTTree->Fill();
        }
        else if( event.second.z < 11 || event.second.z == maxImpDSSD){
            outputEntry = event.second;
            outputTTree->Fill();
        }
    }

    outputTTree->Write();
    ofile->Close();




    //Check for time ordering of AIDA file

//    std::cout << "Defining his" <<std::endl;
//    TFile * file = new TFile("HisFile.root","RECREATE");
//    TH1D * timeDif = new TH1D("TimeDif","",1e6,0,1e6);
//    std::cout << "Starting loop" <<std::endl;
//    ULong64_t previousTimestamp = 0;
//    ULong64_t maxTSShift = 0;
//    while( currentEntryOne < nEntriesOne){
//        chainOne->GetEntry(currentEntryOne);
//        if(aidaOne->T < previousTimestamp){
//            //std::cout << "Warning full file is not time ordered." << std::endl;
//            //std::cout << "Timewarp size: " << previousTimestamp - aidaOne->T << std::endl;
//            if( previousTimestamp - aidaOne->T > maxTSShift){
//                maxTSShift = previousTimestamp - aidaOne->T;
//                std::cout << aidaOne->z << " z" << " " << previousTimestamp - aidaOne->T <<  std::endl;
//            }
//        }
//
//        timeDif->Fill(aidaOne->T - previousTimestamp);
//        previousTimestamp = aidaOne->T;
//        currentEntryOne++;
//    }
//    TCanvas * c1 = new TCanvas("c1","c1",800,800);
//    timeDif->Draw();
//    std::cout << "Max Time warp " << maxTSShift << std::endl;
//    file->Write();

    //------------End of time order check-----------
    std::cout << nEntriesOne+nEntriesTwo << " " << currentEntryOne+currentEntryTwo << " " <<std::endl;



    return 0;
}
