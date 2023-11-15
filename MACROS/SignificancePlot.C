// clang-format off
// for syntax highlighting
#include "../lib/DataLoader.h" 
#include "../lib/FittingFunctions.h"
// MACROS for including libs for CLING
R__ADD_INCLUDE_PATH(lib)
#ifdef _WIN32
R__LOAD_LIBRARY(lib/DataLoader_cpp.dll)
R__LOAD_LIBRARY(lib/FittingFunctions_cpp.dll)
#else
R__LOAD_LIBRARY(lib/DataLoader_cpp.so)
R__LOAD_LIBRARY(lib/FittingFunctions_cpp.so)
#endif
// clang-format on

#include <vector>

#include "TCanvas.h"
#include "TGraph.h"

/**
 * @brief Plots the significance vs. cut value
 */
void SignificancePlot(const char* filepath,
                      const std::vector<double> significanceVector,
                      std::vector<double> cutVector) {
  // plot the significance vs. cut value
  TCanvas* c1 =
      new TCanvas("c1", "Significance vs. Cut Value", 200, 10, 700, 500);
  c1->SetGrid();
  TGraph* gr = new TGraph(significanceVector.size(), &cutVector[0],
                          &significanceVector[0]);
  gr->SetTitle("Significance vs. Cut Value");
  gr->GetXaxis()->SetTitle("Cut Value");
  gr->GetYaxis()->SetTitle("Significance");
  gr->SetMarkerColor(4);
  gr->SetMarkerStyle(21);
  gr->Draw("ALP");
  c1->Update();
  c1->SaveAs(filepath);

  return;
}