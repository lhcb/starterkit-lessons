#include "DataTrans.h"

DECLARE_COMPONENT(DataTrans)

DataTrans::DataTrans(const std::string& name, ISvcLocator* ploc) :
    Algorithm(name, ploc) {
        m_initialized = false;
    }

StatusCode DataTrans::initialize() {
  if( m_initialized ) return StatusCode::SUCCESS;

  info() << "initializing...." << endmsg;
  m_initialized = true;
  return StatusCode::SUCCESS;
}

StatusCode DataTrans::execute() {
  info() << "executing...." << endmsg;
  return StatusCode::SUCCESS;
}

StatusCode DataTrans::finalize() {
  info() << "finalizing...." << endmsg;

  m_initialized = false;
  return StatusCode::SUCCESS;
}

StatusCode DataTrans::beginRun() {
  info() << "beginning new run...." << endmsg;

  m_initialized = true;
  return StatusCode::SUCCESS;
}

StatusCode DataTrans::endRun() {
  info() << "ending new run...." << endmsg;

  m_initialized = true;
  return StatusCode::SUCCESS;
}

