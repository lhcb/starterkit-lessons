#include "DataTrans.h"

DECLARE_COMPONENT(DataProducer)

StatusCode DataProducer::initialize() {
    f_counter = m_starting_value.value();
    return StatusCode::SUCCESS;
}

StatusCode DataProducer::execute() {
    auto vec = new AnyDataWrapper<ThreeVecEx>(ThreeVecEx{f_counter, f_counter+1, f_counter+2});

    put(vec, "/Event/UnknownVec");
    info() << "executing DataProducer:" << f_counter << " ..." << endmsg;
    f_counter++;
    return StatusCode::SUCCESS;
}


DECLARE_COMPONENT(DataConsumer)

StatusCode DataConsumer::execute() {
    auto base = getIfExists<AnyDataWrapperBase>("/Event/UnknownVec");
    const auto i = dynamic_cast<const AnyDataWrapper<ThreeVecEx>*>(base);
    const auto vec = i->getData();
    info() << "executing DataConsumer: ("<< vec <<")..." << endmsg;

    return StatusCode::SUCCESS;
}

DECLARE_COMPONENT(DataConsumerProp)

StatusCode DataConsumerProp::execute() {
    info() << "Using property version: (" << *m_vec.get() << ")" << endmsg;

    return StatusCode::SUCCESS;
}
