#include "HelloWorldEx.h"

DECLARE_COMPONENT(HelloWorldEx)

HelloWorldEx::HelloWorldEx(const std::string& name, ISvcLocator* ploc) :
    Algorithm(name, ploc) {
        m_initialized = false;
    }

StatusCode HelloWorldEx::initialize() {
  if( m_initialized ) return StatusCode::SUCCESS;

  info() << "initializing hello world..." << endmsg;
  m_initialized = true;
  return StatusCode::SUCCESS;
}

StatusCode HelloWorldEx::execute() {
  info() << "executing hello world..." << endmsg;
  return StatusCode::SUCCESS;
}

StatusCode HelloWorldEx::finalize() {
  info() << "finalizing hello world..." << endmsg;

  m_initialized = false;
  return StatusCode::SUCCESS;
}

StatusCode HelloWorldEx::beginRun() {
  info() << "beginning new run...." << endmsg;

  m_initialized = true;
  return StatusCode::SUCCESS;
}

StatusCode HelloWorldEx::endRun() {
  info() << "ending new run...." << endmsg;

  m_initialized = true;
  return StatusCode::SUCCESS;
}

