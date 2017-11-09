#include "DataTransProp.h"

DECLARE_COMPONENT(DataProducerProp)

StatusCode DataProducerProp::initialize() {
    f_counter = m_starting_value.value();
    return StatusCode::SUCCESS;
}

StatusCode DataProducerProp::execute() {
    m_vec.put(ThreeVecEx{f_counter, f_counter+1, f_counter+2});

    info() << "executing DataProducer (prop): " << f_counter << " ..." << endmsg;
    f_counter++;
    return StatusCode::SUCCESS;
}

DECLARE_COMPONENT(DataConsumerProp)

StatusCode DataConsumerProp::execute() {
    info() << "executing DataConsumer (prop): {" << *m_vec.get() << "}" << endmsg;

    return StatusCode::SUCCESS;
}
