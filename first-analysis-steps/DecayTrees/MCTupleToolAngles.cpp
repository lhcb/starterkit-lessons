// Include files
#include "gsl/gsl_sys.h"

// from Gaudi
#include "GaudiKernel/PhysicalConstants.h"
#include "GaudiKernel/Vector3DTypes.h"

// local
#include "MCTupleToolAngles.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Event/MCParticle.h"

using namespace LHCb;
//-----------------------------------------------------------------------------
// Implementation file for class : MCTupleToolAngles
//
// 2009-01-19 : Patrick Koppenburg
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( MCTupleToolAngles )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  MCTupleToolAngles::MCTupleToolAngles( const std::string& type,
                                        const std::string& name,
                                        const IInterface* parent )
    : TupleToolBase ( type, name , parent )
{
  declareInterface<IMCParticleTupleTool>(this);
}

//=============================================================================
// Destructor
//=============================================================================
MCTupleToolAngles::~MCTupleToolAngles() {}

//=============================================================================
// Fill
//=============================================================================
StatusCode MCTupleToolAngles::fill( const LHCb::MCParticle* mother
                                    , const LHCb::MCParticle* mcp
                                    , const std::string& head
                                    , Tuples::Tuple& tuple )
{
  const std::string prefix=fullName(head);

  bool test = true;

  double cosT =-999.;

  if ( 0!=mcp && 0!=mother && mcp!=mother ) cosT = cosTheta(mother->momentum(), mcp->momentum() );
  // fill the tuple:
  test &= tuple->column( prefix+"_TRUECosTheta", cosT );
  if(isVerbose()) test &= tuple->column( prefix+"_TRUETheta", acos(cosT) );
  if ( msgLevel(MSG::DEBUG) && 0!=mcp && 0!=mother)
    debug() << mother->particleID().pid() << " " << mother->momentum() << " "
            << mcp->particleID().pid() << " " << mcp->momentum() << endmsg ;

  return StatusCode(test);
}
