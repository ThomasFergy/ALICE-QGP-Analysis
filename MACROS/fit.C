#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1F.h>
#include <TROOT.h>
#include <TTree.h>

#include <memory>

// clang-format off
// for syntax highlighting
#include "../lib/DataLoader.h" 
#include "../lib/FittingFunctions.h"
// MACROS for including libs
R__ADD_INCLUDE_PATH(lib)
R__LOAD_LIBRARY(lib/DataLoader_cpp.so)
R__LOAD_LIBRARY(lib/FittingFunctions_cpp.so)
// clang-format on

void fit() {
  /////////////////////////////////////////////////////////////////////////////
  // Load histogram for hMassK0Short and fit with DoubleSidedCrystalball
  ////////////////

  std::unique_ptr<TH1F> h1 = std::make_unique<TH1F>(*DataLoader::LoadHist(
      "V0/AnalysisResults.root", "strangeness_tutorial", "hMassK0Short"));

  h1->Draw();
  h1->Sumw2();
  h1->Scale(1. / h1->GetEntries());

  std::unique_ptr<TF1> ff1 = std::make_unique<TF1>(
      "fitDSCB", FittingFunctions::DoubleSidedCrystalballFunction, 0.45, 0.55,
      7);
  ff1->SetParameters(1.5, 1.4, 0.4, 0.4, h1->GetMean(), h1->GetRMS(), 0.007);
  ff1->SetParNames("alpha(low)", "alpha(high)", "n_(low)", "n_(high)", "mean",
                   "sigma", "norm");
  h1->Fit(ff1.get(), "R");
  std::unique_ptr<TCanvas> c1 = std::make_unique<TCanvas>("c1", "c1", 800, 600);
  h1->Draw();
  c1->SaveAs("output/figures/hMassK0Short_fitDSCB.pdf");

  /////////////////////////////////////////////////////////////////////////////
  // Load histogram for hmassLambda and fit with DoubleSidedCrystalball
  ////////////////

  std::unique_ptr<TH1F> h2 = std::make_unique<TH1F>(*DataLoader::LoadHist(
      "V0/AnalysisResults.root", "strangeness_tutorial", "hMassLambda"));

  h2->Draw();
  h2->Sumw2();
  h2->Scale(1. / h2->GetEntries());

  std::unique_ptr<TF1> ff2 = std::make_unique<TF1>(
      "fitDSCB", FittingFunctions::DoubleSidedCrystalballFunction, 1, 1.3, 7);
  ff2->SetParameters(1, 0.5, 80, 0.1, h2->GetMean(), h2->GetRMS(), 0.02);
  ff2->SetParNames("alpha(low)", "alpha(high)", "n_(low)", "n_(high)", "mean",
                   "sigma", "norm");
  h2->Fit(ff2.get(), "R");
  std::unique_ptr<TCanvas> c2 = std::make_unique<TCanvas>("c2", "c2", 800, 600);
  h2->Draw();
  c2->SaveAs("output/figures/hMassLambda_fitDSCB.pdf");
}
