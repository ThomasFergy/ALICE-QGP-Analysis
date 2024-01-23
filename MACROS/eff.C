#include <TCanvas.h>
#include <TFile.h>
#include <TH1F.h>
#include <TROOT.h>

#include <iostream>

void eff() {
  gROOT->Reset();

  // MC with optimal cuts
  TFile* f = new TFile(
      "Results/K0_CUTS_1/"
      "AnalysisResults_optimal_cuts_MC.root");
  f->cd("strangeness_tutorial/genParticles");
  TH1F* h1 = (TH1F*)gDirectory->Get("hPtK0ShortGen");
  if (h1 == nullptr) {
    std::cout << "h1 was not read" << std::endl;
    exit(1);
  }
  h1->Sumw2();
  Double_t norm1 = h1->GetEntries();
  std::cout << norm1 << std::endl;
  f->cd("strangeness_tutorial/kzeroShort");
  TH1F* h2 = (TH1F*)gDirectory->Get("hPtK0ShortTrueRec");
  if (h2 == nullptr) {
    std::cout << "h2 was not read" << std::endl;
    exit(1);
  }
  h2->Sumw2();
  Double_t norm2 = h2->GetEntries();
  std::cout << norm2 << std::endl;
  // Efficiency
  // Creat a new canvas
  TCanvas* c1 = new TCanvas("c1", "c1", 900, 600);
  // Draw heff on the canvas
  TH1F* heff = new TH1F(*h2);
  heff->Divide(h1);
  heff->Draw();
  // Save the canvas
  c1->SaveAs("efficiency.pdf");
}