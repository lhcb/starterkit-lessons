#pragma once

#include <GaudiKernel/AnyDataHandle.h>
#include <GaudiAlg/Producer.h>
#include <tuple>

#include "ThreeVecEx.h"

using namespace Gaudi;

typedef AnyDataHandle<ThreeVecEx> ThreeVecType;

class DataMaker : public Functional::Producer<ThreeVecType()> {
public:
    DataMaker(const std::string& name, ISvcLocator* svcLoc)
             : Functional::Producer( name, svcLoc,
               KeyValue("OutputLocation", {"MyVec"} ) :

    ThreeVecType execute() const override;
};


