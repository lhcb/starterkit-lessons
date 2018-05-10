// Include files
#include "gsl/gsl_sys.h"
// from Gaudi
#include "GaudiKernel/PhysicalConstants.h"
// local
#include "MCTupleToolHierarchy.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Event/Particle.h"
#include "Event/MCParticle.h"

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : EventInfoTupleTool
//
// 2008-02-28 : Stephane Poss
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
// actually acts as a using namespace TupleTool
DECLARE_COMPONENT( MCTupleToolHierarchy )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  MCTupleToolHierarchy::MCTupleToolHierarchy( const std::string& type,
                                              const std::string& name,
                                              const IInterface* parent )
    : TupleToolBase ( type, name , parent )
{
  declareInterface<IMCParticleTupleTool>(this);
}

//=============================================================================
StatusCode MCTupleToolHierarchy::fill( const LHCb::MCParticle*
                                       , const LHCb::MCParticle* mcp
                                       , const std::string& head
                                       , Tuples::Tuple& tuple )
{
  const std::string prefix=fullName(head);
  bool test=true;


  int mc_mother_id = 0;
  int mc_mother_key = 0;
  int mc_gd_mother_id = 0;
  int mc_gd_mother_key = 0;
  int mc_gd_gd_mother_id = 0;
  int mc_gd_gd_mother_key = 0;


  // pointer is ready, prepare the values:
  if( mcp ){
    const MCParticle* mcpmom(0);
    mcpmom = mcp->mother();
    if(mcpmom){
      mc_mother_id = mcpmom->particleID().pid();
      mc_mother_key = mcpmom->key();
      const MCParticle* mcpmom_mom(0);
      mcpmom_mom = mcpmom->mother();
      if(mcpmom_mom){
        mc_gd_mother_id = mcpmom_mom->particleID().pid();
        mc_gd_mother_key = mcpmom_mom->key();
        const MCParticle* mcpmom_mom_mom(0);
        mcpmom_mom_mom = mcpmom_mom->mother();
        if(mcpmom_mom_mom){
          mc_gd_gd_mother_id = mcpmom_mom_mom->particleID().pid();
          mc_gd_gd_mother_key = mcpmom_mom_mom->key();
        }
      }
    }
  }

  // fill the tuple:
  test &= tuple->column( prefix+"_MC_MOTHER_ID", mc_mother_id );
  test &= tuple->column( prefix+"_MC_MOTHER_KEY", mc_mother_key );
  test &= tuple->column( prefix+"_MC_GD_MOTHER_ID", mc_gd_mother_id );
  test &= tuple->column( prefix+"_MC_GD_MOTHER_KEY", mc_gd_mother_key );
  test &= tuple->column( prefix+"_MC_GD_GD_MOTHER_ID", mc_gd_gd_mother_id );
  test &= tuple->column( prefix+"_MC_GD_GD_MOTHER_KEY", mc_gd_gd_mother_key );

  return StatusCode(test);
}
