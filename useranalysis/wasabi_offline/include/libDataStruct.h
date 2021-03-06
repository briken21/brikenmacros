#ifndef __libDataStruct_H
#define __libDataStruct_H

#include "TObject.h"
#include "TROOT.h"
#include <vector>
#include <iostream>
#include <bitset>

#define TDC_N_CHANNEL 64
#define TDC_MAX_MULT 3


#define V1740_DGTZ_CLK_RES 8
#define V1730_DGTZ_CLK_RES 2
#define LUPO_CLK_RES 10

using namespace std;
/*!
  Container for the full beam, tof, beta and pid information
*/

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <string.h>

class WasabiStripData{
public:
    WasabiStripData(){
        ts=-1;
        firstChannel=-1;
        lastChannel= -1;
        energy = 0;
        DSSD = -1;
        ID = 0;
        finets = 9000;
        min_time_strip = -1;
    };
    ~WasabiStripData(){};
    void UpdateEvent(ULong64_t ts_in, int channel_in, Double_t Energy_in,
                     int DSSD_in, UChar_t ID_in, int finets_in){
     
        ts = ts_in;
        if(firstChannel == -1){
            firstChannel = (double)channel_in;
            lastChannel = (double)channel_in;
        }
        else{
            lastChannel = (double)channel_in;
        }

        energy += Energy_in;
        DSSD = (double)DSSD_in;
        ID = ID_in;
        if(finets_in < finets){
            min_time_strip = (double)channel_in;
            finets = finets_in;
        }
        if(ID == 4){
        energy = 2700;
        }
       
    };
    void ResetEvent(){
        ts=-1;
        firstChannel=-1;
        lastChannel= -1;
        energy = 0;
        DSSD = -1;
        ID = 0;
        finets=9000;
        min_time_strip = -1;
    }
    ULong64_t ts;
    Double_t firstChannel;
    Double_t lastChannel;
    Double_t energy;
    Double_t DSSD;
    UChar_t ID;
    double min_time_strip;
    int finets;
};

class AidaTreeData {
public:
    AidaTreeData( )= default;;
    ~AidaTreeData()= default;;
    AidaTreeData(WasabiStripData & xEvent, WasabiStripData & yEvent, int nx_in, int ny_in){
        if(xEvent.ts < yEvent.ts){
            T = xEvent.ts;
        } else{
            T = yEvent.ts;
        }
        E = (xEvent.energy + yEvent.energy)/2.0;
        EX = xEvent.energy;
        EY = yEvent.energy;
        if(xEvent.ID == 5){
        x = (xEvent.lastChannel+xEvent.firstChannel)/2.0;
        y = ((yEvent.lastChannel+yEvent.firstChannel)/2.0)-32.0;
        uint8_t dx = xEvent.lastChannel-xEvent.firstChannel;
        uint8_t dy = yEvent.lastChannel-yEvent.firstChannel;
        Tfast = dx + 0x100 * dy;
        }
        else{
        x = xEvent.min_time_strip;
        y = yEvent.min_time_strip-32.0;
        Tfast = 0;
        }
        z = xEvent.DSSD + 11.0;
        nx = nx_in;
        ny = ny_in;
        nz = 1;
        ID = xEvent.ID;

    };
    AidaTreeData(double det, double Energy, ULong64_t ts){
        T = ts;
        Tfast = 0;
        E = Energy;
        EX = Energy;
        EY = Energy;
        x = 0.;
        y = 0.;
        z = det;
        nx = 0;
        ny = 0;
        nz = 0;
        ID = 4;

    };


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



class BrikenTreeData  {
public:
   BrikenTreeData() { }
   ~BrikenTreeData() {}
   double E;
   uint64_t T;
   uint16_t Id;
   uint16_t type;
   uint16_t Index1;
   uint16_t Index2;
   uint16_t InfoFlag;
   std::string Name;
   void Clear(){
       E = -10;
       T = 0;
       Id = 9999;
       type = 9999;
       Index1 = 9999;
       Index2 = 9999;
       InfoFlag = 9999;
       Name = "";
   };
};


class NIGIRIHit : public TObject {
public:
    //! default constructor
    NIGIRIHit(){
        Clear();
    }
    virtual ~NIGIRIHit(){Clear();}

    virtual void Copy(NIGIRIHit& obj){
        obj.ts_corr_type = ts_corr_type;
        obj.ts = ts;
        obj.sampling_interval = sampling_interval;
        obj.trig_pos = trig_pos;
        obj.ch=ch;
        obj.finets=finets;
        obj.cshort=cshort;
        obj.clong=clong;
        obj.baseline=baseline;
        obj.nsample=nsample;
        obj.pulse=pulse;
        obj.pulse_ap1=pulse_ap1;
        obj.pulse_ap2=pulse_ap1;
    }
    void Clear(){
        ts_corr_type = 0;
        ts = 0;
        sampling_interval = 0;
        trig_pos = 0;
        ch = -1;
        finets  = 0;
        cshort = 0;
        clong  = 0;
        baseline = 0;
        nsample = 0;
        pulse.clear();
        pulse_ap1.clear();
        pulse_ap2.clear();
    }

    void Print(){
        cout<<"ch = "<<ch<<endl;
        cout<<"ch ts = "<<ts<<endl;
        cout<<"sampling_interval = "<<sampling_interval<<endl;
        cout<<"trigger position = "<<trig_pos<<endl;
        cout<<"finets = "<<finets<<endl;
        cout<<"clong = "<<clong<<endl;
        cout<<"pulse size = "<<pulse.size()<<endl;
        cout<<"pulse ap1 size = "<<pulse_ap1.size()<<endl;
        cout<<"pulse ap1 size = "<<pulse_ap2.size()<<endl;
    }
    UChar_t ts_corr_type;
    ULong64_t ts;
    UShort_t sampling_interval;
    UShort_t trig_pos;
    Short_t ch ;//channel number
    Double_t finets;//finets
    Double_t cshort;//charge short / energy 1
    Double_t clong;//charge long / energy 2
    Double_t baseline;//baseline
    Short_t nsample;
    std::vector<UShort_t> pulse;//pulse
    std::vector<UShort_t> pulse_ap1;//analogprobe1
    std::vector<UShort_t> pulse_ap2;//analogprobe2
    std::vector<UShort_t> pulse_dp1;//analogprobe1
    std::vector<UShort_t> pulse_dp2;//analogprobe2
    void processPulseV1740(ULong64_t boardts, Int_t nsbl, UShort_t ledthr,UShort_t trig_pos_in, UShort_t sampling_interval_in){
        ts = boardts;
        trig_pos = trig_pos_in;
        sampling_interval = sampling_interval_in;
        baseline = 0;
        clong  = 0;
        finets = -1;
        Int_t ii=0;
        for (std::vector<UShort_t>::iterator it = pulse.begin() ; it != pulse.end(); ++it){
            double currpulse =(double) *it;
            if (ii<nsbl)
                baseline+=currpulse;
            if (currpulse>clong) {
                clong = currpulse;
                cshort = ii;//position of maximum
            }
            if (currpulse>ledthr&&finets==-1){
                finets = ii;
            }
            ii++;
        }
        baseline = baseline /(Double_t)nsbl;
        //! calculate precise timing of channels
        if (finets>=0){
            ts = ts - trig_pos*sampling_interval+(ULong64_t)finets*sampling_interval;
        }

        //! special function to handle pulse ahead of trigger position
//        if (finets==0){//pulse with trigger
//            baseline = pulse.back();//last element as baseline
//        }
//        //! special function to reject pulse comes very late in time
//        if (cshort == pulse.size()-1&&pulse.size()>nsbl){
//            clong  = 0;
//        }
        //! only  pulse above threshold give energy>0
        if (finets>=0) clong  = clong  - baseline;
        else clong  = -9999;//no data above threshold
    };
    /// \cond CLASSIMP
    ClassDef(NIGIRIHit,1);
    /// \endcond
};

class NIGIRI : public TObject {
public:
    //! default constructor
    NIGIRI(){
        Clear();
    }
    virtual ~NIGIRI(){Clear();}
    void Clear(){
        event_size = 0;
        board_id = -1;
        board_fail_flag = 0;
        event_format_reserved = 0;
        pattern = 0;
        channel_mask = 0;
        trig_ch = -1;

        ts = 0;
        evt = 0;
        b = -1;
        fmult = 0;
        for (size_t idx=0;idx<fhits.size();idx++){
            delete fhits[idx];
        }
        fhits.clear();
    }
    void Print(){
        cout<<"******* Event header: *****"<<endl;
        cout<<"event_size = "<<event_size<<endl;
        cout<<"board_id = "<<board_id<<endl;
        cout<<"board_fail_flag = "<<board_fail_flag<<endl;
        cout<<"event_format_reserved = "<<event_format_reserved<<endl;
        cout<<"trig_ch = "<<trig_ch<<endl;
        cout<<"pattern = "<<pattern<<endl;
        cout<<"channel_mask = "<<channel_mask<<endl;
        cout<<"b = "<<b<<endl;
        cout<<"evt = "<<evt<<endl;
        cout<<"ts = "<<ts<<endl;
        cout<<"fmult = "<<fmult<<endl;
    }
    void DecodeHeaderZsp(int* words,int boardno){
        b = boardno;
        evt = words[2]+1;
        channel_mask = words[1];
        event_size = words[3]/4;
        UInt_t tslsb = (UInt_t) words[5];
        UInt_t tsmsb = (UInt_t) words[4];
        ts = (((ULong64_t)tsmsb<<32)&0xFFFF00000000)|(ULong64_t)tslsb;
        ts = ts*V1740_DGTZ_CLK_RES;
    }
    void DecodeHeaderRaw(int* words,int ipos, int boardno){
        b = boardno;
        event_size = words[ipos+0]&0xFFFFFFF;
        board_id = (words[ipos+1]&0xF8000000)>>27;
        board_fail_flag = (words[ipos+1]&0x4000000)>>26;
        event_format_reserved = (words[ipos+1]&0x1000000)>>24;
        pattern = (words[ipos+1]&0xFFFF00)>>8;
        int channel_mask_lsb = words[ipos+1]&0xFF;
        int channel_mask_msb = (words[ipos+2]&0xFF000000)>>24;
        channel_mask = (channel_mask_msb<<8)|channel_mask_lsb;
        evt = (words[ipos+2]&0xFFFFFF) +1;
        unsigned int trigger_time_tag = words[ipos+3]&0xFFFFFFFF;
        ts = ((unsigned long long)pattern)<<32|(unsigned long long)trigger_time_tag;
        ts = ts*V1740_DGTZ_CLK_RES;
    }
    void MergePulse(NIGIRI* data,ULong64_t boardts, Int_t nsbl, UShort_t* ledthr,UShort_t trig_pos_in, UShort_t sampling_interval_in){
        for (Int_t i=0;i<fmult;i++){
            GetHit(i)->nsample = GetHit(i)->pulse.size()+data->GetHit(i)->pulse.size();
            GetHit(i)->pulse.insert(std::end(GetHit(i)->pulse),std::begin(data->GetHit(i)->pulse),std::end(data->GetHit(i)->pulse));
            GetHit(i)->processPulseV1740(boardts,nsbl,ledthr[i],trig_pos_in,sampling_interval_in);
        }
    }

    virtual void Copy(NIGIRI& obj){
        obj.event_size = event_size;
        obj.board_id = board_id;
        obj.board_fail_flag = board_fail_flag;
        obj.event_format_reserved = event_format_reserved;
        obj.trig_ch = trig_ch;
        obj.pattern = pattern;
        obj.channel_mask = channel_mask;
        obj.evt=evt;
        obj.b=b;
        obj.ts=ts;
        for (vector<NIGIRIHit*>::iterator hitin_it=fhits.begin(); hitin_it!=fhits.end(); hitin_it++){
            NIGIRIHit* clonehit = new NIGIRIHit;
            NIGIRIHit* originhit = *hitin_it;
            originhit->Copy(*clonehit);
            obj.AddHit(clonehit);
        }
        obj.fmult=fmult;
    }
    virtual void CopyHeader(NIGIRI& obj){
        obj.event_size = event_size;
        obj.board_id = board_id;
        obj.board_fail_flag = board_fail_flag;
        obj.event_format_reserved = event_format_reserved;
        obj.trig_ch = trig_ch;
        obj.pattern = pattern;
        obj.channel_mask = channel_mask;
        obj.evt=evt;
        obj.b=b;
        obj.ts=ts;
    }
    Int_t GetMult(){return fmult;}
    NIGIRIHit* GetHit(unsigned short n){return fhits.at(n);}
    void AddHit(NIGIRIHit* hit){
        fmult++;
        fhits.push_back(hit);
    }
    Int_t fmult;

    //! common stuff
    Int_t event_size;
    Int_t board_id;
    Int_t board_fail_flag;
    Int_t event_format_reserved;
    Int_t pattern;
    Int_t channel_mask;
    Short_t trig_ch;

    Int_t evt;//evt number
    Short_t b;//board number
    ULong64_t ts;//timestamp
    std::vector<NIGIRIHit*> fhits;
    /// \cond CLASSIMP
    ClassDef(NIGIRI,1);
    /// \endcond
};

class AIDAFromNIGIRI{
public:
    AIDAFromNIGIRI(NIGIRI & obj, int runNum){
        //Clear maps and vectors
        aida_events.clear();
        xEvents.clear();
        yEvents.clear();
        int nx = 0;
        int ny=0;
        double threshold = 0;
        double match = 600;
        int finetsCut = 31;
        if ( runNum > 26){
          finetsCut = 75;
        }
        if ( board_ID_map[obj.b] == 4 ){
            threshold = 2700.0;
            match = 20e3;
        }
        WasabiStripData strip_event;
        //Get x and y strip events
        //Always 64 events in the vector
        for(auto x = 0; x < obj.fhits.size(); x++){
            if(obj.fhits.at(x)->clong > threshold) {
            if((board_ID_map[obj.b] == 4 && obj.fhits.at(x)->finets < finetsCut)  || board_ID_map[obj.b] == 5){
               if (obj.fhits.at(x)->ch < 32) {
                    nx++;
                    if(obj.fhits.at(x)->ch - strip_event.lastChannel == 1 || strip_event.lastChannel == -1) {
                        strip_event.UpdateEvent(obj.fhits.at(x)->ts, x, obj.fhits.at(x)->clong,
                                                board_dssd_map[obj.b], board_ID_map[obj.b], obj.fhits.at(x)->finets);
                    }
                    else {
                        xEvents.push_back(strip_event);
                        strip_event.ResetEvent();
                        strip_event.UpdateEvent(obj.fhits.at(x)->ts, x, obj.fhits.at(x)->clong,
                                                board_dssd_map[obj.b], board_ID_map[obj.b], obj.fhits.at(x)->finets);
                    }
                } else if ((obj.fhits.at(x)->ch >= 32)) {
                    ny++;
                    if ((obj.fhits.at(x)->ch - strip_event.lastChannel == 1 && strip_event.lastChannel > 31) || strip_event.lastChannel == -1){
                        strip_event.UpdateEvent(obj.fhits.at(x)->ts, x, obj.fhits.at(x)->clong,
                                                board_dssd_map[obj.b], board_ID_map[obj.b], obj.fhits.at(x)->finets);
                    }
                    else {
                        if(strip_event.lastChannel<32){
                            xEvents.push_back(strip_event);
                            strip_event.ResetEvent();
                            strip_event.UpdateEvent(obj.fhits.at(x)->ts, x, obj.fhits.at(x)->clong,
                                                    board_dssd_map[obj.b], board_ID_map[obj.b], obj.fhits.at(x)->finets);
                        }
                        else {
                            yEvents.push_back(strip_event);
                            strip_event.ResetEvent();
                            strip_event.UpdateEvent(obj.fhits.at(x)->ts, x, obj.fhits.at(x)->clong,
                                                    board_dssd_map[obj.b], board_ID_map[obj.b],obj.fhits.at(x)->finets);
                        }
                    }
                }
            }
            }
        }
        if(strip_event.lastChannel<32 && strip_event.lastChannel > -1){
            xEvents.push_back(strip_event);
        }
        else if(strip_event.lastChannel>31){
            yEvents.push_back(strip_event);
        }

        //Match front and back events
        for(auto & xEvent : xEvents){
            for(auto & yEvent : yEvents){

                if(abs(xEvent.energy - yEvent.energy)<match){
                    //Paired event
                    AidaTreeData aidaEvent(xEvent, yEvent, nx,ny);
                    aida_events.emplace(aidaEvent.T, aidaEvent);
                }
            }
        }
    };

    multimap<ULong64_t, AidaTreeData> GetAIDAEvents(){
        return aida_events;
    };
    //Should be a multimap
    multimap<ULong64_t ,AidaTreeData> aida_events;
    double board_dssd_map[8] ={0.,1.,2.,3.,0.,1.,2.,3.};
    UChar_t board_ID_map[8] = {5,5,5,5,4,4,4,4};
    //Vectors for strip events
    vector<WasabiStripData> xEvents;
    vector<WasabiStripData> yEvents;


};

class dEFromNIGIRI{
public:
    dEFromNIGIRI(NIGIRI & obj){
        aida_events.clear();
        for(auto hit : obj.fhits){
            if (hit->clong > 0 && hit->ch >0 && hit->ch < 11){
                AidaTreeData aidaEvent(detectorMap[hit->ch], hit->clong, hit->ts);
                aida_events.emplace(aidaEvent.T, aidaEvent);
            }
        }
        for (auto eventIt : aida_events){
            eventIt.second.nz = aida_events.size();
        }

    };
    multimap<ULong64_t ,AidaTreeData> GetdEEvents() const{
        return aida_events;
    }
    multimap<ULong64_t ,AidaTreeData> aida_events;
    double detectorMap[11]= {0.0,1.,2.,3.,4.,5.,6.,7.,8.,9.,0.};
};

#endif

