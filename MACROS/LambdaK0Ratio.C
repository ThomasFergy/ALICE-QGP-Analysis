
// clang-format off
// for syntax highlighting
#include "../lib/EfficiencyCorrection.h" 
// MACROS for including libs for CLING
R__ADD_INCLUDE_PATH(lib)
#ifdef _WIN32
R__LOAD_LIBRARY(lib/EfficiencyCorrection_cpp.dll)
#else
R__LOAD_LIBRARY(lib/EfficiencyCorrection_cpp.so)
#endif
// clang-format on

void LambdaK0Ratio() {
  std::vector<double> bins;
  for (int i = 0; i < 4 * 7; ++i) {
    bins.push_back(i * 0.25);
  }
  bool draw = true;

  // files
  std::string K0MCFile =
      "Results/K0_CUTS_1/AnalysisResults_optimal_cuts_MC.root";
  std::string LambdaMCFile =
      "Results/LAMBDA_CUTS_4/AnalysisResults_optimal_cuts_MC.root";

  std::string K0DataFile =
      "Results/K0_CUTS_1/AnalysisResults_optimal_cuts_DATA_p_t.root";

  std::string LambdaDataFile =
      "Results/LAMBDA_CUTS_4/AnalysisResults_optimal_cuts_DATA_p_t.root";

  double ptCut_K0 = 1;
  double ptCut_Lambda = 1;

  // get corrected histograms

  TH1F* hK0 = EfficiencyCorrection::EfficiencyCorrectionK0(
      K0DataFile, K0MCFile, bins, ptCut_K0, draw);

  TH1F* hLambda = EfficiencyCorrection::EfficiencyCorrectionLambda(
      LambdaDataFile, LambdaMCFile, bins, ptCut_Lambda, draw);

  // count number of bins for both histograms
  int nBinsK0 = hK0->GetNbinsX();
  int nBinsLambda = hLambda->GetNbinsX();
  std::cout << "Number of bins for K0: " << nBinsK0 << std::endl;
  std::cout << "Number of bins for Lambda: " << nBinsLambda << std::endl;

  // create a new histogram with lambda / k0 ratio
  TH1F* hRatio = new TH1F("hRatio", "hRatio", nBinsK0, &bins[0]);
  hRatio->Divide(hLambda, hK0);
  // save as pdf
  TCanvas* c1 = new TCanvas("c1", "c1", 900, 600);
  hRatio->Draw();
  // dont draw the legend
  hRatio->SetStats(0);
  c1->SaveAs("LambdaK0Ratio.pdf");
}