#pragma once

#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/DataObject.h"

class DataTrans : public GaudiAlgorithm {
public:
    using GaudiAlgorithm::GaudiAlgorithm;
    StatusCode execute() override;
};

class DataProducer : public GaudiAlgorithm {
public:
    DataProducer(const std::string& name,
                               ISvcLocator* pSvc);
    StatusCode initialize() override;
    StatusCode execute() override;

private:
    /// Ugly counter, does not respect restarting runs, threading, etc.
    int f_counter = 0;
};

class DataConsumer : public GaudiAlgorithm {
public:
    using GaudiAlgorithm::GaudiAlgorithm;
    StatusCode execute() override;
};
