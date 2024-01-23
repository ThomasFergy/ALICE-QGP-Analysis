#include <TFile.h>
#include <TTree.h>

void PrintMetaData() {
  /**
   * Simple macro to print the AO2D.root metaData
   */
  TFile *f = new TFile("data/V0Data/AO2D.root");
  TDirectory *dir = (TDirectory *)f->Get("metaData");
  dir->Print();
}