#pragma once

#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/DataObject.h"

class DataTrans : public Algorithm {
public:
    using Algorithm::Algorithm;
    StatusCode execute() override;
};

class DataProducer : public Algorithm {
public:
    DataProducer(const std::string& name,
                               ISvcLocator* pSvc);
    StatusCode execute() override;

private:
    /// Ugly counter, does not respect restarting runs, threading, etc.
    int f_counter = 0;
};

class DataConsumer : public Algorithm {
public:
    using Algorithm::Algorithm;
    StatusCode execute() override;
};
