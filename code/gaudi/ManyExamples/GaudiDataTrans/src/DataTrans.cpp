#include "DataTrans.h"
#include "GaudiExamples/Counter.h"

DECLARE_COMPONENT(DataTrans)

StatusCode DataTrans::execute() {
  info() << "executing DataTrans..." << endmsg;
  return StatusCode::SUCCESS;
}

DECLARE_COMPONENT(DataProducer)

DataProducer::DataProducer(const std::string& name,
                           ISvcLocator* pSvc) : GaudiAlgorithm (name, pSvc) {
    declareProperty("StartingValue", f_counter, "Starting value");

}
StatusCode DataProducer::initialize() {
    return StatusCode::SUCCESS;
}

StatusCode DataProducer::execute() {
    auto counter = new Gaudi::Examples::Counter();
    counter->set(f_counter);
    put(counter, "/Event/Unknown");
    info() << "executing DataProducer:" << f_counter << " ..." << endmsg;
    f_counter++;
    return StatusCode::SUCCESS;
}


DECLARE_COMPONENT(DataConsumer)

StatusCode DataConsumer::execute() {
    Gaudi::Examples::Counter* counter = get<Gaudi::Examples::Counter>("/Event/Unknown");
    info() << "executing DataConsumer ("<< counter->value() <<")..." << endmsg;
    counter->increment();
    return StatusCode::SUCCESS;
}


