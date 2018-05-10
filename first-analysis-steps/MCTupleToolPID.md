// Include files
#include "gsl/gsl_sys.h"
#include "Event/MCParticle.h"

// local
#include "MCTupleToolPID.h"

// from Gaudi
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include <string>

//using namespace LHCb;
//-----------------------------------------------------------------------------
// Implementation file for class : MCTupleToolPID
//
// 2009-11-19 : Rob Lambert
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( MCTupleToolPID )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
MCTupleToolPID::MCTupleToolPID( const std::string& type,
                                const std::string& name,
                                const IInterface* parent )
  : TupleToolBase ( type, name , parent )
{
  declareInterface<IMCParticleTupleTool>(this);
}

//=============================================================================
// Destructor
//=============================================================================
MCTupleToolPID::~MCTupleToolPID() {}

//=============================================================================
// Fill
//=============================================================================
StatusCode MCTupleToolPID::fill( const LHCb::MCParticle*
                                 , const LHCb::MCParticle* mcp
                                 , const std::string& head
                                 , Tuples::Tuple& tuple )
{
  const std::string prefix=fullName(head);
  bool test = true;

  if (msgLevel(MSG::DEBUG)) debug() << "MCTupleToolPID::fill " << head << endmsg ;

  int mcPid = 0;

  if (msgLevel(MSG::VERBOSE)) verbose() << "MCTupleToolPID::fill mcp " << mcp << endmsg ;
  // pointer is ready, prepare the values:
  if( mcp )
  {
    mcPid = mcp->particleID().pid();
  }
  if (msgLevel(MSG::VERBOSE)) verbose() << "MCTupleToolPID::fill filling " << head << endmsg ;

  test &= tuple->column( prefix + "_ID", mcPid );

  if (msgLevel(MSG::VERBOSE)) verbose() << "MCTupleToolPID::fill bye " << head << endmsg ;

  return StatusCode(test);
}
