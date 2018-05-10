// $Id: TupleToolTriggerRecoStats.h,v 1.2 2010-01-26 15:39:27 rlambert Exp $
#ifndef TUPLETOOLTRIGGERRECOSTATS_H
#define TUPLETOOLTRIGGERRECOSTATS_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IEventTupleTool.h"            // Interface


/** @class TupleToolTriggerRecoStats TupleToolTriggerRecoStats.h
 *
 *  Counts the number of particles per type
 *
 *  @author Patrick Koppenburg
 *  @date   2009-07-30
 */
class TupleToolTriggerRecoStats : public TupleToolBase, virtual public IEventTupleTool {
public:
  /// Standard constructor
  TupleToolTriggerRecoStats( const std::string& type,
                             const std::string& name,
                             const IInterface* parent);

  virtual ~TupleToolTriggerRecoStats( ); ///< Destructor
  StatusCode fill( Tuples::Tuple& ) override;///< Fill tuple

protected:

private:

  std::vector<std::string> m_locations ; ///< Hlt locations to look at

};
#endif // TUPLETOOLTRIGGERRECOSTATS_H
