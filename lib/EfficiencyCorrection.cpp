#include "EfficiencyCorrection.h"

double calculateError(double x, double deltaX, double y, double deltaY) {
  double relativeError =
      (1.0 / y) * sqrt(pow(deltaX, 2) + pow((x * deltaY / pow(y, 2)), 2));
  return relativeError;
}

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
  // set the errors
  for (size_t i = 0; i < bins.size() - 1; ++i) {
    double x = hptrue_rebin->GetBinContent(i + 1);
    double deltaX = hptrue_rebin->GetBinError(i + 1);
    double y = hpt_rebin->GetBinContent(i + 1);
    double deltaY = hpt_rebin->GetBinError(i + 1);
    double relativeError = calculateError(x, deltaX, y, deltaY);
    heff->SetBinError(i + 1, relativeError);
  }
  if (draw) {
    heff->Draw();
    // dont draw the legend
    heff->SetStats(0);
    std::string fileName = "Efficiency_";
    switch (v0Type) {
      case V0Type::K0:
        fileName += "K0.root";
        break;
      case V0Type::Lambda:
        fileName += "Lambda.root";
        break;
      case V0Type::AntiLambda:
        fileName += "AntiLambda.root";
        break;
      case V0Type::LambdaAntiLambda:
        fileName += "LambdaAntiLambda.root";
        break;
        // default:
        //  std::cout << "Invalid V0Type: " << static_cast<int>(v0Type)
        //            << std::endl;
        //  exit(1);
        //  break;
    }
    std::string outputPath = "Results/" + fileName;
    heff->SaveAs(outputPath.c_str());
  }
  return heff;
}

TH1F* EfficiencyCorrection::eff(std::string MCFileLambda,
                                std::string MCFileAntiLambda,
                                std::vector<double>& bins, V0Type v0Type,
                                bool draw) {
  gROOT->Reset();

  TFile* f1 = new TFile(MCFileLambda.c_str());
  TFile* f2 = new TFile(MCFileAntiLambda.c_str());

  // save generated pt histogram as hpt
  f1->cd("strangeness_tutorial/genParticles");
  TH1F* hpt1 = (TH1F*)gDirectory->Get("hPtLambdaGen");
  f2->cd("strangeness_tutorial/genParticles");
  TH1F* hpt2 = (TH1F*)gDirectory->Get("hPtAntiLambdaGen");

  if (hpt1 == nullptr) {
    std::cout << "hpt was not read" << std::endl;
    // say which file and which histogram was not read
    std::cout << MCFileLambda << std::endl;
    exit(1);
  }
  if (hpt2 == nullptr) {
    std::cout << "hpt was not read" << std::endl;
    // say which file and which histogram was not read
    std::cout << MCFileAntiLambda << std::endl;
    exit(1);
  }
  hpt1->Sumw2();
  hpt2->Sumw2();

  // save true reconstructed pt histogram as hptrue
  f1->cd("strangeness_tutorial/kLambda");
  TH1F* hptrue1 = (TH1F*)gDirectory->Get("hPtLambdaTrueRec");

  f2->cd("strangeness_tutorial/kAntiLambda");
  TH1F* hptrue2 = (TH1F*)gDirectory->Get("hPtAntiLambdaTrueRec");

  if (hptrue1 == nullptr) {
    std::cout << "hptrue was not read" << std::endl;
    // say which file and which histogram was not read
    std::cout << MCFileLambda << std::endl;
    exit(1);
  }
  if (hptrue2 == nullptr) {
    std::cout << "hptrue was not read" << std::endl;
    // say which file and which histogram was not read
    std::cout << MCFileAntiLambda << std::endl;
    exit(1);
  }
  hptrue1->Sumw2();
  hptrue2->Sumw2();

  // add hpt1 and hpt2 into hpt
  TH1F* hpt = (TH1F*)hpt1->Clone("hpt");
  hpt->Add(hpt2);
  // add hptrue1 and hptrue2 into hptrue
  TH1F* hptrue = (TH1F*)hptrue1->Clone("hptrue");
  hptrue->Add(hptrue2);
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
  for (size_t i = 0; i < bins.size() - 1; ++i) {
    double x = hptrue_rebin->GetBinContent(i + 1);
    double deltaX = hptrue_rebin->GetBinError(i + 1);
    double y = hpt_rebin->GetBinContent(i + 1);
    double deltaY = hpt_rebin->GetBinError(i + 1);
    double relativeError = calculateError(x, deltaX, y, deltaY);
    heff->SetBinError(i + 1, relativeError);
  }
  if (draw) {
    heff->Draw();
    // dont draw the legend
    heff->SetStats(0);
    std::string fileName = "Efficiency_COMBINED_LAMBDAS.root";
    std::string outputPath = "Results/" + fileName;
    heff->SaveAs(outputPath.c_str());
  }

  // double all bins in heff
  for (size_t i = 0; i < bins.size() - 1; ++i) {
    double binLow = bins[i] + 1e-10;
    double binHigh = bins[i + 1] - 1e-10;
    heff->SetBinContent(i + 1, heff->GetBinContent(i + 1) * 2);
  }
  return heff;
}

TH1F* EfficiencyCorrection::EfficiencyCorrectionHist(
    std::string DataFile, std::string MCFileLambda,
    std::string MCFileAntiLambda, std::vector<double>& bins, double ptCut,
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
      f->cd("strangeness_tutorial/kLambdaAndAntiLambda");
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

  TH1F* heff = eff(MCFileLambda, MCFileAntiLambda, binsFiltered, v0Type, draw);
  TH1F* hpt_rebin = new TH1F("hpt_rebin", "hpt_rebin", binsFiltered.size() - 1,
                             &binsFiltered[0]);

  for (size_t i = 0; i < binsFiltered.size() - 1; ++i) {
    double binLow = binsFiltered[i] + 1e-10;
    double binHigh = binsFiltered[i + 1] - 1e-10;
    // find integral between binsFiltered[i] and binsFiltered[i+1]
    int count = hpt->Integral(hpt->FindBin(binLow), hpt->FindBin(binLow));
    hpt_rebin->SetBinContent(i + 1, count);
    hpt_rebin->SetBinError(i + 1, TMath::Sqrt(count));
  }

  hpt_rebin->Sumw2();

  // Efficiency correction
  TH1F* hpt_corrected = new TH1F("hpt_corrected", "hpt_corrected",
                                 binsFiltered.size() - 1, &binsFiltered[0]);

  hpt_corrected->Divide(hpt_rebin, heff);
  hpt_corrected->GetXaxis()->SetRangeUser(
      0, binsFiltered[binsFiltered.size() - 1]);
  if (draw) {
    // Draw heff on the canvas
    hpt_corrected->SetMarkerStyle(20);
    hpt_corrected->SetMarkerSize(0.7);
    hpt_corrected->SetMarkerColor(kBlack);
    hpt_corrected->SetLineColor(kBlack);
    hpt_corrected->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hpt_corrected->GetYaxis()->SetTitle("Efficiency corrected counts");

    hpt_corrected->Draw();
    // dont draw the legend
    hpt_corrected->SetStats(0);
    std::string fileName = "Corrected_Efficiency_";
    switch (v0Type) {
      case V0Type::K0:
        fileName += "K0.root";
        hpt_corrected->SetTitle("Efficiency corrected p_{T} for K^{0}_{S}");
        break;
      case V0Type::Lambda:
        fileName += "Lambda.root";
        hpt_corrected->SetTitle("Efficiency corrected p_{T} for #Lambda");
        break;
      case V0Type::AntiLambda:
        fileName += "AntiLambda.root";
        hpt_corrected->SetTitle("Efficiency corrected p_{T} for #bar{#Lambda}");
        break;
      case V0Type::LambdaAntiLambda:
        fileName += "LambdaAntiLambda.root";
        hpt_corrected->SetTitle(
            "Efficiency corrected p_{T} for #Lambda and "
            "#bar{#Lambda}");
        break;
        // default:
        //   std::cout << "Invalid V0Type: " << static_cast<int>(v0Type)
        //             << std::endl;
        //   exit(1);
        //   break;
    }
    std::string outputPath = "Results/" + fileName;
    hpt_corrected->SaveAs(outputPath.c_str());
  }
  return hpt_corrected;
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
      f->cd("strangeness_tutorial/kLambdaAndAntiLambda");
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
    hpt_rebin->SetBinError(i + 1, TMath::Sqrt(count));
  }

  hpt_rebin->Sumw2();

  // Efficiency correction
  TH1F* hpt_corrected = new TH1F("hpt_corrected", "hpt_corrected",
                                 binsFiltered.size() - 1, &binsFiltered[0]);

  hpt_corrected->Divide(hpt_rebin, heff);
  hpt_corrected->GetXaxis()->SetRangeUser(
      0, binsFiltered[binsFiltered.size() - 1]);
  if (draw) {
    // Draw heff on the canvas

    hpt_corrected->SetMarkerStyle(20);
    hpt_corrected->SetMarkerSize(0.7);
    hpt_corrected->SetMarkerColor(kBlack);
    hpt_corrected->SetLineColor(kBlack);
    hpt_corrected->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hpt_corrected->GetYaxis()->SetTitle("Efficiency corrected counts");

    hpt_corrected->Draw();
    // dont draw the legend
    hpt_corrected->SetStats(0);
    std::string fileName = "Corrected_Efficiency_";
    switch (v0Type) {
      case V0Type::K0:
        fileName += "K0.root";
        hpt_corrected->SetTitle("Efficiency corrected p_{T} for K^{0}_{S}");
        break;
      case V0Type::Lambda:
        fileName += "Lambda.root";
        hpt_corrected->SetTitle("Efficiency corrected p_{T} for #Lambda");
        break;
      case V0Type::AntiLambda:
        fileName += "AntiLambda.root";
        hpt_corrected->SetTitle("Efficiency corrected p_{T} for #bar{#Lambda}");
        break;
      case V0Type::LambdaAntiLambda:
        fileName += "LambdaAntiLambda.root";
        hpt_corrected->SetTitle(
            "Efficiency corrected p_{T} for #Lambda and "
            "#bar{#Lambda}");
        break;
        // default:
        //   std::cout << "Invalid V0Type: " << static_cast<int>(v0Type)
        //             << std::endl;
        //   exit(1);
        //   break;
    }
    std::string outputPath = "Results/" + fileName;
    hpt_corrected->SaveAs(outputPath.c_str());
  }
  return hpt_corrected;
}

TH1F* EfficiencyCorrection::getUncorrectedHist(std::string DataFile,
                                               std::vector<double>& bins,
                                               double ptCut, V0Type v0Type,
                                               bool draw) {
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
      f->cd("strangeness_tutorial/kLambdaAndAntiLambda");
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

  TH1F* hpt_rebin = new TH1F("hpt_rebin", "hpt_rebin", binsFiltered.size() - 1,
                             &binsFiltered[0]);

  for (size_t i = 0; i < binsFiltered.size() - 1; ++i) {
    double binLow = binsFiltered[i] + 1e-10;
    double binHigh = binsFiltered[i + 1] - 1e-10;
    // find integral between binsFiltered[i] and binsFiltered[i+1]
    int count = hpt->Integral(hpt->FindBin(binLow), hpt->FindBin(binLow));
    hpt_rebin->SetBinContent(i + 1, count);
    hpt_rebin->SetBinError(i + 1, TMath::Sqrt(count));
  }

  hpt_rebin->Sumw2();
  return hpt_rebin;
}

double EfficiencyCorrection::LevyTsallisK0(double* x, double* par) {
  // THIS IS UNUSED SO FAR. NEEDS TO BE GENERALISED FOR ALL V0 TYPES
  // Extracting parameters
  double C = par[0];
  double T = par[1];
  double n = par[2];

  // Fixed parameters
  double m = 0.497648;  // mass in GeV/c^2
  double pt = x[0];

  // Calculating the function
  double numerator = C * (n - 1) * (n - 2) * pt;
  double denominator = n * T * (n * T + m * (n - 2));
  double factor = 1 + (sqrt(pt * pt + m * m) - m) / (n * T);

  double result = numerator / denominator * pow(factor, -n);

  return result;
}

double EfficiencyCorrection::LevyTsallisLambda(double* x, double* par) {
  // THIS IS UNUSED SO FAR. NEEDS TO BE GENERALISED FOR ALL V0 TYPES
  // Extracting parameters
  double C = par[0];
  double T = par[1];
  double n = par[2];

  // Fixed parameters
  double m = 1.115683;  // mass in GeV/c^2
  double pt = x[0];

  // Calculating the function
  double numerator = C * (n - 1) * (n - 2) * pt;
  double denominator = n * T * (n * T + m * (n - 2));
  double factor = 1 + (sqrt(pt * pt + m * m) - m) / (n * T);

  double result = numerator / denominator * pow(factor, -n);

  return result;
}

TH1F* EfficiencyCorrection::ApplyFit(TH1F* h, std::vector<double>& bins,
                                     double ptCut, V0Type v0Type,
                                     std::vector<double> params) {
  TH1F* hcopy = (TH1F*)h->Clone("hcopy");
  TF1* ff1;
  double mass;
  if (v0Type == V0Type::K0) {
    ff1 = new TF1("fit", LevyTsallisK0, 0, 10, 3);
    mass = 0.497648;
  } else if (v0Type == V0Type::Lambda || v0Type == V0Type::AntiLambda) {
    ff1 = new TF1("fit", LevyTsallisLambda, 0, 10, 3);
    mass = 1.115683;
  } else {
    std::cout << "Invalid V0Type: " << static_cast<int>(v0Type) << std::endl;
    exit(1);
  }
  ff1->SetParameters(params[0], params[1], params[2]);
  ff1->SetParNames("C", "T", "n");

  h->Fit(ff1, "R");
  h->SaveAs("LastFit.root");

  // make blank histogram with defined bins
  TH1F* hreturn = new TH1F("hcopy", "hcopy", bins.size() - 1, &bins[0]);

  // for bins with a center less than ptCut, set the content to evaluate the
  // fit
  for (size_t i = 0; i < bins.size() - 1; ++i) {
    double binCenter = (bins[i] + bins[i + 1]) / 2;
    int bin = hreturn->FindBin(binCenter);
    if (binCenter < ptCut) {
      // find which bin of copy corresponds to binCenter
      hreturn->SetBinContent(bin, ff1->Eval(binCenter));
      hreturn->SetBinError(bin, 0);  // temporary, need to calculate the
                                     // error properly later
    } else {
      hreturn->SetBinContent(bin,
                             hcopy->GetBinContent(hcopy->FindBin(binCenter)));
      hreturn->SetBinError(bin, hcopy->GetBinError(hcopy->FindBin(binCenter)));
    }
  }
  return hreturn;
}

TH1F* EfficiencyCorrection::getRebinHist(TH1F* h, std::vector<double>& bins) {
  TH1F* hrebin = new TH1F("hrebin", "hrebin", bins.size() - 1, &bins[0]);
  for (size_t i = 0; i < bins.size() - 1; ++i) {
    double binLow = bins[i] + 1e-10;
    double binHigh = bins[i + 1] - 1e-10;
    // find integral between bins[i] and bins[i+1]
    int count = h->Integral(h->FindBin(binLow), h->FindBin(binLow));
    hrebin->SetBinContent(i + 1, count);
    hrebin->SetBinError(i + 1, TMath::Sqrt(count));  // temporary, need to
                                                     // calculate the error
                                                     // properly later
  }
  return hrebin;
}