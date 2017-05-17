#pragma once

#include <GaudiKernel/AnyDataHandle.h>
#include <GaudiAlg/Consumer.h>

#include "ThreeVecEx.h"


class DataConsumer : public Gaudi::Functional::Consumer<void(const ThreeVecEx&)> {
public:
    DataConsumer(const std::string& name, ISvcLocator* svcLoc)
             : Consumer( name, svcLoc,
               KeyValue("InputLocation", {"MyVec"})) {}

    void operator()(const ThreeVecEx&) const override;
};


