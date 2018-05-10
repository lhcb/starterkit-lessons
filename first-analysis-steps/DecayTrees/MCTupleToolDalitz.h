// $Id: MCTupleToolDalitz.h,v 1.3 2010-01-26 15:39:25 rlambert Exp $
#ifndef MCTUPLETOOLDALITZ_H
#define MCTUPLETOOLDALITZ_H 1

// Include files
// from Gaudi
#include "Kernel/IMCParticleTupleTool.h"
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/Escape.h"
#include "Event/MCParticle.h"
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleProperty.h"

// Boost
#include <boost/lexical_cast.hpp>

/** @class MCTupleToolDalitz MCTupleToolDalitz.h
 *
 *  Fills square masses of all combinations of direct daughters
 *
 *  @author Patrick Koppenburg
 *  @date   2009-02-17
 */
class MCTupleToolDalitz : public TupleToolBase,
                          virtual public IMCParticleTupleTool
{

public:

  /// Standard constructor
  MCTupleToolDalitz( const std::string& type,
                     const std::string& name,
                     const IInterface* parent);

  virtual ~MCTupleToolDalitz( ); ///< Destructor

  /// The filling method
  StatusCode fill( const LHCb::MCParticle*
                   , const LHCb::MCParticle*
                   , const std::string&
                   , Tuples::Tuple& ) override;

private:

  // Horrid way of sharing code. Should use inheritance instead ...
#include "TupleToolDalitz.icpp"

};
#endif // MCTUPLETOOLDALITZ_H
