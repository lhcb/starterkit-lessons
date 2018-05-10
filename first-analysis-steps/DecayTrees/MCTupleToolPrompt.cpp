// $Id: $
// Include files

// local
#include "MCTupleToolPrompt.h"

//using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : MCTupleToolPrompt
//
// 2011-06-13 : Philip John Hunt
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( MCTupleToolPrompt )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  MCTupleToolPrompt::MCTupleToolPrompt( const std::string& type,
                                        const std::string& name,
                                        const IInterface* parent )
    : TupleToolBase ( type, name , parent )
    , m_ppSvc(0)
{
  declareInterface<IMCParticleTupleTool>(this);

  // maximum lifetime of short-lived particles (ns)
  declareProperty( "MaxLifetime", m_maxLifetime = 1e-7
                   , "Maximum lifetime of short-lived particles (ns)" );
  declareProperty( "StoreLongLivedParticleID", m_storeLongLivedPid=true);
}

StatusCode MCTupleToolPrompt::initialize()
{
  StatusCode sc = TupleToolBase::initialize();
  if (sc.isFailure()) return sc;

  if (isVerbose()) m_storeLongLivedPid = true;

  sc = service( "LHCb::ParticlePropertySvc", m_ppSvc );

  return sc;
}

StatusCode MCTupleToolPrompt::fill( const LHCb::MCParticle*
                                    , const LHCb::MCParticle* mcp
                                    , const std::string& head
                                    , Tuples::Tuple& tuple )
{
  const std::string prefix = fullName(head);

  bool test=true;

  int longLivedPid=0;
  int longLivedKey=0;

  int isPrompt=-1;

  int mcParentPid=0;
  int mcParentKey=0;

  double lcl_lifetime(0);

  const LHCb::MCParticle *mcp_parent = mcp;

  const LHCb::ParticleProperty * lclPprop = NULL;

  if (msgLevel(MSG::DEBUG)) debug() << "MCTupleToolPrompt::fill " << head << endmsg;

  if (mcp)
  {
    isPrompt=1;
    while( isPrompt && mcp_parent->mother() )
    {
      mcp_parent = mcp_parent->mother();

      mcParentPid=mcp_parent->particleID().pid();
      mcParentKey=mcp_parent->key();

      lclPprop = m_ppSvc->find( mcp_parent->particleID() );

      // Some particle IDs are not known to the ppSvc
      // Log a warning if that's the case and assign a large negative lifetime
      if (lclPprop)
      {
        lcl_lifetime = lclPprop->lifetime();
        if(msgLevel(MSG::DEBUG))
        {
          debug() << "Particle: " << lclPprop->evtGenName()
                  << "  lifetime: " << lcl_lifetime << endmsg;
        }
      } else {
        warning() << "Particle ID '" << mcp_parent->particleID()
                  << "' not found in particle property service" << endmsg;
        lcl_lifetime = -999.0;
      }

      if ( lcl_lifetime > m_maxLifetime*Gaudi::Units::ns )
      {
        isPrompt=0;
        longLivedPid=mcParentPid;
        longLivedKey=mcParentKey;
      }
    }
  }

  if (msgLevel(MSG::DEBUG)) debug() << "MCTupleToolPrompt::fill - filling "
                                    << head << endmsg;

  test &= tuple->column( prefix+"_MC_ISPROMPT", isPrompt );
  if (m_storeLongLivedPid)
  {
    test &= tuple->column( prefix+"_MC_LONGLIVED_ID", longLivedPid );
    test &= tuple->column( prefix+"_MC_LONGLIVED_KEY", longLivedKey );
  }
  return StatusCode(test);
}

//=============================================================================
// Destructor
//=============================================================================
MCTupleToolPrompt::~MCTupleToolPrompt() {}

//=============================================================================
