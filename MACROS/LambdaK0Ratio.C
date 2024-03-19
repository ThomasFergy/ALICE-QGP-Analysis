
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

double calculateError2(double x, double deltaX, double y, double deltaY) {
  double relativeError =
      (1.0 / y) * sqrt(pow(deltaX, 2) + pow((x * deltaY / pow(y, 2)), 2));
  return relativeError;
}

void LambdaK0Ratio() {
  std::vector<double> bins = {0,   0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9,
                              1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9,
                              2,   2.2, 2.4, 2.6, 2.8, 3,   3.2, 3.4, 3.6, 3.8,
                              4,   4.5, 5,   5.5, 6,   7,   8,   10};

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

  double ptCut_K0 = 0;
  double ptCut_Lambda = 0;
  double ptCut_AntiLambda = 0;

  std::string testMCFile = "output/V0MC/AnalysisResults_default_cuts_MC.root";
  // std::string testDataFile =
  //     "output/V0Data/AnalysisResults_default_cuts_p_t.root";

  // corrected K0
  TH1F* K0EffCorrected = EfficiencyCorrection::EfficiencyCorrectionHist(
      K0DataFile, testMCFile, bins, ptCut_K0, V0Type::K0, draw);

  // corrected Lambda
  TH1F* LambdaEffCorrected = EfficiencyCorrection::EfficiencyCorrectionHist(
      LambdaDataFile, testMCFile, testMCFile, bins, ptCut_Lambda,
      V0Type::Lambda, draw);

  // corrected AntiLambda
  TH1F* AntiLambdaEffCorrected = EfficiencyCorrection::EfficiencyCorrectionHist(
      AntiLambdaDataFile, testMCFile, testMCFile, bins, ptCut_AntiLambda,
      V0Type::AntiLambda, draw);

  // lambda + antilambda / 2 K0 ratio
  TH1F* LambdaAntiLambdaToK0Ratio = (TH1F*)LambdaEffCorrected->Clone();
  LambdaAntiLambdaToK0Ratio->Divide(K0EffCorrected);
  LambdaAntiLambdaToK0Ratio->SetMarkerStyle(20);
  LambdaAntiLambdaToK0Ratio->SetMarkerSize(0.8);
  LambdaAntiLambdaToK0Ratio->SetMarkerColor(kBlack);
  LambdaAntiLambdaToK0Ratio->SetLineColor(kBlack);
  LambdaAntiLambdaToK0Ratio->SetLineWidth(1);
  LambdaAntiLambdaToK0Ratio->SetStats(kFALSE);
  // fix errors
  for (int i = 1; i <= LambdaAntiLambdaToK0Ratio->GetNbinsX(); i++) {
    double x = LambdaEffCorrected->GetBinContent(i);
    double deltaX = LambdaEffCorrected->GetBinError(i);
    double y = AntiLambdaEffCorrected->GetBinContent(i);
    double deltaY = AntiLambdaEffCorrected->GetBinError(i);
    double relativeError = calculateError2(x, deltaX, y, deltaY);
    LambdaAntiLambdaToK0Ratio->SetBinError(
        i, relativeError * LambdaAntiLambdaToK0Ratio->GetBinContent(i));
  }
  LambdaAntiLambdaToK0Ratio->SaveAs("Results/LambdaAntiLambdaToK0Ratio.root");
}
