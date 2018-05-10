#ifndef TUPLETOOLISOLATIONTWOBODY_H
#define TUPLETOOLISOLATIONTWOBODY_H 1

#include "Kernel/IDistanceCalculator.h"
#include "Kernel/IParticleTupleTool.h"
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IRelatedPVFinder.h"
#include "Event/RecVertex.h"

#include "MuonDet/MuonBasicGeometry.h"
#include "MuonDet/DeMuonDetector.h"

class IParticle2MCAssociator;
struct IParticleCombiner;
struct IDVAlgorithm;


//#include "IIsolation.h"            // Interface

//using namespace LHCb;

/** @class TupleToolIsolationTwoBody TupleToolIsolationTwoBody.h
 *  Implementation for a "Particle Isolation" tool for particles from
 *  twobody decays.
 *  It return the degree of isolation of two particles with a common mother.
 *
 *  Currently it returns the numbers of non-isolating particles associated
 *  with each of the input particles.
 *
 *  @author Fatima Soomro, based on the code by Giampiero Mancinelli giampi@cppm.in2p3.fr in DaVinciTools
 *  @date   2012-03-17
 */
class TupleToolIsolationTwoBody  : public TupleToolBase,
                                   public virtual IParticleTupleTool
{

public:

  /// Standard constructor
  TupleToolIsolationTwoBody(const std::string& type,
                            const std::string& name,
                            const IInterface* parent );

  /// Standard initialization
  StatusCode initialize() override;

  virtual ~TupleToolIsolationTwoBody( ); ///< Destructor

public:

  StatusCode fill( const LHCb::Particle*, const LHCb::Particle*,
                   const std::string & head, Tuples::Tuple& tuple ) override;

private:

  /** The method for the evaluation of the degree of isolation ("number
   *  of non-isolating tracks") of the input set of particles.
   *
   *  @code
   *
   *   // get the tool
   *  const IParticleIsolation* tool = ...;
   *  Particle::ConstVector vdaugh = ... ;
   *  int iso[2] = ... ;
   *  --> for new (#5) definition (see LHCb-INT-2010-011)
   *  --> other criteria defined in the same note are not yet implemented
   *  float massiso[2]     =  invariant mass of non-isolating particles
   *  float chi2iso[2]     = chi2 of particle made with non-isolating particles
   *  const RecVertex* PV = ...;
   *  const RecVertex* SV = ...;
   *
   *   // use the tool to evaluate the degrees of isolation
   *  StatusCode sc = tool ->getIso(vdaugh,iso,massiso,chi2iso,PV,SV);
   *
   *  @endcode
   *
   *  @param vdaugh   (input) pointer to the input set particles (2 particles)
   *  @param iso      (output) number(s) of non-isolating particles (2 isolations)
   *  @param massiso      (output) invariant mass of non-isolating particles
   *  @param chi2iso      (output) chi2 of particle made with non-isolating particles
   *  @param PV       (input) (optional) primary vertex
   *  @param SV       (input) (optional) secondary vertex (of mother)
   *
   *  @return status code
   */
  /*   virtual StatusCode getIso(LHCb::Particle::ConstVector parts,  */
  /*                             int* iso, float* massiso, float* chi2iso, */
  /*                             const LHCb::VertexBase* PV,  */
  /*                             const LHCb::VertexBase* SV) const ; */

  virtual StatusCode getIso(const LHCb::Particle*, std::string, Tuples::Tuple& );

  StatusCode MuChi2( const LHCb::Particle* P, const std::string& head, Tuples::Tuple& tuple);

  /** Defines whether or not a particle is not isolating according to the
      BsMuMu Roadmap, using an Hlt criterium
  */
  void IsHltGood(Gaudi::XYZPoint o,
                 Gaudi::XYZVector p,
                 Gaudi::XYZPoint o_mu,
                 Gaudi::XYZVector
                 p_mu, Gaudi::XYZPoint PV, int&
                 hltgood, double& fc) const;
  /** given two tracks returns the closest point, the doca and the angle
      between the tracks
  */
  void InCone(Gaudi::XYZPoint o1,
              Gaudi::XYZVector p1,
              Gaudi::XYZPoint o2,
              Gaudi::XYZVector p2,
              Gaudi::XYZPoint& vtx, double&
              doca, double& angle) const;
  /** given two particles and the vector between the PV and vertex defined
   *  by them, it returns :
   *   fc=\frac{\left|\vec{P_{\mu}}+\vec{P_{tr}}\right|\cdot\alpha^{\mu+tr,PV}}
   *   {\left|\vec{P_{\mu}}+\vec{P_{tr}}\right|\cdot\alpha^{\mu+tr,PV}+P_{T\mu}
   *   +P_{Ttr}}
   *  where $P_{T\mu}$ and  $P_{Ttr}$ are the transverse momenta (with respect
   *  to the beam line) of the two tracks.
   */
  double pointer (Gaudi::XYZVector vtx,
                  Gaudi::XYZVector P_tr,
                  Gaudi::XYZVector P_mu) const;

  /// return the arcosine between two vectors
  double arcosine(Gaudi::XYZVector p1,
                  Gaudi::XYZVector p2) const;

  /** given two vectors and two application points finds for the two relevant
   *  straight lines the closest points along the lines and the middle one
   *  between the such points
   */
  void closest_point(Gaudi::XYZPoint o,
                     Gaudi::XYZVector p,
                     Gaudi::XYZPoint o_mu,
                     Gaudi::XYZVector
                     p_mu, Gaudi::XYZPoint& close1,
                     Gaudi::XYZPoint& close2,
                     Gaudi::XYZPoint& vertex, bool&
                     fail) const;

  const LHCb::MCParticle* ancestor(const LHCb::MCParticle*) const;

private:

  const IDistanceCalculator* m_Geom;
  std::string m_ParticlePath;

  bool m_MuChi2;
  /// cut on opening angle between tracks
  double m_angle;
  /// cut on pointing (hlt) (see LHCb-INT-2010-011)
  double m_fc;
  /// cut on distance of closest approach of tracks
  double m_doca;
  /// cut on particles minimum impact parameter with respect to PV
  double m_ips;
  /// cut on distance (low) between two track vertex and and the mother SV
  double m_svdis;
  /// cut on distance (high) between two track vertex and and the mother SV
  double m_svdis_h;
  /// cut on distance (low) between two track vertex and and the mother PV
  double m_pvdis;
  /// cut on distance (high) between two track vertex and and the mother PV
  double m_pvdis_h;
  /// cut on ghost probability
  double m_ghost;
  /// cut on clone probability
  double m_clone;
  /// cut on trk chi2
  double m_trchi2;
  /// cut on hltgood
  double m_hltgood;
  int m_tracktype;
  std::string  m_PVInputLocation;
  double ratio( double p1, double p2) const;

  //  bool m_tuple;
  bool m_isMC;
  IParticle2MCAssociator* m_p2mcAssoc;
  const IParticleCombiner* m_combiner;
  IDVAlgorithm* m_dva;

  std::string m_p2mcAssocType;
};
#endif // TUPLETOOLISOLATIONTWOBODY_H
