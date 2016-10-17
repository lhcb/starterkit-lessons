#include "HelloWorld.h"

DECLARE_COMPONENT(HelloWorld)

HelloWorld::HelloWorld(const std::string& name, ISvcLocator* ploc) :
    Algorithm(name, ploc) {
        m_initialized = false;
    }

StatusCode HelloWorld::initialize() {
  if( m_initialized ) return StatusCode::SUCCESS;

  info() << "initializing...." << endmsg;
  m_initialized = true;
  return StatusCode::SUCCESS;
}

StatusCode HelloWorld::execute() {
  info() << "executing...." << endmsg;
  return StatusCode::SUCCESS;
}

StatusCode HelloWorld::finalize() {
  info() << "finalizing...." << endmsg;

  m_initialized = false;
  return StatusCode::SUCCESS;
}

StatusCode HelloWorld::beginRun() {
  info() << "beginning new run...." << endmsg;

  m_initialized = true;
  return StatusCode::SUCCESS;
}

StatusCode HelloWorld::endRun() {
  info() << "ending new run...." << endmsg;

  m_initialized = true;
  return StatusCode::SUCCESS;
}

