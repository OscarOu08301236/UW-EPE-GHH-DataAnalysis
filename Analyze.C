#define Analyze_cxx
// The class definition in Analyze.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("Analyze.C")
// root> T->Process("Analyze.C","some options")
// root> T->Process("Analyze.C+")
//


#include "Analyze.h"
#include <TH2.h>
#include <TStyle.h>

//Definition section

TH1* chi2Hist = NULL;
TH1* hist2 = NULL;
TH1* scatterplot1 = NULL;
TH1* scatterplot2 = NULL;
TH1* scatterplot3 = NULL;

TCanvas* c1 = new TCanvas("c1", "c1",134,254,700,502);

void Analyze::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   //Initialization section
    chi2Hist = new TH1D("chi2","Histogram of Chi2", 100, 0, 20);
    chi2Hist->GetXaxis()->SetTitle("chi2");
    chi2Hist->GetYaxis()->SetTitle("number of events");
    
    hist2 = new TH1D("ebeam","Histogram of Ebeam", 100, 149, 151);
    hist2->GetXaxis()->SetTitle("ebeam");
    hist2->GetYaxis()->SetTitle("number of events");
    
    scatterplot1 = new TH2D("chi2 versus ebea","scatterplot of chi2 versus ebeam", 100, 0, 20, 100, 149, 151);
    scatterplot1->GetXaxis()->SetTitle("chi2");
    scatterplot1->GetYaxis()->SetTitle("ebeam");
    
    scatterplot2 = new TH2D("chi2 versus theta","scatterplot of chi2 versus theta", 100, 0, 2, 100, 0, 0.22);
    scatterplot2->GetXaxis()->SetTitle("chi2");
    scatterplot2->GetYaxis()->SetTitle("theta");
    
    scatterplot3 = new TH2D("Eloss versus zv","scatterplot of Eloss versus zv", 100, 0, 30, 100, 0, 150);
    scatterplot3->GetXaxis()->SetTitle("Eloss");
    scatterplot3->GetYaxis()->SetTitle("zv");
        
    TString option = GetOption();
}

void Analyze::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

    TString option = GetOption();

}

Bool_t Analyze::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   //Don’t delete this line! Without it the program will crash.
    fReader.SetLocalEntry(entry);
   
   //Loop section
   //Probably want GetEntry(entry) here.
    GetEntry(entry);
    chi2Hist->Fill(*chi2);
    hist2->Fill(*ebeam);
    scatterplot1->Fill(*chi2, *ebeam);
    
    Double_t pt = TMath::Sqrt((*px)*(*px) + (*py)*(*py)); // calculate pt
    Double_t theta = TMath::ATan2(pt,*pz); // calculate theta
    if (*chi2 < 1.5 && theta < 0.15) // apply cut to theta and chi2
    {
        scatterplot2->Fill(*chi2, theta);
    }

    Double_t emean = TMath::Sqrt((*px)*(*px) + (*py)*(*py) + (*pz)*(*pz)); // calculate emean
    Double_t eloss = *ebeam - emean; // calculate eloss
    scatterplot3->Fill(eloss, *zv);
    
    return kTRUE;
}

void Analyze::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void Analyze::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

   //Warp-up section
    c1->Divide(4,1); // create mutiple plot in one canvas
    
    c1->cd(1); // go to the first graph section of canvas c1
    chi2Hist->Draw("e");
    
    c1->cd(2);
    hist2->Fit("gaus");
    gStyle->SetOptFit(1011);
    hist2->Draw("e");
    
    c1->cd(3);
    scatterplot1->Draw();
    
    c1->cd(4);
    scatterplot3->Draw();
    
    // create output file to store the grpah
    TFile* outputfile = new TFile("analysis.root", "recreate");
    scatterplot1->Write();
    scatterplot2->Write();
    outputfile->Close();
}
