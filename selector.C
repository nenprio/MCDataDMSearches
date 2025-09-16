#define selector_cxx
// The class definition in unn.h has been generated automatically
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
// root> T->Process("selector.C")
// root> T->Process("selector.C","some options")
// root> T->Process("selector.C+")
//

#include "TLegend.h"
#include "selector.h"
#include <TH2.h>
#include <TStyle.h>

void selector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
}

void selector::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   h_ID1 = new TH1F("ID_1","#mu^{+}#mu^{-}",280,150,1550);
   fOutput->Add(h_ID1);

   h_ID2 = new TH1F("ID_2","#mu^{+}#mu^{-}",280,150,1550);
   fOutput->Add(h_ID2);

   h_ID3 = new TH1F("ID_3","#mu^{+}#mu^{-}",280,150,1550);
   fOutput->Add(h_ID3);

   h_ID4 = new TH1F("ID_4","#mu^{+}#mu^{-}",100,0,1);
   fOutput->Add(h_ID4);

}

Bool_t selector::Process(Long64_t entry)
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
   //GetEnrty(entry);

    if(*mun_ProbNNmu > 0.95 && *mup_ProbNNmu > 0.95){
      if(*mup_PT > 10 && *mun_PT > 10){
	if(*mun_OWNPV_CHI2 < 8 && *mup_OWNPV_CHI2 < 8) h_ID1->Fill(*A_MM);
        if(*mun_OWNPV_CHI2 < 16 && *mup_OWNPV_CHI2 < 16) h_ID2->Fill(*A_MM);
        if(*mun_OWNPV_CHI2 < 30 && *mup_OWNPV_CHI2 < 30) h_ID3->Fill(*A_MM);
       }
    }

   return kTRUE;
}

void selector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void selector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
   TH1F *fHist_ID1 = dynamic_cast<TH1F *>(fOutput->FindObject(Form("ID_1")));
   TH1F *fHist_ID2 = dynamic_cast<TH1F *>(fOutput->FindObject(Form("ID_2")));
   TH1F *fHist_ID3 = dynamic_cast<TH1F *>(fOutput->FindObject(Form("ID_3")));
   //TH1F *fHist_ID4 = dynamic_cast<TH1F *>(fOutput->FindObject(Form("ID_4")));


   if (fHist_ID1 && fHist_ID2 && fHist_ID3) {
      TCanvas *ID = new TCanvas("ID","ID",200,10,900,700);
      gStyle->SetOptStat(0);
      //TPad *pad1 = new TPad("pad1","The pad with the function",0.05,0.1,0.95,0.95,21);       
      fHist_ID3->SetTitle(" ");
      //pad1->Draw();
      fHist_ID3->SetLineColor(kRed);
      fHist_ID1->SetLineColor(kGreen);
      fHist_ID3->Draw("hist");
      fHist_ID1->Draw("same hist");
      fHist_ID3->GetXaxis()->SetTitle("ProbNNmu");
      fHist_ID3->GetXaxis()->SetTitleOffset(1.0);
      fHist_ID3->GetYaxis()->SetTitleOffset(1.4);
      fHist_ID3->GetYaxis()->SetTitle("Candidates/5 [MeV/c^2]");

      TLegend *leg1 = new TLegend(0.65, 0.65, 0.89, 0.89);
      leg1->SetFillColor(kWhite);
      leg1->SetLineColor(kWhite);
      //leg1->AddEntry(fHist_ID1,"mup_ProbNNmu");
      //leg1->AddEntry(fHist_ID3,"mun_ProbNNmu");
      //leg1->AddEntry(fHist_ID3,"ProbNNmu > 0.9");
      leg1->Draw();

      //gPad->SetLogx();
      //gPad->SetLogy();

      ID->cd();
      ID->Update();

      TFile *outHistFile = TFile::Open("result.root","RECREATE");
      outHistFile->cd();
      fHist_ID1->Write();
      fHist_ID2->Write();
      fHist_ID3->Write();
      outHistFile->Close();
   } else {
      Warning("Terminate", "histogram not found");
   }

}

