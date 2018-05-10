// $Id: TupleToolTrackIsolation.h,v 1.2 2010-01-26 15:48:57 rlambert Exp $
#ifndef TUPLETOOLTRACKISOLATION_H
#define TUPLETOOLTRACKISOLATION_H 1

// Include files
// from DaVinci, this is a specialized GaudiAlgorithm
#include "Kernel/IParticleTupleTool.h"
#include "DecayTreeTupleBase/TupleToolBase.h"

/** @class TupleToolTrackIsolation TupleToolTrackIsolation.h
 *
 * \brief Fill track isolation for DecayTreeTuple.
 *    Open up a cone around head, exclude all tracks
 *    that are in the decay descriptor
 *    (i.e. that belong to the decay you are looking for),
 *    build the variables with the remaining tracks.
 *
 * - head_cmult : Number of tracks inside cone.
 * - head_cp : Summed p inside cone
 * - head_cpt : Summed pt inside cone
 * - head_cpx : Summed px inside cone
 * - head_cpy : Summed py inside cone
 * - head_cpz : Summed pz inside cone
 *
 * If Verbose, or other flags are set:
 *
 * Asymmetry variables
 * - head_pasy : (head_P - head_cp)/(head_P + head_cp)
 * - head_ptasy : (head_PT - head_cpt)/(head_PT + head_cpt)
 * - head_pxasy : (head_Px - head_cpx)/(head_Px + head_cpx)
 * - head_pyasy : (head_Py - head_cpy)/(head_Py + head_cpy)
 * - head_pzasy : (head_Pz - head_cpz)/(head_Pz + head_cpz)
 * Delta angle variables
 * - head_DeltaEta : Difference in eta between summed tracks and head
 * - head_DeltaPhi : Difference in phi between summed tracks and head
 *
 *
 * Options:
 *
 * - MinConeAngle: Set the minimal deltaR of the cone (default = 0.5), in radians
 * - MaxConeAngle: Set the maximum deltaR of the cone (default = 1.0), in radians
 * - StepSize: Set the step of deltaR between two iterations (default = 0.1), in radians
 * - TrackType: Set the type of tracks which are considered inside the cone (default = 3)
 * - FillAsymmetry: Flag to fill the asymmetry variables (default = false)
 * - FillDeltaAngles: Flag to fill the delta angle variables (default = false)
 * \sa DecayTreeTuple
 *
 *  @author Michel De Cian
 *  @date   2009-08-04
 */


class TupleToolTrackIsolation : public TupleToolBase,
                                virtual public IParticleTupleTool
{

public:

  /// Standard constructor
  TupleToolTrackIsolation( const std::string& type,
                           const std::string& name,
                           const IInterface* parent );

  /// Loop over differnt conesizes and fill the variables into the tuple
  StatusCode fill( const LHCb::Particle*
                   , const LHCb::Particle*
                   , const std::string&
                   , Tuples::Tuple& ) override;

  virtual ~TupleToolTrackIsolation( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization

private:

  double m_minConeAngle;
  double m_maxConeAngle;
  double m_stepSize;
  int m_trackType;
  bool m_fillAsymmetry;
  bool m_fillDeltaAngles;

  std::vector<const LHCb::Particle*> m_decayParticles;

private:

  /// Save all particles in your decay descriptor in a vector
  void saveDecayParticles( const LHCb::Particle *top);

  /// Calculate properties of your remaining tracks inside the cone
  std::pair< std::vector<double>, int> ConeP( const LHCb::Particle *part,
                                              const LHCb::Tracks* tracks,
                                              const double rcut );

  /// Check if your track belongs to your decay or not
  bool isTrackInDecay(const LHCb::Track* track);

};

#endif // TUPLETOOLTRACKISOLATION_H
