#include "GaudiTemplateAlgorithm.h"

#include "GaudiKernel/MsgStream.h"

DECLARE_COMPONENT(GaudiTemplateAlgorithm)

//GaudiTemplateAlgorithm::GaudiTemplateAlgorithm(const std::string& name, ISvcLocator* ploc) :
//    Algorithm(name, ploc) {
//        // Code here runs when the object gets created
//    }


//StatusCode GaudiTemplateAlgorithm::initialize() {
//  
//  StatusCode sc = Algorithm::initialize(); // must be executed first
//  if(sc.isFailure() ) return sc; // Initialize failed, propogate
//
//  info() << "Hello World: Inilializing..." << endmsg;
//  return StatusCode::SUCCESS;
//}

StatusCode GaudiTemplateAlgorithm::execute() {
  info() << "GaudiTemplateAlg: Executing..." << endmsg;
  return StatusCode::SUCCESS;
}

//StatusCode GaudiTemplateAlgorithm::finalize() {
//  info() << "Hello World: Finalizing..." << endmsg;
//  return Algorithm::finalize(); // must be executed last
//}

//StatusCode GaudiTemplateAlgorithm::beginRun() {
//  info() << "Hello World: Begining run..." << endmsg;
//  return StatusCode::SUCCESS;
//}

//StatusCode GaudiTemplateAlgorithm::endRun() {
//  info() << "Hello World: Ending run..." << endmsg;
//  return StatusCode::SUCCESS;
//}

