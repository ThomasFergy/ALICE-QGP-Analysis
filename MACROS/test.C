#include <TFile.h>
#include <TTree.h>

#include <iostream>

// prints out all dcanegtopv, dcapostopv, v0cospa, dcav0dau, and v0radius vlaues
// for all events in the AOD file.
void test() {
  // Load AOD file
  TFile *f = TFile::Open("data/AO2D.root");

  // enter first key
  f->cd("DF_2270075478217600");

  // entre tree O2v0_001
  TTree *t = (TTree *)gDirectory->Get("O2v0_001");
  t->GetListOfBranches()->Print();

  // Print number of events
  // std::cout << "Number of events: " << t->GetEntries() << std::endl;

  // Load branches
  // float dcanegtopv, dcapostopv, v0cospa, dcav0dau, v0radius;
  // t->SetBranchAddress("dcanegtopv", &dcanegtopv);
  // t->SetBranchAddress("dcapostopv", &dcapostopv);
  // t->SetBranchAddress("v0cospa", &v0cospa);
  // t->SetBranchAddress("dcav0dau", &dcav0dau);
  // t->SetBranchAddress("v0radius", &v0radius);
}