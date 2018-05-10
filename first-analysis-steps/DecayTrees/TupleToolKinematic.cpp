// $Id: TupleToolKinematic.cpp,v 1.6 2010-04-12 12:34:58 rlambert Exp $
// Include files

// local
#include "TupleToolKinematic.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Event/Particle.h"

#include "Kernel/IParticleTransporter.h"

//-----------------------------------------------------------------------------
// Implementation file for class : EventInfoTupleTool
//
// 2007-11-07 : Jeremie Borel
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
// actually acts as a using namespace TupleTool
DECLARE_COMPONENT( TupleToolKinematic )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolKinematic::TupleToolKinematic( const std::string& type,
                                          const std::string& name,
                                          const IInterface* parent )
    : TupleToolBase ( type, name , parent )
    , m_transporter()
    , m_transporterName ("ParticleTransporter:PUBLIC")
{
  declareInterface<IParticleTupleTool>(this);

  declareProperty( "Transporter", m_transporterName );
}

//=============================================================================

StatusCode TupleToolKinematic::initialize()
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  m_transporter = tool<IParticleTransporter>(m_transporterName, this);

  return sc;
}

//=============================================================================

StatusCode TupleToolKinematic::fill( const LHCb::Particle* mother
                                     , const LHCb::Particle* P
                                     , const std::string& head
                                     , Tuples::Tuple& tuple )
{
  const std::string prefix=fullName(head);

  bool test = true;
  if( P )
  {
    test &= tuple->column( prefix+"_P", P->p() );
    test &= tuple->column( prefix+"_PT", P->pt() );

    // momentum components
    test &= tuple->column( prefix+"_P", P->momentum() );

    // reference point:
    if(isVerbose()) test &= tuple->column( prefix+"_REFP", P->referencePoint() );
    // mass before fit (what CombinationCut cuts on)
    //if(isVerbose() && !(P->isBasicParticle()) ) test &= tuple->column( prefix+"_PreFitMass", preFitMass(P) );

    if( !P->isBasicParticle() ||  P->particleID().pid()  ==  111 ) test &= tuple->column( prefix+"_MM", P->measuredMass() );
    if( !P->isBasicParticle() ||  P->particleID().pid()  ==  111 ) test &= tuple->column( prefix+"_MMERR", P->measuredMassErr() );
    test &= tuple->column( prefix+"_M", P->momentum().M() );

    if(isVerbose()&& mother &&P->isBasicParticle() && P->charge() != 0 ) {
      const LHCb::Vertex* originvtx = mother->endVertex();
      if(originvtx){
        double zvtx = originvtx->position().Z();

        LHCb::Particle transParticle;
        if(!m_transporter) Error("null pointer m_transporter !!!!");
        test &= m_transporter->transport(P, zvtx, transParticle);

        test &= tuple->column( prefix+"_AtVtx_P", transParticle.momentum() );
      }
    }
  }
  else
  {
    return StatusCode::FAILURE;
  }
  return StatusCode(test);
}

//=============================================================================

double TupleToolKinematic::preFitMass(const LHCb::Particle* p) const 
{
  Gaudi::LorentzVector Mom ;
  for ( SmartRefVector< LHCb::Particle >::const_iterator d = p->daughters().begin();
        d != p->daughters().end() ; ++d){
    Mom += (*d)->momentum();
  }
  return Mom.M() ;
}
