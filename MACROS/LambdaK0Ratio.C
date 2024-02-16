
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

  double ptCut_K0 = 1.5;
  double ptCut_Lambda = 1.5;

  // make efficiency plots
  TH1F* hK0Eff = EfficiencyCorrection::eff(K0MCFile, bins, V0Type::K0, draw);

  TH1F* hLambdaEff =
      EfficiencyCorrection::eff(LambdaMCFile, bins, V0Type::Lambda, draw);
}