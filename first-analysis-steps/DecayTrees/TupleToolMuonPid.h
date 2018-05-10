// $Id: TupleToolMuonPid.h,v 1.4 2010-03-04 14:02:03 xieyu Exp $
#ifndef TUPLETOOLMUONID_H
#define TUPLETOOLMUONID_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface
#include "MuonDet/MuonBasicGeometry.h"
#include "MuonDet/DeMuonDetector.h"
//#include "MuonDet/MuonNamespace.h"

// From event packages
#include "Event/MuonPID.h"
#include "Event/Track.h"
#include "Event/MuonCoord.h"
#include "Event/MuonDigit.h"


/** @class TupleToolMuonPid TupleToolMuonPid.h
 *
 *  @author Kazu Akiba
 *  @date   2011-09-01
 *
 */
class TupleToolMuonPid : public TupleToolBase, virtual public IParticleTupleTool {
public:
  /// Standard constructor
  TupleToolMuonPid( const std::string& type,
                    const std::string& name,
                    const IInterface* parent);

  StatusCode initialize() override;    ///< Algorithm initialization

  virtual ~TupleToolMuonPid(){}; ///< Destructor

  StatusCode fill( const LHCb::Particle*, const LHCb::Particle*
                   , const std::string&, Tuples::Tuple& ) override;

  StatusCode trackExtrapolate(const LHCb::Track *pTrack, std::vector<double> &trackX, std::vector<double> &trackY );
  std::vector<int> findTrackRegions(const LHCb::Track *pTrack);

private:
  DeMuonDetector*  m_mudet;
  unsigned int m_NStation;     // Number of stations
  unsigned int m_NRegion;      //Number of regions
  std::vector<double> m_stationZ; // station position
  std::vector<double> m_trackX; // position of track in x(mm) in each station
  std::vector<double> m_trackY; // position of track in y(mm) in each station

  //Names of the station
  std::vector<std::string> m_stationNames;


};
#endif // TUPLETOOLMUONID_H
