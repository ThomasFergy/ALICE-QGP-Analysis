#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1F.h>
#include <TLine.h>
#include <TROOT.h>
#include <TTree.h>

#include <memory>

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
  // Draw 3 sigma lines on plots?
  bool draw3Sigma = true;

  /////////////////////////////////////////////////////////////////////////////
  // Load histogram for hMassK0Short and fit with DoubleSidedCrystalball
  ////////////////

  std::unique_ptr<TH1F> h1 = std::make_unique<TH1F>(*DataLoader::LoadHist(
      "data/V0/AnalysisResults.root", "strangeness_tutorial", "hMassK0Short"));

  double xLow = 0.48;
  double xHigh = 0.52;
  h1->SetAxisRange(xLow, xHigh);
  h1->Sumw2();
  h1->Scale(1. / h1->GetEntries());

  std::unique_ptr<TF1> ff1 = std::make_unique<TF1>(
      "fitDSCB", FittingFunctions::DoubleSidedCrystalballFunction, xLow, xHigh,
      7);
  ff1->SetParameters(1.5, 1.4, 0.4, 0.4, h1->GetMean(), h1->GetRMS(), 0.007);
  ff1->SetParNames("alpha(low)", "alpha(high)", "n_(low)", "n_(high)", "mean",
                   "sigma", "norm");
  h1->Fit(ff1.get(), "R");
  std::unique_ptr<TCanvas> c1 = std::make_unique<TCanvas>("c1", "c1", 800, 600);
  h1->Draw();

  if (draw3Sigma) {
    double sigma = ff1->GetParameter(5);
    double mean = ff1->GetParameter(4);
    double xMin = mean - 3 * sigma;
    double xMax = mean + 3 * sigma;
    std::unique_ptr<TLine> l1 = std::make_unique<TLine>(xMin, 0, xMin, 0.1);
    std::unique_ptr<TLine> l2 = std::make_unique<TLine>(xMax, 0, xMax, 0.1);
    l1->SetLineColor(kRed);
    l2->SetLineColor(kRed);
    l1->Draw("same");
    l2->Draw("same");
    c1->SaveAs("output/figures/hMassK0Short_fitDSCB.pdf");
  } else {
    c1->SaveAs("output/figures/hMassK0Short_fitDSCB.pdf");
  }

  /////////////////////////////////////////////////////////////////////////////
  // Load histogram for hmassLambda and fit with DoubleSidedCrystalball
  ////////////////

  std::unique_ptr<TH1F> h2 = std::make_unique<TH1F>(*DataLoader::LoadHist(
      "data/V0/AnalysisResults.root", "strangeness_tutorial", "hMassLambda"));

  xLow = 1.075;
  xHigh = 1.15;
  h2->Sumw2();
  h2->SetAxisRange(xLow, xHigh);
  h2->Scale(1. / h2->GetEntries());

  std::unique_ptr<TF1> ff2 = std::make_unique<TF1>(
      "fitDSCB", FittingFunctions::DoubleSidedCrystalballFunction, xLow, xHigh,
      7);
  ff2->SetParameters(1, 0.5, 80, 0.1, h2->GetMean(), h2->GetRMS(), 0.02);
  ff2->SetParNames("alpha(low)", "alpha(high)", "n_(low)", "n_(high)", "mean",
                   "sigma", "norm");
  h2->Fit(ff2.get(), "R");
  std::unique_ptr<TCanvas> c2 = std::make_unique<TCanvas>("c2", "c2", 800, 600);
  h2->Draw();

  if (draw3Sigma) {
    double sigma = ff2->GetParameter(5);
    double mean = ff2->GetParameter(4);
    double xMin = mean - 3 * sigma;
    double xMax = mean + 3 * sigma;
    std::unique_ptr<TLine> l1 = std::make_unique<TLine>(xMin, 0, xMin, 0.1);
    std::unique_ptr<TLine> l2 = std::make_unique<TLine>(xMax, 0, xMax, 0.1);
    l1->SetLineColor(kRed);
    l2->SetLineColor(kRed);
    l1->Draw("same");
    l2->Draw("same");
    c2->SaveAs("output/figures/hMassLambda_fitDSCB.pdf");
  } else {
    c2->SaveAs("output/figures/hMassLambda_fitDSCB.pdf");
  }
}
