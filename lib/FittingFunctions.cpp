#include "FittingFunctions.h"

#include <cmath>

double FittingFunctions::DoubleSidedCrystalballFunction(double* x,
                                                        double* par) {
  double alpha_l = par[0];
  double alpha_h = par[1];
  double n_l = par[2];
  double n_h = par[3];
  double mean = par[4];
  double sigma = par[5];
  double N = par[6];
  double t = (x[0] - mean) / sigma;
  double result = 0.0;
  double fact1TLessMinosAlphaL = alpha_l / n_l;
  double fact2TLessMinosAlphaL = (n_l / alpha_l) - alpha_l - t;
  double fact1THihgerAlphaH = alpha_h / n_h;
  double fact2THigherAlphaH = (n_h / alpha_h) - alpha_h + t;

  if (-alpha_l <= t && alpha_h >= t) {
    result = exp(-0.5 * t * t);
  } else if (t < -alpha_l) {
    result = exp(-0.5 * alpha_l * alpha_l) *
             pow(fact1TLessMinosAlphaL * fact2TLessMinosAlphaL, -n_l);

  } else if (t > alpha_h) {
    result = exp(-0.5 * alpha_h * alpha_h) *
             pow(fact1THihgerAlphaH * fact2THigherAlphaH, -n_h);
  }
  return N * result;
}