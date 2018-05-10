#ifndef JBOREL_TUPLETOOLTRACKINFO_H
#define JBOREL_TUPLETOOLTRACKINFO_H 1

// Include files
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface

/** @class TupleToolTrackInfo TupleToolTrackInfo.h 
 *
 * \brief Fill track information for DecayTreeTuple
 *
 * - X_TRACK_CHI2NDOF : track chi2/ndof
 * - X_TRACK_TYPE : track type
 * - X_TRACK_PCHI2 : track Chi2 probability
 * - X_TRACK_GhostProb : Ghost probability (run NeuralNetTmva to fill it)
 * - X_TRACK_CloneDist : Only available for 2009 data
 *
 * If Verbose:
 *
 * - X_TRACK_CHI2 : track chi2
 * - X_TRACK_NDOF : track ndof
 * - X_TRACK_VeloCHI2NDOF : Track fit velo chi2/nDoF
 * - X_TRACK_TCHI2NDOF : Track fit T chi2/nDoF
 * - X_TRACK_VELO_UTID : hopefully unique double constructed from multiplying all Velo hit IDs
 * - X_TRACK_TT_UTID : hopefully unique double constructed from multiplying all TT hit IDs
 * - X_TRACK_IT_UTID : hopefully unique double constructed from multiplying all IT hit IDs
 * - X_TRACK_OT_UTID : hopefully unique double constructed from multiplying all OT hit IDs
 * - X_TRACK_VP_UTID : hopefully unique double constructed from multiplying all VP hit IDs
 * - X_TRACK_UT_UTID : hopefully unique double constructed from multiplying all UT hit IDs
 * - X_TRACK_FT_UTID : hopefully unique double constructed from multiplying all FT hit IDs
 * - X_TRACK_nVeloHits : Number of Velo hits on the track
 * - X_TRACK_nVeloRHits : Number of Velo R hits on the track
 * - X_TRACK_nVeloPhiHits : Number of Velo phi hits on the track
 * - X_TRACK_nVeloPileUpHits : Number of Velo pile-up hits on the track
 * - X_TRACK_nTTHits : Number of TT hits on the track
 * - X_TRACK_nITHits : Number of IT hits on the track
 * - X_TRACK_nOTHits : Number of OT hits on the track
 * - X_TRACK_nVPHits : Number of VP hits on the track
 * - X_TRACK_nUTHits : Number of UT hits on the track
 * - X_TRACK_nFTHits : Number of FT hits on the track
 * - X_TRACK_FirstMeasurementX: x position of state at 'FirstMeasurement'
 * - X_TRACK_FirstMeasurementY: y position of state at 'FirstMeasurement'
 * - X_TRACK_FirstMeasurementZ: z position of state at 'FirstMeasurement'
 * - X_TRACK_History: Algorithm which the track was made with
 * - X_TRACK_qOverp : q/p of state at 'FirstMeasurement'
 * - X_TRACK_Tx : x slope of state at 'FirstMeasurement'
 * - X_TRACK_Ty : y slope of state at 'FirstMeasurement'
 *
 * \sa DecayTreeTuple
 *
 *  @author Stephane Poss
 *  @date   2008-03-17
 */
class TupleToolTrackInfo : public TupleToolBase, virtual public IParticleTupleTool {
public:
  /// Standard constructor
  TupleToolTrackInfo( const std::string& type,
                      const std::string& name,
                      const IInterface* parent);

  virtual ~TupleToolTrackInfo(){}; ///< Destructor

  StatusCode fill( const LHCb::Particle*
                   , const LHCb::Particle*
                   , const std::string&
                   , Tuples::Tuple& ) override;
private:
  bool m_storeFirstMeasurement ; ///< If true stores first measurement
};

#endif
