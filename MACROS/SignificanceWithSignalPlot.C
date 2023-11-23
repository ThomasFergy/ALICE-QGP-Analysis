#include <vector>

#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1F.h"

/**
 * @brief Plots the significance vs. cut value as well as the frac left of
 * signal
 */
void SignificanceWithSignalPlot(const char* filepath, const char* Title,
                                const char* xlable, const char* ylable,
                                const std::vector<double> significanceVector,
                                std::vector<double> cutVector,
                                std::vector<double> errorVector,
                                std::vector<double> DataSignalVector,
                                std::vector<double> MCSignalVector,
                                std::vector<double> DataSignalErrorVector,
                                std::vector<double> MCSignalErrorVector) {
  // Create a canvas
  TCanvas* c1 = new TCanvas("c1", "Significance", 200, 10, 700, 500);
  // draw the significance on the first pad
  // draw frac of signal left for both MC and DATA on the second pad
  c1->Divide(1, 2);
  c1->cd(1);
  // Create a graph with error bars
  TGraph* gr = new TGraphErrors(significanceVector.size(), &cutVector[0],
                                &significanceVector[0], 0, &errorVector[0]);

  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(0.5);
  gr->Draw("AP");

  // change signal data to be signal left
  double index0Value = DataSignalVector[0];
  for (int i = 0; i < DataSignalVector.size(); i++) {
    DataSignalVector[i] = DataSignalVector[i] / index0Value;
    DataSignalErrorVector[i] = DataSignalErrorVector[i] / index0Value;
  }

  index0Value = MCSignalVector[0];
  for (int i = 0; i < MCSignalVector.size(); i++) {
    MCSignalVector[i] = MCSignalVector[i] / index0Value;
    MCSignalErrorVector[i] = MCSignalErrorVector[i] / index0Value;
  }

  // draw second pad
  c1->cd(2);
  TGraph* gr2 = new TGraphErrors(DataSignalVector.size(), &cutVector[0],
                                 &DataSignalVector[0], 0, 0);

  gr2->SetMarkerColor(2);
  gr2->SetMarkerStyle(20);
  // make size smaller
  gr2->SetMarkerSize(0.5);

  TGraph* gr3 = new TGraphErrors(MCSignalVector.size(), &cutVector[0],
                                 &MCSignalVector[0], 0, 0);

  gr3->SetMarkerColor(4);
  gr3->SetMarkerStyle(20);
  // make size smaller
  gr3->SetMarkerSize(0.5);

  // draw the graphs on the second pad simultaneously
  gr2->Draw("AP");
  gr3->Draw("P");

  // Save the plot
  c1->SaveAs(filepath);
}