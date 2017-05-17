#include "DataTrans.h"
#include "Event/ThreeVecEx.h"

DECLARE_COMPONENT(DataProducer)

DataProducer::DataProducer(const std::string& name,
                           ISvcLocator* pSvc) : GaudiAlgorithm (name, pSvc) {
    declareProperty("StartingValue", f_counter, "Starting value");

}

StatusCode DataProducer::execute() {
    auto vec = new LHCb::ThreeVecEx();
    vec->setX(f_counter);
    vec->setY(f_counter+1);
    vec->setZ(f_counter+2);

    put(vec, "/Event/UnknownVec");
    info() << "executing DataProducer:" << f_counter << " ..." << endmsg;
    f_counter++;
    return StatusCode::SUCCESS;
}


DECLARE_COMPONENT(DataConsumer)

StatusCode DataConsumer::execute() {
    auto vec = get<LHCb::ThreeVecEx>("/Event/UnknownVec");
    info() << "executing DataConsumer: ("<< *vec <<")..." << endmsg;
    return StatusCode::SUCCESS;
}


