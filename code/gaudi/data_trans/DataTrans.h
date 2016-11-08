#pragma once

#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/MsgStream.h"

class DataTrans : public Algorithm {
public:
    DataTrans(const std::string& name, ISvcLocator* pSvcLocator); 
    StatusCode initialize() override;
    StatusCode execute() override;

private:
    bool m_initialized;
};

class DataProducer : public Algorithm {
public:
    DataProducer(const std::string& name, ISvcLocator* pSvcLocator); 
    StatusCode initialize() override;
    StatusCode execute() override;

private:
    bool m_initialized;
};

class DataConsumer : public Algorithm {
public:
    DataConsumer(const std::string& name, ISvcLocator* pSvcLocator); 
    StatusCode initialize() override;
    StatusCode execute() override;

private:
    bool m_initialized;
};
