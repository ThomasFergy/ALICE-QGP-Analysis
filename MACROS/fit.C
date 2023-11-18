#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1F.h>
#include <TLine.h>
#include <TROOT.h>
#include <TTree.h>

#include <chrono>
#include <fstream>
#include <memory>
#include <string>

// clang-format off
// for syntax highlighting
#include "../lib/DataLoader.h" 
#include "../lib/FittingFunctions.h"
// MACROS for including libs for CLING
R__ADD_INCLUDE_PATH(lib)
#ifdef _WIN32
R__LOAD_LIBRARY(lib/DataLoader_cpp.dll)
R__LOAD_LIBRARY(lib/FittingFunctions_cpp.dll)
#else
R__LOAD_LIBRARY(lib/DataLoader_cpp.so)
R__LOAD_LIBRARY(lib/FittingFunctions_cpp.so)
#endif
// clang-format on

void fit() {
  /////////////////////////////////////////////////////////////////////////////
  // Load histogram for hMassK0Short and fit with DoubleSidedCrystalball
  ////////////////

  int numOfSigmas = 5;
  bool MC = false;

  double xLow;
  double xHigh;
  std::unique_ptr<TH1F> h1 = std::make_unique<TH1F>();
  if (MC) {
    h1 = std::make_unique<TH1F>(
        *DataLoader::LoadHist("output/V0MC/AnalysisResults_dcanegtopv_0.0.root",
                              "strangeness_tutorial", "hMassK0Short"));
    xLow = 0.46;
    xHigh = 0.54;
  } else {
    h1 = std::make_unique<TH1F>(*DataLoader::LoadHist(
        "output/V0Data/AnalysisResults_dcanegtopv_0.0.root",
        "strangeness_tutorial", "hMassK0Short"));
    xLow = 0.45;
    xHigh = 0.54;
  }
  h1->SetAxisRange(xLow, xHigh);
  h1->Sumw2();

  std::unique_ptr<TF1> ff1 = std::make_unique<TF1>(
      "fitDSCB", FittingFunctions::DSCBWithPolynomial, xLow, xHigh, 10);
  ff1->SetParameters(1.2, 1.2, 1.4, 1.4, h1->GetMean(), h1->GetRMS(), 3000, 1,
                     1, 1);
  ff1->SetParNames("alpha(low)", "alpha(high)", "n_(low)", "n_(high)", "mean",
                   "sigma", "norm");
  h1->Fit(ff1.get(), "R");
  std::unique_ptr<TCanvas> c1 = std::make_unique<TCanvas>("c1", "c1", 800, 600);
  h1->Draw();

  // draw pol2
  std::unique_ptr<TF1> pol2_1 = std::make_unique<TF1>(
      "pol4", FittingFunctions::Polynomial2, xLow, xHigh, 5);
  pol2_1->SetParameters(ff1->GetParameter(7), ff1->GetParameter(8),
                        ff1->GetParameter(9));
  pol2_1->SetLineColor(kGreen);
  pol2_1->Draw("same");

  // draw numOfSigmas sigma lines
  std::unique_ptr<TLine> l1_1 = std::make_unique<TLine>(
      ff1->GetParameter(4) - numOfSigmas * ff1->GetParameter(5), 0,
      ff1->GetParameter(4) - numOfSigmas * ff1->GetParameter(5),
      1.1 * h1->GetMaximum());
  l1_1->SetLineColor(kBlack);
  l1_1->Draw("same");
  std::unique_ptr<TLine> l2_1 = std::make_unique<TLine>(
      ff1->GetParameter(4) + numOfSigmas * ff1->GetParameter(5), 0,
      ff1->GetParameter(4) + numOfSigmas * ff1->GetParameter(5),
      1.1 * h1->GetMaximum());
  l2_1->SetLineColor(kBlack);
  l2_1->Draw("same");

  if (MC) {
    c1->SaveAs("output/figures/hMassK0ShortMC_fitDSCB.pdf");
  } else {
    c1->SaveAs("output/figures/hMassK0ShortDATA_fitDSCB.pdf");
  }

  // count number of entries in numOfSigmas sigma range
  double SPlusB_1 = h1->Integral(
      h1->FindBin(ff1->GetParameter(4) - numOfSigmas * ff1->GetParameter(5)),
      h1->FindBin(ff1->GetParameter(4) + numOfSigmas * ff1->GetParameter(5)));

  // count cumulative number of Y value for pol4 in numOfSigmas sigma range for
  // each bins
  double B_1 = 0;
  double SBinErr = 0;
  for (int i = h1->FindBin(ff1->GetParameter(4) -
                           numOfSigmas * ff1->GetParameter(5));
       i <=
       h1->FindBin(ff1->GetParameter(4) + numOfSigmas * ff1->GetParameter(5));
       i++) {
    B_1 += pol2_1->Eval(h1->GetBinCenter(i));

    std::cout << h1->GetBinError(i) << std::endl;
    SBinErr += h1->GetBinError(i) * h1->GetBinError(i);
  }

  double S_1 = SPlusB_1 - B_1;

  double significance_1 = S_1 / sqrt(B_1 + S_1);

  std::cout << "Significance: " << significance_1 << std::endl;

  double Serr = sqrt(SBinErr);

  double SignificanceErr = 0.5 * (1 / sqrt(S_1)) * Serr;

  std::cout << "$$$" << SignificanceErr << "$$$" << std::endl;
  std::cout << "$$$" << Serr << "$$$" << std::endl;

  /////////////////////////////////////////////////////////////////////////////
  // Load histogram for hmassLambda and fit with DoubleSidedCrystalball
  ////////////////

  std::unique_ptr<TH1F> h2 = std::make_unique<TH1F>();

  if (MC) {
    h2 = std::make_unique<TH1F>(
        *DataLoader::LoadHist("output/V0MC/AnalysisResults_dcanegtopv_0.0.root",
                              "strangeness_tutorial", "hMassLambda"));
    xLow = 1.08;
    xHigh = 1.15;
  } else {
    h2 = std::make_unique<TH1F>(*DataLoader::LoadHist(
        "output/V0Data/AnalysisResults_dcanegtopv_0.0.root",
        "strangeness_tutorial", "hMassLambda"));
    xLow = 1.085;
    xHigh = 1.145;
  }
  h2->Sumw2();
  h2->SetAxisRange(xLow, xHigh);

  std::unique_ptr<TF1> ff2 = std::make_unique<TF1>(
      "fitDSCB", FittingFunctions::DSCBWithPolynomial, xLow, xHigh, 10);
  ff2->SetParameters(1, 1.6, 0.1, 0.1, h2->GetMean(), h2->GetRMS(), 3000, 1, 1,
                     1);
  ff2->SetParNames("alpha(low)", "alpha(high)", "n_(low)", "n_(high)", "mean",
                   "sigma", "norm");
  h2->Fit(ff2.get(), "R");
  std::unique_ptr<TCanvas> c2 = std::make_unique<TCanvas>("c2", "c2", 800, 600);
  h2->Draw();

  // draw pol4
  std::unique_ptr<TF1> pol2_2 = std::make_unique<TF1>(
      "pol4", FittingFunctions::Polynomial2, xLow, xHigh, 5);
  pol2_2->SetParameters(ff2->GetParameter(7), ff2->GetParameter(8),
                        ff2->GetParameter(9));
  pol2_2->SetLineColor(kGreen);
  pol2_2->Draw("same");

  // draw numOfSigmas sigma lines
  std::unique_ptr<TLine> l1_2 = std::make_unique<TLine>(
      ff2->GetParameter(4) - numOfSigmas * ff2->GetParameter(5), 0,
      ff2->GetParameter(4) - numOfSigmas * ff2->GetParameter(5),
      1.1 * h2->GetMaximum());
  l1_2->SetLineColor(kBlack);
  l1_2->Draw("same");
  std::unique_ptr<TLine> l2_2 = std::make_unique<TLine>(
      ff2->GetParameter(4) + numOfSigmas * ff2->GetParameter(5), 0,
      ff2->GetParameter(4) + numOfSigmas * ff2->GetParameter(5),
      1.1 * h2->GetMaximum());
  l2_2->SetLineColor(kBlack);
  l2_2->Draw("same");

  if (MC) {
    c2->SaveAs("output/figures/hMassLambdaMC_fitDSCB.pdf");
  } else {
    c2->SaveAs("output/figures/hMassLambdaDATA_fitDSCB.pdf");
  }

  // count number of entries in numOfSigmas sigma range
  double SPlusB_2 = h2->Integral(
      h2->FindBin(ff2->GetParameter(4) - numOfSigmas * ff2->GetParameter(5)),
      h2->FindBin(ff2->GetParameter(4) + numOfSigmas * ff2->GetParameter(5)));

  // count cumulative number of Y value for pol4 in numOfSigmas sigma range for
  // each bins
  double B_2 = 0;
  for (int i = h2->FindBin(ff2->GetParameter(4) -
                           numOfSigmas * ff2->GetParameter(5));
       i <=
       h2->FindBin(ff2->GetParameter(4) + numOfSigmas * ff2->GetParameter(5));
       i++) {
    B_2 += pol2_2->Eval(h2->GetBinCenter(i));
  }

  double S_2 = SPlusB_2 - B_2;

  double significance_2 = S_2 / sqrt(B_2 + S_2);

  std::cout << "Significance: " << significance_2 << std::endl;
}