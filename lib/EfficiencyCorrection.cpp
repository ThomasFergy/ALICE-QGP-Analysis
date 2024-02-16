#include "EfficiencyCorrection.h"

TH1F* EfficiencyCorrection::eff(std::string MCFile, std::vector<double>& bins,
                                V0Type v0Type, bool draw) {
  gROOT->Reset();

  TFile* f = new TFile(MCFile.c_str());

  // save generated pt histogram as hpt
  f->cd("strangeness_tutorial/genParticles");
  TH1F* hpt = nullptr;
  switch (v0Type) {
    case V0Type::K0:
      hpt = (TH1F*)gDirectory->Get("hPtK0ShortGen");
      break;
    case V0Type::Lambda:
      hpt = (TH1F*)gDirectory->Get("hPtLambdaGen");
      break;
    case V0Type::AntiLambda:
      hpt = (TH1F*)gDirectory->Get("hPtAntiLambdaGen");
      break;
    case V0Type::LambdaAntiLambda:
      hpt = (TH1F*)gDirectory->Get("hPtLambdaAntiLambdaGen");
      break;
      // default:
      //   std::cout << "Invalid V0Type: " << static_cast<int>(v0Type) <<
      //   std::endl;
      //  exit(1);
      // break;
  }

  if (hpt == nullptr) {
    std::cout << "hpt was not read" << std::endl;
    // say which file and which histogram was not read
    std::cout << MCFile << std::endl;
    exit(1);
  }
  hpt->Sumw2();

  // save true reconstructed pt histogram as hptrue
  TH1F* hptrue = nullptr;
  switch (v0Type) {
    case V0Type::K0:
      f->cd("strangeness_tutorial/kzeroShort");
      hptrue = (TH1F*)gDirectory->Get("hPtK0ShortTrueRec");
      break;
    case V0Type::Lambda:
      f->cd("strangeness_tutorial/kLambda");
      hptrue = (TH1F*)gDirectory->Get("hPtLambdaTrueRec");
      break;
    case V0Type::AntiLambda:
      f->cd("strangeness_tutorial/kAntiLambda");
      hptrue = (TH1F*)gDirectory->Get("hPtAntiLambdaTrueRec");
      break;
    case V0Type::LambdaAntiLambda:
      f->cd("strangeness_tutorial/kLambdaAndAntiLambda");
      hptrue = (TH1F*)gDirectory->Get("hPtLambdaAntiLambdaTrueRec");
      // default:
      //   std::cout << "Invalid V0Type: " << static_cast<int>(v0Type) <<
      //   std::endl;
      // exit(1);
      // break;
  }

  if (hptrue == nullptr) {
    std::cout << "hptrue was not read" << std::endl;
    // say which file and which histogram was not read
    std::cout << MCFile << std::endl;
    exit(1);
  }
  hptrue->Sumw2();

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
    hpt_rebin->SetBinContent(i + 1, count);
    hpt_rebin->SetBinError(i + 1, TMath::Sqrt(count));

    count = hptrue->Integral(hptrue->FindBin(binLow), hptrue->FindBin(binLow));
    hptrue_rebin->SetBinContent(i + 1, count);
    hptrue_rebin->SetBinError(i + 1, TMath::Sqrt(count));
  }

  // Draw heff on the canvas
  TH1F* heff = new TH1F("heff", "heff", bins.size() - 1, &bins[0]);
  heff->SetTitle("Efficiencies");
  heff->Divide(hptrue_rebin, hpt_rebin);
  if (draw) {
    TCanvas* c1 = new TCanvas("c1", "c1", 900, 600);
    heff->Draw();
    // dont draw the legend
    heff->SetStats(0);
    std::string fileName = "Efficiency_";
    switch (v0Type) {
      case V0Type::K0:
        fileName += "K0.pdf";
        break;
      case V0Type::Lambda:
        fileName += "Lambda.pdf";
        break;
      case V0Type::AntiLambda:
        fileName += "AntiLambda.pdf";
        break;
      case V0Type::LambdaAntiLambda:
        fileName += "LambdaAntiLambda.pdf";
        break;
        // default:
        //  std::cout << "Invalid V0Type: " << static_cast<int>(v0Type)
        //            << std::endl;
        //  exit(1);
        //  break;
    }
    c1->SaveAs(fileName.c_str());
    delete c1;
  }
  return heff;
}

TH1F* EfficiencyCorrection::EfficiencyCorrectionHist(std::string DataFile,
                                                     std::string MCFile,
                                                     std::vector<double>& bins,
                                                     double ptCut,
                                                     V0Type v0Type, bool draw) {
  TFile* f = new TFile(DataFile.c_str());
  TH1F* hpt = nullptr;
  switch (v0Type) {
    case V0Type::K0:
      f->cd("strangeness_tutorial/kzeroShort");
      hpt = (TH1F*)gDirectory->Get("hPtK0ShortSelected");
      break;
    case V0Type::Lambda:
      f->cd("strangeness_tutorial/kLambda");
      hpt = (TH1F*)gDirectory->Get("hPtLambdaSelected");
      break;
    case V0Type::AntiLambda:
      f->cd("strangeness_tutorial/kAntiLambda");
      hpt = (TH1F*)gDirectory->Get("hPtAntiLambdaSelected");
      break;
    case V0Type::LambdaAntiLambda:
      f->cd("strangeness_tutorial/kLambdaAntiLambda");
      hpt = (TH1F*)gDirectory->Get("hPtLambdaAntiLambdaSelected");
      break;
      // default:
      //   std::cout << "Invalid V0Type: " << static_cast<int>(v0Type) <<
      //   std::endl; exit(1);
      //  break;
  }
  if (hpt == nullptr) {
    std::cout << "hpt was not read" << std::endl;
    // say which file and which histogram was not read
    std::cout << DataFile << std::endl;
    exit(1);
  }
  hpt->Sumw2();
  Double_t norm1 = hpt->GetEntries();

  // Rebin the histograms using custom bins
  // create a new histogram with the custom bins
  // filter out the bins that are below the ptCut
  std::vector<double> binsFiltered;
  for (size_t i = 0; i < bins.size() - 1; ++i) {
    if (bins[i] >= ptCut) {
      binsFiltered.push_back(bins[i]);
    }
  }

  TH1F* heff = eff(MCFile, binsFiltered, v0Type, draw);
  TH1F* hpt_rebin = new TH1F("hpt_rebin", "hpt_rebin", binsFiltered.size() - 1,
                             &binsFiltered[0]);

  for (size_t i = 0; i < binsFiltered.size() - 1; ++i) {
    double binLow = binsFiltered[i] + 1e-10;
    double binHigh = binsFiltered[i + 1] - 1e-10;
    // find integral between binsFiltered[i] and binsFiltered[i+1]
    int count = hpt->Integral(hpt->FindBin(binLow), hpt->FindBin(binLow));
    hpt_rebin->SetBinContent(i + 1, count);
  }

  hpt_rebin->Sumw2();

  // Efficiency correction
  TH1F* hpt_corrected = new TH1F("hpt_corrected", "hpt_corrected",
                                 binsFiltered.size() - 1, &binsFiltered[0]);

  hpt_corrected->Divide(hpt_rebin, heff);
  hpt_corrected->GetXaxis()->SetRangeUser(
      0, binsFiltered[binsFiltered.size() - 1]);
  if (draw) {
    // Create a new canvas
    TCanvas* c2 = new TCanvas("c2", "c2", 900, 600);

    // Draw heff on the canvas
    hpt_corrected->SetTitle("Efficiency corrected pT");

    hpt_corrected->Draw();
    // dont draw the legend
    hpt_corrected->SetStats(0);
    std::string fileName = "efficiency_corrected_";
    switch (v0Type) {
      case V0Type::K0:
        fileName += "K0.pdf";
        break;
      case V0Type::Lambda:
        fileName += "Lambda.pdf";
        break;
      case V0Type::AntiLambda:
        fileName += "AntiLambda.pdf";
        break;
      case V0Type::LambdaAntiLambda:
        fileName += "LambdaAntiLambda.pdf";
        break;
        // default:
        //   std::cout << "Invalid V0Type: " << static_cast<int>(v0Type)
        //             << std::endl;
        //   exit(1);
        //   break;
    }
    c2->SaveAs(fileName.c_str());
    delete c2;
  }
  return hpt_corrected;
}