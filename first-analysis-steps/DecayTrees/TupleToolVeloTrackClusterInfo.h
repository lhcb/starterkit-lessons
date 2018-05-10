#ifndef JBOREL_TUPLETOOLTRACKINFO_H
#define JBOREL_TUPLETOOLTRACKINFO_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface
#include "Event/VeloCluster.h"
#include "Event/Track.h"
#include "Event/Particle.h"
#include "TrackInterfaces/IVeloClusterPosition.h"
#include "TrackInterfaces/IVeloExpectation.h"
#include "TrackInterfaces/IMeasurementProvider.h"

/** @class TupleToolVeloTrackClusterInfo TupleToolVeloTrackClusterInfo.h jborel/TupleToolVeloTrackClusterInfo.h
 *
 * _TRACK_VELO_TOTALCHARGE         sum of the adc counts of the velo clusters
 * _TRACK_VELO_AVERAGECHARGE       average adc counts
 * _TRACK_VELO_CLUSTERSONTRACK     how many velo clusters on this track
 * _TRACK_VELO_STATION_ADC         adc counts indexed in a vector
 * _TRACK_VELO_STATION_VECTOR      vector with the sensor numbers of the clusters same index as the adc.
 *
 *  @author Kazu Akiba
 *  @date   2012-12-12
 */
class TupleToolVeloTrackClusterInfo : public TupleToolBase, virtual public IParticleTupleTool {
public:
  /// Standard constructor
  TupleToolVeloTrackClusterInfo( const std::string& type,
		    const std::string& name,
		    const IInterface* parent);

  /// Declare properties
  Gaudi::Property< std::string > m_locationClusters{this, "VeloClusterLocation", LHCb::VeloClusterLocation::Default, "Location of VELO clusters"};

  StatusCode fill( const LHCb::Particle*
			   , const LHCb::Particle*
			   , const std::string&
			   , Tuples::Tuple& ) override;

};

#endif // JBOREL_TUPLETOOLTRACKINFO_H
