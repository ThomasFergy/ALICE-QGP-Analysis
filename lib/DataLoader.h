#ifndef DATALOADER_H
#define DATALOADER_H

#include <TFile.h>
#include <TH1F.h>
#include <TROOT.h>
#include <TTree.h>

#include <iostream>

/**
 * Class containing usefull functions for loading data.
 */
class DataLoader {
 public:
  /**
   * Load a histogram from a file.
   */
  static TH1F *LoadHist(const char *filename, const char *treename,
                        const char *histname);
};

#endif
