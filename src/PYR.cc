#include "GeneratorInterface/PompytInterface/interface/PYR.h"
#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandFlat.h"

CLHEP::HepRandomEngine* randomEngine;

extern "C" {
  double pyr_(int* idummy)
  {
    //return randomEngine->flat();
    double test = 0.01;
    return test;
  }
}
