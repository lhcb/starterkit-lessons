#include "TupleToolRICHPid.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Event/Particle.h"
#include "Event/RichPID.h"

#include "RichUtils/RichTrackSegment.h"

//-----------------------------------------------------------------------------
// Implementation file for class : EventInfoTupleTool
//
// 02 Nov 20010 : Andrew Powell
//-----------------------------------------------------------------------------

using namespace LHCb;

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolRICHPid::TupleToolRICHPid( const std::string& type,
                                    const std::string& name,
                                    const IInterface* parent )
  : TupleToolBase ( type, name , parent )
{
  declareInterface<IParticleTupleTool>(this);
}

//=============================================================================

StatusCode TupleToolRICHPid::fill( const Particle*
                                   , const Particle* P
                                   , const std::string& head
                                   , Tuples::Tuple& tuple )
{

  const std::string prefix = fullName(head);
  if( P )
  {
    bool test = true;

    int Assign_PDGID = P->particleID().pid();
    Rich::ParticleIDType Assign_RICHID = Rich::Unknown;
    if(abs(Assign_PDGID)==211)
    {
      Assign_RICHID = Rich::Pion;
    }
    else if(abs(Assign_PDGID)==321)
    {
      Assign_RICHID = Rich::Kaon;
    }
    else if(abs(Assign_PDGID)==11)
    {
      Assign_RICHID = Rich::Electron;
    }
    else if(abs(Assign_PDGID)==13)
    {
      Assign_RICHID = Rich::Muon;
    }
    else if(abs(Assign_PDGID)==2212)
    {
      Assign_RICHID = Rich::Proton;
    }

    if( !P->isBasicParticle() ) return StatusCode(test); // no rich info for composite!

    if( isPureNeutralCalo(P) )return StatusCode(test); // no rich information for calo neutrals

    const ProtoParticle* proto = P->proto();

    if( proto )
    {
      test &= tuple->column(  prefix+"_RICHDLLe"
                              ,proto->info(ProtoParticle::RichDLLe,-1000));

      test &= tuple->column(  prefix+"_RICHDLLmu"
                              ,proto->info(ProtoParticle::RichDLLmu,-1000));

      test &= tuple->column(  prefix+"_RICHDLLpi"
                              ,proto->info(ProtoParticle::RichDLLpi,-1000));

      test &= tuple->column(  prefix+"_RICHDLLK"
                              ,proto->info(ProtoParticle::RichDLLk,-1000));

      test &= tuple->column(  prefix+"_RICHDLLp"
                              ,proto->info(ProtoParticle::RichDLLp,-1000));

      test &= tuple->column(  prefix+"_RICHDLLbt"
                              ,proto->info(ProtoParticle::RichDLLbt,-1000));

      if( !tuple->column( prefix+"_RICHBestID", proto->richPID() ?
                          (proto->richPID()->bestParticleID()) : -2)) return StatusCode::FAILURE;

      if( !tuple->column( prefix+"_RICHThreshold", proto->richPID() ?
                          (proto->richPID()->isAboveThreshold(Assign_RICHID)) : -1)) return StatusCode::FAILURE;

      if( !tuple->column( prefix+"_RICHThresholdEl", proto->richPID() ?
                          (proto->richPID()->isAboveThreshold(Rich::Electron)) : -1)) return StatusCode::FAILURE;

      if( !tuple->column( prefix+"_RICHThresholdMu", proto->richPID() ?
                          (proto->richPID()->isAboveThreshold(Rich::Muon)) : -1)) return StatusCode::FAILURE;

      if( !tuple->column( prefix+"_RICHThresholdPi", proto->richPID() ?
                          (proto->richPID()->isAboveThreshold(Rich::Pion)) : -1)) return StatusCode::FAILURE;

      if( !tuple->column( prefix+"_RICHThresholdKa", proto->richPID() ?
                          (proto->richPID()->isAboveThreshold(Rich::Kaon)) : -1)) return StatusCode::FAILURE;

      if( !tuple->column( prefix+"_RICHThresholdPr", proto->richPID() ?
                          (proto->richPID()->isAboveThreshold(Rich::Proton)) : -1)) return StatusCode::FAILURE;

      if( !tuple->column( prefix+"_RICHAerogelUsed", proto->richPID() ?
                          (proto->richPID()->traversedRadiator(Rich::Aerogel)) : -1)) return StatusCode::FAILURE;

      if( !tuple->column( prefix+"_RICH1GasUsed", proto->richPID() ?
                          (proto->richPID()->traversedRadiator(Rich::Rich1Gas)) : -1)) return StatusCode::FAILURE;

      if( !tuple->column( prefix+"_RICH2GasUsed", proto->richPID() ?
                          (proto->richPID()->traversedRadiator(Rich::Rich2Gas)) : -1)) return StatusCode::FAILURE;

      if( !tuple->column( prefix+"_TRACK_Eta", proto->track() ?
                          (proto->track()->pseudoRapidity()) : -4.0)) return StatusCode::FAILURE;

      if( !tuple->column( prefix+"_TRACK_Phi", proto->track() ?
                          (proto->track()->phi()) : -4.0)) return StatusCode::FAILURE;

      return StatusCode(test);
    }
  }
  return StatusCode::FAILURE;
}

// Declarations of the Tool Factory
// actually acts as a using namespace TupleTool
DECLARE_COMPONENT( TupleToolRICHPid )
