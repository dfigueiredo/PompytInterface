#include "GeneratorInterface/PompytInterface/interface/PYR.h"

extern "C" {
  double pyr_(int* idummy)
  {
    double r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return r;
  }
}
