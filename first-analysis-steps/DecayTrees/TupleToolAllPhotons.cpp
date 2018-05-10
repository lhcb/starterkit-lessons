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
#include "TupleToolAllPhotons.h"
#include "TrackInterfaces/ITrackExtrapolator.h"
#include "TrackInterfaces/ITrackStateProvider.h"
#include "Kernel/IParticle2MCAssociator.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolAllPhotons
//
// 2017-05-25 : Dan Johnson, Victor Coco, Kazu Akiba
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolAllPhotons::TupleToolAllPhotons(const std::string& type,
                                         const std::string& name,
                                         const IInterface* parent)
    : TupleToolBase(type, name, parent) {
  declareInterface<IEventTupleTool>(this);
}

StatusCode TupleToolAllPhotons::initialize() {
  StatusCode sc = TupleToolBase::initialize();
  if (sc.isFailure()) return sc;

  info() << "Will fill all particles from " << m_location << " up to size " << m_max << endmsg;

  m_pvf = tool<IRelatedPVFinder>(DaVinci::DefaultTools::PVRelator, this);
  if (!m_pvf) return Error("Couldn't get PVRelator");

  m_dist = tool<IDistanceCalculator>(DaVinci::DefaultTools::Distance, this);
  if (!m_dist) {
    return Error("Unable to retrieve the IDistanceCalculator tool");
  }
  m_extrapolator = tool<ITrackStateProvider>("TrackStateProvider", this);

  // MC associators
  for ( const auto &iMCAss : m_p2mcAssocTypes ) {
    m_p2mcAssocs.push_back(tool<IParticle2MCAssociator>(iMCAss, this));
  }

  return sc;
}

//=============================================================================
StatusCode TupleToolAllPhotons::fill(Tuples::Tuple& tup) {
  const std::string prefix = fullName();

  LHCb::Particle::Range parts;
  if (exist<LHCb::Particle::Range>(m_location)) {
    parts = get<LHCb::Particle::Range>(m_location);
  } else
    return Warning("Nothing found at " + m_location, StatusCode::SUCCESS, 1);

  // Fill the tuple
  bool test = true;

  std::vector<double> PX, PY, PZ, PE, CL, Prob;
  std::vector<bool> ISMUON;
  std::vector<int> PID, TCAT, TPID;
  std::vector<double> vCaloTrMatch;
  std::vector<double> vCaloDepositID;
  std::vector<double> vShowerShape;
  std::vector<double> vClusterMass;
  std::vector<double> vCaloNeutralSpd;
  std::vector<double> vCaloNeutralPrs;
  std::vector<double> vCaloNeutralEcal;
  std::vector<double> vCaloNeutralHcal2Ecal;
  std::vector<double> vCaloNeutralE49;
  std::vector<double> vCaloNeutralID;
  std::vector<double> vPhotonID;
  std::vector<double> vIsPhoton;

  for (const auto& p : parts) {
    if (PID.size() == m_max) {
      Warning("Reached maximum size of array", StatusCode::SUCCESS).ignore();
      break;
    }
    PID.push_back(p->particleID().pid());
    TPID.push_back(TID(p));
    PX.push_back(p->momentum().x());
    PY.push_back(p->momentum().y());
    PZ.push_back(p->momentum().z());
    PE.push_back(p->momentum().T());
    const LHCb::ProtoParticle* pp = p->proto();
    if (pp) {
      vCaloTrMatch.push_back(pp->info(LHCb::ProtoParticle::CaloTrMatch, -1000));
      vCaloDepositID.push_back(
          pp->info(LHCb::ProtoParticle::CaloDepositID, -1000));
      vShowerShape.push_back(pp->info(LHCb::ProtoParticle::ShowerShape, -1000));
      vClusterMass.push_back(pp->info(LHCb::ProtoParticle::ClusterMass, -1000));
      vCaloNeutralSpd.push_back(
          pp->info(LHCb::ProtoParticle::CaloNeutralSpd, -1000));
      vCaloNeutralPrs.push_back(
          pp->info(LHCb::ProtoParticle::CaloNeutralPrs, -1000));
      vCaloNeutralEcal.push_back(
          pp->info(LHCb::ProtoParticle::CaloNeutralEcal, -1000));
      vCaloNeutralHcal2Ecal.push_back(
          pp->info(LHCb::ProtoParticle::CaloNeutralHcal2Ecal, -1000));
      vCaloNeutralE49.push_back(
          pp->info(LHCb::ProtoParticle::CaloNeutralE49, -1000));
      vCaloNeutralID.push_back(
          pp->info(LHCb::ProtoParticle::CaloNeutralID, -1000));
      vPhotonID.push_back(pp->info(LHCb::ProtoParticle::PhotonID, -1000));
      vIsPhoton.push_back(pp->info(LHCb::ProtoParticle::IsPhoton, -1000));
    }
  }
  unsigned int siz = PID.size();

  test &= (PE.empty() || PE.size() == siz);                // check
  test &= (vIsPhoton.empty() || vIsPhoton.size() == siz);  // check
  if (!test) {
    err() << "Inconsistent array sizes " << siz << " and " << PE.size()
          << " and " << vIsPhoton.size() << endmsg;
    return StatusCode(test);
  }

  test &= tup->farray(prefix + "AllPhotons_PID", PID, prefix + "nParts", m_max);
  test &=
      tup->farray(prefix + "AllPhotons_TPID", TPID, prefix + "nParts", m_max);
  test &= tup->farray(prefix + "AllPhotons_PX", PX, prefix + "nParts", m_max);
  test &= tup->farray(prefix + "AllPhotons_PY", PY, prefix + "nParts", m_max);
  test &= tup->farray(prefix + "AllPhotons_PZ", PZ, prefix + "nParts", m_max);
  test &= tup->farray(prefix + "AllPhotons_PE", PE, prefix + "nParts", m_max);

  test &= tup->farray(prefix + "AllPhotons_CaloTrMatch", vCaloTrMatch,
                      prefix + "nParts", m_max);
  test &= tup->farray(prefix + "AllPhotons_CaloDepositID", vCaloDepositID,
                      prefix + "nParts", m_max);
  test &= tup->farray(prefix + "AllPhotons_ShowerShape", vShowerShape,
                      prefix + "nParts", m_max);
  test &= tup->farray(prefix + "AllPhotons_ClusterMass", vClusterMass,
                      prefix + "nParts", m_max);
  test &= tup->farray(prefix + "AllPhotons_CaloNeutralSpd", vCaloNeutralSpd,
                      prefix + "nParts", m_max);
  test &= tup->farray(prefix + "AllPhotons_CaloNeutralPrs", vCaloNeutralPrs,
                      prefix + "nParts", m_max);
  test &= tup->farray(prefix + "AllPhotons_CaloNeutralEcal", vCaloNeutralEcal,
                      prefix + "nParts", m_max);
  test &= tup->farray(prefix + "AllPhotons_CaloNeutralHcal2Ecal",
                      vCaloNeutralHcal2Ecal, prefix + "nParts", m_max);
  test &= tup->farray(prefix + "AllPhotons_CaloNeutralE49", vCaloNeutralE49,
                      prefix + "nParts", m_max);
  test &= tup->farray(prefix + "AllPhotons_CaloNeutralID", vCaloNeutralID,
                      prefix + "nParts", m_max);
  test &= tup->farray(prefix + "AllPhotons_PhotonID", vPhotonID,
                      prefix + "nParts", m_max);
  test &= tup->farray(prefix + "AllPhotons_IsPhoton", vIsPhoton,
                      prefix + "nParts", m_max);

  return StatusCode(test);
}
//============================================================================
int TupleToolAllPhotons::TID(const LHCb::Particle* P) {
  Assert(!m_p2mcAssocs.empty(),
         "The DaVinci smart associator(s) have not been initialized!");
  const LHCb::MCParticle* mcp(NULL);
  if (P) {
    // assignedPid = P->particleID().pid();
    if (msgLevel(MSG::VERBOSE))
      verbose() << "Getting related MCP to " << P << endmsg;
    for (const auto& iMCAss : m_p2mcAssocs) {
      mcp = iMCAss->relatedMCP(P);
      if (mcp) break;
    }
    if (msgLevel(MSG::VERBOSE)) verbose() << "Got mcp " << mcp << endmsg;
  }
  // pointer is ready, prepare the values:
  if (mcp)
    return mcp->particleID().pid();
  else
    return 0;
}

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolAllPhotons )
