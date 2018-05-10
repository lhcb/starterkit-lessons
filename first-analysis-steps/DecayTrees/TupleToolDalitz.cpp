#include "Event/Particle.h"

#include "TupleToolDalitz.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolDalitz
//
// 2009-02-17 : Patrick Koppenburg
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolDalitz )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolDalitz::TupleToolDalitz( const std::string& type,
                                    const std::string& name,
                                    const IInterface* parent )
    : TupleToolBase ( type, name , parent )
    , m_ppSvc(0)
{
  declareInterface<IParticleTupleTool>(this);

}
//=============================================================================
// Destructor
//=============================================================================
TupleToolDalitz::~TupleToolDalitz() {}

//=============================================================================
//=============================================================================
// Fill
//=============================================================================
StatusCode TupleToolDalitz::fill( const LHCb::Particle* mother
                                  , const LHCb::Particle* part
                                  , const std::string& head
                                  , Tuples::Tuple& tuple )
{
  const std::string prefix=fullName(head);

  if (msgLevel(MSG::VERBOSE)) verbose() << "Dalitz fill " << prefix << " " << mother
                                        << " " << part << endmsg ;
  if (0==part) return StatusCode::FAILURE ;
  const LHCb::Particle::ConstVector& dauts = part->daughtersVector() ;
  if ( 2 >= dauts.size() ){
    debug() << "Will not fill Dalitz of two body decay " << prefix << endmsg;
    return StatusCode::SUCCESS;
  }
  if (part->particleID().abspid()==98){
    debug() << "Will not fill Dalitz for particle type CELLjet " << endmsg;
    return StatusCode::SUCCESS;
  }

  return fill(dauts,head,tuple,(part->particleID().pid()<0)) ;
}
