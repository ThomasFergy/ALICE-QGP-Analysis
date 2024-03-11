
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

  double ptCut_K0 = 0.2;
  double ptCut_Lambda = 0.6;
  double ptCut_AntiLambda = 0.6;

  // get efficiencies for k0
  TH1F* hK0Eff = EfficiencyCorrection::eff(K0MCFile, bins, V0Type::K0, draw);

  hK0Eff->Rebin(2);

  // half all counts in the histogram
  for (int i = 1; i <= hK0Eff->GetNbinsX(); i++) {
    hK0Eff->SetBinContent(i, hK0Eff->GetBinContent(i) / 2);
  }

  // change title etc
  hK0Eff->SetTitle("Efficiency for K^{0}_{S}");
  hK0Eff->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  hK0Eff->GetYaxis()->SetTitle("Efficiency");

  hK0Eff->SetMarkerStyle(20);
  hK0Eff->SetMarkerSize(0.7);
  hK0Eff->SetMarkerColor(kBlack);
  hK0Eff->SetLineColor(kBlack);

  hK0Eff->SaveAs("Results/K0Eff.root");

  bins = {0, 0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.6, 2, 2.4, 2.6, 3, 3.5, 4, 6, 10};

  // get corrected lambda to K0 ratio
  TH1F* hK0 = EfficiencyCorrection::EfficiencyCorrectionHist(
      K0DataFile, K0MCFile, bins, 0, V0Type::K0, draw);

  TH1F* hLambda = EfficiencyCorrection::EfficiencyCorrectionHist(
      LambdaDataFile, LambdaMCFile, AntiLambdaMCFile, bins, 0, V0Type::Lambda,
      draw);

  // get uncorrected K0 and saveas

  TH1F* hK0Uncorrected = EfficiencyCorrection::getUncorrectedHist(
      K0DataFile, bins, 0, V0Type::K0, draw);

  hK0Uncorrected->SetTitle("Uncorrected K^{0}_{S}");
  hK0Uncorrected->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  hK0Uncorrected->GetYaxis()->SetTitle("Counts");
  hK0Uncorrected->SetMarkerStyle(20);
  hK0Uncorrected->SetMarkerSize(0.7);
  hK0Uncorrected->SetMarkerColor(kBlack);
  hK0Uncorrected->SetLineColor(kBlack);
  hK0Uncorrected->SaveAs("Results/K0Uncorrected.root");

  // do the same for lambda
  TH1F* hLambdaUncorrected = EfficiencyCorrection::getUncorrectedHist(
      LambdaDataFile, bins, 0, V0Type::Lambda, draw);

  hLambdaUncorrected->SetTitle("Uncorrected #Lambda");
  hLambdaUncorrected->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  hLambdaUncorrected->GetYaxis()->SetTitle("Counts");
  hLambdaUncorrected->SetMarkerStyle(20);
  hLambdaUncorrected->SetMarkerSize(0.7);
  hLambdaUncorrected->SetMarkerColor(kBlack);
  hLambdaUncorrected->SetLineColor(kBlack);
  hLambdaUncorrected->SaveAs("Results/LambdaUncorrected.root");

  // bins = {0,   0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1,
  //         1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2,   2.2, 2.4, 2.6,
  //         2.8, 3,   3.2, 3.4, 3.6, 3.8, 4,   4.5, 5,   5.5, 6,   10};

  bins = {0,   0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1,
          1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2,   2.2, 2.4, 2.6,
          2.8, 3,   3.2, 3.4, 3.6, 3.8, 4,   4.5, 5,   5.5, 6};

  // uncorrected anti lambda to lambda ratio
  TH1F* hAntiLambdaUncorrected = EfficiencyCorrection::getUncorrectedHist(
      AntiLambdaDataFile, bins, 0, V0Type::AntiLambda, draw);

  hLambdaUncorrected = EfficiencyCorrection::getUncorrectedHist(
      LambdaDataFile, bins, 0, V0Type::Lambda, draw);

  TH1F* hAntiLambda = (TH1F*)hAntiLambdaUncorrected->Clone("hAntiLambda");

  hAntiLambda->Divide(hLambdaUncorrected);

  hAntiLambda->SetTitle("Uncorrected #bar{#Lambda} / #Lambda");
  hAntiLambda->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  hAntiLambda->GetYaxis()->SetTitle("#bar{#Lambda} / #Lambda");
  hAntiLambda->SetMarkerStyle(20);
  hAntiLambda->SetMarkerSize(0.7);
  hAntiLambda->SetMarkerColor(kBlack);
  hAntiLambda->SetLineColor(kBlack);
  hAntiLambda->SaveAs("Results/AntiLambdaLambdaRatioUncorrected.root");
}
