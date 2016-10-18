#pragma once

#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/MsgStream.h"

class DataTrans : public Algorithm {
public:
    DataTrans(const std::string& name, ISvcLocator* pSvcLocator); 
    StatusCode initialize() override;
    StatusCode execute() override;
    StatusCode finalize() override;
    StatusCode beginRun() override;
    StatusCode endRun() override;

private:
    bool m_initialized;
};
