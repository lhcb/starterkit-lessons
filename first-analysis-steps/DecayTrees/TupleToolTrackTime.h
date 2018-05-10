#ifndef DecayTreeTupleTracking_TupleToolTrackTime_H
#define DecayTreeTupleTracking_TupleToolTrackTime_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface
#include "GaudiKernel/ToolHandle.h"
#include "TrackInterfaces/ITrackFitter.h"
#include "LHCbMath/ValueWithError.h"

/** @class TupleToolTrackTime TupleToolTrackTime.h
 *  \brief TupleTool to calculate timing information on the track
 *
 *  @author Francesco Dettori
 *  @date   2016-26-02
 */

class TupleToolTrackTime : public TupleToolBase,
                           virtual public IParticleTupleTool
{

public:

  /// Standard constructor
  TupleToolTrackTime( const std::string& type,
                      const std::string& name,
                      const IInterface* parent);

  virtual ~TupleToolTrackTime(); ///< Destructor

   StatusCode initialize() override;

public:

  StatusCode fill( const LHCb::Particle*
                   , const LHCb::Particle*
                   , const std::string&
                   , Tuples::Tuple& ) override;

private:

  Gaudi::Math::ValueWithError computeTrackT0( const LHCb::Track& track );


private:

  ToolHandle<ITrackFitter> m_trackfitter;

};

#endif // DecayTreeTupleTracking_TupleToolTrackTime_H
