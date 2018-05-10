#include "TupleToolJets.h"
#include <math.h>
#include <boost/bind.hpp>
#include "boost/lexical_cast.hpp"

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolJets
// Autor: Albert Bursche
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolJets )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolJets::TupleToolJets( const std::string& type,
                                const std::string& name,
                                const IInterface* parent )
    : TupleToolJetsBase ( type, name , parent )

{
  declareInterface<IParticleTupleTool>(this);
}

//=============================================================================

StatusCode TupleToolJets::fill( const LHCb::Particle* /* top */,
                                const LHCb::Particle* p,
                                const std::string& head ,
                                Tuples::Tuple& tuple )
{
  m_tuple = &tuple;
  bool test = true;
  m_p = p;
  m_head = m_extraName+head;
  test &= WriteJetToTuple(p,m_head);
  return StatusCode(test);
}
