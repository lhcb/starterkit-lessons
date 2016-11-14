#include "DataMaker.h"
#include <GaudiKernel/MsgStream.h>

DECLARE_COMPONENT(DataMaker)

ThreeVecType DataMaker::operator() {
    info() << "executing DataMaker" << endmsg;
    return ThreeVecEx{1, 2, 3};
}

