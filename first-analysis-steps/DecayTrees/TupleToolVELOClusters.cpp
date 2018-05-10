// Include files

// from Gaudi
#include "TupleToolVELOClusters.h"
#include "Event/Track.h"
#include "Event/ODIN.h"

// local
#include "TupleToolVELOClusters.h"

//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolVELOClusters::TupleToolVELOClusters( const std::string& type,
                                              const std::string& name,
                                              const IInterface* parent )
  : TupleToolBase ( type, name, parent )
{
  declareInterface<IEventTupleTool>(this);
}

StatusCode TupleToolVELOClusters::fill( Tuples::Tuple& tup )
{
  const std::string prefix = fullName();

  // Fill the tuple
  bool test = true;
  std::vector<double> stations;
  std::vector<double> stationADC;

  std::vector<int> vids; vids.reserve(1000);
  LHCb::Tracks* tracksCont = get<LHCb::Tracks>( LHCb::TrackLocation::Default );
  if ( !tracksCont || tracksCont->empty() )
  {
    return StatusCode::FAILURE;
  }
  for (LHCb::Tracks::iterator iter = tracksCont->begin();
       iter != tracksCont->end(); ++iter){
    const std::vector<LHCb::LHCbID>& ids = (*iter)->lhcbIDs();
    for ( std::vector<LHCb::LHCbID>::const_iterator it = ids.begin(); it != ids.end(); ++it )
    {
      if ( it->isVelo() ) vids.push_back(it->veloID());
    } // lhcbids
  } // tracks

  std::sort(vids.begin(), vids.end());
  std::vector<int>::iterator u = std::unique(vids.begin(), vids.end());
  vids.resize(u - vids.begin());

  LHCb::VeloClusters* clusters = getIfExists<LHCb::VeloClusters>( evtSvc() , m_locationClusters );
  if ( !clusters || clusters->empty() )
  {
    Warning( "No VeloClusters found at location " + m_locationClusters + " for this event " ).ignore();
    return StatusCode::SUCCESS;
  }
  // MDN loop over the clusters
  for ( LHCb::VeloClusters::iterator iterV = clusters->begin(); iterV != clusters->end(); ++iterV )
  {
    // check if used on track
    if (std::find(vids.begin(), vids.end(),(*iterV)->channelID()) == vids.end())
    {

      if ( msgLevel(MSG::DEBUG) )
        debug() << "hit is not on track "
                << (*iterV)->isRType() <<  " "<< (*iterV)->isPhiType() <<  " "
                << (*iterV)->isPileUp() << endmsg;

      const double mysensor = (double)(*iterV)->channelID().sensor();
      stations.push_back(mysensor);
      const double adc = (*iterV)->totalCharge();
      stationADC.push_back(adc);

    }
  }

  const int unusedvelo = clusters->size() - vids.size() ;

  //info() << " n unused =  " << unusedvelo << endmsg;
  test &= tup->column( prefix+"nUnusedVeloClusters", unusedvelo);
  test &= tup->farray( prefix+"UNUSED_VELO_STATION_ADC", stationADC.begin(), stationADC.end(), "nClusters" , 100);
  test &= tup->farray( prefix+"UNUSED_VELO_STATION_VECTOR", stations.begin(), stations.end(), "nClusters", 100);

  return StatusCode(test);
}


// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolVELOClusters )
