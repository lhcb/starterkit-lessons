#ifndef TUPLETOOLRECOSTATS_H
#define TUPLETOOLRECOSTATS_H 1

// Include files
// from Gaudi
#include "Kernel/IEventTupleTool.h"            // Interface
#include "DecayTreeTupleBase/TupleToolBase.h"

#include "Event/Track.h"
#include "Event/RecSummary.h"

/** @class TupleToolParticleStats TupleToolParticleStats.h
 *
 *  Fills Reco stats, from RecSummary
 *
 *  @author Patrick Koppenburg, Fatima Soomro, Jibo He
 *  @date   2009-02-11
 */
class TupleToolParticleStats : public TupleToolBase,
                           virtual public IEventTupleTool
{

public:

  /// Standard constructor
  TupleToolParticleStats( const std::string& type,
                      const std::string& name,
                      const IInterface* parent);

  virtual ~TupleToolParticleStats( ); ///< Destructor
  StatusCode fill( Tuples::Tuple& ) override;///< Fill tuple
//  int unusedVelo();
private:
  std::vector<std::string> m_locations ; ///< Hlt locations to look at

};

#endif // TUPLETOOLRECOSTATS_H

