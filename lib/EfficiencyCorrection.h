#ifndef EFFICIENCYCORRECTION_H
#define EFFICIENCYCORRECTION_H

#include <TCanvas.h>
#include <TFile.h>
#include <TH1F.h>
#include <TMath.h>
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
};

#endif