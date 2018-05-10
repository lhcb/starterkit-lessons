// $Id: TupleToolVeloTrackMatch.h
#ifndef TupleToolVeloTrackMatch_H
#define TupleToolVeloTrackMatch_H 1

#include "Kernel/IParticleTupleTool.h"
#include "DecayTreeTupleBase/TupleToolBase.h"

/** @class TupleToolVeloTrackMatch TupleToolVeloTrackMatch.h
* \brief Fill information about velo tracks matching the flight direction of a downstream composite candidate
* The tool searches for velo tracks at LHCb::TrackLocation::Velo around a given direction, re-running the Velo tracking beforehand may be necessary
*
* -The cone angle can be set using the property "ConeRadius", the default value is 0.05
*  This is an approximation of the true angle, it is sqrt( (slopedifference_X)^2 + (slopedifference_Y)^2  ),
*  Where slopedifference is the difference in slope between the velo track and the reference direction
*
* -The minimum distance of the VELO tracks from the best PV can be set using the property "MaxIPwrtBestPV", the default value is 0,5 mm
*
* -If ConeAroundMomentum=True:
*    The reference direction is set to be the 3-momentum of the downstream candidate
*    if False (default value) is the flight direction determined by the decay vertex and the associate PV
*
*  -If sxratio(syratio) is the ratio between the slope dx/dz(dy/dz) of the velo track wrt the reference direction
*  we define R
*  R=sqrt( (1-sxratio)^2 + (1-syratio)^2  )
*  the "best velo" track is the one with the smaller R
*
*    head_VM_cand_slope_X: dx/dz of reference direction
*    head_VM_cand_slope_Y: dy/dz of reference direction
*    head_VM_nvelotracks: number of velo tracks inside the cone
*    head_VM_R: R of best velo track
*    head_VM_IP: IP of best velo track wrt best PV
*    head_VM_CD: cone angle best velo track
*    head_VM_chi2: chi2 of velo track fit
*    head_VM_index: index of best velo track
*    head_VM_nMeas: number of measurements for the best velo track
*    head_VM_fpos_X: X position at first state for best velo track
*    head_VM_fpos_Y: Y position at first state for best velo track
*    head_VM_fpos_Z: Z position at first state for best velo track
*    head_VM_fpos_err_XX: XX error matrix of first state for best velo track
*    head_VM_fpos_err_YY: YY error matrix of first state for best velo track
*    head_VM_fpos_err_XY: ZZ error matrix of first state for best velo track
*    head_VM_sloperatio_X: slope ratio X of best velo track wrt reference direction
*    head_VM_sloperatio_Y: slope ratio Y of best velo track wrt reference direction
*    head_VM_slope_X: dx/dz slope of best velo track
*    head_VM_slope_Y: dy/dz slope of best velo track
*    head_VM_slope_err_XX: XX error matrix of the slope for best velo track
*    head_VM_slope_err_YY: YY error matrix of the slope for best velo track
*    head_VM_slope_err_XY: YY error matrix of the slope for best velo track
*
*  -If Verbose=True
*  Same meaning as amove but for all velo tracks in the cone
*    head_VM_array_fpos_X
*    head_VM_array_fpos_Y
*    head_VM_array_fpos_Z
*    head_VM_array_fpos_err_XX
*    head_VM_array_fpos_err_YY
*    head_VM_array_fpos_err_XY
*    head_VM_array_slope_X
*    head_VM_array_slope_Y
*    head_VM_array_slope_err_XX
*    head_VM_array_slope_err_YY
*    head_VM_array_slope_err_XY
*    head_VM_array_sloperatio_X
*    head_VM_array_sloperatio_Y
*    head_VM_array_R
*    head_VM_array_IP
*    head_VM_array_CD
*    head_VM_array_chi2
*    head_VM_array_nMeas
*
*  @author Andrea Contu
*  @date   2013-11-21
*/
class TupleToolVeloTrackMatch  : public TupleToolBase, public virtual IParticleTupleTool{

  public:

  /// Standard constructor
    TupleToolVeloTrackMatch(const std::string& type,
                            const std::string& name,
                            const IInterface* parent );

  /// Standard initialization
    StatusCode initialize() override;

    virtual ~TupleToolVeloTrackMatch( ); ///< Destructor

  public:

    StatusCode fill( const LHCb::Particle*, const LHCb::Particle*,
                    const std::string & head, Tuples::Tuple& tuple ) override;

  private:



  private:
    IDVAlgorithm* m_dva;
    double m_slopediff;
    bool m_momentumcone;
    bool m_verbose;
    double m_maxVbestPVdist;
};
#endif // TupleToolVeloTrackMatch_H

