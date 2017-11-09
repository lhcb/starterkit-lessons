#include "DataConsumer.h"
#include <GaudiKernel/MsgStream.h>

DECLARE_COMPONENT(DataConsumer)

void DataConsumer::operator()(const ThreeVecEx &input) const{
    info() << "executing DataConsumer: " << input << endmsg;
}

