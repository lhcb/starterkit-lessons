// Include files

// from Gaudi
#include "DetDesc/IGeometryInfo.h"
#include "Event/MuonCoord.h"
#include "Kernel/LHCbID.h"
#include "Kernel/MuonTileID.h"
#include "Kernel/Particle2MCLinker.h"


// local
#include "TupleToolMuonPid.h"

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
TupleToolMuonPid::TupleToolMuonPid( const std::string& type,
                                    const std::string& name,
                                    const IInterface* parent )
  : TupleToolBase ( type, name , parent )
{
  declareInterface<IParticleTupleTool>(this);
}

//=============================================================================

//=============================================================================
StatusCode TupleToolMuonPid::initialize()
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;
  // Get Basic Muon geometry info
  m_NStation = 0;
  m_NRegion = 0;
  MuonBasicGeometry basegeometry( detSvc(),msgSvc());
  m_NStation= basegeometry.getStations();
  m_NRegion = basegeometry.getRegions();
  unsigned int i=0;
  while(i<m_NStation){
    m_stationNames.push_back(basegeometry.getStationName(i));
    if (msgLevel(MSG::DEBUG)) debug()   <<" station "<<i<<" "<<m_stationNames[i]<<endmsg;
    i++;
  }
  m_mudet=getDet<DeMuonDetector>("/dd/Structure/LHCb/DownstreamRegion/Muon");
  //m_mudet=getDet<DeMuonDetector>(DeMuonLocation::Default);
  for(unsigned int station = 0 ; station < m_NStation ; station++ ){
    m_stationZ.push_back(m_mudet->getStationZ(station));
  }
  if(!m_mudet) return StatusCode::FAILURE;
  return sc;

}
//=============================================================================

StatusCode TupleToolMuonPid::fill( const Particle*
                                   , const Particle* P
                                   , const std::string& head
                                   , Tuples::Tuple& tuple )
{
  const std::string prefix=fullName(head);
  if (msgLevel(MSG::DEBUG)) debug() << "TupleToolMuonPid::fill: ++++++++ Filling info for particle with key " << P->key() << " head: " << head << " ++++++++ " << endmsg;
  if( P ){
    bool test = true;
    test &= tuple->column( prefix+"_ID", P->particleID().pid() );
    if( !P->isBasicParticle() ) return StatusCode(test); // no rich info for composite!
    if( isPureNeutralCalo(P)) return StatusCode(test); // no rich information for calo neutrals
    const ProtoParticle* proto = P->proto();
    if( proto ){
      // Combined DLLs
      test &= tuple->column( prefix+"_CombDLLMu"
                             ,proto->info(LHCb::ProtoParticle::additionalInfo::CombDLLmu,-1000));
      test &= tuple->column( prefix+"_ProbNNmu"
                             ,proto->info(LHCb::ProtoParticle::additionalInfo::ProbNNmu,-1000));
      test &= tuple->column( prefix+"_ProbNNghost"
                             ,proto->info(LHCb::ProtoParticle::additionalInfo::ProbNNghost,-1000));
      test &= tuple->column( prefix+"_InMuonAcc"
                             ,proto->info(LHCb::ProtoParticle::additionalInfo::InAccMuon, -1000));
      bool hasMuon = false;
      bool isMuon = false;
      bool isMuonLoose = false;
      //bool isMuonTight = false;
      double muonLLmu = -99999;
      double muonLLbg = -99999;
      const MuonPID * muonPID = proto->muonPID();
      int NShared  = -1;
      double muonDist2 = -99999;
      if(muonPID) {
        hasMuon = true;
        isMuon = muonPID->IsMuon();
        isMuonLoose = muonPID->IsMuonLoose();
        //isMuonTight = muonPID->IsMuonTight();
        NShared = muonPID->nShared();
        muonLLmu= muonPID->MuonLLMu();
        muonLLbg= muonPID->MuonLLBg();
        //std::cout << "before get Tracks" << std::endl;
        const LHCb::Track* track = muonPID->muonTrack();
        if (track) {
          //std::cout << "before get muonDist2" << std::endl;
          muonDist2 = track->info(LHCb::Track::AdditionalInfo::MuonDist2, -1000);
        }
      }

      // Find region in M2 or M3 from track extrapolation, used to calculate muLL
      const LHCb::Track *pTrack = proto->track();
      std::vector<int> trackRegion(m_NStation,-1);

      //find track region
      trackRegion=findTrackRegions(pTrack);
      int region=trackRegion[1];   // M2
      if (region<0) {
        if (msgLevel(MSG::DEBUG)) debug() <<  " Track extrapolation in station 2 gives non-existent region "<< endmsg;
        region=trackRegion[2];   // M3
      }
      if (region<0 && msgLevel(MSG::DEBUG)) debug() <<  " Track extrapolation in station 3 gives non-existent region " << endmsg;

      if (msgLevel(MSG::DEBUG)) debug() << "TupleToolMuonPid::fill:Track info: p = " << pTrack->p() << " Region in M2/M3: " << region
                                        << " IsMuonLoose = " << isMuonLoose << " IsMuon = " << isMuon << " Dist2 = " << muonDist2
                                        << " muonLLmu = " << muonLLmu << " muonLLbg = " << muonLLbg << endmsg;

      //std::cout << "before add MuonDist2 to ntuple" << std::endl;
      test &= tuple->column( prefix+"_MuonDist2",  muonDist2);
      test &= tuple->column( prefix+"_regionInM2", region);
      int isMuonFromProto = -2000;
      test &= tuple->column( prefix+"_hasMuon", hasMuon);
      test &= tuple->column( prefix+"_isMuon", isMuon);
      test &= tuple->column( prefix+"_isMuonLoose", isMuonLoose);
      //test &= tuple->column( prefix+"_isMuonTight", isMuonTight);
      test &= tuple->column( prefix+"_NShared", NShared);
      test &= tuple->column( prefix+"_MuonLLmu", muonLLmu);
      test &= tuple->column( prefix+"_MuonLLbg", muonLLbg);

      isMuonFromProto = ((int) (proto->info(LHCb::ProtoParticle::additionalInfo::MuonPIDStatus, -999)))%2;
      test &= tuple->column( prefix+"_isMuonFromProto", isMuonFromProto );

      //const LHCb::Track* track = proto->track();
      //if (track) {
      //  test &= tuple->column( prefix+"_MuonDist2",  track->info(LHCb::Track::AdditionalInfo::MuonDist2, -1000));
      //}

      return StatusCode(test);
    }
  }
  return StatusCode::FAILURE;
}

//=====================================================================
// Extrapolate track to Muon system and find region in each chamber. Region in M2 is used in the dll calculation
// Code copied/adapted from MuonIDAlg. J. Helder. 04/2013
//=====================================================================

StatusCode TupleToolMuonPid::trackExtrapolate(const LHCb::Track *pTrack, std::vector<double> &trackX, std::vector<double> &trackY ){

  // get first state for the preSel cut
  const LHCb::State * state1 = &(pTrack->firstState());
  // get state closest to M1 for extrapolation
  const LHCb::State * state = &(pTrack->closestState(9450.));

  if(!state1) return Error(" Failed to get 1st state from track ");
  if(!state)  return Error(" Failed to get state from track ");

  // get the momentum (MeV/c)
  //double momentum = state->p();
  //double momentumPre = state1->p();
  //if (msgLevel(MSG::DEBUG)) debug() << "trackExtrapolate: momentum = " << momentum << " momentumPre = " << momentumPre << endmsg;

  // Project the state into the muon stations
  for(unsigned int station = 0; station < m_NStation ; station++){
    // x(z') = x(z) + (dx/dz * (z' - z))
    trackX.push_back(state->x() + ( state->tx() *
                                    (m_stationZ[station] - state->z()) ));
    trackY.push_back(state->y() + ( state->ty() *
                                    (m_stationZ[station] - state->z()) ));
  }
  if (msgLevel(MSG::DEBUG)) {
    debug() << "trackExtrapolate: trackX = " << trackX << endmsg;
    debug() << "trackExtrapolate: trackY = " << trackY << endmsg;
  }
  return StatusCode::SUCCESS;
}

//=====================================================================
std::vector<int> TupleToolMuonPid::findTrackRegions(const LHCb::Track *pTrack){
  //=====================================================================
  // comment: Returns the muon detector region of the extrapolated track;
  // authors: G. Lanfranchi & S. Furcas & X. Cid Vidal,
  // date:    10/3/11
  //=====================================================================

  std::vector<double> trackX;
  std::vector<double> trackY;
  trackExtrapolate(pTrack,trackX,trackY);

  //info() << "findTrackRegions: trackX = " << trackX << endmsg;
  //info() << "findTrackRegions: trackY = " << trackY << endmsg;


  std::vector<int> trackRegion(m_NStation,-1);
  // Region of the track extrapolated:

  for (unsigned int sta=0;sta<m_NStation; sta++){

    int chnum = -1;
    int regnum = -1;

    m_mudet->Pos2StChamberNumber(trackX[sta],trackY[sta],sta,chnum,regnum).ignore();
    //if (msgLevel(MSG::DEBUG)) debug() << "TupleToolMuonPid: Station = " << sta << " chnum = " << chnum << " regnum = " << regnum  << endmsg;
    trackRegion[sta] = regnum;
    if (trackRegion[sta]<0 && msgLevel(MSG::DEBUG) )
      debug() << format(" Track extrapolation in station %d gives not-existent region ",sta) << endmsg;
  }

  if (msgLevel(MSG::DEBUG)) debug() << "TrackRegions: " << trackRegion << endmsg;
  return trackRegion;
}

// Declaration of the Tool Factory
// actually acts as a using namespace TupleTool
DECLARE_COMPONENT( TupleToolMuonPid )
