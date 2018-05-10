// Include files

// local
#include "TupleToolVeloTrackClusterInfo.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Event/Particle.h"
//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolVeloTrackClusterInfo
//
// 2012-12-12 : Kazu Akiba
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
// actually acts as a using namespace TupleTool
DECLARE_COMPONENT( TupleToolVeloTrackClusterInfo )

  //=============================================================================
  // Standard constructor, initializes variables
  //=============================================================================
TupleToolVeloTrackClusterInfo::TupleToolVeloTrackClusterInfo( const std::string& type,
    const std::string& name,
    const IInterface* parent )
: TupleToolBase ( type, name , parent )
{
  declareInterface<IParticleTupleTool>(this);

}

//=============================================================================

StatusCode TupleToolVeloTrackClusterInfo::fill( const LHCb::Particle*
    , const LHCb::Particle* P
    , const std::string& head
    , Tuples::Tuple& tuple )
{
  const std::string prefix = fullName(head);

  const size_t maxStationsInVeloTrack = 100;
  bool test = true;
  if ( !P ) return StatusCode::FAILURE;

  //first just return if the particle isn't supposed to have a track
  if ( !P->isBasicParticle() ) return StatusCode::SUCCESS;

  const LHCb::ProtoParticle* protop = P->proto();
  if (!protop) return StatusCode::SUCCESS;

  const LHCb::Track* track = protop->track();
  if (!track) return StatusCode::SUCCESS;

  else test &= tuple->column( prefix+"_TRACK_TCHI2NDOF",-1.);
  std::vector<double> stations;
  std::vector<double> stationADC;
  double averageCharge =0;
  double totalChargeOnTrack=0;
  double nClustersOnTrack =0;
  const std::vector<LHCb::LHCbID>& ids = (track)->lhcbIDs();
  for ( std::vector<LHCb::LHCbID>::const_iterator it = ids.begin(); it != ids.end(); ++it ){
    if (it->isVelo() == true) {
      const LHCb::VeloChannelID vcID = (it)->veloID();
      //Veloclusters:
      LHCb::VeloClusters *m_clusters = get<LHCb::VeloClusters>( m_locationClusters );
      debug() << "  -> number of clusters found in TES: "  << m_clusters->size() <<endmsg;
      debug() << "Getting cluster "  << endmsg;
      LHCb::VeloCluster *cluster = m_clusters->object( vcID );
      if(!cluster) return StatusCode::SUCCESS;
      if ( 0 == cluster ) {
        error() << "Missing cluster with ID " << vcID << endmsg;
        continue;
      }
      int sensor = it->veloID().sensor();
      double mysensor = sensor;
      stations.push_back(mysensor);
      double adc = cluster->totalCharge();
      stationADC.push_back(adc);
      averageCharge+=adc;
      nClustersOnTrack++;
    }
  } // lhcbids

  totalChargeOnTrack=averageCharge;
  if (nClustersOnTrack>0) averageCharge/=nClustersOnTrack;
  else averageCharge = -1000;

  int nVeloClustersOnTrack = (int) nClustersOnTrack;
  test &= tuple->column( prefix+"_TRACK_VELO_TOTALCHARGE", totalChargeOnTrack);
  test &= tuple->column( prefix+"_TRACK_VELO_AVERAGECHARGE", averageCharge);
  test &= tuple->column( prefix+"_TRACK_VELO_CLUSTERSONTRACK", nVeloClustersOnTrack);
  //for (int ii = 0; ii <stations.size(); ii++) info() << "  stations check  " << ii << " " << stations[ii] << endmsg;
  test &= tuple->farray( prefix+"_TRACK_VELO_STATION_ADC", stationADC.begin(), stationADC.end(), "nClusters" , maxStationsInVeloTrack);
  //for (int ii = 0; ii <stationADC.size(); ii++) info() << "  stationsADC check  " << ii << " " << stationADC[ii] << endmsg;
  test &= tuple->farray( prefix+"_TRACK_VELO_STATION_VECTOR", stations.begin(), stations.end(), "nClusters", maxStationsInVeloTrack);

  return StatusCode(test);
}
