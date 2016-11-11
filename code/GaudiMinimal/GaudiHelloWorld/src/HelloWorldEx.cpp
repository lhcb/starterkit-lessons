#include "HelloWorldEx.h"

DECLARE_COMPONENT(HelloWorldEx)

HelloWorldEx::HelloWorldEx(const std::string& name, ISvcLocator* ploc) :
    Algorithm(name, ploc) {
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

StatusCode HelloWorldEx::beginRun() {
  info() << "Hello World: Begining run..." << endmsg;
  return StatusCode::SUCCESS;
}

StatusCode HelloWorldEx::endRun() {
  info() << "Hello World: Ending run..." << endmsg;
  return StatusCode::SUCCESS;
}

