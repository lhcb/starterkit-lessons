// $Id: TupleToolGeneration.h,v 1.3 2010-01-26 15:39:26 rlambert Exp $
#ifndef JBOREL_TUPLETOOLGENERATION_H
#define JBOREL_TUPLETOOLGENERATION_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IEventTupleTool.h"            // Interface

class ITupleTool;

/** @class TupleToolGeneration TupleToolGeneration.h jborel/TupleToolGeneration.h
 *
 * \brief b quark content and number of pp interactions for DecayTreeTuple
 *
 * Tuple columns:
 * - Collisions
 * - HeaviestQuark in collision
 * - ProcessType in collision
 * - HeaviestQuarkInEvent
 *
 * Only data that can be extracted without accessing Sm/MCParticles to be added here
 *
 * \sa DecayTreeTuple
 *  @author P. Koppenburg
 *  @date   2008-07-01
 */
class TupleToolGeneration : public TupleToolBase,
                            virtual public IEventTupleTool
{

public:

  /// Standard constructor
  TupleToolGeneration( const std::string& type,
                       const std::string& name,
                       const IInterface* parent);

  ~TupleToolGeneration( ){}; ///< Destructor

  StatusCode fill( Tuples::Tuple& ) override;

};
#endif // JBOREL_TUPLETOOLGENERATION_H
