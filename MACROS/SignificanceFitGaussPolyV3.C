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
 * @brief Fit the histogram with DSCB and pol1, and calculate significance
 */
void SignificanceFitGaussPolyV3(const bool isMC, const bool isK0,
                                const double xLow, const double xHigh,
                                const char* filename, const char* outputname,
                                std::vector<double> fit_params,
                                const int numOfSigmas = 3) {
  std::unique_ptr<TH1F> h1 = std::make_unique<TH1F>();
  if (!isMC) {
    if (isK0) {
      h1 = std::make_unique<TH1F>(*DataLoader::LoadHist(
          filename, "strangeness_tutorial", "hMassK0Short"));
    } else {
      h1 = std::make_unique<TH1F>(*DataLoader::LoadHist(
          filename, "strangeness_tutorial", "hMassLambda"));
    }
  } else {
    if (isK0) {
      h1 = std::make_unique<TH1F>(
          *DataLoader::LoadHist(filename, "strangeness_tutorial", "kzeroShort",
                                "hMassK0ShortTrueRec"));
    } else {
      h1 = std::make_unique<TH1F>(*DataLoader::LoadHist(
          filename, "strangeness_tutorial", "kLambda", "hMassLambdaTrueRec"));
    }
  }

  h1->SetAxisRange(xLow, xHigh);
  h1->Sumw2();

  std::unique_ptr<TF1> ff1 = std::make_unique<TF1>(
      "fit", FittingFunctions::GaussianWithHorizontalLine, xLow, xHigh, 5);

  double* fit_params_arr = &fit_params[0];

  // may as well use these values as initial guesses
  fit_params_arr[0] = h1->GetMean();

  // set initial guesses
  ff1->SetParameters(fit_params_arr[0], fit_params_arr[1], fit_params_arr[2],
                     fit_params_arr[3], fit_params_arr[4]);

  ff1->SetParNames("mean", "sigma", "N", "pol1_0", "pol1_1");
  h1->Fit(ff1.get(), "R");
  std::unique_ptr<TCanvas> c1 = std::make_unique<TCanvas>("c1", "c1", 800, 600);
  h1->Draw();

  // draw line
  std::unique_ptr<TF1> pol2 = std::make_unique<TF1>(
      "pol2", FittingFunctions::GaussianWithHorizontalLine, xLow, xHigh, 2);
  pol2->SetParameters(ff1->GetParameter(3), ff1->GetParameter(4));
  pol2->SetLineColor(kGreen);
  pol2->Draw("same");

  // count number of entries in numOfSigmas sigma range
  double SPlusB = h1->Integral(
      h1->FindBin(ff1->GetParameter(0) - numOfSigmas * ff1->GetParameter(1)),
      h1->FindBin(ff1->GetParameter(0) + numOfSigmas * ff1->GetParameter(1)));

  double B = pol2->Integral(
                 ff1->GetParameter(0) - numOfSigmas * ff1->GetParameter(1),
                 ff1->GetParameter(0) + numOfSigmas * ff1->GetParameter(1)) /
             h1->GetBinWidth(0);
  double SPlusBBinErr = 0;
  for (int i = h1->FindBin(ff1->GetParameter(0) -
                           numOfSigmas * ff1->GetParameter(1));
       i <=
       h1->FindBin(ff1->GetParameter(0) + numOfSigmas * ff1->GetParameter(1));
       i++) {
    // count bin error for S
    SPlusBBinErr += h1->GetBinError(i) * h1->GetBinError(i);
  }

  double S = SPlusB - B;

  double significance = S / sqrt(B + S);

  std::cout << "$$$" << significance << "$$$" << std::endl;

  double Serr = sqrt(SPlusBBinErr);  // assume B is negligible

  double SignificanceErr = 0.5 * (1 / sqrt(S)) * Serr;

  std::cout << "$$$" << SignificanceErr << "$$$" << std::endl;

  std::cout << "$$$" << S << "$$$" << std::endl;

  std::cout << "$$$" << Serr << "$$$" << std::endl;

  // stdout all fit parameters
  std::cout << "Fit parameters: " << std::endl;
  for (int i = 0; i < 5; i++) {
    std::cout << "$$$" << ff1->GetParName(i) << "$$$" << ff1->GetParameter(i)
              << "$$$" << std::endl;
  }

  std::string outputdir = "output/figures/batch_mass_plots/";
  // make output directory if it doesn't exist
  std::string command = "mkdir -p " + outputdir;
  system(command.c_str());
  // add .gitingore to outputdir
  std::string gitKeepPath = outputdir + ".gitkeep";
  command = "touch " + gitKeepPath;
  system(command.c_str());
  std::string outputname_str = outputdir + outputname;

  c1->SaveAs(outputname_str.c_str());
}