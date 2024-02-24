
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
  std::vector<double> bins = {0,   0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9,
                              1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9,
                              2,   2.2, 2.4, 2.6, 2.8, 3,   3.2, 3.4, 3.6, 3.8,
                              4,   4.5, 5,   6,   7,   8,   10};

  // bins = {0,    0.1, 0.2, 0.3, 0.4, 0.5, 0.6,  0.7, 0.8,  0.9,
  //         1.0,  1.1, 1.2, 1.3, 1.4, 1.5, 1.75, 2,   2.25, 2.5,
  //         2.75, 3,   3.5, 4,   4.5, 5,   6,    8,   10};

  // bins = {};

  // for (int i = 0; i < 100; i++) {
  //   bins.push_back(i * 0.1);
  // }

  bool draw = true;

  // files
  std::string K0MCFile =
      "Results/K0_CUTS_1/AnalysisResults_optimal_cuts_MC.root";

  std::string LambdaMCFile =
      "Results/LAMBDA_CUTS_4/AnalysisResults_optimal_cuts_MC.root";

  std::string AntiLambdaMCFile =
      "Results/LAMBDA_CUTS_4/AnalysisResults_optimal_cuts_ANTI_MC.root";

  std::string K0DataFile =
      "Results/K0_CUTS_1/AnalysisResults_optimal_cuts_DATA_p_t.root";

  std::string LambdaDataFile =
      "Results/LAMBDA_CUTS_4/AnalysisResults_optimal_cuts_DATA_p_t.root";

  std::string AntiLambdaDataFile =
      "Results/LAMBDA_CUTS_4/AnalysisResults_optimal_cuts_ANTI_DATA_p_t.root";

  double ptCut_K0 = 0.2;
  double ptCut_Lambda = 1;
  double ptCut_AntiLambda = 1;

  // uncorrected lambda to anti-lambda ratio

  TH1F* hLambda = EfficiencyCorrection::getUncorrectedHist(
      LambdaDataFile, bins, 0, V0Type::Lambda, draw);

  TH1F* hAntiLambda = EfficiencyCorrection::getUncorrectedHist(
      AntiLambdaDataFile, bins, 0, V0Type::AntiLambda, draw);

  // divide the histograms
  hAntiLambda->Divide(hLambda);
  hAntiLambda->SetTitle("#Lambda / #bar{#Lambda} ratio uncorrected");
  hAntiLambda->SaveAs("Results/UncorrectedLambdaAntiLambdaRatio.root");

  // corrected lambda to anti-lambda ratio

  hLambda = EfficiencyCorrection::EfficiencyCorrectionHist(
      LambdaDataFile, LambdaMCFile, bins, 0, V0Type::Lambda, draw);

  hAntiLambda = EfficiencyCorrection::EfficiencyCorrectionHist(
      AntiLambdaDataFile, AntiLambdaMCFile, bins, 0, V0Type::AntiLambda, draw);

  // divide the histograms
  hAntiLambda->Divide(hLambda);
  hAntiLambda->SetTitle("#Lambda / #bar{#Lambda} ratio Corrected");
  hAntiLambda->SaveAs("Results/CorrectedLambdaAntiLambdaRatio.root");

  // uncorrected lambda to K0 ratio

  TH1F* hK0 = EfficiencyCorrection::getUncorrectedHist(K0DataFile, bins, 0,
                                                       V0Type::K0, draw);

  hLambda = EfficiencyCorrection::getUncorrectedHist(LambdaDataFile, bins, 0,
                                                     V0Type::Lambda, draw);

  // divide the histograms lambda / K0
  hLambda->Divide(hK0);
  hLambda->SetTitle("#Lambda / K^{0} ratio uncorrected");
  hLambda->SaveAs("Results/UncorrectedLambdaK0Ratio.root");

  // corrected lambda to K0 ratio

  hK0 = EfficiencyCorrection::EfficiencyCorrectionHist(
      K0DataFile, K0MCFile, bins, 0, V0Type::K0, draw);

  hLambda = EfficiencyCorrection::EfficiencyCorrectionHist(
      LambdaDataFile, LambdaMCFile, bins, 0, V0Type::Lambda, draw);

  // divide the histograms lambda / K0
  hLambda->Divide(hK0);
  hLambda->SetTitle("#Lambda / K^{0} ratio Corrected");
  hLambda->SaveAs("Results/CorrectedLambdaK0Ratio.root");

  // uncorrected anti-lambda to K0 ratio

  hK0 = EfficiencyCorrection::getUncorrectedHist(K0DataFile, bins, 0,
                                                 V0Type::K0, draw);

  hAntiLambda = EfficiencyCorrection::getUncorrectedHist(
      AntiLambdaDataFile, bins, 0, V0Type::AntiLambda, draw);

  // divide the histograms anti-lambda / K0
  hAntiLambda->Divide(hK0);
  hAntiLambda->SetTitle("#bar{#Lambda} / K^{0} ratio uncorrected");
  hAntiLambda->SaveAs("Results/UncorrectedAntiLambdaK0Ratio.root");

  // corrected anti-lambda to K0 ratio

  hK0 = EfficiencyCorrection::EfficiencyCorrectionHist(
      K0DataFile, K0MCFile, bins, 0, V0Type::K0, draw);

  hAntiLambda = EfficiencyCorrection::EfficiencyCorrectionHist(
      AntiLambdaDataFile, AntiLambdaMCFile, bins, 0, V0Type::AntiLambda, draw);

  // divide the histograms anti-lambda / K0
  // log scale for x axis
  hAntiLambda->Divide(hK0);
  hAntiLambda->SetTitle("#bar{#Lambda} / K^{0} ratio Corrected");
  hAntiLambda->SaveAs("Results/CorrectedAntiLambdaK0Ratio.root");
}