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

void SignificanceFit(const bool isMC, const double xLow, const double xHigh,
                     const char* filename, std::vector<double> fit_params,
                     const int numOfSigmas = 5) {
  std::unique_ptr<TH1F> h1 = std::make_unique<TH1F>();
  h1 = std::make_unique<TH1F>(
      *DataLoader::LoadHist(filename, "strangeness_tutorial", "hMassK0Short"));
  h1->SetAxisRange(xLow, xHigh);
  h1->Sumw2();

  std::unique_ptr<TF1> ff1 = std::make_unique<TF1>(
      "fitDSCB", FittingFunctions::DSCBWithPolynomial, xLow, xHigh, 12);

  double* fit_params_arr = &fit_params[0];
  ff1->SetParameters(fit_params_arr);
  ff1->SetParNames("alpha(low)", "alpha(high)", "n_(low)", "n_(high)", "mean",
                   "sigma", "norm");
  h1->Fit(ff1.get(), "R");
  std::unique_ptr<TCanvas> c1 = std::make_unique<TCanvas>("c1", "c1", 800, 600);
  h1->Draw();

  std::unique_ptr<TF1> pol2_1 = std::make_unique<TF1>(
      "pol2", FittingFunctions::Polynomial2, xLow, xHigh, 3);
  pol2_1->SetParameters(ff1->GetParameter(7), ff1->GetParameter(8),
                        ff1->GetParameter(9));

  // count number of entries in numOfSigmas sigma range
  double SPlusB_1 = h1->Integral(
      h1->FindBin(ff1->GetParameter(4) - numOfSigmas * ff1->GetParameter(5)),
      h1->FindBin(ff1->GetParameter(4) + numOfSigmas * ff1->GetParameter(5)));

  // count cumulative number of Y value for pol4 in
  // numOfSigmas sigma range for each bins
  double B_1 = 0;
  for (int i = h1->FindBin(ff1->GetParameter(4) -
                           numOfSigmas * ff1->GetParameter(5));
       i <=
       h1->FindBin(ff1->GetParameter(4) + numOfSigmas * ff1->GetParameter(5));
       i++) {
    B_1 += pol2_1->Eval(h1->GetBinCenter(i));
  }

  double S_1 = SPlusB_1 - B_1;

  double significance_1 = S_1 / sqrt(B_1 + S_1);

  std::cout << "Significance: " << significance_1 << std::endl;

  c1->SaveAs("output/tmp/test.pdf");
}