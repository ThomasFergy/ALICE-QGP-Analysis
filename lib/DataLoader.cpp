#include "DataLoader.h"

TH1F *DataLoader::LoadHist(const char *filename, const char *treename,
                           const char *histname) {
  gROOT->Reset();
  TFile *f = new TFile(filename);
  f->cd(treename);
  TH1F *h1 = (TH1F *)gDirectory->Get(histname);
  if (!h1) {
    std::cout << "histogram was not read\n";
    exit(1);
  }
  return h1;
}

TH1F *DataLoader::LoadHist(const char *filename, const char *treename1,
                           const char *treename2, const char *histname) {
  gROOT->Reset();

  TFile *f = new TFile(filename);
  // enter the first TDirectoryFile
  f->cd();
  TDirectoryFile *dir1 = (TDirectoryFile *)gDirectory->Get(treename1);
  // enter the second TDirectoryFile
  dir1->cd();
  TDirectoryFile *dir2 = (TDirectoryFile *)gDirectory->Get(treename2);
  // enter the TTree
  dir2->cd();

  TH1F *h1 = (TH1F *)gDirectory->Get(histname);
  if (!h1) {
    std::cout << "histogram was not read\n";
    exit(1);
  }
  return h1;
}