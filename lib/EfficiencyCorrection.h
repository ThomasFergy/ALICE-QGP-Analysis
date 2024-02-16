#ifndef EFFICIENCYCORRECTION_H
#define EFFICIENCYCORRECTION_H

#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TGaxis.h>
#include <TGraph.h>
#include <TH1F.h>
#include <TROOT.h>

#include <iostream>
#include <vector>

enum class V0Type { K0, Lambda, AntiLambda, LambdaAntiLambda };

class EfficiencyCorrection {
 public:
  static TH1F* eff(std::string MCFile, std::vector<double>& bins, V0Type v0Type,
                   bool draw = false);

  static TH1F* EfficiencyCorrectionHist(std::string DataFile,
                                        std::string MCFile,
                                        std::vector<double>& bins, double ptCut,
                                        V0Type v0Type, bool draw = false);

  // THIS IS UNUSED SO FAR. NEEDS TO BE GENERALISED FOR ALL V0 TYPES
  static TH1F* ApplyFit(TH1F* h, std::vector<double>& bins, double ptCut,
                        V0Type v0Type);

  // THIS IS UNUSED SO FAR. NEEDS TO BE GENERALISED FOR ALL V0 TYPES
  static double LevyTsallis(double* x, double* par);
};

#endif