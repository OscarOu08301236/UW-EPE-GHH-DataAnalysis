#define AnalyzeVariables_cxx
// The class definition in AnalyzeVariables.h has been generated automatically
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
// root> T->Process("AnalyzeVariables.C")
// root> T->Process("AnalyzeVariables.C","some options")
// root> T->Process("AnalyzeVariables.C+")
//

#include "AnalyzeVariables.h"
#include <TH2.h>
#include <TStyle.h>

TH1* hist1 = NULL;
TH1* hist2 = NULL;

TCanvas* ptCanvas = NULL;
TCanvas* thetaCanvas = NULL;

void AnalyzeVariables::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

    hist1 = new TH1D("pt","Histogram of pt", 100, 0, 40);
    hist1->GetXaxis()->SetTitle("pt");
    hist1->GetYaxis()->SetTitle("momenta(GeV)");
    ptCanvas = new TCanvas("c1", "pt canvas", 10, 10, 700, 500);
    
    hist2 = new TH1D("theta","Histogram of theta", 100, 0, 0.22);
    hist2->GetXaxis()->SetTitle("theta");
    hist2->GetYaxis()->SetTitle("value");
    thetaCanvas = new TCanvas("c2", "theta canvas", 10, 10, 700, 500);
    
    TString option = GetOption();
}

void AnalyzeVariables::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t AnalyzeVariables::Process(Long64_t entry)
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

    fReader.SetLocalEntry(entry);
    
    GetEntry(entry);
    
    // take px and py from the data and calculate pt
    Double_t pt = TMath::Sqrt((*px)*(*px) + (*py)*(*py));
    hist1->Fill(pt);
    
    // take pt and pz to calculate theta
    Double_t theta = TMath::ATan2(pt,*pz);
    hist2->Fill(theta);

    return kTRUE;
}

void AnalyzeVariables::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void AnalyzeVariables::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

    ptCanvas->cd();
    hist1->Draw("e");
    
    thetaCanvas->cd();
    hist2->Draw("e");
}
