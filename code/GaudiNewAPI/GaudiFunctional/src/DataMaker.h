#pragma once

#include <GaudiKernel/AnyDataHandle.h>
#include <GaudiAlg/Producer.h>

#include "ThreeVecEx.h"

typedef ThreeVecEx ThreeVecType;

class DataMaker : public Functional::Producer<ThreeVecType(),
                         Functional::Traits::useDefaults> {
public:
    DataMaker(const std::string& name, ISvcLocator* svcLoc)
             : Functional::Producer( name, svcLoc,
               KeyValue("InputData", {"MyVec"} ) :

    ThreeVecType execute() const override;
};


