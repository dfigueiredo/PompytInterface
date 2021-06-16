// -*- C++ -*-
//
// Package:    GeneratorInterface/PompytProducer
// Class:      PompytProducer
// 
/**\class PompytProducer PompytProducer.cc GeneratorInterface/PompytProducer/plugins/PompytProducer.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
*/
//
// Original Author:  Diego Figueiredo
//         Created:  Wed, 16 Jun 2021 12:31:55 GMT
//
//

// system include files
#include "time.h"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/PluginManager/interface/PluginManager.h"
#include "FWCore/Framework/interface/MakerMacros.h"

// class include files
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "HepMC/GenEvent.h"
#include "HepMC/PythiaWrapper6_4.h"
#include "HepMC/IO_HEPEVT.h"

#include "CLHEP/Random/RandFlat.h"
#include "GeneratorInterface/PompytInterface/interface/Pompyt.h"
#include "GeneratorInterface/PompytInterface/interface/PYR.h"

#include "GeneratorInterface/Herwig7Interface/interface/Proxy.h"
#include "GeneratorInterface/Herwig7Interface/interface/RandomEngineGlue.h"

#include "GeneratorInterface/Herwig7Interface/interface/RandomEngineGlue.h"
#include "GeneratorInterface/Herwig7Interface/interface/HerwigUIProvider.h"

#include <ThePEG/Repository/EventGenerator.h>
#include <ThePEG/EventRecord/Event.h>
#include <ThePEG/Vectors/HepMCTraits.h>

using namespace edm;
using namespace std;

#define PYGIVE pygive_
extern "C" {
  void PYGIVE(const char*,int length);
}

//
// class declaration
//

namespace CLHEP {
  class HepRandomEngine;
  class RandFlat;
}

class PompytProducer : public edm::stream::EDProducer<> {
  public:
    explicit PompytProducer(const edm::ParameterSet&);
    ~PompytProducer();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    virtual void beginStream(edm::StreamID) override;
    virtual void produce(edm::Event&, const edm::EventSetup&) override;
    virtual void endStream() override;

    //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
    //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

    /// Interface to the PYGIVE pythia routine, with add'l protections
    bool call_pygive(const std::string& iParm);
    void clear();

    // ----------member data ---------------------------
    HepMC::GenEvent  *evt;

    /// Pythia PYLIST Verbosity flag
    unsigned int pythiaPylistVerbosity_;

    /// HepMC verbosity flag
    bool pythiaHepMCVerbosity_;

    /// Events to print if verbosity
    unsigned int maxEventsToPrint_;

    // external cross section and filter efficiency
    double comenergy;
    double extCrossSect;
    double extFilterEff;

    //pompyt
    double pomz;       //+-1 pomeron goes in +-z direction
    int pompdiss;      //1- elastic,2-pdiss
    double xi_max;     //max xi
    double pdiss_b0;   // B0 for pdiss events
    double pdiss_ap;   // alpha' for pdiss events
    double pdiss_glu;  // fraction of gluon in p-diss fragmentation 
    double pdiss_mmax; // max MY mass
    int                     eventNumber_;

    HepMC::IO_HEPEVT conv2;

};

//
// constants, enums and typedefs
//


//
// static data member definitions
//
