#ifndef TUPLETOOLCONEISOLATION_H
#define TUPLETOOLCONEISOLATION_H 1

// Include files
#include "Kernel/IParticleTupleTool.h"
#include "DecayTreeTupleBase/TupleToolBase.h"

/** @class TupleToolConeIsolation TupleToolConeIsolation.h
 *
 * \brief Fill track isolation for DecayTreeTuple.
 *    Open an eta-phi cone around the head, exclude all tracks that are in the decay descriptor (i.e. that belong to the decay you are looking for), and build variables with the remaining tracks
 *
 * Options:
 *
 * - MinConeSize: set the minimum cone size (default = 0.4)
 * - MaxConeSize: set the maximum cone size (default = 0.6)
 * - SizeStep: set the cone size step between two iterations (default = 0.1)
 * - FillCharged: flag to fill the charged cone (default = true)
 * - ExtraParticlesLocation: set the type of particles that are considered in the charged cone (default = "Phys/StdAllNoPIDsMuons/Particles")
 * - TrackType: set the type of tracks that are considered in the charged cone (default = 3)
 * - FillNeutral: flag to fill the neutral cone (default = true)
 * - ExtraPhotonsLocation: set the type of particles that are considered in the neutral cone (default = "Phys/StdLooseAllPhotons/Particles")
 * - FillAsymmetry: flag to fill the asymmetry variables (default = false)
 * - FillDeltas: flag to fill the delta variables (default = false)
 * - FillIsolation: flag to fill the isolation variables (default = true)
 * - FillMaxPt: flag to fill the momentum of the max-pT object in the cone (default = true)
 * - MaxPtParticleLocation: set the type of max-pT particles that are considered in the charged cone (default = "Phys/StdAllLooseMuons/Particles")
 * - FillComponents: flag to fill the components of all variables (default = false)
 * - FillPi0Info: flag to fill the resolved pi0 info (default = false)
 * - FillMergedPi0Info: flag to fill the merged pi0 info (default = false)
 * - PizerosLocation: set the resolved pi0 type considered for combination with the seed (default = Phys/StdLoosePi02gg/Particles)
 * - MergedPizerosLocation: set the merged pi0 type considered for combination with the seed (default = Phys/StdLooseMergedPi0/Particles)
 * - MinhPi0Mass: Set the lower seed-Pi0 invariant mass cut (default = 0 MeV)
 * - MaxhPi0Mass: Set the upper seed-Pi0 invariant mass cut (default = 5000 MeV)
 *
 * If Verbose, all options are set to true
 *
 * Cone prefix:
 *
 * - head_cc: charged cone
 * - head_nc: neutral cone
 *
 * The pi0 variables are not dependent on the cone size
 *
 * Variables (XX = cc or nc):
 *
 * - head_XX_mult : number of objects inside the cone
 * - head_XX_sPT : scalar-summed pT of the objects inside the cone
 * - head_XX_vPT : vector-summed pT of the objects inside the cone
 * - head_XX_P : x, y and z components of the cone momentum
 *
 * - head_XX_asy_P : momentum asymmetry between the head and the cone defined as (head_P - head_XX_P) / (head_P + head_XX_P)
 * - head_XX_asy_P : x, y, z and transverse components of the momentum asymmetry
 *
 * - head_XX_deltaEta : difference in eta between the head and the cone
 * - head_XX_deltaPhi : difference in phi between the head and the cone
 *
 * - head_XX_IT : transverse isolation of the head in the cone, defined as head_PT / (head_P + head_XX_P)_T
 * - head_IT : transverse isolation of the head in the charged and neutral cones, defined as head_PT / (head_P + head_cc_P + head_nc_P)_T
 *
 * - head_cc_maxPt_Q : charge of the max-pT object in the charged cone
 * - head_XX_maxPt_P : x, y, z (and e) components of the max-pT object momentum in the cone
 * - head_MasshPi0: invariant mass of the seed-Pi0 combinations
 * - head_Pi0_DeltaR: DeltaR between the seed and the pi0 directions
 * - head_Pi0_E, head_Pi0_PX, head_Pi0_PY, head_Pi0_PZ: four momentum of the pi0
 * - head_Pi0_M: invariant mass of the pi0
 * - head_Pi0Ph1_CL, head_Pi0Ph2_CL: confidence levels of the (photon) pi0 daughters
 * - For the merged pi0, the names of the variables are labelled replacing Pi0 by MergPi0
 *
 * \sa DecayTreeTuple, TupleToolTrackIsolation
 *
 *  @author Simone Bifani, Michel De Cian, Adlene Hicheur
 *  @date   2016-03-10
 *
 */


class TupleToolConeIsolation : public TupleToolBase, virtual public IParticleTupleTool {

public:

  /// Standard constructor
  TupleToolConeIsolation( const std::string &type,
			  const std::string &name,
			  const IInterface *parent );

  virtual ~TupleToolConeIsolation(); ///< Destructor

  StatusCode initialize() override; ///< Algorithm initialization

  /// Loop over different cone sizes and fill the variables into the tuple
  StatusCode fill( const LHCb::Particle*,
                   const LHCb::Particle*,
                   const std::string&,
                   Tuples::Tuple& ) override;

protected:

private:

  double m_minConeSize;
  double m_maxConeSize;
  double m_sizeStep;
  double m_MinhPi0Mass;
  double m_MaxhPi0Mass;

  std::string  m_extraParticlesLocation;
  std::string  m_maxPtParticleLocation;
  std::string  m_extraPhotonsLocation;
  std::string  m_PizerosLocation;
  std::string  m_MergedPizerosLocation;

  int m_trackType;

  bool m_fillCharged;
  bool m_fillNeutral;
  bool m_fillPi0Info;
  bool m_fillMergedPi0Info;
  bool m_fillAsymmetry;
  bool m_fillDeltas;
  bool m_fillIsolation;
  bool m_fillMaxPt;

  bool m_fillComponents;

  std::vector<const LHCb::Particle*> m_decayParticles;

  /// Save all the particles in the decay descriptor in a vector
  void saveDecayParticles( const LHCb::Particle *top );

  /// Calculate properties of the charged cone
  StatusCode ChargedCone( const LHCb::Particle  *seed,
			  const LHCb::Particles *parts,
			  const LHCb::Particles *maxPts,
			  const double rcut,
			  int &multiplicity,
			  double &scalarPt,
			  std::vector<double> &momentum,
			  std::vector<double> &maxPt );

  /// Calculate properties of the neutral cone
  StatusCode NeutralCone( const LHCb::Particle  *seed,
			  const LHCb::Particles *photons,
			  const double rcut,
			  int &multiplicity,
			  double &scalarPt,
			  std::vector<double> &momentum,
			  std::vector<double> &maxPt,
			  double &AvConfLevel,
			  double &MaxConfLevel,
                          double &LR1,
                          double &LR2,
                          double &LR3,
                          double &LR4);

  /// Check if a track belongs to the decay
  bool isTrackInDecay( const LHCb::Track *track );

  //fill pi0-track mass info
  StatusCode pi0Comb( const LHCb::Particle  *seed,
		      const LHCb::Particles *pi0s,
                      int &multiplicity,
                      std::vector<double> &InvMass,
		      std::vector<double> &Pi0PX,
		      std::vector<double> &Pi0PY,
		      std::vector<double> &Pi0PZ,
                      std::vector<double> &Pi0E,
                      std::vector<double> &Pi0M,
                      std::vector<double> &Pi0DeltaR,
		      std::vector<double> &Pi0Photon1CL,
		      std::vector<double> &Pi0Photon2CL);

};

#endif // TUPLETOOLCONEISOLATION_H
