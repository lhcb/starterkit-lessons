#include "DataTrans.h"

DECLARE_COMPONENT(DataTrans)

StatusCode DataTrans::execute() {
  info() << "executing DataTrans..." << endmsg;
  return StatusCode::SUCCESS;
}


DECLARE_COMPONENT(DataProducer)

DataProducer::DataProducer(const std::string& name,
                           ISvcLocator* pSvc) : Algorithm (name, pSvc) {
    declareProperty("StartingValue", f_counter, "Starting value"); 
}

StatusCode DataProducer::execute() {
  info() << "executing DataProducer:" << f_counter << " ..." << endmsg;
  f_counter++;
  return StatusCode::SUCCESS;
}


DECLARE_COMPONENT(DataConsumer)

StatusCode DataConsumer::execute() {
  info() << "executing DataConsumer..." << endmsg;
  return StatusCode::SUCCESS;
}


