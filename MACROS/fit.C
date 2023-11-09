#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1F.h>
#include <TLine.h>
#include <TROOT.h>
#include <TTree.h>

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

  std::unique_ptr<TH1F> h1 = std::make_unique<TH1F>(*DataLoader::LoadHist(
      "data/V0/AnalysisResults.root", "strangeness_tutorial", "hMassK0Short"));

  double xLow = 0.45;
  double xHigh = 0.54;
  h1->SetAxisRange(xLow, xHigh);
  h1->Sumw2();

  std::unique_ptr<TF1> ff1 = std::make_unique<TF1>(
      "fitDSCB", FittingFunctions::DSCBWithPolynomial, xLow, xHigh, 12);
  ff1->SetParameters(1.2, 1.2, 1.4, 1.4, h1->GetMean(), h1->GetRMS(), 3000, 1,
                     1, 1, 1);
  ff1->SetParNames("alpha(low)", "alpha(high)", "n_(low)", "n_(high)", "mean",
                   "sigma", "norm");
  h1->Fit(ff1.get(), "R");
  std::unique_ptr<TCanvas> c1 = std::make_unique<TCanvas>("c1", "c1", 800, 600);
  h1->Draw();

  // draw pol4
  std::unique_ptr<TF1> pol4_1 = std::make_unique<TF1>(
      "pol4", FittingFunctions::Polynomial4, xLow, xHigh, 5);
  pol4_1->SetParameters(ff1->GetParameter(7), ff1->GetParameter(8),
                        ff1->GetParameter(9), ff1->GetParameter(10),
                        ff1->GetParameter(11));
  pol4_1->SetLineColor(kGreen);
  pol4_1->Draw("same");

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

  c1->SaveAs("output/figures/hMassK0Short_fitDSCB.pdf");

  // find integral of DSCB in numOfSigmas sigma range
  double integral_1 =
      ff1->Integral(ff1->GetParameter(4) - numOfSigmas * ff1->GetParameter(5),
                    ff1->GetParameter(4) + numOfSigmas * ff1->GetParameter(5));

  // find integral of pol4 in numOfSigmas sigma range
  double integralPol4_1 = pol4_1->Integral(
      ff1->GetParameter(4) - numOfSigmas * ff1->GetParameter(5),
      ff1->GetParameter(4) + numOfSigmas * ff1->GetParameter(5));

  double SPlusB_1 = integral_1;
  double B_1 = integralPol4_1;
  double S_1 = SPlusB_1 - B_1;

  double significance_1 = S_1 / sqrt(B_1 + S_1);

  std::cout << "Significance: " << significance_1 << std::endl;

  /////////////////////////////////////////////////////////////////////////////
  // Load histogram for hmassLambda and fit with DoubleSidedCrystalball
  ////////////////

  std::unique_ptr<TH1F> h2 = std::make_unique<TH1F>(*DataLoader::LoadHist(
      "data/V0/AnalysisResults.root", "strangeness_tutorial", "hMassLambda"));

  xLow = 1.085;
  xHigh = 1.145;
  h2->Sumw2();
  h2->SetAxisRange(xLow, xHigh);

  std::unique_ptr<TF1> ff2 = std::make_unique<TF1>(
      "fitDSCB", FittingFunctions::DSCBWithPolynomial, xLow, xHigh, 12);
  ff2->SetParameters(1, 1.6, 0.1, 0.1, h2->GetMean(), h2->GetRMS(), 3000, 1, 1,
                     1, 1);
  ff2->SetParNames("alpha(low)", "alpha(high)", "n_(low)", "n_(high)", "mean",
                   "sigma", "norm");
  h2->Fit(ff2.get(), "R");
  std::unique_ptr<TCanvas> c2 = std::make_unique<TCanvas>("c2", "c2", 800, 600);
  h2->Draw();

  // draw pol4
  std::unique_ptr<TF1> pol4_2 = std::make_unique<TF1>(
      "pol4", FittingFunctions::Polynomial4, xLow, xHigh, 5);
  pol4_2->SetParameters(ff2->GetParameter(7), ff2->GetParameter(8),
                        ff2->GetParameter(9), ff2->GetParameter(10),
                        ff2->GetParameter(11));
  pol4_2->SetLineColor(kGreen);
  pol4_2->Draw("same");

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

  c2->SaveAs("output/figures/hMassLambda_fitDSCB.pdf");

  // find integral of DSCB in numOfSigmas sigma range
  double integral_2 =
      ff2->Integral(ff2->GetParameter(4) - numOfSigmas * ff2->GetParameter(5),
                    ff2->GetParameter(4) + numOfSigmas * ff2->GetParameter(5));

  // find integral of pol4 in numOfSigmas sigma range
  double integralPol4_2 = pol4_1->Integral(
      ff1->GetParameter(4) - numOfSigmas * ff1->GetParameter(5),
      ff1->GetParameter(4) + numOfSigmas * ff1->GetParameter(5));

  double SPlusB_2 = integral_2;
  double B_2 = integralPol4_2;
  double S_2 = SPlusB_2 - B_2;

  double significance_2 = S_2 / sqrt(B_2 + S_2);

  std::cout << "Significance: " << significance_2 << std::endl;

  bool writeToFile = false;
  if (writeToFile) {
    std::ofstream myfile;
    myfile.open("output/data.txt");
    myfile << "K0Short:\n";
    myfile << "   Cut Parameter: dcanegtopv"
           << "   N/A"
           << "\n";
    myfile << "   Cut Value: "
           << "   N/A"
           << "\n";
    myfile << "   Significance: " << significance_1 << "\n";
    myfile << "   S: " << S_1 << "\n";
    myfile << "   B: " << B_1 << "\n";
    myfile << "   S+B: " << SPlusB_1 << "\n";
    myfile << "Lambda:\n";
    myfile << "   Cut Parameter: dcanegtopv"
           << "   N/A"
           << "\n";
    myfile << "   Cut Value: "
           << "   N/A"
           << "\n";
    myfile << "   Significance: " << significance_2 << "\n";
    myfile << "   S: " << S_2 << "\n";
    myfile << "   B: " << B_2 << "\n";
    myfile << "   S+B: " << SPlusB_2 << "\n";
    myfile.close();
  }
}