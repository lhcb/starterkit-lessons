#pragma once

#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/AnyDataWrapper.h"
#include "GaudiKernel/AnyDataHandle.h"
#include <iostream>

struct ThreeVecEx {
    int x;
    int y;
    int z;
};

std::ostream& operator<< (std::ostream& io, const ThreeVecEx& vec) {
    return io << vec.x << " "
              << vec.y << " "
              << vec.z;
}

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


class DataConsumerProp : public GaudiAlgorithm {
public:
    //using GaudiAlgorithm::GaudiAlgorithm;
    DataConsumerProp(const std::string& name, ISvcLocator* svcLoc): GaudiAlgorithm(name, svcLoc) {}
    StatusCode execute() override;
private:
    AnyDataHandle<ThreeVecEx> m_vec{"/Event/UnknownVec", Gaudi::DataHandle::Reader, this};
};
