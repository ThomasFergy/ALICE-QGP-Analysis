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
  static double DoubleSidedCrystalballFunction(double* x, double* par);
};

#endif