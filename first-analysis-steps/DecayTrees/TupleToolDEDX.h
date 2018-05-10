// $Id: TupleToolDEDX_Solution.h,v 1.1 2009/06/11 16:22:29 rlambert Exp $
#ifndef TUPLETOOLDEDX_H
#define TUPLETOOLDEDX_H 1

//Include from VELOPID:
//#include "Velo/VeloMonitorBase.h"
#include "Event/VeloCluster.h"
#include "Event/Track.h"
#include "Event/Particle.h"
#include "TrackInterfaces/IVeloClusterPosition.h"
#include "TrackInterfaces/IVeloExpectation.h"
#include "TrackInterfaces/IMeasurementProvider.h"
#include "GaudiAlg/GaudiTupleAlg.h"
//#include "GaudiAlg/GaudiTool.h"
#include "GaudiAlg/TupleObj.h"
#include "Event/MCHit.h"

//Linkers, track2mc
#include "Linker/LinkedTo.h"
#include "Linker/LinkerWithKey.h"
#include "Linker/LinkerTool.h"
#include "Linker/LinkedFrom.h"
#include "Kernel/Track2MC.h"
//tuple:
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
#include <MCInterfaces/IMCParticleSelector.h>
//End VeloPID


// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "Kernel/IParticleTupleTool.h"            // Interface

/** @class TupleToolDEDX TupleToolDEDX.h jborel/TupleToolDEDX.h
 *
 * \brief Skeleton for DaVinci Tutorial 7
 *
 *
 *
 * \sa DecayTreeTuple
 *
 *  @author Rob Lambert
 *  @date   2009-06-09
 */
class TupleToolDEDX : public GaudiTool, virtual public IParticleTupleTool {
    public:
//Start VeloPID:
	LHCb::Tracks*       tracks;
	LHCb::VeloClusters* m_clusters;
	LHCb::MCParticles* m_MCParticles;
	LHCb::MCHits* m_MCHits;
	LHCb::ProtoParticles* protoParticles;
	unsigned long m_event;
//	// std::string asctName();
//	//From Tomasz:
//	typedef LinkerTool<LHCb::VeloCluster, LHCb::MCHit> AsctTool;
//	typedef AsctTool::DirectType Table;
//	typedef Table::Range Range;
//	typedef Table::iterator iterator;
	std::string m_asctLocation;
//	IMCParticleSelector* m_mcSelector;   /// MC selector
//	virtual StatusCode loopOnTracks();
//	StatusCode plotCluster(LHCb::VeloCluster* cluster,std::string ClusterType, double theta=-400.,double prap = -400.);
	void quickSort(double arr[], int left, int right);
	StatusCode plotSensorsADC(double& adc,std::string corr,std::string& ClusterType,int sensor_num = -400);
	bool m_MC;
	bool m_AngleCorrection;
	std::string m_clusterCont;
	std::string m_trackCont;
	std::string m_MCCont;
	std::string m_track2MC;
	std::string m_MCHitsCont;
	std::string m_protoParticlesCont;

	//End VELOPID

  TupleToolDEDX( const std::string& type,
		    const std::string& name,
		    const IInterface* parent);

  virtual ~TupleToolDEDX(){}; ///< Destructor


  StatusCode fill( const LHCb::Particle*
			   , const LHCb::Particle*
			   , const std::string&
			   , Tuples::Tuple& ) override;
private:
  bool m_abs;

};

#endif // TUPLETOOLDEDX_H
