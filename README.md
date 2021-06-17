# PompytInterface

Pompyt is a legacy MC from HERA H1 experiment. It generates diffractive physics process taking into consideration HERA data, as well as, pomeron trajectories and pomeron parton densities.
Pompyt is a diffractive hard scattering in pp, gamma-p and ep collisions, based on pomeron flux and pomeron parton densities (several options included). Also pion exchange is included. Parton showers and Lund hadronization to give complete events. 

Please, see the references [1](https://arxiv.org/pdf/hep-ph/9809374.pdf), [2](https://inspirehep.net/literature/361297), [3](https://cds.cern.ch/record/1379834?ln=pt) and [4](https://arxiv.org/pdf/hep-ph/9808486.pdf).

```bash
cmsrel CMSSW_10_6_18
cd CMSSW_10_6_18/src
cmsenv
mkdir GeneratorInterface/
cd GeneratorInterface
git clone https://github.com/dfigueiredo/PompytInterface.git
cd PompytInterface
scram b -j 8
cd test/
cmsRun Test_Z_minus_M2000_cff.py (or Test_Z_plus_M2000_cff.py)
```

# Pompyt Options

| Options       | Explanation | Comments |
| ------------- |:-------------:|-------------:|
| pythiaPylistVerbosity | (int) [1, 2, 3, 5, 7, 11, 12, 13] | Hadronizer verbosity option |
| pythiaHepMCVerbosity | (bool) | HepMC verbosity |
| maxEventsToPrint | (int) Max number of events | Printed only when verbosity is different than zero |
| comEnergy | (double) center of mass energy  | [GeV] |
| pomZ | (int) [-1 or 1] direction of the pomeron  | (-1: diffractive proton pz < 0 ) |
|  |  | (1: diffractive proton pz > 0) |
| pomPdiss | (int) [1 or 2] | (1: pp->Xp) |
|  | | (2: pp->XY) |
| xiMax | (double) Max. generated xi |  |
| pdiss_B0 | (double) traj. parameter, slop of cross-section |  |
| pdiss_AP | (double) traj. parameter of interception |  |
| pdiss_GLU | (double) fract. of gluon in the proton dissociation |  |
| pdiss_MMAX | (double) max mass of the diffractive system |  |
| PythiaParameters | (pset) parameters from pythia (for diffractive system) |  |

# Experts

Values of t_min and t_max are hard-coded into the interface (src/PompytProducer.cc). As well as the switches for the reggeon/pomeron flux (src/pompyt.f).

