// $Id: TupleToolAngles.cpp,v 1.5 2010-01-26 15:39:26 rlambert Exp $
// Include files
#include "gsl/gsl_sys.h"

// from Gaudi
#include "GaudiKernel/PhysicalConstants.h"
#include "GaudiKernel/Vector3DTypes.h"

// local
#include "TupleToolAngles.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Event/Particle.h"

using namespace LHCb;
//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolAngles
//
// 2009-01-19 : Patrick Koppenburg
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolAngles )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolAngles::TupleToolAngles( const std::string& type,
                                    const std::string& name,
                                    const IInterface* parent )
    : TupleToolBase ( type, name , parent ),
      m_wrtMother(0)
{
  declareInterface<IParticleTupleTool>(this);
  declareProperty("WRTMother",m_wrtMother=true,
                  "Turn false to fill angles with respect to top of tree");
}

//=============================================================================
// Destructor
//=============================================================================
TupleToolAngles::~TupleToolAngles() {}

//=============================================================================
// Fill
//=============================================================================
StatusCode TupleToolAngles::fill( const LHCb::Particle* top
                                  , const LHCb::Particle* part
                                  , const std::string& head
                                  , Tuples::Tuple& tuple )
{
  const std::string prefix=fullName(head);

  bool test = true;
  const LHCb::Particle* mother=top;
  if(m_wrtMother) mother=findMother(top,part);
  if ( 0==part || 0==mother || part==mother ) return StatusCode::SUCCESS ;
  double cosT = cosTheta(mother->momentum(), part->momentum() );
  // fill the tuple:
  test &= tuple->column( prefix+"_CosTheta", cosT );
  if(isVerbose()) test &= tuple->column( prefix+"_Theta", acos(cosT) );
  if ( msgLevel(MSG::DEBUG)) debug() << mother->particleID().pid() << " " << mother->momentum() << " "
                                     << part->particleID().pid() << " " << part->momentum() << endmsg ;
  return StatusCode(test) ;

}
//=============================================================================
// Fill
//=============================================================================

const LHCb::Particle* TupleToolAngles::findMother( const LHCb::Particle* top
                                                   , const Particle* P ) const
{
  if( top == P || top->isBasicParticle() ) return 0;

  const SmartRefVector< LHCb::Particle >& dau = top->daughters ();
  if( dau.empty() ) return 0;

  SmartRefVector< LHCb::Particle >::const_iterator it;
  for( it = dau.begin(); dau.end()!=it; ++it ){
    if( P == *it ){ // I found the daughter
      return top;
    }
  }

  // vertex not yet found, get deeper in the decay:
  for( it = dau.begin(); dau.end()!=it; ++it ){
    if( P != *it && !(*it)->isBasicParticle() ){
      const LHCb::Particle* tm = findMother( *it, P );
      if( tm ) return tm;
    }
  }
  return 0;
}
