#include <TCanvas.h>
#include <TFile.h>
#include <TH1F.h>
#include <TMath.h>
#include <TROOT.h>

#include <iostream>
#include <vector>

TH1F* eff(std::vector<double>& bins, bool draw = false) {
  gROOT->Reset();

  std::string MCFile =
      "Results/LAMBDA_CUTS_4/AnalysisResults_optimal_cuts_MC.root";
  TFile* f = new TFile(MCFile.c_str());

  // save generated pt histogram as hpt
  f->cd("strangeness_tutorial/genParticles");
  TH1F* hpt = (TH1F*)gDirectory->Get("hPtLambdaGen");
  if (hpt == nullptr) {
    std::cout << "hpt was not read" << std::endl;
    exit(1);
  }
  hpt->Sumw2();
  Double_t norm1 = hpt->GetEntries();
  std::cout << norm1 << std::endl;

  // save true reconstructed pt histogram as hptrue
  f->cd("strangeness_tutorial/kLambda");
  TH1F* hptrue = (TH1F*)gDirectory->Get("hPtLambdaTrueRec");
  if (hptrue == nullptr) {
    std::cout << "hptrue was not read" << std::endl;
    exit(1);
  }
  hptrue->Sumw2();
  Double_t norm2 = hptrue->GetEntries();
  std::cout << norm2 << std::endl;

  // Rebin the histograms using custom bins
  // create a new histogram with the custom bins
  TH1F* hpt_rebin =
      new TH1F("hpt_rebin", "hpt_rebin", bins.size() - 1, &bins[0]);

  TH1F* hptrue_rebin =
      new TH1F("hptrue_rebin", "hptrue_rebin", bins.size() - 1, &bins[0]);

  for (size_t i = 0; i < bins.size() - 1; ++i) {
    double binLow = bins[i] + 1e-10;
    double binHigh = bins[i + 1] - 1e-10;
    // find integral between bins[i] and bins[i+1]
    int count = hpt->Integral(hpt->FindBin(binLow), hpt->FindBin(binLow));
    std::cout << "Bin " << i << " has " << count << " entries" << std::endl;
    hpt_rebin->SetBinContent(i + 1, count);

    count = hptrue->Integral(hptrue->FindBin(binLow), hptrue->FindBin(binLow));
    std::cout << "Bin " << i << " has " << count << " entries" << std::endl;
    hptrue_rebin->SetBinContent(i + 1, count);
  }

  hpt_rebin->Sumw2();
  hptrue_rebin->Sumw2();

  // Draw heff on the canvas
  TH1F* heff = new TH1F("heff", "heff", bins.size() - 1, &bins[0]);
  heff->SetTitle("Efficiencies");
  heff->Divide(hptrue_rebin, hpt_rebin);
  if (draw) {
    TCanvas* c1 = new TCanvas("c1", "c1", 900, 600);
    heff->Draw();
    // dont draw the legend
    heff->SetStats(0);
    c1->SaveAs("efficiency.pdf");
  }
  return heff;
}

void EfficiencyCorrectionLambda() {
  std::vector<double> bins = {0, 0.4, 0.8, 1.2, 1.6, 2, 3, 4, 5, 6, 7};
  bool draw = true;

  double ptCut = 0;

  std::string DataFile =
      "Results/LAMBDA_CUTS_4/AnalysisResults_optimal_cuts_DATA_p_t.root";
  TFile* f = new TFile(DataFile.c_str());

  // save pt histogram as hpt
  f->cd("strangeness_tutorial/kLambda");
  TH1F* hpt = (TH1F*)gDirectory->Get("hPtLambdaSelected");
  if (hpt == nullptr) {
    std::cout << "hpt was not read" << std::endl;
    exit(1);
  }
  hpt->Sumw2();
  Double_t norm1 = hpt->GetEntries();
  std::cout << norm1 << std::endl;

  // Rebin the histograms using custom bins
  // create a new histogram with the custom bins
  // filter out the bins that are below the ptCut
  std::vector<double> binsFiltered;
  for (size_t i = 0; i < bins.size() - 1; ++i) {
    if (bins[i] >= ptCut) {
      binsFiltered.push_back(bins[i]);
    }
  }

  TH1F* heff = eff(binsFiltered, draw);
  TH1F* hpt_rebin = new TH1F("hpt_rebin", "hpt_rebin", binsFiltered.size() - 1,
                             &binsFiltered[0]);

  for (size_t i = 0; i < binsFiltered.size() - 1; ++i) {
    double binLow = binsFiltered[i] + 1e-10;
    double binHigh = binsFiltered[i + 1] - 1e-10;
    // find integral between binsFiltered[i] and binsFiltered[i+1]
    int count = hpt->Integral(hpt->FindBin(binLow), hpt->FindBin(binLow));
    std::cout << "Bin " << i << " has " << count << " entries" << std::endl;
    hpt_rebin->SetBinContent(i + 1, count);
  }

  hpt_rebin->Sumw2();

  // Efficiency correction
  // Create a new canvas
  TCanvas* c2 = new TCanvas("c2", "c2", 900, 600);

  // Draw heff on the canvas
  TH1F* hpt_corrected = new TH1F("hpt_corrected", "hpt_corrected",
                                 binsFiltered.size() - 1, &binsFiltered[0]);
  hpt_corrected->SetTitle("Efficiency corrected pT");
  hpt_corrected->Divide(hpt_rebin, heff);

  // stop x axis orign from being displaced
  hpt_corrected->GetXaxis()->SetRangeUser(-0.1, 7);

  hpt_corrected->Draw();
  // dont draw the legend
  hpt_corrected->SetStats(0);
  c2->SaveAs("efficiency_corrected.pdf");
}