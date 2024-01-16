#include <iostream>
#include <vector>

#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TLine.h"
#include "TMultiGraph.h"
#include "TStyle.h"

double calculateRatioError(double A, double delta_A, double B, double delta_B) {
  double ratio = A / B;
  double relative_error_A = delta_A / A;
  double relative_error_B = delta_B / B;

  // Propagation of uncertainties formula
  double ratio_error =
      ratio * sqrt(pow(relative_error_A, 2) + pow(relative_error_B, 2));

  return ratio_error;
}

/**
 * @brief Plots the significance vs. cut value as well as the frac left of
 * signal
 */
void SignificanceWithSignalPlot(
    const char* filepath, const char* Title, const char* xlable,
    const char* ylable, const std::vector<double> significanceVector,
    std::vector<double> cutVector, std::vector<double> errorVector,
    std::vector<double> DataSignalVector, std::vector<double> MCSignalVector,
    std::vector<double> DataSignalErrorVector,
    std::vector<double> MCSignalErrorVector, bool drawLine = false) {
  TGraph* gr1 = new TGraphErrors(significanceVector.size(), &cutVector[0],
                                 &significanceVector[0], 0, &errorVector[0]);

  gStyle->SetTitleFontSize(0.1);
  gStyle->SetTitleXSize(0.06);
  gStyle->SetTitleYSize(0.06);

  gr1->SetMarkerStyle(20);
  gr1->SetMarkerSize(0.5);

  double index0Value = DataSignalVector[0];
  double index0Error = DataSignalErrorVector[0];
  for (int i = 0; i < DataSignalVector.size(); i++) {
    DataSignalVector[i] = DataSignalVector[i] / index0Value;
    DataSignalErrorVector[i] =
        calculateRatioError(DataSignalVector[i], DataSignalErrorVector[i],
                            index0Value, index0Error);
  }

  index0Value = MCSignalVector[0];
  index0Error = MCSignalErrorVector[0];
  for (int i = 0; i < MCSignalVector.size(); i++) {
    MCSignalVector[i] = MCSignalVector[i] / index0Value;
    MCSignalErrorVector[i] = calculateRatioError(
        MCSignalVector[i], MCSignalErrorVector[i], index0Value, index0Error);
  }

  TGraph* gr2a =
      new TGraphErrors(DataSignalVector.size(), &cutVector[0],
                       &DataSignalVector[0], 0, &DataSignalErrorVector[0]);

  gr2a->SetMarkerStyle(33);
  gr2a->SetMarkerSize(1);

  TGraph* gr2b =
      new TGraphErrors(MCSignalVector.size(), &cutVector[0], &MCSignalVector[0],
                       0, &MCSignalErrorVector[0]);

  gr2b->SetMarkerColor(2);
  gr2b->SetMarkerStyle(33);
  gr2b->SetMarkerSize(0.9);

  if (drawLine) {
    // fit polynomial to gr1
    TF1* pol = new TF1("pol", "pol5");
    pol->SetParameters(248, 46, 0, 0, 0);
    gr1->Fit(pol, "Q");
    // draw pol2
    gr1->GetFunction("pol")->SetLineColor(kBlue);
    gr1->GetFunction("pol")->SetLineWidth(2);
    gr1->GetFunction("pol")->SetLineStyle(2);

    // print x value at max value of polynomial
    double xMax = gr1->GetFunction("pol")->GetMaximumX();
    std::cout << "xMax: " << xMax << std::endl;

    // draw vertical line at xMax over full y range
    double yMin = gr1->GetYaxis()->GetXmin();
    double yMax = gr1->GetYaxis()->GetXmax();
    std::cout << "yMin: " << yMin << std::endl;
    std::cout << "yMax: " << yMax << std::endl;
    TLine* line = new TLine(xMax, yMin, xMax, yMax);
    line->SetLineColor(kGreen);
    line->SetLineWidth(2);
    line->SetLineStyle(2);
    gr1->GetListOfFunctions()->Add(line);

    yMin = gr2a->GetYaxis()->GetXmin();
    yMax = gr2a->GetYaxis()->GetXmax();
    std::cout << "yMin: " << yMin << std::endl;
    std::cout << "yMax: " << yMax << std::endl;
    TLine* line2 = new TLine(xMax, yMin, xMax, yMax);
    line2->SetLineColor(kGreen);
    line2->SetLineWidth(2);
    line2->SetLineStyle(2);
    gr2a->GetListOfFunctions()->Add(line2);
  }

  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 600);

  // Create a pad for the first graph (top)
  TPad* pad1 = new TPad("pad1", "pad1", 0, 0.5, 1, 1);
  pad1->SetBottomMargin(0);  // No margin at the bottom
  pad1->Draw();
  pad1->cd();

  // Draw the first graph on the top pad
  gr1->Draw("AP");
  // make title bigger
  gr1->SetTitle(Title);
  gr1->GetYaxis()->SetTitle("#frac{S}{#sqrt{S+B}}");
  gr1->GetYaxis()->SetTitleOffset(0.7);

  // Create a pad for the second graph (bottom)
  canvas->cd();
  TPad* pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.5);
  pad2->SetTopMargin(0);
  pad2->SetGridy();
  pad2->Draw();
  pad2->cd();

  // Draw the second graph on the bottom pad
  gr2a->Draw("AP");
  gr2a->SetTitle();
  gr2b->Draw("P");
  gr2b->SetTitle();
  gr2a->GetXaxis()->SetTitleOffset(0.7);
  gr2a->GetYaxis()->SetTitleOffset(0.7);
  gr2a->GetXaxis()->SetTitle(xlable);
  gr2a->GetYaxis()->SetTitle("Fraction of Signal Left");

  // Legend for mc and data
  TLegend* legend = new TLegend(0.8, 0.83, 0.98, 1);
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);
  legend->AddEntry(gr2a, "Data", "p");
  legend->AddEntry(gr2b, "MC", "p");
  legend->Draw();

  canvas->cd();

  canvas->Update();
  canvas->SaveAs(filepath);
}