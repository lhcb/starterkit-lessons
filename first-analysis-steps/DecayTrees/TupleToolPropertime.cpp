// $Id: TupleToolPropertime.cpp,v 1.7 2010-04-23 09:34:39 pkoppenb Exp $
// Include files

// from Gaudi
#include "GaudiKernel/SmartIF.h"

#include <Kernel/GetIDVAlgorithm.h>
#include <Kernel/IDVAlgorithm.h>
#include <Kernel/ILifetimeFitter.h>

// local
#include "TupleToolPropertime.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Event/Particle.h"
#include "Event/Vertex.h"

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : EventInfoTupleTool
//
// 2007-11-07 : Jeremie Borel
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
// actually acts as a using namespace TupleTool
DECLARE_COMPONENT( TupleToolPropertime )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolPropertime::TupleToolPropertime( const std::string& type,
                                            const std::string& name,
                                            const IInterface* parent )
    : TupleToolBase ( type, name , parent )
    , m_dva(0)
    , m_fit(0)
{
  declareInterface<IParticleTupleTool>(this);
  // true determines proper time of all particles w.r.t. their best PV
  // false (default) determines proper time w.r.t. mother particle
  declareProperty("FitToPV", m_fitToPV = false );
}//=============================================================================

StatusCode TupleToolPropertime::initialize()
{
  if( ! TupleToolBase::initialize() ) return StatusCode::FAILURE;

  m_dva = Gaudi::Utils::getIDVAlgorithm ( contextSvc(), this ) ;
  if (0==m_dva) return Error("Couldn't get parent DVAlgorithm",
                             StatusCode::FAILURE);

  m_fit = m_dva->lifetimeFitter();
  if( !m_fit )
  {
    return Error("Unable to retrieve the ILifetimeFitter tool");
  }

  return StatusCode::SUCCESS;
}

//=============================================================================

StatusCode TupleToolPropertime::fill( const Particle* mother
                                      , const Particle* P
                                      , const std::string& head
                                      , Tuples::Tuple& tuple )
{

  const std::string prefix=fullName(head);

  Assert( m_fit && P,
          "Should not happen, you are inside TupleToolPropertime.cpp" );

  // no proper-time for basic parts.
  if( P->isBasicParticle() ) return StatusCode::SUCCESS;

  const VertexBase* originVtx = NULL;
  if( m_fitToPV ){
    originVtx = m_dva->bestVertex( P );
  }
  else {
    if( mother != P ){
      originVtx = originVertex( mother, P ); // the origin vertex is
                                             // somewhere in the decay
    } else { // the origin vertex is the primary.
      originVtx = m_dva->bestVertex( mother );
    }
  }

  if( originVtx ){} // I'm happy
  else {
    Error("Can't get the origin vertex");
    return StatusCode::FAILURE;
  }

  double pt   = -100;
  double ept  = -100;
  double chi2 = -100;
  StatusCode sc = m_fit->fit ( *originVtx, *P , pt, ept, chi2 );

  if( !sc ){
    Warning("The propertime fit failed").ignore();
    pt   = -100;
    ept  = -100;
    chi2 = -100;
  }

  bool test = true;
  test &= tuple->column( prefix+"_TAU" , pt ); // nanoseconds
  test &= tuple->column( prefix+"_TAUERR" , ept );
  test &= tuple->column( prefix+"_TAUCHI2" , chi2 );

  return StatusCode(test);

}

const Vertex* TupleToolPropertime::originVertex( const Particle* top
                                                 , const Particle* P ) const
{
  if( top == P || P->isBasicParticle() ) return NULL;

  const SmartRefVector< LHCb::Particle >& dau = top->daughters ();
  if( dau.empty() ) return NULL;

  for( SmartRefVector<LHCb::Particle>::const_iterator it = dau.begin(); 
       dau.end() != it; ++it )
  {
    if( P == *it ){
      return top->endVertex();
    }
  }

  // vertex not yet found, get deeper in the decay:
  for( SmartRefVector<LHCb::Particle>::const_iterator it = dau.begin(); 
       dau.end() != it; ++it )
  {
    if( P != *it && !(*it)->isBasicParticle() )
    {
      const Vertex* vv = originVertex( *it, P );
      if( vv ) return vv;
    }
  }
  return NULL;
}
