#pragma once

#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/MsgStream.h"

class HelloWorld : public Algorithm {
public:
    HelloWorld(const std::string& name, ISvcLocator* pSvcLocator); 
    StatusCode initialize() override;
    StatusCode execute() override;
    StatusCode finalize() override;
    StatusCode beginRun() override;
    StatusCode endRun() override;

private:
    bool m_initialized;

    /// These data members are used in the execution of this algorithm
    /// They are set in the initialisation phase by the job options service
    IntegerProperty m_int;
    DoubleProperty  m_double;
    StringProperty  m_string;
};
