# PompytInterface

Migrating PompytInterface from CMSSW_4_2_X to CMSSW_10_6_X series.

'''bash
cmsrel CMSSW_10_6_18
cd CMSSW_10_6_18/src
cmsenv
mkdir GeneratorInterface/
cd GeneratorInterface
git clone https://github.com/dfigueiredo/PompytInterface.git
cd PompytInterface
scram b -j 8
cd test/
cmsRun TestPompyt_v2.py
'''
