#include <vector>

#include "TCanvas.h"
#include "TGraph.h"
#include "TH1.h"

/**
 * @brief Plots the significance vs. cut value
 */
void SignificancePlot(const char* filepath, const char* Title,
                      const char* xlable, const char* ylable,
                      const std::vector<double> significanceVector,
                      std::vector<double> cutVector) {
  // plot the significance vs. cut value
  // do not connect the points
  TCanvas* c1 =
      new TCanvas("c1", "Significance vs. Cut Value", 200, 10, 700, 500);
  c1->SetGrid();
  TGraph* gr = new TGraph(significanceVector.size(), &cutVector[0],
                          &significanceVector[0]);
  // do not connect the points
  gr->SetLineWidth(0);
  gr->SetTitle(Title);
  gr->GetXaxis()->SetTitle(xlable);
  gr->GetYaxis()->SetTitle(ylable);
  gr->SetMarkerColor(4);
  // circle
  gr->SetMarkerStyle(8);
  gr->Draw("ALP");
  c1->Update();
  c1->SaveAs(filepath);
  return;
}