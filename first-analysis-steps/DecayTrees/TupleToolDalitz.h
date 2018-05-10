// $Id: TupleToolDalitz.h,v 1.3 2010-01-26 15:39:26 rlambert Exp $
#ifndef TUPLETOOLDALITZ_H
#define TUPLETOOLDALITZ_H 1

// Include files
// from Gaudi
#include "Kernel/IParticleTupleTool.h"
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/Escape.h"
#include "Event/Particle.h"
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleProperty.h"
// Boost
#include <boost/lexical_cast.hpp>






/** @class TupleToolDalitz TupleToolDalitz.h
 *
 *  Fills square masses of all combinations of direct daughters
 *
 *  @author Patrick Koppenburg
 *  @date   2009-02-17
 */
class TupleToolDalitz : public TupleToolBase,
                        virtual public IParticleTupleTool
{

public:

  /// Standard constructor
  TupleToolDalitz( const std::string& type,
                   const std::string& name,
                   const IInterface* parent);

  virtual ~TupleToolDalitz( ); ///< Destructor

  /// The filling method
  StatusCode fill( const LHCb::Particle*
                   , const LHCb::Particle*
                   , const std::string&
                   , Tuples::Tuple& ) override;

private:

  // Horrid way of sharing code. Should use inheritance instead ...
#include "TupleToolDalitz.icpp"

};

#endif // TUPLETOOLDALITZ_H
