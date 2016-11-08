#include "DataTrans.h"

DECLARE_COMPONENT(DataTrans)

DataTrans::DataTrans(const std::string& name, ISvcLocator* ploc) :
    Algorithm(name, ploc) {
        m_initialized = false;
    }

StatusCode DataTrans::initialize() {
  if( m_initialized ) return StatusCode::SUCCESS;

  info() << "initializing DataTrans...." << endmsg;
  m_initialized = true;
  return StatusCode::SUCCESS;
}

StatusCode DataTrans::execute() {
  info() << "executing...." << endmsg;
  return StatusCode::SUCCESS;
}

DECLARE_COMPONENT(DataProducer)

DataProducer::DataProducer(const std::string& name, ISvcLocator* ploc) :
    Algorithm(name, ploc) {
        m_initialized = false;
    }

StatusCode DataProducer::initialize() {
  if( m_initialized ) return StatusCode::SUCCESS;

  info() << "initializing DataProducer...." << endmsg;
  m_initialized = true;
  return StatusCode::SUCCESS;
}

StatusCode DataProducer::execute() {
  info() << "executing...." << endmsg;
  return StatusCode::SUCCESS;
}

DECLARE_COMPONENT(DataConsumer)

DataConsumer::DataConsumer(const std::string& name, ISvcLocator* ploc) :
    Algorithm(name, ploc) {
        m_initialized = false;
    }

StatusCode DataConsumer::initialize() {
  if( m_initialized ) return StatusCode::SUCCESS;

  info() << "initializing DataConsumer...." << endmsg;
  m_initialized = true;
  return StatusCode::SUCCESS;
}

StatusCode DataConsumer::execute() {
  info() << "executing DataConsumer...." << endmsg;
  return StatusCode::SUCCESS;
}


