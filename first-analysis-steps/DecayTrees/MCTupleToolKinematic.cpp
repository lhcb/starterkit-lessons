// Include files
#include "gsl/gsl_sys.h"

// from Gaudi
#include "GaudiKernel/PhysicalConstants.h"
#include "GaudiKernel/Vector3DTypes.h"

// local
#include "MCTupleToolKinematic.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Event/MCParticle.h"

using namespace LHCb;
//-----------------------------------------------------------------------------
// Implementation file for class : MCTupleToolKinematic
//
// 2009-01-19 : Patrick Koppenburg
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( MCTupleToolKinematic )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
MCTupleToolKinematic::MCTupleToolKinematic( const std::string& type,
                                            const std::string& name,
                                            const IInterface* parent )
  : TupleToolBase ( type, name , parent )
{
  declareInterface<IMCParticleTupleTool>(this);

  // Store kinetic information from the associated candidate
  declareProperty( "StoreKineticInfo",  m_storeKinetic = true );

  // Store the end and origin true vertex information
  declareProperty( "StoreVertexInfo",  m_storeVertexes = true );

  // Store the propertime information for associated composite particle
  declareProperty( "StorePropertimeInfo", m_storePT = true );

  // Store propertime and endvertex also for stable particles
  // This is needed in case you study, for example, K-> pi pi pi
  // As the Kaon is considerad stable in LHCb
  // False by default as it will store properime also of pions, electrons etc
  declareProperty( "StoreStablePropertime", m_storeStablePropertime = false);


}
//=============================================================================
// Destructor
//=============================================================================
MCTupleToolKinematic::~MCTupleToolKinematic() {}

//=============================================================================
// initialize
//=============================================================================

StatusCode MCTupleToolKinematic::initialize()
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return StatusCode::FAILURE;
  if ( isVerbose() )
  {
    m_storePT = m_storeVertexes = m_storeKinetic = true;
  }
  return sc;
}
//=============================================================================
// Fill
//=============================================================================
StatusCode MCTupleToolKinematic::fill( const LHCb::MCParticle*
                                       , const LHCb::MCParticle* mcp
                                       , const std::string& head
                                       , Tuples::Tuple& tuple )
{
  const std::string prefix = fullName(head);
  bool test = true;

  if (msgLevel(MSG::DEBUG)) debug() << "MCTupleToolKinematic::fill " << head << endmsg ;

  double mcTau = -1;
  double mcPT = 0;

  Gaudi::XYZVector endVertex, originVertex;
  Gaudi::LorentzVector trueP;
  bool hasOsc = false ;

  if ( msgLevel(MSG::VERBOSE) ) verbose() << "MCTupleToolKinematic::fill mcp " << mcp << endmsg ;
  // pointer is ready, prepare the values:
  if ( mcp )
  {
    trueP = mcp->momentum();
    mcPT  = mcp->pt();
    if (msgLevel(MSG::VERBOSE)) verbose() << "      " << trueP << endmsg ;

    originVertex = mcp->originVertex()->position();
    if (msgLevel(MSG::VERBOSE)) verbose() << "     origin vertex position " << originVertex << endmsg ;

    if ( !isStable(mcp) || m_storeStablePropertime )
    {
      const SmartRefVector<LHCb::MCVertex> & endVertices = mcp->endVertices();
      if (msgLevel(MSG::VERBOSE)) verbose() << "     vertices " << endVertices.size() << endmsg ;

      const LHCb::MCVertex * mcV = NULL;
      if ( !endVertices.empty() )
      {
        for ( SmartRefVector<LHCb::MCVertex>::const_iterator v =
                endVertices.begin(); v != endVertices.end(); ++v ){
          if( (*v)->type() == LHCb::MCVertex::DecayVertex || (*v)->type() == LHCb::MCVertex::OscillatedAndDecay
              || (*v)->type() == LHCb::MCVertex::HadronicInteraction ){
            mcV = *v;
            break;
          }
        }
      }else{
        Warning( "No end vertices for " + prefix ).ignore();
      }

      if ( mcV )
      {
        endVertex = mcV->position();
      }
      else
      {
        Warning( "NULL end vertex for " + prefix ).ignore();
      }

      if (msgLevel(MSG::VERBOSE)) verbose() << "     end vertex " << endVertex << endmsg ;

      // lifetime
      if ( mcV && m_storePT )
      {
        const Gaudi::XYZVector dist = endVertex - originVertex;
        // copied from DecayChainNTuple //
        mcTau = trueP.M() * dist.Dot( trueP.Vect() ) / trueP.Vect().mag2();
        mcTau /= Gaudi::Units::c_light;  // nanoseconds
        hasOsc = mcp->hasOscillated() ;
        if (msgLevel(MSG::DEBUG))
        {
          debug() << head << " " << mcp->particleID().pid() << " time " << mcTau << " oscil" << hasOsc << endmsg ;
        }
      }
    }
  }

  if (msgLevel(MSG::VERBOSE)) verbose() << "MCTupleToolKinematic::fill filling " << head << endmsg ;

  // fill the tuple:
  //test &= tuple->column( head+"_TRUEID", mcPid );
  if ( m_storeKinetic )
  {
    test &= tuple->column( prefix + "_TRUEP_", trueP );
    test &= tuple->column( prefix+"_TRUEPT", mcPT );
  }

  if ( m_storeVertexes )
  {
    test &= tuple->column( prefix + "_TRUEORIGINVERTEX_", originVertex );
    test &= tuple->column( prefix + "_TRUEENDVERTEX_", endVertex );
    test &= tuple->column( prefix + "_TRUEISSTABLE",  isStable(mcp));
  }

  if ( m_storePT )
  {
    test &= tuple->column( prefix + "_TRUETAU", mcTau );
    if ( isVerbose() )
    {
      test &= tuple->column( prefix + "_OSCIL", hasOsc );
    }
  }

  if (msgLevel(MSG::VERBOSE)) verbose() << "MCTupleToolKinematic::fill bye " << prefix << endmsg ;

  return StatusCode(test);
}
