// $Id: TupleToolPid.cpp,v 1.6 2010-03-04 14:02:03 xieyu Exp $
// Include files

// local
#include "TupleToolPid.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Event/Particle.h"
//-----------------------------------------------------------------------------
// Implementation file for class : EventInfoTupleTool
//
// 2007-11-07 : Jeremie Borel
//-----------------------------------------------------------------------------

using namespace LHCb;

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolPid::TupleToolPid( const std::string& type,
                            const std::string& name,
                            const IInterface* parent )
  : TupleToolBase ( type, name , parent )
{
  declareInterface<IParticleTupleTool>(this);
}

//=============================================================================

StatusCode TupleToolPid::fill( const Particle*
                               , const Particle* P
                               , const std::string& head
                               , Tuples::Tuple& tuple )
{
  if ( P )
  {
    const std::string prefix = fullName(head);

    bool test = true;
    test &= tuple->column( prefix+"_ID", P->particleID().pid() );

    if ( !P->isBasicParticle() ) return StatusCode(test); // no PID info for composite!
    if ( isPureNeutralCalo(P)  ) return StatusCode(test); // no PID information for calo neutrals

    const ProtoParticle* proto = P->proto();
    if ( proto )
    {

      // Combined DLLs
      test &= tuple->column( prefix+"_PIDe"
                             ,proto->info(LHCb::ProtoParticle::CombDLLe,-1000));
      test &= tuple->column( prefix+"_PIDmu"
                             ,proto->info(LHCb::ProtoParticle::CombDLLmu,-1000));
      test &= tuple->column( prefix+"_PIDK"
                             ,proto->info(LHCb::ProtoParticle::CombDLLk,-1000));
      test &= tuple->column( prefix+"_PIDp"
                             ,proto->info(LHCb::ProtoParticle::CombDLLp,-1000));

      // The MVA probabilities
      // There is one for each hypothesis
      test &= tuple->column( prefix+"_ProbNNe"
                             ,proto->info(LHCb::ProtoParticle::ProbNNe,-1000));
      test &= tuple->column( prefix+"_ProbNNk"
                             ,proto->info(LHCb::ProtoParticle::ProbNNk,-1000));
      test &= tuple->column( prefix+"_ProbNNp"
                             ,proto->info(LHCb::ProtoParticle::ProbNNp,-1000));
      test &= tuple->column( prefix+"_ProbNNpi"
                             ,proto->info(LHCb::ProtoParticle::ProbNNpi,-1000));
      test &= tuple->column( prefix+"_ProbNNmu"
                             ,proto->info(LHCb::ProtoParticle::ProbNNmu,-1000));
      test &= tuple->column( prefix+"_ProbNNghost"
                             ,proto->info(LHCb::ProtoParticle::ProbNNghost,-1000));

      // Muon
      const MuonPID * muonPID = proto->muonPID();
      test &= tuple->column( prefix+"_hasMuon", muonPID != NULL );
      test &= tuple->column( prefix+"_isMuon",  muonPID ? muonPID->IsMuon() : false );

      // RICH
      const RichPID * richPID = proto->richPID();
      test &= tuple->column( prefix+"_hasRich", richPID != NULL );
      // Moved some RICH information variables to default since they are useful to determine
      // how the PID information has been obtained.
      test &= tuple->column( prefix+"_UsedRichAerogel",
                             richPID ? richPID->usedAerogel()  : false );
      test &= tuple->column( prefix+"_UsedRich1Gas",
                             richPID ? richPID->usedRich1Gas() : false );
      test &= tuple->column( prefix+"_UsedRich2Gas",
                             richPID ? richPID->usedRich2Gas() : false );
      test &= tuple->column( prefix+"_RichAboveElThres",
                             richPID ? richPID->electronHypoAboveThres() : false );
      test &= tuple->column( prefix+"_RichAboveMuThres",
                             richPID ? richPID->muonHypoAboveThres()     : false );
      test &= tuple->column( prefix+"_RichAbovePiThres",
                             richPID ? richPID->pionHypoAboveThres()     : false );
      test &= tuple->column( prefix+"_RichAboveKaThres",
                             richPID ? richPID->kaonHypoAboveThres()     : false );
      test &= tuple->column( prefix+"_RichAbovePrThres",
                             richPID ? richPID->protonHypoAboveThres()   : false );

      // CALO
      test &= tuple->column( prefix+"_hasCalo", !proto->calo().empty() );

      // The kitchen sink ...
      if ( UNLIKELY(isVerbose()) )
      {

        // RICH variables
        test &= tuple->column( prefix+"_RichDLLe",  
                               proto->info(LHCb::ProtoParticle::RichDLLe,-1000) );
        test &= tuple->column( prefix+"_RichDLLmu",
                               proto->info(LHCb::ProtoParticle::RichDLLmu,-1000) );
        test &= tuple->column( prefix+"_RichDLLpi",
                               proto->info(LHCb::ProtoParticle::RichDLLpi,-1000) );
        test &= tuple->column( prefix+"_RichDLLk",
                               proto->info(LHCb::ProtoParticle::RichDLLk,-1000) );
        test &= tuple->column( prefix+"_RichDLLp", 
                               proto->info(LHCb::ProtoParticle::RichDLLp,-1000) );
        test &= tuple->column( prefix+"_RichDLLbt",
                               proto->info(LHCb::ProtoParticle::RichDLLbt,-1000) );

        // Muon variables
        test &= tuple->column( prefix+"_InAccMuon", 
                               proto->info(LHCb::ProtoParticle::InAccMuon,false)>0.5 );
        test &= tuple->column( prefix+"_isMuonLoose",  muonPID ? muonPID->IsMuonLoose() : false );
        test &= tuple->column( prefix+"_isMuonTight",  muonPID ? muonPID->IsMuonTight() : false );
        test &= tuple->column( prefix+"_MuonMuLL",     muonPID ? muonPID->MuonLLMu()    : -1000 );
        test &= tuple->column( prefix+"_MuonBkgLL",    muonPID ? muonPID->MuonLLBg()    : -1000 );
        test &= tuple->column( prefix+"_MuonNShared",  muonPID ? muonPID->nShared()     : -1    );
        test &= tuple->column( prefix+"_MuonChi2Corr", muonPID ? muonPID->chi2Corr()    : -1.   );

        // ECAL variables
        test &= tuple->column( prefix+"_InAccEcal", 
                               proto->info(LHCb::ProtoParticle::InAccEcal,false)>0.5 );
        test &= tuple->column( prefix+"_CaloEcalE",
                               proto->info(LHCb::ProtoParticle::CaloEcalE,-10000.));
        test &= tuple->column( prefix+"_EcalPIDe",
                               proto->info(LHCb::ProtoParticle::EcalPIDe,-10000.));
        test &= tuple->column( prefix+"_EcalPIDmu",
                               proto->info(LHCb::ProtoParticle::EcalPIDmu,-10000.));

        // HCAL
        test &= tuple->column( prefix+"_InAccHcal", 
                               proto->info(LHCb::ProtoParticle::InAccHcal,false)>0.5 );
        test &= tuple->column( prefix+"_CaloHcalE",
                               proto->info(LHCb::ProtoParticle::CaloHcalE,-10000.));
        test &= tuple->column( prefix+"_HcalPIDe",
                               proto->info(LHCb::ProtoParticle::HcalPIDe,-10000.));
        test &= tuple->column( prefix+"_HcalPIDmu",
                               proto->info(LHCb::ProtoParticle::HcalPIDmu,-10000.));

        // PRS
        test &= tuple->column( prefix+"_InAccPrs", 
                               proto->info(LHCb::ProtoParticle::InAccPrs,false)>0.5 );
        test &= tuple->column( prefix+"_PrsPIDe",
                               proto->info(LHCb::ProtoParticle::PrsPIDe,-10000.));
        test &= tuple->column( prefix+"_CaloPrsE", 
                               proto->info(LHCb::ProtoParticle::CaloPrsE,-999) );


        // SPD
        test &= tuple->column( prefix+"_InAccSpd", 
                               proto->info(LHCb::ProtoParticle::InAccSpd,false)>0.5 );
        test &= tuple->column( prefix+"_CaloSpdE", 
                               proto->info(LHCb::ProtoParticle::CaloSpdE,-999) );

        // BREM
        test &= tuple->column( prefix+"_InAccBrem", 
                               proto->info(LHCb::ProtoParticle::InAccBrem,false)>0.5 );
        test &= tuple->column( prefix+"_BremPIDe",
                               proto->info(LHCb::ProtoParticle::BremPIDe,-10000.));

        // VELO
        test &= tuple->column( prefix+"_VeloCharge",
                               proto->info(LHCb::ProtoParticle::VeloCharge,-10000.));

      }

      return StatusCode(test);
    }
  }

  return StatusCode::FAILURE;
}

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolPid )
