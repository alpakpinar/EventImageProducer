// -*- C++ -*-
//
// Package:    PhysicsTools/EventImageProducer
// Class:      EventImageProducer
// 
/**\class EventImageProducer EventImageProducer.cc PhysicsTools/EventImageProducer/plugins/EventImageProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Alp Akpinar
//         Created:  Mon, 13 Sep 2021 18:04:40 GMT
//
//


// system include files
#include <memory>
#include <cmath>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/NanoAOD/interface/FlatTable.h"

#include <TH2.h>

//
// class declaration
//

class EventImageProducer : public edm::stream::EDProducer<> {
   public:
      explicit EventImageProducer(const edm::ParameterSet&);
      
      ~EventImageProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
      
      // produces<nanoaod::FlatTable>();
      

   private:
      virtual void beginStream(edm::StreamID) override;
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      virtual void endStream() override;

      // File service to write ROOT output
      edm::Service<TFileService> m_fileservice;

      edm::EDGetTokenT<pat::PackedCandidateCollection> pfToken_;
      
      // 2D histogram to save eta/phi based event images
      TH2F *h_eventImage;

      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
EventImageProducer::EventImageProducer(const edm::ParameterSet& iConfig):
   pfToken_(consumes<pat::PackedCandidateCollection>(iConfig.getParameter<edm::InputTag>("pfCands")))
{
   //register your products
/* Examples
   produces<ExampleData2>();

   //if do put with a label
   produces<ExampleData2>("label");
 
   //if you want to put into the Run
   produces<ExampleData2,InRun>();
*/
   // usesResource("TFileService");
   h_eventImage = m_fileservice->make<TH2F>("eta_phi_img", "Event image: Eta-phi map from PF candidates", 50, -5, 5, 50, -M_PI, M_PI);
   
}


EventImageProducer::~EventImageProducer()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
EventImageProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   // Read the PF candidates collection
   edm::Handle<pat::PackedCandidateCollection> pfCandidates;
   iEvent.getByToken(pfToken_, pfCandidates);

   // Loop over PF candidates
   int numCandidates = pfCandidates->size();
   
   for (int i=0; i < numCandidates; i++) {
      const pat::PackedCandidate pfCandidate = (*pfCandidates)[i];

      // Read eta and phi values for the PF candidate we're looking at 
      // These will be used to construct event images
      float pfEta = pfCandidate.eta();
      float pfPhi = pfCandidate.phi();
      float pfEnergy = pfCandidate.energy();

      // Fill the 2D histogram for our event image
      h_eventImage->Fill(pfEta, pfPhi, pfEnergy);
   }

}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
EventImageProducer::beginStream(edm::StreamID)
{
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
EventImageProducer::endStream() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
EventImageProducer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
EventImageProducer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
EventImageProducer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
EventImageProducer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
EventImageProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(EventImageProducer);
