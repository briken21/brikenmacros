#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <pthread.h>

#include <iostream>

#include "TThread.h"
#include "TCanvas.h"
#include "TH1.h"

TCanvas * c1=0;
TH1F* hrate;

#define FIFOPATH "/tmp/myratefifo"

struct refesh_thread_argument
{
  unsigned int refreshinterval;
};
static TThread *refesh_thread = 0;
static TThread *fifo_thread = 0;

void rateupdate(void * ptr){
    refesh_thread_argument* ta = (refesh_thread_argument *) ptr;
    unsigned int my_refreshinterval = ta->refreshinterval;
    time_t last_time = time(0);  //force an update on start
    //cout<<"refesh thread initialized, current time since January 1, 1970"<<last_time<<endl;
    while (1)
    {
        time_t x = time(0);
        if ( x - last_time > my_refreshinterval)
      {
        last_time = x;
        //std::cout<<"update"<<std::endl;
        if (c1!=0){
            c1->cd(0);
            c1->Modified();
            c1->Update();
        }
       }
    }
}

void viewfifo(void* ptr){

    std::cout<<"View FIFO"<<std::endl;
    /* Open FIFO FD */
    int fifopipe;
    if((fifopipe = open(FIFOPATH, O_RDWR)) == -1){
      printf("makefifo: Can't open %s\n", FIFOPATH);
    }
    int bufin=10;
    Int_t i=0;
    c1=new TCanvas("c1","c1",990,700);
    hrate=new TH1F("hrate","hrate",3600,0,3600);
    hrate->SetFillColor(2);
    c1->cd(0);
    hrate->Draw("hist");
    while(1){
        //usleep(1000000);
        ssize_t nbytesread = read(fifopipe, (char *)&bufin, sizeof(bufin));
        if (nbytesread==0) {
            std::cout<<"fifo read error!"<<std::endl;
            return 0;
        }
        int boardn=(int)((bufin&0xFF00000)>>24);
        double raterate = (double)(bufin&0xFFFFFF)/1000.;
        //std::cout<<"b="<<boardn<<"rate="<<raterate<<std::endl;
//        c1->cd(0);
        hrate->Fill(i,10);
        i++;
    }
}

void viewfifos()
{
    refesh_thread_argument* ta = new refesh_thread_argument;
    ta->refreshinterval = 3;

    fifo_thread = new TThread(viewfifo,ta);
    refesh_thread = new TThread(rateupdate,ta);
    fifo_thread->Run();
    refesh_thread->Run();



}
