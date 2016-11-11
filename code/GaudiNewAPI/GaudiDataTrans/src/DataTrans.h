#pragma once

#include <GaudiAlg/GaudiAlgorithm.h>
#include <GaudiKernel/Property.h>
#include <GaudiKernel/MsgStream.h>
#include <GaudiKernel/AnyDataWrapper.h>
#include <GaudiKernel/AnyDataHandle.h>
#include "ThreeVecEx.h"


class DataProducer : public GaudiAlgorithm {
public:
    using GaudiAlgorithm::GaudiAlgorithm;
    StatusCode initialize() override;
    StatusCode execute() override;

private:
    Gaudi::Property<int> m_starting_value{this, "StartingValue", 0};
    int f_counter = 0;
};

class DataConsumer : public GaudiAlgorithm {
public:
    using GaudiAlgorithm::GaudiAlgorithm;
    StatusCode execute() override;
};

