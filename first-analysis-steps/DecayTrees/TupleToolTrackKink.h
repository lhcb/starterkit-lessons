#ifndef DecayTreeTupleTracking_TupleToolTrackKink_H
#define DecayTreeTupleTracking_TupleToolTrackKink_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface
#include "GaudiKernel/ToolHandle.h"
#include "TrackInterfaces/ITrackFitter.h"
/** @class TupleToolTrackKink TupleToolTrackKink.h
 *  \brief TupleTool to reconstruct and save information about kinks on given tracks
 *
 *  Two methods are provided:
 *   - KinkChi2
 *   - KinkFitChi2
 *  The following variables are stored:
 *  - _TRACK_kinkChi2, _TRACK_kinkChi2z
 *  - _TRACK_kinkFitChi2, _TRACK_kinkFitDeltaR, _TRACK_kinkFitDeltaRE, _TRACK_kinkFitDeltaRZ
 *
 *  @author Francesco Dettori
 *  @date   2013-23-02
 */

class TupleToolTrackKink : public TupleToolBase,
                           virtual public IParticleTupleTool
{

public:

  /// Standard constructor
  TupleToolTrackKink( const std::string& type,
                      const std::string& name,
                      const IInterface* parent);

  virtual ~TupleToolTrackKink(); ///< Destructor

   StatusCode initialize() override;

public:

  StatusCode fill( const LHCb::Particle*
                   , const LHCb::Particle*
                   , const std::string&
                   , Tuples::Tuple& ) override;

private:

  StatusCode kinkChi2( const LHCb::Track& track, double &chi2kink,
                       double &zkink) const;

  StatusCode fitKink(const LHCb::Track &track,
                     double &kink_deltaR, double &kink_deltaR_err,
                     double &kink_chi2,
                     double &kink_z);

private:

  typedef ROOT::Math::SMatrix<double, 3, 3> Matrix3x3;
  typedef ROOT::Math::SMatrix<double, 5, 3> Matrix5x3;
  typedef ROOT::Math::SMatrix<double, 3, 5> Matrix3x5;
  typedef ROOT::Math::SMatrix<double, 5, 5> Matrix5x5;

private:

  ToolHandle<ITrackFitter> m_trackfitter;

};

#endif // DecayTreeTupleTracking_TupleToolTrackKink_H
