
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
  std::vector<double> bins = {0,   0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8,
                              0.9, 1.0, 1.2, 1.4, 1.6, 1.8, 2,   2.5, 3,
                              3.5, 4,   4.5, 5,   6,   7,   8,   10};
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

  ////////////////////////////////////////////////////////////////
  // Lambda to K0 ratio
  TH1F* hK0 = EfficiencyCorrection::EfficiencyCorrectionHist(
      K0DataFile, K0MCFile, bins, ptCut_K0, V0Type::K0, draw);
  TH1F* hLambda = EfficiencyCorrection::EfficiencyCorrectionHist(
      LambdaDataFile, LambdaMCFile, bins, ptCut_Lambda,
      V0Type::LambdaAntiLambda, draw);

  // divide the histograms
  TH1F* hRatio = (TH1F*)hLambda->Clone("hRatio");
  hRatio->Divide(hK0);
  hRatio->Draw();
  hRatio->SaveAs("LambdaK0Ratio.root");

  ////////////////////////////////////////////////////////////////
  // Indv Lambda to K0 ratio
  TH1F* hLambdaIndv = EfficiencyCorrection::EfficiencyCorrectionHist(
      LambdaDataFile, LambdaMCFile, bins, ptCut_Lambda, V0Type::Lambda, draw);

  // divide the histograms
  hRatio = (TH1F*)hLambdaIndv->Clone("hRatio");
  hRatio->Divide(hK0);
  hRatio->Draw();
  hRatio->SaveAs("LambdaIndvK0Ratio.root");
}