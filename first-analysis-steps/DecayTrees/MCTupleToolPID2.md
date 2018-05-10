// $Id: MCTupleToolPID.h,v 1.2 2010-01-26 15:39:26 rlambert Exp $
#ifndef MCTUPLETOOLPID_H
#define MCTUPLETOOLPID_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IMCParticleTupleTool.h"            // Interface
#include "Event/MCParticle.h"            // Interface

/** @class MCTupleToolPID MCTupleToolPID.h
 *
 *  Fill MC Particle
 *
 * - head_ID : pid
 *
 *
 * \sa MCTupleToolPID, DecayTreeTuple, MCDecayTreeTuple
 *
 *  @author Rob Lambert
 *  @date   2009-11-19
 */

class MCTupleToolPID : public TupleToolBase,
                       virtual public IMCParticleTupleTool
{

public:

  /// Standard constructor
  MCTupleToolPID( const std::string& type,
                  const std::string& name,
                  const IInterface* parent);

  virtual ~MCTupleToolPID( ); ///< Destructor

  StatusCode fill( const LHCb::MCParticle*
                   , const LHCb::MCParticle*
                   , const std::string&
                   , Tuples::Tuple& ) override;

};

#endif // MCTUPLETOOLPID_H
