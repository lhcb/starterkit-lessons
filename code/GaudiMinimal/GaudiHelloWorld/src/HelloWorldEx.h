#pragma once

#include "GaudiKernel/Algorithm.h"

class HelloWorldEx : public GaudiAlgorithm {
public:
    /// Needs a constructor or delagating constructor
    HelloWorldEx(const std::string& name, ISvcLocator* pSvcLocator); 
    
    /// Runs at algorithm initialization
    StatusCode initialize() override;
    
    /// This runs every event
    StatusCode execute() override;

    /// Any finalization goes here
    StatusCode finalize() override;
};
