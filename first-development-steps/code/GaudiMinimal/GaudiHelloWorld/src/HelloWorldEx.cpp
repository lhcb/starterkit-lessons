#include "HelloWorldEx.h"

#include "GaudiKernel/MsgStream.h"

DECLARE_COMPONENT(HelloWorldEx)

HelloWorldEx::HelloWorldEx(const std::string& name, ISvcLocator* ploc) :
    GaudiAlgorithm(name, ploc) {
        // Code here runs when the object gets created
    }


StatusCode HelloWorldEx::initialize() {
  
  StatusCode sc = Algorithm::initialize(); // must be executed first
  if(sc.isFailure() ) return sc; // Initialize failed, propogate

  info() << "Hello World: Inilializing..." << endmsg;
  return StatusCode::SUCCESS;
}

StatusCode HelloWorldEx::execute() {
  info() << "Hello World: Executing..." << endmsg;
  return StatusCode::SUCCESS;
}

StatusCode HelloWorldEx::finalize() {
  info() << "Hello World: Finalizing..." << endmsg;
  return Algorithm::finalize(); // must be executed last
}

