// $Id: TupleToolPhotonInfo.cpp,v 1.1 2010-05-21 17:03:11 odescham Exp $
// Include files

// local
#include "TupleToolPhotonInfo.h"
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
#include "Event/Particle.h"
#include "GaudiKernel/IRegistry.h" //

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolPhotonInfo
//
// 2008-10-31 :(Happy Halloween)
// Yasmine Amhis
//-----------------------------------------------------------------------------

using namespace Gaudi;
using namespace LHCb;

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolPhotonInfo )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolPhotonInfo::TupleToolPhotonInfo( const std::string& type,
                                            const std::string& name,
                                            const IInterface* parent )
    : TupleToolBase ( type, name , parent ),
      m_PhotonID(22)
{
  declareInterface<IParticleTupleTool>(this);
}

//=============================================================================

StatusCode TupleToolPhotonInfo::fill(const Particle* , const Particle* P
                                     ,const std::string& head
                                     ,Tuples::Tuple& tuple ){
  const std::string prefix=fullName(head);

  bool filltuple = true;
  if( P ){
    if (P->particleID().pid() == m_PhotonID &&  isPureNeutralCalo(P)) {

      const LHCb::ProtoParticle* proto = P->proto();
      if(NULL == proto)return StatusCode::SUCCESS;

      double match = proto->info(LHCb::ProtoParticle::CaloTrMatch, 9999.);
      double pid = proto->info(LHCb::ProtoParticle::PhotonID, 0.);
      double cnv  = ( proto->info(LHCb::ProtoParticle::CaloDepositID, 0.)<0) ? 1 : 0;
      double prs  = proto->info(LHCb::ProtoParticle::CaloNeutralPrs, 0.);
      double shape  = proto->info(LHCb::ProtoParticle::ShowerShape, 0.);

      filltuple &= tuple->column( prefix+"_Converted", cnv );
      filltuple &= tuple->column( prefix+"_Prs", prs );
      filltuple &= tuple->column( prefix+"_Matching", match );
      filltuple &= tuple->column( prefix+"_PID", pid );
      filltuple &= tuple->column( prefix+"_CL" , P->confLevel() );
      filltuple &= tuple->column( prefix+"_ShowerShape", shape );


    }//this is a photon
  }//get the particle

  return StatusCode(filltuple);

}



