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
#include <vector>

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

/**
 * @brief Fit the histogram with DSCB and pol2, and calculate significance
 */
void SignificanceFit(const bool isMC, const bool isK0, const double xLow,
                     const double xHigh, const char* filename,
                     const char* outputname, std::vector<double> fit_params,
                     const int numOfSigmas = 5) {
  std::unique_ptr<TH1F> h1 = std::make_unique<TH1F>();
  if (isK0) {
    h1 = std::make_unique<TH1F>(*DataLoader::LoadHist(
        filename, "strangeness_tutorial", "hMassK0Short"));
  } else {
    h1 = std::make_unique<TH1F>(
        *DataLoader::LoadHist(filename, "strangeness_tutorial", "hMassLambda"));
  }
  h1->SetAxisRange(xLow, xHigh);
  h1->Sumw2();

  std::unique_ptr<TF1> ff1 = std::make_unique<TF1>(
      "fitDSCB", FittingFunctions::DSCBWithPolynomial, xLow, xHigh, 10);

  double* fit_params_arr = &fit_params[0];

  // may as well use these values as initial guesses
  fit_params_arr[4] = h1->GetMean();
  fit_params_arr[5] = h1->GetRMS();

  // set initial guesses
  ff1->SetParameters(fit_params_arr[0], fit_params_arr[1], fit_params_arr[2],
                     fit_params_arr[3], fit_params_arr[4], fit_params_arr[5],
                     fit_params_arr[6], fit_params_arr[7], fit_params_arr[8],
                     fit_params_arr[9]);

  ff1->SetParNames("alpha(low)", "alpha(high)", "n_(low)", "n_(high)", "mean",
                   "sigma", "norm");
  h1->Fit(ff1.get(), "R");
  std::unique_ptr<TCanvas> c1 = std::make_unique<TCanvas>("c1", "c1", 800, 600);
  h1->Draw();

  // draw pol2
  std::unique_ptr<TF1> pol2 = std::make_unique<TF1>(
      "pol2", FittingFunctions::Polynomial2, xLow, xHigh, 3);
  pol2->SetParameters(ff1->GetParameter(7), ff1->GetParameter(8),
                      ff1->GetParameter(9));
  pol2->SetLineColor(kGreen);
  pol2->Draw("same");

  // count number of entries in numOfSigmas sigma range
  double SPlusB = h1->Integral(
      h1->FindBin(ff1->GetParameter(4) - numOfSigmas * ff1->GetParameter(5)),
      h1->FindBin(ff1->GetParameter(4) + numOfSigmas * ff1->GetParameter(5)));

  // count cumulative number of Y value for pol4 in
  // numOfSigmas sigma range for each bins
  double B = 0;
  for (int i = h1->FindBin(ff1->GetParameter(4) -
                           numOfSigmas * ff1->GetParameter(5));
       i <=
       h1->FindBin(ff1->GetParameter(4) + numOfSigmas * ff1->GetParameter(5));
       i++) {
    B += pol2->Eval(h1->GetBinCenter(i));
  }

  double S = SPlusB - B;

  double significance = S / sqrt(B + S);

  std::cout << "$$$" << significance << "$$$" << std::endl;

  // stdout signif

  // combine two const char* into one
  std::string outputdir = "output/figures/batch_mass_plots/";
  std::string outputname_str = outputdir + outputname;

  c1->SaveAs(outputname_str.c_str());
}