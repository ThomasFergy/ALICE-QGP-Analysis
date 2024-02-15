#ifndef EFFICIENCYCORRECTION_H
#define EFFICIENCYCORRECTION_H

#include <TCanvas.h>
#include <TFile.h>
#include <TH1F.h>
#include <TMath.h>
#include <TROOT.h>

#include <iostream>
#include <vector>

class EfficiencyCorrection {
 public:
  static TH1F* effK0(std::string MCFile, std::vector<double>& bins,
                     bool draw = false);

  static TH1F* effLambda(std::string MCFile, std::vector<double>& bins,
                         bool draw = false);

  static TH1F* effAntiLambdaIndv(std::string MCFile, std::vector<double>& bins,
                                 bool draw = false);

  static TH1F* effLambdaIndv(std::string MCFile, std::vector<double>& bins,
                             bool draw = false);

  static TH1F* EfficiencyCorrectionK0(std::string DataFile, std::string MCFile,
                                      std::vector<double>& bins, double ptCut,
                                      bool draw = false);

  static TH1F* EfficiencyCorrectionLambda(std::string DataFile,
                                          std::string MCFile,
                                          std::vector<double>& bins,
                                          double ptCut, bool draw = false);

  static TH1F* EfficiencyCorrectionAntiLambdaIndv(std::string DataFile,
                                                  std::string MCFile,
                                                  std::vector<double>& bins,
                                                  double ptCut,
                                                  bool draw = false);

  static TH1F* EfficiencyCorrectionLambdaIndv(std::string DataFile,
                                              std::string MCFile,
                                              std::vector<double>& bins,
                                              double ptCut, bool draw = false);
};

#endif