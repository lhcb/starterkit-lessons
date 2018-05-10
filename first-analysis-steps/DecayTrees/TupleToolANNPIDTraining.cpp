// $Id: $

#include "TupleToolANNPIDTraining.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolANNPIDTraining
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolANNPIDTraining )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolANNPIDTraining::TupleToolANNPIDTraining( const std::string& type,
                                                  const std::string& name,
                                                  const IInterface* parent )
: TupleToolBase ( type, name, parent ),
  m_tuple       ( NULL )
{
  declareInterface<IParticleTupleTool>(this);
}

//=============================================================================

StatusCode TupleToolANNPIDTraining::initialize()
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  m_tuple =
    tool<ANNGlobalPID::IChargedProtoANNPIDTupleTool>( "ANNGlobalPID::ChargedProtoANNPIDTupleTool",
                                                      "Tuple", this );

  return sc;
}

//=============================================================================

StatusCode TupleToolANNPIDTraining::fill( const LHCb::Particle*
                                          , const LHCb::Particle* P
                                          , const std::string& /* head */
                                          , Tuples::Tuple& tuple )
{
  //const std::string prefix = fullName(head);

  // Fill the ANNPID variables
  // Note, prefix is not used here, so can only use this tool on a single particle at a time...
  return m_tuple->fill( tuple, P );
}
