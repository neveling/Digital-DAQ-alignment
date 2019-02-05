#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <iostream>
#include <TTree.h>
#include <TGraph.h>
#include <TCanvas.h>

int MidasTac;
int MidasEvtNum;
double X1pos;

double DigTacEnergy;
int DigEvtNum;
double DigTrigTime;
double DigGeRawEnergy[48];
double DigGeTime[48];

double lowratio1=0.02;
double hiratio1=1.0;
double lowratio2=1.4;
double hiratio2=10.0;

int FindNextDigitalEvent(int MidasEventNumber, int DigitalEventSkip, TTree* MidasTree, TTree *DigitalTree);

void FileComparePA()
{
    TCanvas *c1 = new TCanvas("c1","c1",800,800);
    
    TFile *midasfile = new TFile("sorted00337.root");
    TTree *midastree = (TTree*)midasfile->Get("DATA");
    TFile *digfile = new TFile("runBaGeL_337-tree.root");
    TTree *digtree = (TTree*)digfile->Get("DATA"); 
    
    TH2D* hTacVsEvtJJ = new TH2D("hTacVsEvtJJ","TAC vs evt num for dig daq",100,2300.,2400.,1000,0.,1000.);
    TH2D* hTacVsEvtK600 = new TH2D("hTacVsEvtK600","TAC vs evt num for K600 daq",100,2300.,2400.,1000,0.,1000.);
    TH2D* hTacVsEvtJJC = new TH2D("hTacVsEvtJJC","corrected TAC vs evt num for dig daq",200,2600.,2800.,1000,0.,1000.);
    TH2D* hTacVsEvtK600C = new TH2D("hTacVsEvtK600C","corrected TAC vs evt num for K600 daq",200,2600.,2800.,1000,0.,1000.);
    
    TH1D* hTACJJ = new TH1D("hTACJJ","",600,0.,12000.);
    TH1D* hTACK600 = new TH1D("hTACK600","",500,0.,850.);
    
    TH2D* hEventVsRatio = new TH2D("hEventVsRatio","Event number vs Ratio between midas and digital DAQs",20000,0,20000,1000,0,10);
    TH2D* hEventVsRatioC = new TH2D("hEventVsRatioC","Event number vs Ratio between midas and digital DAQs",20000,0,20000,1000,0,10);
    
    TH2D* hEGevsX = new TH2D("hEGevsX","Dig DAQ HPGe vs X1",800,0,800,1000,0,30000);
    
    TGraph *gTACValues = new TGraph();
    gTACValues->SetName("gTACValues");
    
    midastree->SetBranchAddress("TAC",&MidasTac);
    midastree->SetBranchAddress("evtcounter",&MidasEvtNum);
    midastree->SetBranchAddress("X1pos",&X1pos);
    
    digtree->SetBranchAddress("tac_energy",&DigTacEnergy);
    digtree->SetBranchAddress("Evtnum",&DigEvtNum);
    digtree->SetBranchAddress("trig_time",&DigTrigTime);
    digtree->SetBranchAddress("Ge_rawEnergy",&DigGeRawEnergy[0]);
    digtree->SetBranchAddress("Ge_time",&DigGeTime[0]);
    
    int totalentriesmidas = midastree->GetEntries();
    int totalentriesdig = digtree->GetEntries();
    
    float TACratio=0;
    double tst=0;
    int counter=0;
    int tstevtnr1=-100;
    int tstevtnr2=-100;
    double GeE;
    double X1;
    double trigtime=-100;
    double getime[48];
    double getime1,getime2;
    double timediff;
    double tacratio=0;
    int skip=0;
    int badflag=0;
    int printlownum=2250;
    int printhinum=3300;
    int TACGraphcounter = 0;
    int DigitalEventSkip = 0;
    
    for(int i=10; i<totalentriesmidas/2; i++)
    {
        digtree->GetEntry(i);
        midastree->GetEntry(i);
        tacratio = 0.08*DigTacEnergy/(MidasTac-200);
        //         tacratioPA = (DigTacEnergy+2524.64)/13.358/MidasTac;
        hEventVsRatio->Fill(MidasEvtNum,tacratio); 
    }
    
    for(int i=10; i<totalentriesmidas/2; i++)
//     for(int i=0; i<3300; i++)
    {
        
        digtree->GetEntry(i+DigitalEventSkip);
        hTacVsEvtJJ ->Fill(DigEvtNum,0.08*DigTacEnergy);
        hTACJJ ->Fill(0.08*DigTacEnergy);
        
        midastree->GetEntry(i);
        hTacVsEvtK600 ->Fill(MidasEvtNum,MidasTac-200); 
        hTACK600->Fill(MidasTac);
        
        
        tacratio = 0.08*DigTacEnergy/(MidasTac-200);
        
        //hEventVsRatio->Fill(MidasEvtNum,tacratio); 
        
        if(MidasEvtNum>printlownum && MidasEvtNum<printhinum)
        {
            cout<<" step i = "<< i << " with DigitalEventSkip = "<< DigitalEventSkip << "  and tacratio = " << tacratio<< endl;
        }
        
        if((tacratio>lowratio1 && tacratio<hiratio1) || tacratio>lowratio2)
        {
            DigitalEventSkip = FindNextDigitalEvent(i,DigitalEventSkip,midastree,digtree);
            
            digtree->GetEntry(i+DigitalEventSkip);
            tacratio = 0.08*DigTacEnergy/(MidasTac-200);
        }
        
        /*
         *        if( (tacratio>lowratio1 && tacratio<hiratio1) || (tacratio>lowratio2 && tacratio<hiratio2) )
         *        {
         *            digtree->GetEntry(i+skip+1);
         *            skip++;
         *            tacratio = 0.08*DigTacEnergy/(MidasTac-200);
         *            if(MidasEvtNum>printlownum && MidasEvtNum<printhinum)
         *            {
         *                cout<<"first if: step i = "<< i << " with skip = "<< skip << "  and tacratio = " << tacratio<< endl;
    }
    
    
    if( (tacratio>lowratio1 && tacratio<hiratio1) || (tacratio>lowratio2) )
    {
    digtree->GetEntry(i+skip+2);
    skip++;
    tacratio = 0.08*DigTacEnergy/(MidasTac-200);
    if(MidasEvtNum>printlownum && MidasEvtNum<printhinum)
    {
    cout<<"2nd if:  step i = "<< i << " with skip = "<< skip << "  and tacratio = " << tacratio<< endl;
    }
    
    if( (tacratio>lowratio1 && tacratio<hiratio1) || (tacratio>lowratio2) )
    {
    digtree->GetEntry(i+skip+3);
    skip++;
    tacratio = 0.08*DigTacEnergy/(MidasTac-200);
    if(MidasEvtNum>printlownum && MidasEvtNum<printhinum)
    {
    cout<<"3rd if:  step i = "<< i << " with skip = "<< skip << "  and tacratio = " << tacratio<< endl;
    }
    
    if( (tacratio>lowratio1 && tacratio<hiratio1) || (tacratio>lowratio2) )
    {
    digtree->GetEntry(i+skip+4);
    skip++;
    tacratio = 0.08*DigTacEnergy/(MidasTac-200);
    if(MidasEvtNum>printlownum && MidasEvtNum<printhinum)
    {
    cout<<"4th if:  step i = "<< i << " with skip = "<< skip << "  and tacratio = " << tacratio<< endl;
    }
    
    if( (tacratio>lowratio1 && tacratio<hiratio1) || (tacratio>lowratio2) )
    {
    digtree->GetEntry(i+skip+5);
    skip++;
    tacratio = 0.08*DigTacEnergy/(MidasTac-200);
    if(MidasEvtNum>printlownum && MidasEvtNum<printhinum)
    {
    cout<<"5th if:  step i = "<< i << " with skip = "<< skip << "  and tacratio = " << tacratio<< endl;
    }
    }
    }
    }
    }
    }
    */
        
        if( (tacratio>lowratio1 && tacratio<hiratio1) || (tacratio>lowratio2) )
        {		
            cout<<"Bad tac ratio at  i = "<< i << " with skip = "<< skip << "  and tacratio = " << tacratio<< endl;
            
            cout << "**********" << endl << endl;
            //break;
            //badflag=1;
        }
        else if(tacratio>0)
        {
            gTACValues->SetPoint(TACGraphcounter,MidasTac,DigTacEnergy);
            TACGraphcounter++;
        }
        
        //hEventVsRatioC->Fill(MidasEvtNum,tacratio); 
        if(DigTacEnergy!=0.0)
        {
            hEventVsRatioC->Fill(MidasEvtNum,tacratio); 
            hTacVsEvtJJC ->Fill(DigEvtNum-skip,0.08*DigTacEnergy);
            hTacVsEvtK600C ->Fill(MidasEvtNum,MidasTac-200); 
            
            hEGevsX->Fill(X1pos,DigGeRawEnergy[11]);
        }
        
    }
    
    //hTACK600->Draw("col");
    //hTACJJ->SetLineColor(2);
    //hTACJJ->Draw("same");
    
    hEventVsRatioC->Draw("col");
    
    hTacVsEvtJJC->Draw("box");
    hTacVsEvtK600->SetLineColor(3);
    hTacVsEvtK600C->Draw("same,col");
    
    //hEGevsX->Draw("col");
    gTACValues->Draw("A*");
    //     gTACValues->Fit("pol1");
    c1->Update();
    
    hEventVsRatioC->Draw("col");
}

int FindNextDigitalEvent(int MidasEventNumber, int DigitalEventSkip, TTree* MidasTree, TTree *DigitalTree)
{
    cout << "Bad TAC ratio - going to try to find the next good digital TAC event" << endl;
    
    int result = DigitalEventSkip; //Return number of skipped events to get a good correlation again
    
    cout << "Starting with skipping " << result << " events" << endl;    
    
    bool MatchEvents = false;
    bool PositiveMove = true;
    int StepSize = 1;
    
    while(abs(result-DigitalEventSkip)<100 && !MatchEvents)
    {
        cout << "DigitalEventSkip = " << result << endl;
        cout << "This is " << result-DigitalEventSkip << " more skips since the last matching event" << endl;
        
        MidasTree->GetEntry(MidasEventNumber);
        DigitalTree->GetEntry(MidasEventNumber+result);
        
        double tacratio = 0.08*DigTacEnergy/(MidasTac-200);
        cout << "tacratio: " << tacratio << endl;
        
        if(!((tacratio>lowratio1 && tacratio<hiratio1) || tacratio>lowratio2))
        {
            MatchEvents = true;
            cout << "Found GOOD matching event" << endl;
        }
        else if(PositiveMove)
        {
            result = result+StepSize;
            PositiveMove = false;
        }
        else if(!PositiveMove)
        {
            result = result - StepSize;
            PositiveMove = true;
        }
        StepSize++;
    }
    
    cout << endl;
    
    return result;
}



/*
 * TH2F* hTimeDiff2D = new TH2F("hTimeDiff2D","Time between trigger and last Ge event",2000,-10000.,10000.,48,0.,48.);
 * TH1F* hTimeDiff   = new TH1F("hTimeDiff",  "Time between trigger and last Ge event",2000,-10000.,10000.);
 * 
 * hTacVsEvtJJ->Draw("col");
 * hTacVsEvtK600->SetLineColor(3);
 * hTacVsEvtK600->Draw("same,col");
 * 
 * 
 * 
 * 
 * 
 */
