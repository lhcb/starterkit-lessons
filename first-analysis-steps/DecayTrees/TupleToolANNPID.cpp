// $Id: $

#include "TupleToolANNPID.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolANNPID
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolANNPID::TupleToolANNPID( const std::string& type,
                                  const std::string& name,
                                  const IInterface* parent )
  : TupleToolBase ( type, name, parent )
{
  declareInterface<IParticleTupleTool>(this);
  declareProperty( "ANNPIDTunes", m_pidTunes =
      { "MC12TuneV2","MC12TuneV3","MC12TuneV4","MC15TuneV1"
          //,"MC15TuneDNNV1","MC15TuneFLAT4dV1","MC15TuneCatBoostV1"
          }
                   );
  // PID types. Deuteron by default is not included at the moment.
  declareProperty( "PIDTypes", m_pidTypes =
  { "Electron","Muon","Pion","Kaon","Proton","Ghost" } ); 
}

//=============================================================================

StatusCode TupleToolANNPID::initialize()
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  m_pidTool =
    tool<ANNGlobalPID::IChargedProtoANNPIDTool>( "ANNGlobalPID::ChargedProtoANNPIDTool",
                                                 "ChargedProtoANNPID" );

  // by default all types are off
  m_pidIsOn.fill(false);
  // turn on as required
  for ( const auto & p : m_pidTypes )
  {
    if      ( "Electron" == p ) { m_pidIsOn[El] = true; }
    else if ( "Muon"     == p ) { m_pidIsOn[Mu] = true; }
    else if ( "Pion"     == p ) { m_pidIsOn[Pi] = true; }
    else if ( "Kaon"     == p ) { m_pidIsOn[Ka] = true; }
    else if ( "Proton"   == p ) { m_pidIsOn[Pr] = true; }
    else if ( "Deuteron" == p ) { m_pidIsOn[De] = true; }
    else if ( "Ghost"    == p ) { m_pidIsOn[Gh] = true; }
  }
    
  return sc;
}

//=============================================================================

StatusCode TupleToolANNPID::fill( const LHCb::Particle*
                                  , const LHCb::Particle* P
                                  , const std::string& head
                                  , Tuples::Tuple& tuple )
{
  StatusCode sc = StatusCode::SUCCESS;

  // Only run on charged stable particles
  if ( !P || !P->proto() || !P->proto()->track() ) return sc;

  // get the prefix
  const auto prefix = fullName(head);

  // Loop over PID tunes
  for ( const auto& pidTune : m_pidTunes )
  {
    // Fill the ANNPID variables for those that are defined
    ANNGlobalPID::IChargedProtoANNPIDTool::RetType res;
    // Electrons
    if ( sc && m_pidIsOn[El] )
    {
      res = m_pidTool->annPID(P->proto(),LHCb::ParticleID(11),pidTune);
      if ( res.status ) { sc = tuple->column( prefix+"_"+pidTune+"_ProbNNe", res.value ); }
    }
    // Muons
    if ( sc && m_pidIsOn[Mu] )
    {
      res = m_pidTool->annPID(P->proto(),LHCb::ParticleID(13),pidTune);
      if ( res.status ) { sc = tuple->column( prefix+"_"+pidTune+"_ProbNNmu", res.value ); }
    }
    // Pions
    if ( sc && m_pidIsOn[Pi] )
    {
      res = m_pidTool->annPID(P->proto(),LHCb::ParticleID(211),pidTune);
      if ( res.status ) { sc = tuple->column( prefix+"_"+pidTune+"_ProbNNpi", res.value ); }
    }
    // Kaons
    if ( sc && m_pidIsOn[Ka] )
    {
      res = m_pidTool->annPID(P->proto(),LHCb::ParticleID(321),pidTune);
      if ( res.status ) { sc = tuple->column( prefix+"_"+pidTune+"_ProbNNk", res.value ); }
    }
    // Protons
    if ( sc && m_pidIsOn[Pr] )
    {
      res = m_pidTool->annPID(P->proto(),LHCb::ParticleID(2212),pidTune);
      if ( res.status ) { sc = tuple->column( prefix+"_"+pidTune+"_ProbNNp", res.value ); }
    }
    // Deuterons
    if ( sc && m_pidIsOn[De] )
    {
      res = m_pidTool->annPID(P->proto(),LHCb::ParticleID(1000010020),pidTune);
      if ( res.status ) { sc = tuple->column( prefix+"_"+pidTune+"_ProbNNd", res.value ); }
    }
    // Ghosts
    if ( sc && m_pidIsOn[Gh] )
    {
      res = m_pidTool->annPID(P->proto(),LHCb::ParticleID(0),pidTune);
      if ( res.status ) { sc = tuple->column( prefix+"_"+pidTune+"_ProbNNghost", res.value ); }
    }
  }

  // return
  return sc;
}

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolANNPID )
