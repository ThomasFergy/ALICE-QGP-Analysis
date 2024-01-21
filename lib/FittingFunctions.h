#ifndef FITTINGFUNCTIONS_H
#define FITTINGFUNCTIONS_H

/**
 * Class containing usefull functions for fitting data to.
 */

class FittingFunctions {
 public:
  /**
   * Crystalball function.
   */
  static double Polynomial2(double* x, double* par);

  static double DoubleSidedCrystalballFunction(double* x, double* par);

  static double Gaussian(double* x, double* par);

  static double DSCBWithPolynomial(double* x, double* par);

  static double GaussianWithPolynomial(double* x, double* par);

  static double HorizontalLinePlusGenericLine(double* x, double* par);
};

#endif