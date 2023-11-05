#include "DataLoader.h"

TH1F *DataLoader::LoadHist(const char *filename, const char *treename,
                           const char *histname) {
  gROOT->Reset();
  TFile *f = new TFile(filename);
  f->cd(treename);
  TH1F *h1 = (TH1F *)gDirectory->Get(histname);
  if (!h1) {
    std::cout << "h was not read\n";
    exit(1);
  }
  return h1;
}