#pragma once

#include <GaudiAlg/GaudiAlgorithm.h>
#include <GaudiKernel/Property.h>
#include <GaudiKernel/MsgStream.h>
#include <GaudiKernel/AnyDataHandle.h>
#include "ThreeVecEx.h"


class DataProducerProp : public GaudiAlgorithm {
public:
    //  ill-formed: using GaudiAlgorithm::GaudiAlgorithm;
    DataProducerProp(const std::string& name, ISvcLocator* svcLoc): GaudiAlgorithm(name, svcLoc) {} 
    StatusCode initialize() override;
    StatusCode execute() override;

private:
    Gaudi::Property<int> m_starting_value{this, "StartingValue", 0};
    AnyDataHandle<ThreeVecEx> m_vec{"/Event/UnknownVec2", Gaudi::DataHandle::Writer, this};
    int f_counter = 0;
};


class DataConsumerProp : public GaudiAlgorithm {
public:
    //using GaudiAlgorithm::GaudiAlgorithm;
    DataConsumerProp(const std::string& name, ISvcLocator* svcLoc): GaudiAlgorithm(name, svcLoc) {}
    StatusCode execute() override;
private:
    AnyDataHandle<ThreeVecEx> m_vec{"/Event/UnknownVec2", Gaudi::DataHandle::Reader, this};
};
