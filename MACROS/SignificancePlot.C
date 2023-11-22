#include <vector>

#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1F.h"

/**
 * @brief Plots the significance vs. cut value
 */
void SignificancePlot(const char* filepath, const char* Title,
                      const char* xlable, const char* ylable,
                      const std::vector<double> significanceVector,
                      std::vector<double> cutVector,
                      std::vector<double> errorVector) {
  // Create a canvas
  TCanvas* c1 = new TCanvas("c1", "Significance", 200, 10, 700, 500);
  c1->SetGrid();
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(2);
  c1->SetTickx(1);
  c1->SetTicky(1);
  //  c1->SetLeftMargin(0.15);
  // c1->SetRightMargin(0.05);
  // c1->SetTopMargin(0.05);
  c1->SetBottomMargin(0.15);
  c1->SetFrameBorderMode(0);
  c1->SetFrameBorderMode(0);

  // Create a graph with error bars
  TGraph* gr = new TGraphErrors(significanceVector.size(), &cutVector[0],
                                &significanceVector[0], 0, &errorVector[0]);

  // set x range
  gr->GetXaxis()->SetRangeUser(-0.1, 2.1);
  gr->SetMarkerColor(4);
  gr->SetMarkerStyle(20);
  gr->SetTitle(Title);
  gr->GetXaxis()->SetTitle(xlable);
  gr->GetYaxis()->SetTitle(ylable);
  gr->Draw("AP");

  // Save the plot
  c1->SaveAs(filepath);

  // Clean up
  delete c1;
  delete gr;

  return;
}