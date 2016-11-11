#pragma once

#include "GaudiKernel/Algorithm.h"

/// For simplicity, this inherits from Algorithm. Most will inherit from GaudiAlgorithm instead
class HelloWorldEx : public Algorithm {
public:
    /// Needs a constructor or delagating constructor
    HelloWorldEx(const std::string& name, ISvcLocator* pSvcLocator); 
    
    /// Runs at algorithm initialization
    StatusCode initialize() override;
    
    /// This runs every event
    StatusCode execute() override;

    /// Any finalization goes here
    StatusCode finalize() override;
    
    /// This runs at the beginning of a run
    StatusCode beginRun() override;

    /// This runs at the end of a run
    StatusCode endRun() override;
};
