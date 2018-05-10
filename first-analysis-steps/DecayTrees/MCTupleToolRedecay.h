// $Id: MCTupleToolRedecay.h,v 1.0 2017-01-26 15:39:26 dmuller Exp $
#ifndef MCTUPLETOOLREDECAY_H
#define MCTUPLETOOLREDECAY_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IEventTupleTool.h"  // Interface

/** @class MCTupleToolRedecay MCTupleToolRedecay.h
 *
 * \brief Decodes evtTime to add redecay information to the ntuple.
 * RD_org_eventNumber and RD_org_runNumber contain the event and run number
 * of the original event to allow to group events stemming from the same
 * original event.
 * RD_i_event gives the index of the ReDecay event
 *
 * Tuple columns:
 * - RD_org_eventNumber, RD_org_runNumber, RD_i_event
 *
 *  @author Dominik Muller
 *  @date   2017-1-26
 */
class MCTupleToolRedecay : public TupleToolBase,
                           virtual public IEventTupleTool {
  public:
  /// Standard constructor
  MCTupleToolRedecay(const std::string& type, const std::string& name,
                     const IInterface* parent);

  virtual ~MCTupleToolRedecay();  ///< Destructor
  StatusCode fill(Tuples::Tuple&) override;
};
#endif  // MCTUPLETOOLREDECAY_H
