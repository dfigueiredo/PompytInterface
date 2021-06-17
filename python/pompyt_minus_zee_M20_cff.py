import FWCore.ParameterSet.Config as cms

generator = cms.EDProducer("PompytProducer",
    pythiaPylistVerbosity = cms.untracked.int32(1),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    maxEventsToPrint = cms.untracked.int32(3),
    comEnergy = cms.untracked.double(7000.0),
    ##  pomeron direction
    pomZ = cms.untracked.double(-1.0),
    ##  pomPdiss = 1 (pp->Xp),  2( pp->XY)
    pomPdiss = cms.untracked.int32(1),
    ##  xi max
    xiMax = cms.untracked.double(0.2),
    pdiss_B0 = cms.untracked.double(1.5),
    pdiss_AP = cms.untracked.double(0.016),
    pdiss_GLU = cms.untracked.double(0.5),
    pdiss_MMAX = cms.untracked.double(2000.0),
    PythiaParameters = cms.PSet(
        pythiaUESettings = cms.vstring(
             'MSUB(1)=1         !Incl Z0/gamma* production',
             'MSTP(43)=3', 
             'MDME(174,1)=0     !Z decay into d dbar', 
             'MDME(175,1)=0     !Z decay into u ubar', 
             'MDME(176,1)=0     !Z decay into s sbar', 
             'MDME(177,1)=0     !Z decay into c cbar', 
             'MDME(178,1)=0     !Z decay into b bbar', 
             'MDME(179,1)=0     !Z decay into t tbar', 
             'MDME(182,1)=1     !Z decay into e- e+', 
             'MDME(183,1)=0     !Z decay into nu_e nu_ebar', 
             'MDME(184,1)=0     !Z decay into mu- mu+', 
             'MDME(185,1)=0     !Z decay into nu_mu nu_mubar', 
             'MDME(186,1)=0     !Z decay into tau- tau+', 
             'MDME(187,1)=0     !Z decay into nu_tau nu_taubar'
        ),
        parameterSets = cms.vstring('pythiaUESettings','pytParameters'),
        pytParameters = cms.vstring('MSEL=0','CKIN(1)=20.')
    )
)


