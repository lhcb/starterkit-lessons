// $Id: TupleToolPi0Info.cpp,v 1.4 2010-01-26 15:39:26 rlambert Exp $
// Include files

// local
#include "TupleToolPi0Info.h"
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
#include "Event/Particle.h"
#include "CaloUtils/CaloMomentum.h"
#include "GaudiKernel/IRegistry.h" //

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolPi0Info
//
// 2008-10-31 :(Happy Halloween)
// Yasmine Amhis
//-----------------------------------------------------------------------------

using namespace Gaudi;
using namespace LHCb;

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolPi0Info )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolPi0Info::TupleToolPi0Info( const std::string& type,
                                      const std::string& name,
                                      const IInterface* parent )
    : TupleToolBase ( type, name , parent )
{
  declareInterface<IParticleTupleTool>(this);
  declareProperty("RequireMCTruth",m_RequireMCTruth = false);
}

//=============================================================================
StatusCode TupleToolPi0Info::fill(const Particle* , const Particle* P
                                  ,const std::string& head
                                  ,Tuples::Tuple& tuple )
{
  const std::string prefix=fullName(head);

  bool filltuple = true;
  if ( P )
  {
    if (P->particleID().pid() == m_Pi0ID &&  isPureNeutralCalo(P)) 
    {
      if ( msgLevel(MSG::DEBUG) ) 
      {
        debug() << " Make the special case for the pi0  " <<  P->particleID().pid() << endmsg;
        debug() << " The pi0s has : " <<  P->daughters().size()   << " daughters  "<< endmsg;
      }
      const int Type = ( !P->daughters().empty() ? 2 : 1 );
      // 2 = resolved, 1 = merged.

      filltuple &= tuple->column( prefix+"_Type", Type );
      filltuple &= tuple->column( prefix+"_CL"  , P->confLevel() );

      // daughter info in case of merged pi0
      if ( Type == 1 )
      {        
        const auto hypo = P->proto()->calo().front();
        if ( LHCb::CaloHypo::Pi0Merged != hypo->hypothesis() )
        {
          // == extract SplitPhotons hypos
          const auto& hypos = hypo->hypos();
          if ( hypos.size() > 1 )
          {
            const auto g1 = *(hypos.begin() );
            const auto g2 = *(hypos.begin()+1 );
            filltuple &= tuple->column( prefix+"_gamma0_PT" , LHCb::CaloMomentum(g1).pt() );
            filltuple &= tuple->column( prefix+"_gamma1_PT" , LHCb::CaloMomentum(g2).pt() );
          } 
        }
      }
      
      if ( m_RequireMCTruth ) 
      {
        Warning("MC association with TupleToolPi0Info is deprecated - please use the standard TupleToolMCTruth"
                , StatusCode::SUCCESS).ignore();
        if ( msgLevel(MSG::DEBUG) ) 
          debug() << "Get the association done for a pi0 " << endmsg;
        const MCParticle* MCPi0 = nullptr;
        const MCParticle* MCPi0Mother = nullptr;
        const MCParticle* MCPi0GrandMother = nullptr;
        const MCParticle* MCPi0GrandGrandMother = nullptr;

        int  MCPi0_id =0;
        int  MCPi0Mother_id =0;
        int  MCPi0GrandMother_id =0;
        int  MCPi0GrandGrandMother_id =0;
        int  MCPi0_key =0;
        int  MCPi0Mother_key =0;
        int  MCPi0GrandMother_key =0;
        int  MCPi0GrandGrandMother_key =0;
        double weight = 1;

        MCPi0 = getMCPi0(P,weight);
        if ( MCPi0 ) 
        {
          MCPi0_id  = MCPi0->particleID().pid();
          MCPi0_key = MCPi0->key();
          MCPi0Mother = MCPi0->mother() ;
        }//particleMC info
        if ( MCPi0Mother )
        {
          MCPi0Mother_id  = MCPi0Mother->particleID().pid();
          MCPi0Mother_key = MCPi0Mother->key();
          MCPi0GrandMother = MCPi0Mother->mother() ;
        }//particle mother MC info

        if ( MCPi0GrandMother ) 
        {
          MCPi0GrandMother_id   = MCPi0GrandMother->particleID().pid();
          MCPi0GrandMother_key  = MCPi0GrandMother->key();
          MCPi0GrandGrandMother = MCPi0GrandMother->mother()  ;
        }
        if ( MCPi0GrandGrandMother ) 
        {
          MCPi0GrandGrandMother_key = MCPi0GrandGrandMother->key();
          MCPi0GrandGrandMother_id  = MCPi0GrandGrandMother->particleID().pid();
        }

        filltuple &= tuple->column( prefix+"_MCPi0_id", MCPi0_id  );
        filltuple &= tuple->column( prefix+"_MCPi0_key", MCPi0_key  );

        filltuple &= tuple->column( prefix+"_MCPi0Mother_id", MCPi0Mother_id );
        filltuple &= tuple->column( prefix+"_MCPi0Mother_key", MCPi0Mother_key );

        filltuple &= tuple->column( prefix+"_MCPi0GrandMother_id", MCPi0GrandMother_id  );
        filltuple &= tuple->column( prefix+"_MCPi0GrandMother_key", MCPi0GrandMother_key );

        filltuple &= tuple->column( prefix+"_MCPi0GrandGrandMother_id", MCPi0GrandGrandMother_id  );
        filltuple &= tuple->column( prefix+"_MCPi0GrandGrandMother_key", MCPi0GrandGrandMother_key  );
        if ( msgLevel(MSG::DEBUG) ) 
          debug() << "This is the MC part associated to your  pi0 -->   "
                  <<  MCPi0_id  << "   and its  mother   -->"
                  <<  MCPi0Mother_id   << endmsg;
      }//require MC truth
    }//this is a pi0
  }//get the particle

  return StatusCode(filltuple);
}

//================================================================================
//Here you get the protoparticle corresponding to your pi0 candidate
//================================================================================
TupleToolPi0Info::mcRange TupleToolPi0Info::getRange(const LHCb::Particle* part)
{
  const auto* proto = ( part ? part->proto() : nullptr );
  return ( proto ? getRange(proto) : mcRange() );
}

//================================================================================
//Here you get the relation table between the protoparticles and the MC particles
//================================================================================
TupleToolPi0Info::mcRange TupleToolPi0Info::getRange(const LHCb::ProtoParticle* proto)
{
  if ( !proto ) return mcRange();
  std::string locationhard;
  if( 0 == proto->charge() )
  {
    if(  context() == "HLT")
    {
      debug ()  <<" You are running in the Hlt context   "  <<   context() << endmsg;
      locationhard = "Relations/Hlt/ProtoP/Neutrals";
    }
    else
    {
      debug ()  <<"You are running in the Offline context   "  <<  context() << endmsg;
      locationhard = "Relations/Rec/ProtoP/Neutrals";
    }
  } //check neutral
  const mcTable* table = get<mcTable> ( locationhard ) ;
  // create protoP<->MC output relation table
  if ( msgLevel(MSG::DEBUG) ) 
    debug()  <<"ProtoP<->MC output relation table    " <<  table  << endmsg;
  return ( table ? table->relations( proto ) : mcRange() );
}
//=======================================================================================

const LHCb::MCParticle* TupleToolPi0Info::getMCPi0(const LHCb::Particle* part, double& weight )
{
  // Return the highest-weighted matching MCPart WITH THE SAME particleID than part->ID
  // if not the highest-weighted among ALL matching MCPart then  weight = -weight
  // If not found return the highest-weighted matching MCPART (irrespective of part->ID)

  weight = 0;
  if ( !part ) return nullptr;

  double weightMax =- 99999;
  double weightPart = -99999;
  LHCb::MCParticle* mcBest = nullptr;
  LHCb::MCParticle* mcPart = nullptr;

  if ( !part->daughters().empty() )
  {
    //==============================
    // special case of ResolvedPi0s
    //==============================
    const auto & daughters = part->daughters();
    auto ig1 = daughters.begin();
    auto ig2 = ig1+1;
    auto range1 = getRange( *ig1 );
    auto range2 = getRange( *ig2 );
    if ( range1.empty() || range2.empty() ) return nullptr;
    for ( const auto r1 : range1 )
    {
      for ( const auto r2 : range2 )
      {
        auto w = r1.weight();
        if ( r2.weight() > r1.weight() ) w = r2.weight();
        if ( r1.to() == r2.to() ) w = r1.weight() + r2.weight();
        if ( w > weightMax )
        {
          weightMax = w;
          mcBest = r1.to();
          if ( r2.weight() > r1.weight() ) mcBest = r2.to();
        }
        if ( m_Pi0ID == abs(r1.to()->particleID().pid()) && r1.to() == r2.to() )
        {
          if ( w > weightPart )
          {
            mcPart = r1.to();
            weightPart = w;
          }
        }
      }
    }
  }
  else
  {
    //==============
    // Pi0 merged
    //==============
    auto range = getRange(part);
    if ( range.empty() ) return nullptr;
    for ( const auto r : range )
    {
      auto w = r.weight();
      if ( w > weightMax ) 
      {
        weightMax = w;
        mcBest = r.to();
      }
      if ( m_Pi0ID == abs(r.to()->particleID().pid()) )
      {
        if ( w > weightPart ) 
        {
          mcPart = r.to();
          weightPart = w;
        }
      }
    }
  }
  //
  if ( mcPart )
  {
    weight = weightPart;
    if ( weightPart < weightMax ) weight = -weightPart;
    return mcPart;
  }
  else if ( mcBest ) 
  {
    weight = weightMax;
    return mcBest;
  }
  else
  {
    weight = 0;
    return nullptr;
  }
}
//==================================================================================================
