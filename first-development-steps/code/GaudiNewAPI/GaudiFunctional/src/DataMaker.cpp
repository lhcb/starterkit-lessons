#include "DataMaker.h"
#include <GaudiKernel/MsgStream.h>

DECLARE_COMPONENT(DataMaker)

ThreeVecEx DataMaker::operator()() const{
    info() << "executing DataMaker" << endmsg;
    return ThreeVecEx{1, 2, 3};
}

