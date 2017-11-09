#pragma once

#include <GaudiKernel/AnyDataHandle.h>
#include <GaudiAlg/Producer.h>

#include "ThreeVecEx.h"


class DataMaker : public Gaudi::Functional::Producer<ThreeVecEx()> {
public:
    DataMaker(const std::string& name, ISvcLocator* svcLoc)
             : Producer( name, svcLoc,
               KeyValue("OutputLocation", {"MyVec"})) {}

    ThreeVecEx operator()() const override;
};


