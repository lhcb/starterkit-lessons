// Include files

// from Gaudi
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
// local
#include "TupleToolTrackTime.h"
#include "Event/Track.h"
#include "Event/FitNode.h"
#include "Event/TrackFitResult.h"
#include "Event/ChiSquare.h"
#include "TrackKernel/TrackTraj.h"
#include "Event/KalmanFitResult.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "Event/Track.h"
#include "Event/OTMeasurement.h"
#include "Event/FitNode.h"
#include "LHCbMath/ValueWithError.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolTrackTime
//
// 2016-02-26 : Francesco Dettori
//              francesco.dettori@cern.ch
//
// Based on code from W. Hulsbergen
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolTrackTime )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolTrackTime::TupleToolTrackTime( const std::string& type,
                                        const std::string& name,
                                        const IInterface* parent )
: TupleToolBase ( type, name , parent ),
  m_trackfitter("TrackMasterFitter",this)
{
  declareInterface<IParticleTupleTool>(this);
  declareProperty( "TrackFitter", m_trackfitter ) ;
}

//=============================================================================
// Destructor
//=============================================================================
TupleToolTrackTime::~TupleToolTrackTime() {}

StatusCode TupleToolTrackTime::initialize()
{
  StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  sc = m_trackfitter.retrieve() ;

  return sc;
}

//=============================================================================
StatusCode TupleToolTrackTime::fill( const LHCb::Particle*,
                                     const LHCb::Particle* P,
                                     const std::string& head,
                                     Tuples::Tuple& tuple )
{
  if (msgLevel(MSG::VERBOSE)) verbose() << " filling particle " << endmsg;
  const std::string prefix = fullName(head);
  bool test = true;

  if ( !P ) return StatusCode::FAILURE;

  //first just return if the particle isn't supposed to have a track
  if ( !P->isBasicParticle() ) return StatusCode::SUCCESS;

  const LHCb::ProtoParticle* protop = P->proto();
  if (!protop){
    if (msgLevel(MSG::DEBUG))debug() << "Protoparticle not found for this particle" << endmsg;
    return StatusCode::SUCCESS;
  }

  const LHCb::Track* track = protop->track();
  if (!track){
    if (msgLevel(MSG::DEBUG))debug() << "Track not found for this particle" << endmsg;
    return StatusCode::SUCCESS;
  }

  if (msgLevel(MSG::DEBUG))
    debug() << prefix << " " << track->type()
            << " "+prefix+"_TRACK_CHI2 " << track->chi2() << endmsg ;
  if (msgLevel(MSG::VERBOSE)) verbose() << *track << endmsg ;


  Gaudi::Math::ValueWithError track_t0(0,0);
  try
  {
    const LHCb::Track *clone  = track->cloneWithKey();

    StatusCode sc = m_trackfitter->operator()( const_cast<LHCb::Track&>(*clone) ) ;
    if (sc.isSuccess())
    {
      track_t0 = computeTrackT0(*clone);
      if (msgLevel(MSG::DEBUG)) debug() << " Obtained t0: " << track_t0 << endmsg;
    }else{
      Warning( "Unable to refit track: track time will have default value 0" ).ignore();
    }
    delete clone;

  }catch(const GaudiException & )
  {
    Warning( "There was a problem accessing data for track refit" ).ignore();
  }


  // First method
  test &= tuple->column( prefix+"_TRACK_time",  track_t0.value() );
  test &= tuple->column( prefix+"_TRACK_time_err", track_t0.error() );


  return StatusCode::SUCCESS;
}



Gaudi::Math::ValueWithError TupleToolTrackTime::computeTrackT0( const LHCb::Track& track )
{

  if (msgLevel(MSG::VERBOSE)) verbose() << " Computing track t0 " << endmsg;


  double sumwtimeres(0) ;
  double sumw(0) ;


  if (msgLevel(MSG::VERBOSE)) verbose() << " track nodes " << track.nodes().size() << endmsg;

  // Check if fit result is available (this should always be the case)
  const LHCb::TrackFitResult* fitresult = track.fitResult() ;
  Gaudi::Math::ValueWithError rc(0,0) ;

  if ( !fitresult ) return rc;

  // Get nodes from track
  const LHCb::TrackFitResult::NodeContainer& nodes = fitresult->nodes() ;

  // iterate over all track nodes
  for( const LHCb::Node* node: nodes ) {
    if ( !node )
    {
      Warning( "Nodes not found, lets skip this entry" ).ignore();
      continue;
    }
    if( ! node->hasMeasurement() ) continue;

    if (msgLevel(MSG::VERBOSE)) verbose() << " Node type " <<  node->type()
                                          << "  node measurement type " << node->measurement().type() << endmsg;

    if( node->type() == LHCb::Node::HitOnTrack &&
        node->measurement().type() == LHCb::Measurement::OT ) {

      // get fit node
      const LHCb::FitNode* fitnode = dynamic_cast<const LHCb::FitNode*>(node);
      const LHCb::OTMeasurement* measurement = dynamic_cast<const LHCb::OTMeasurement*>(&node->measurement());
      bool drifttimeWasUsed =
        measurement->driftTimeStrategy() == LHCb::OTMeasurement::FitTime ||
        measurement->driftTimeStrategy() == LHCb::OTMeasurement::FitDistance ;
      if( drifttimeWasUsed ) {

        double timeresidual(0) ;
        double timeresidualV(0) ;

        if( measurement->driftTimeStrategy() == LHCb::OTMeasurement::FitDistance ) {
          // compute the residual in the drifttime
          // Get the rt relation
          const OTDet::RtRelation& rtr = measurement->module().rtRelation() ;

          // Get distance from wire
          double radius = std::min( (double)rtr.rmax(), std::abs(fitnode->doca()) ) ;

          // Get expected drifttime
          OTDet::DriftTimeWithError predictedtime = rtr.drifttimeWithError( radius ) ;

          // get dt/dr
          double dtdr                             = rtr.dtdr( radius ) ;

          // Get measure drit time
          double measureddrifttime = measurement->driftTimeFromY( fitnode->state().y() ) ;

          // Compute residual difference
          timeresidual  = measureddrifttime - predictedtime.val ;

          // this is the easiest. it would be better to first compute the error on the radius, then ...
          timeresidualV = fitnode->errResidual2() * dtdr * dtdr ;
        } else {

          timeresidual =  fitnode->residual() ;
          timeresidualV = fitnode->errResidual2() ;
        }
        // Compute sum of weights and sum of weighted times
        if( timeresidualV > 0 ) {
          double weight = 1/timeresidualV ;
          sumwtimeres += weight * timeresidual ;
          sumw += weight ;
        }
      }
    }
  }


  // Return weighted average
  if( sumw > 0 )
    rc = Gaudi::Math::ValueWithError( sumwtimeres/sumw, 1/sumw ) ;
  if (msgLevel(MSG::DEBUG))debug() << "Returning time " << rc << endmsg;

  return rc ;
}
