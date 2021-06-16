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

#include "GeneratorInterface/PompytInterface/interface/PompytProducer.h"

//used for defaults
static const unsigned long kNanoSecPerSec = 1000000000;
static const unsigned long kAveEventPerSec = 200;

namespace {
 CLHEP::HepRandomEngine& getEngineReference(edm::StreamID stream)
  {
   
   Service<RandomNumberGenerator> rng;
   if(!rng.isAvailable()) {
    throw cms::Exception("Configuration")
       << "The RandomNumberProducer module requires the RandomNumberGeneratorService\n"
          "which appears to be absent.  Please add that service to your configuration\n"
          "or remove the modules that require it.";
   }

// The Service has already instantiated an engine.  Make contact with it.
   return (rng->getEngine(stream));
  }
}

PompytProducer::PompytProducer(const edm::ParameterSet& iConfig):
  evt(0),
  pythiaPylistVerbosity_ (iConfig.getUntrackedParameter<int>("pythiaPylistVerbosity",0)),
  pythiaHepMCVerbosity_ (iConfig.getUntrackedParameter<bool>("pythiaHepMCVerbosity",false)),
  maxEventsToPrint_ (iConfig.getUntrackedParameter<int>("maxEventsToPrint",1)),
  comenergy(iConfig.getUntrackedParameter<double>("comEnergy",14000.)),
  extCrossSect(iConfig.getUntrackedParameter<double>("crossSection", -1.)),
  extFilterEff(iConfig.getUntrackedParameter<double>("filterEfficiency", -1.)),
  pomz(iConfig.getUntrackedParameter<double>("pomZ", -1.)),
  pompdiss(iConfig.getUntrackedParameter<int>("pomPdiss",1)),
  xi_max(iConfig.getUntrackedParameter<double>("xiMax",0.1)),
  pdiss_b0(iConfig.getUntrackedParameter<double>("pdiss_B0",1.5)),
  pdiss_ap(iConfig.getUntrackedParameter<double>("pdiss_AP",0.016)),
  pdiss_glu(iConfig.getUntrackedParameter<double>("pdiss_GLU",0.5)),
  pdiss_mmax(iConfig.getUntrackedParameter<double>("pdiss_MMAX",2000.0)),
  eventNumber_(0)
{

  //register your products
  /* Examples
     produces<ExampleData2>();

  //if do put with a label
  produces<ExampleData2>("label");

  //if you want to put into the Run
  produces<ExampleData2,InRun>();
  */
  //now do what ever other initialization is needed

  // PYLIST Verbosity Level
  // Valid PYLIST arguments are: 1, 2, 3, 5, 7, 11, 12, 13
  pythiaPylistVerbosity_ = iConfig.getUntrackedParameter<int>("pythiaPylistVerbosity",0);

  // HepMC event verbosity Level
  pythiaHepMCVerbosity_ = iConfig.getUntrackedParameter<bool>("pythiaHepMCVerbosity",false);

  //Max number of events printed on verbosity level 
  maxEventsToPrint_ = iConfig.getUntrackedParameter<int>("maxEventsToPrint",0);

  // Set PYTHIA parameters in a single ParameterSet
  ParameterSet pythia_params =
    iConfig.getParameter<ParameterSet>("PythiaParameters") ;

  // The parameter sets to be read (default, min bias, user ...) in the
  // proper order.
  vector<string> setNames =
    pythia_params.getParameter<vector<string> >("parameterSets");

  // Loop over the sets
  for ( unsigned i=0; i<setNames.size(); ++i ) {

    string mySet = setNames[i];

    // Read the PYTHIA parameters for each set of parameters
    vector<string> pars =
      pythia_params.getParameter<vector<string> >(mySet);

    if (mySet != "SLHAParameters" && mySet != "CSAParameters"){

      // Loop over all parameters and stop in case of mistake
      for( vector<string>::const_iterator
	  itPar = pars.begin(); itPar != pars.end(); ++itPar ) {

	static string sRandomValueSetting("MRPY(1)");
	if( 0 == itPar->compare(0,sRandomValueSetting.size(),sRandomValueSetting) ) {
	  throw edm::Exception(edm::errors::Configuration,"PythiaError")
	    <<" attempted to set random number using pythia command 'MRPY(1)' this is not allowed.\n  Please use the RandomNumberGeneratorService to set the random number seed.";
	}
	if( ! call_pygive(*itPar) ) {
	  throw edm::Exception(edm::errors::Configuration,"PythiaError")
	    <<" pythia did not accept the following \""<<*itPar<<"\"";
	}
      }
    }
  }

  pompyt1.pomi[2-1]=pomz;        // pomeron direction
  pomcut1.pomcut[1-1]=0.0001;    // xi min
  pomcut1.pomcut[2-1]=xi_max;    // xi max
  pomcut1.pomcut[3-1]=-3.;       // t max
  pomcut1.pomcut[4-1]=0.;        // t min
  pompyt1.ipom[1-1]=pompdiss;    // pp->Xp (1) or pp->XY (2)
  pompyt1.pomi[19-1]=pdiss_b0;   // B0 for pdiss events
  pompyt1.pomi[20-1]=pdiss_ap;   // alpha' for pdiss events
  pomcut1.pomcut[10-1]=pdiss_glu;
  pomcut1.pomcut[11-1]=1.1;
  pomcut1.pomcut[12-1]=pdiss_mmax;

  call_pomini(comenergy);  
  cout << endl; 

}


PompytProducer::~PompytProducer()
{

  // do anything here that needs to be done at destruction time
  // (e.g. close files, deallocate resources etc.)

  call_pomout();

}


//
// member functions
//

// ------------ method called to produce the data  ------------
  void
PompytProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  /* This is an event example
  //Read 'ExampleData' from the Event
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);

  //Use the ExampleData to create an ExampleData2 which 
  // is put into the Event
  iEvent.put(std::make_unique<ExampleData2>(*pIn));
  */

  /* this is an EventSetup example
  //Read SetupData from the SetupRecord in the EventSetup
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
  */

  //  call_pyevnt();      // generate one event with Pythia
  call_pompyt();

  // convert to stdhep format
  call_pyhepc( 1 );

  // convert stdhep (hepevt) to hepmc
  HepMC::GenEvent* evt = conv2.read_next_event();
  evt->set_signal_process_id(pypars.msti[0]);
  evt->set_event_scale(pypars.pari[16]);
  ++eventNumber_;
  evt->set_event_number(eventNumber_);
  evt->weights().push_back( pomcut1.pomcut[9-1] );

  //******** Verbosity ********
  if(iEvent.id().event() <= maxEventsToPrint_ &&
      (pythiaPylistVerbosity_ || pythiaHepMCVerbosity_)) {

    // Prints PYLIST info
    if(pythiaPylistVerbosity_) {
      call_pylist(pythiaPylistVerbosity_);
    }

    // Prints HepMC event
    if(pythiaHepMCVerbosity_) {
      cout << "Event process = " << pypars.msti[0] << endl
	<< "----------------------" << endl;
      evt->print();
    }
  }

  return;


}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
  void
PompytProducer::beginStream(edm::StreamID)
{
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
PompytProducer::endStream() {
}

// ------------ method called when starting to processes a run  ------------
/*
   void
   PompytProducer::beginRun(edm::Run const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when ending the processing of a run  ------------
/*
   void
   PompytProducer::endRun(edm::Run const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when starting to processes a luminosity block  ------------
/*
   void
   PompytProducer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when ending the processing of a luminosity block  ------------
/*
   void
   PompytProducer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PompytProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


bool
PompytProducer::call_pygive(const std::string& iParm ) {

  int numWarn = pydat1.mstu[26]; //# warnings
  int numErr = pydat1.mstu[22];// # errors

  //call the fortran routine pygive with a fortran string
  PYGIVE( iParm.c_str(), iParm.length() );
  //  PYGIVE( iParm );  
  //if an error or warning happens it is problem
  return pydat1.mstu[26] == numWarn && pydat1.mstu[22] == numErr;

}

//define this as a plug-in
DEFINE_FWK_MODULE(PompytProducer);
