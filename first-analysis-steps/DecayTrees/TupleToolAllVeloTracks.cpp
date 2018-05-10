// from Gaudi
#include "Event/Track.h"
#include "Event/RecSummary.h"
#include "Event/VertexBase.h"
#include "Event/RecVertex.h"
#include "Kernel/DefaultDVToolTypes.h"
#include "Event/VeloCluster.h"
#include "Event/Particle.h"
#include "TrackInterfaces/IVeloClusterPosition.h"
#include "TrackInterfaces/IVeloExpectation.h"
#include "TrackInterfaces/IMeasurementProvider.h"
// local
#include "TupleToolAllVeloTracks.h"
#include "TrackInterfaces/ITrackExtrapolator.h"
#include "TrackInterfaces/ITrackStateProvider.h"
#include "Kernel/IParticle2MCAssociator.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolAllVeloTracks
//
// 2017-05-25 : Dan Johnson, Victor Coco, Kazu Akiba
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolAllVeloTracks::TupleToolAllVeloTracks(const std::string& type,
                                               const std::string& name,
                                               const IInterface* parent)
    : TupleToolBase(type, name, parent) {
  declareInterface<IEventTupleTool>(this);
}

StatusCode TupleToolAllVeloTracks::initialize() {
  StatusCode sc = TupleToolBase::initialize();
  if (sc.isFailure()) return sc;

  return sc;
}

//=============================================================================
StatusCode TupleToolAllVeloTracks::fill(Tuples::Tuple& tup) {
  const std::string prefix = fullName();
  bool test = true;

  std::vector<double> vSlopeX;
  std::vector<double> vSlopeY;
  std::vector<double> vPointX;
  std::vector<double> vPointY;
  std::vector<double> vPointZ;
  std::vector<double> vChi2pDOF;
  std::vector<int> vType;
  std::vector<int> vKey;

  const LHCb::Tracks* tracks = get<LHCb::Tracks*>(m_location);
  if (!tracks || tracks->empty()) return StatusCode::FAILURE;
  for (const auto& tr : (*tracks)) {
    // Only save tracks with a velo segment (long, velo or upstream)
    if (tr->type() != LHCb::Track::Types::Velo &&
        tr->type() != LHCb::Track::Types::Upstream && tr->type() != LHCb::Track::Types::Long)
      continue;
    Gaudi::XYZPoint pos = tr->position();
    Gaudi::XYZVector slop = tr->slopes();
    vSlopeX.push_back(slop.X());
    vSlopeY.push_back(slop.Y());
    vPointX.push_back(pos.X());
    vPointY.push_back(pos.Y());
    vPointZ.push_back(pos.Z());
    vType.push_back(tr->type());
    vKey.push_back(tr->key());
    vChi2pDOF.push_back(tr->chi2PerDoF());
  }  // tracks
  test &= tup->farray(prefix + "VeloSegTr_slopeX", vSlopeX,
                      prefix + "nVeloSegTracks", m_max);
  test &= tup->farray(prefix + "VeloSegTr_slopeY", vSlopeY,
                      prefix + "nVeloSegTracks", m_max);
  test &= tup->farray(prefix + "VeloSegTr_pointX", vPointX,
                      prefix + "nVeloSegTracks", m_max);
  test &= tup->farray(prefix + "VeloSegTr_pointY", vPointY,
                      prefix + "nVeloSegTracks", m_max);
  test &= tup->farray(prefix + "VeloSegTr_pointZ", vPointZ,
                      prefix + "nVeloSegTracks", m_max);
  test &= tup->farray(prefix + "VeloSegTr_chi2pdof", vChi2pDOF,
                      prefix + "nVeloSegTracks", m_max);
  test &= tup->farray(prefix + "VeloSegTr_type", vType,
                      prefix + "nVeloSegTracks", m_max);
  test &= tup->farray(prefix + "VeloSegTr_key", vKey, prefix + "nVeloSegTracks",
                      m_max);
  return StatusCode(test);
}

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolAllVeloTracks )
