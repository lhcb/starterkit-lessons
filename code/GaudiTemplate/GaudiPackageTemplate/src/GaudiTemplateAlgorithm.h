#pragma once

#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/Property.h"

class GaudiTemplateAlgorithm : public GaudiAlgorithm {
public:
    /// Needs a constructor or delagating constructor
    // GaudiTemplateAlgorithm(const std::string& name, ISvcLocator* pSvcLocator); 
    using GaudiAlgorithm::GaudiAlgorithm;
    
    /// Runs at algorithm initialization
    // StatusCode initialize() override;
    
    /// This runs every event
    StatusCode execute() override;

    /// Any finalization goes here
    // StatusCode finalize() override;
    
    /// This runs at the beginning of a run
    // StatusCode beginRun() override;

    /// This runs at the end of a run
    // StatusCode endRun() override;
};
