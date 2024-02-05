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
};

double FittingFunctions::Gaussian(double* x, double* par) {
  double mean = par[0];
  double sigma = par[1];
  double N = par[2];
  double t = (x[0] - mean) / sigma;
  double result = exp(-0.5 * t * t);
  return N * result;
};

double FittingFunctions::Polynomial2(double* x, double* par) {
  double result = par[0] + par[1] * x[0] + par[2] * x[0] * x[0];
  return result;
};

double FittingFunctions::DSCBWithPolynomial(double* x, double* par) {
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
  double polyPars[3] = {
      par[7],
      par[8],
      par[9],
  };
  return N * result + Polynomial2(x, polyPars);
};

double FittingFunctions::GaussianWithPolynomial(double* x, double* par) {
  double mean = par[0];
  double sigma = par[1];
  double N = par[2];
  double t = (x[0] - mean) / sigma;
  double result = exp(-0.5 * t * t);
  double polyPars[3] = {
      par[3],
      par[4],
      par[5],
  };
  return N * result + Polynomial2(x, polyPars);
};

double FittingFunctions::GaussianWithHorizontalLine(double* x, double* par) {
  double mean = par[0];
  double sigma = par[1];
  double N = par[2];
  double t = (x[0] - mean) / sigma;
  double result = exp(-0.5 * t * t);
  return N * result + par[3] + par[4] * x[0];
};

double FittingFunctions::HorizontalLinePlusGenericLine(double* x, double* par) {
  double meetingPoint = par[0];
  double line1Height = par[1];
  double line2C = par[2];
  double line2M = par[3];

  if (x[0] < meetingPoint) {
    return line1Height;
  } else {
    return line2C + line2M * x[0];
  }
};