// Include files

// from Gaudi
#include "Event/MCHeader.h"
#include "Event/MCVertex.h"
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
// local
#include "MCTupleToolRedecay.h"

//-----------------------------------------------------------------------------
// Implementation file for class : MCTupleToolRedecay
//
// 2017-1-26 : Dominik Muller
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( MCTupleToolRedecay )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
MCTupleToolRedecay::MCTupleToolRedecay(const std::string& type,
                                       const std::string& name,
                                       const IInterface* parent)
    : TupleToolBase(type, name, parent) {
  declareInterface<IEventTupleTool>(this);
}

//=============================================================================
// Destructor
//=============================================================================
MCTupleToolRedecay::~MCTupleToolRedecay() {}

//=============================================================================
StatusCode MCTupleToolRedecay::fill(Tuples::Tuple& tuple) {
  const std::string prefix = fullName();

  if (msgLevel(MSG::VERBOSE)) verbose() << "getting MCHeader" << endmsg;
  if (!exist<LHCb::MCHeader>(LHCb::MCHeaderLocation::Default)) {
    Warning("No MCHeader at " + LHCb::MCHeaderLocation::Default,
            StatusCode::SUCCESS, 1);
    return StatusCode::SUCCESS;
  }
  const LHCb::MCHeader* mch =
      get<LHCb::MCHeader>(LHCb::MCHeaderLocation::Default);
  auto time = mch->evtTime();
  // for ReDecay, the unused evtTime contains the cantor pairing of
  // the original event run and event number. Decomposing it now
  // following https://en.wikipedia.org/wiki/Pairing_function.
  // Magic

  auto w = std::floor((sqrtl(8 * time + 1) - 1) / 2);
  auto t = (w*w + w) / 2;
  unsigned long long RD_org_runNumber = time - t;
  unsigned long long RD_org_eventNumber = w - RD_org_runNumber;
  unsigned int RD_i_event = mch->evtNumber() - RD_org_eventNumber;

  if (!(tuple->column(prefix + "RD_org_eventNumber", RD_org_eventNumber)))
    return StatusCode::FAILURE;
  if (!(tuple->column(prefix + "RD_org_runNumber", RD_org_runNumber)))
    return StatusCode::FAILURE;
  if (!(tuple->column(prefix + "RD_i_event", RD_i_event)))
    return StatusCode::FAILURE;

  return StatusCode::SUCCESS;
}
