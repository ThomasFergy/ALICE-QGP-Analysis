
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
  double ptCut_Lambda = 0.6;
  double ptCut_AntiLambda = 0.6;

  std::string testMCFile = "output/V0MC/AnalysisResults_default_cuts_MC.root";

  // get test efficiencies from file
  TH1F* hK0Eff = EfficiencyCorrection::eff(K0MCFile, bins, V0Type::K0, draw);
  TH1F* hLambdaEff =
      EfficiencyCorrection::eff(LambdaMCFile, bins, V0Type::Lambda, draw);
  TH1F* hAntiLambdaEff = EfficiencyCorrection::eff(AntiLambdaMCFile, bins,
                                                   V0Type::AntiLambda, draw);
}
