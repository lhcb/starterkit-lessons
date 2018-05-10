// Include files

// from Gaudi
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
// local
#include "TupleToolTrackKink.h"
#include "Event/Track.h"
#include "Event/FitNode.h"
#include "Event/TrackFitResult.h"
#include "Event/ChiSquare.h"
#include "TrackKernel/TrackTraj.h"
#include "Event/KalmanFitResult.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolTrackKink
//
// 2013-02-23 : Francesco Dettori
//              francesco.dettori@cern.ch
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolTrackKink )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolTrackKink::TupleToolTrackKink( const std::string& type,
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
TupleToolTrackKink::~TupleToolTrackKink() {}

StatusCode TupleToolTrackKink::initialize()
{
  StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  sc = m_trackfitter.retrieve() ;

  return sc;
}

//=============================================================================
StatusCode TupleToolTrackKink::fill( const LHCb::Particle*,
                                     const LHCb::Particle* P,
                                     const std::string& head,
                                     Tuples::Tuple& tuple )
{
  const std::string prefix = fullName(head);
  bool test = true;

  if ( !P ) return StatusCode::FAILURE;

  //first just return if the particle isn't supposed to have a track
  if ( !P->isBasicParticle() ) return StatusCode::SUCCESS;

  const LHCb::ProtoParticle* protop = P->proto();
  if (!protop) return StatusCode::SUCCESS;

  const LHCb::Track* track = protop->track();
  if (!track) return StatusCode::SUCCESS;

  if (msgLevel(MSG::DEBUG))
    debug() << prefix << " " << track->type()
            << " "+prefix+"_TRACK_CHI2 " << track->chi2() << endmsg ;
  if (msgLevel(MSG::VERBOSE)) verbose() << *track << endmsg ;

  if (msgLevel(MSG::DEBUG))
    debug() << "Going to refit track " << endmsg;

  double kinkchi2(-999.), zkink(-999.);
  StatusCode sc = m_trackfitter->operator()( const_cast<LHCb::Track&>(*track) ) ;
  if (sc.isSuccess())
  {
    sc = kinkChi2(*track, kinkchi2, zkink);
  }else{
    Warning( "Unable to refit track: kinkChi2 will have default value -999" ).ignore();
  }

  // First method
  test &= tuple->column( prefix+"_TRACK_kinkChi2",  kinkchi2 );
  test &= tuple->column( prefix+"_TRACK_kinkChi2z", zkink );

  // second method
  double kink_deltaR(-999), kink_deltaR_err(-999), kink_deltaR_z(-999), kink_chi2(-999);
  sc = fitKink(*track,kink_deltaR, kink_deltaR_err, kink_chi2, kink_deltaR_z);

  test &= tuple->column( prefix+"_TRACK_kinkFitChi2", kink_chi2);
  test &= tuple->column( prefix+"_TRACK_kinkFitDeltaR",  kink_deltaR);
  test &= tuple->column( prefix+"_TRACK_kinkFitDeltaRE", kink_deltaR_err);
  test &= tuple->column( prefix+"_TRACK_kinkFitDeltaRZ", kink_deltaR_z);

  return StatusCode::SUCCESS;
}

//=============================================================================
StatusCode TupleToolTrackKink::kinkChi2( const LHCb::Track& track,
                                         double &chi2kink,
                                         double &zkink) const
{
  chi2kink = 0;
  zkink    = 0;

  // Get Track fit result
  const LHCb::TrackFitResult* fitresult = track.fitResult() ;

  if ( fitresult )
  {

    const LHCb::TrackFitResult::NodeContainer& nodes = fitresult->nodes() ;

    // Loop on nodes and get the ones with measurement
    std::vector< const LHCb::FitNode* > measnodes ;
    measnodes.reserve( nodes.size()) ;
    BOOST_FOREACH(  const LHCb::Node* node, nodes )
    {
      if ( !node )
      {
        return Warning( "Nodes not found, lets skip this entry" );

      }
      if( node->hasMeasurement() )
        measnodes.push_back( static_cast<const LHCb::FitNode*>(node) ) ;
    }

    int N = measnodes.size() ;
    if ( N<1 )
    {
      return Warning( "No measurement nodes found" );
    }

    // Get chi2 in the forward direction up to each node
    std::vector<double> chi2forward(N,0) ;
    chi2forward[0] = (measnodes[0] ?   measnodes[0]->deltaChi2Forward().chi2() :0.) ;
    for( int i=1; i<N; ++i)
    {
      chi2forward[i] = chi2forward[i-1] + ( measnodes[i] ? measnodes[i]->deltaChi2Forward().chi2() : 0.0 );
    }

    // Get chi2 in the backward direction up to each node
    std::vector<double> chi2backward(N,0) ;
    chi2backward[N-1] = (measnodes[N-1] ? measnodes[N-1]->deltaChi2Backward().chi2() : 0.) ;
    for( int i=N-2; i>=0; --i)
    {
      chi2backward[i]  = (measnodes[i] ? chi2backward[i+1] + measnodes[i]->deltaChi2Backward().chi2() :0.) ;
    }

    const double chi2tot = std::max( chi2forward[N-1], chi2backward[0] ) ;

    // find the kink as the pont with largest chi2 variation
    const int minkinkstep=4 ;
    const size_t ndof = 5 ;
    int ikink(0) ;
    for( int i=minkinkstep-1; i<N-minkinkstep+1; ++i )
    {
      const double thischi2kink = (chi2tot - chi2forward[i] - chi2backward[i+1]) / ndof ;
      //info() << " kinkchi2: thischi2kink " << thischi2kink << endmsg;
      if( thischi2kink > chi2kink )
      {
        ikink = i ;
        chi2kink = thischi2kink ;
      }
    }
    // Get z position of the kink
    zkink = 0.5 * ( measnodes[ikink]->z() + measnodes[ikink+1]->z() ) ;

  }
  else
  {
    Warning( "No fit result found", StatusCode::SUCCESS ).ignore();
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
StatusCode TupleToolTrackKink::fitKink( const LHCb::Track &input_track,
                                        double &kink_deltaR, double &kink_deltaR_err,
                                        double &kink_chi2, double &kink_z )
{

  // Scan in z or in nodes
  const LHCb::TrackFitResult* fitresult = input_track.fitResult();
  bool upstream = (input_track.fitResult()->nodes().front()->z() >
                   input_track.fitResult()->nodes().back()->z()) ;

  double max_chi2(-1);
  if( fitresult )
  {
    if (msgLevel(MSG::VERBOSE))
      verbose() << "fitKink: got fit result " << endmsg;

    const LHCb::TrackFitResult::NodeContainer& nodes = fitresult->nodes() ;
    //    const std::vector<LHCb::Measurement*>& measures = fitresult->measurements();

    std::vector< const LHCb::FitNode* > measnodes ;
    measnodes.reserve( nodes.size()) ;
    // Get only measnodes
    BOOST_FOREACH( const LHCb::Node* node, nodes )
    {
      if ( !node )
      {
        return Warning( "Nodes not found, lets skip this entry" );
      }
      if( node->hasMeasurement() )
      {
        measnodes.push_back( static_cast<const LHCb::FitNode*>(node) ) ;
      }
    }

    // Loop on measnodes and get states
    BOOST_FOREACH(  const LHCb::FitNode* node, measnodes )
    {
      const LHCb::State& stateUp   = upstream ? node->filteredStateForward()  : node->predictedStateForward() ;
      const LHCb::State& stateDown = upstream ? node->predictedStateBackward() : node->filteredStateBackward() ;

      // Get vector of difference between the two states ant its covariance
      Gaudi::TrackVector delta_zero  = stateUp.stateVector() - stateDown.stateVector() ;
      Gaudi::TrackSymMatrix cov = stateUp.covariance() + stateDown.covariance() ;
      if ( delta_zero(0) == 0 ) continue;

      // Set up matrix of constraints
      Matrix3x5 Dconstr;
      Dconstr(0,0) = Dconstr(1,1) = Dconstr(2,4) = 1;
      Matrix5x3 DconstrT;
      DconstrT(0,0) = DconstrT(1,1) = DconstrT(4,2) = 1; // Faster than Transpose

      //      Matrix3x5 tmp_m = Dconstr * cov;
      Matrix3x3 DVD =  Dconstr * cov * DconstrT;
      //        3x3 =  3x5     * 5x5 * 5x3
      Matrix3x3 DVDinv = DVD;

      if (msgLevel(MSG::VERBOSE))
      {
        verbose() << "covariance " << std::endl
                  << cov << endmsg;
        verbose() << "DMATRIX" << std::endl
                  << Dconstr << std::endl
                  << DconstrT << endmsg;
        verbose() << "DVD " << std::endl
                  << DVD << endmsg;
      }

      if ( !DVDinv.Invert() )
      {
        return Warning( "No inverse matrix, no kink fit chi2" );
      }
      if (msgLevel(MSG::DEBUG))
        debug() << "DVD inverted " << std::endl
                << DVDinv << endmsg;

      Matrix5x5 H = cov * DconstrT * DVDinv * Dconstr;
      //      5x5 = 5x5 *   5x3    *  3x3   * 3*5

      Gaudi::Vector3 lambda = DVDinv * Dconstr *delta_zero;
      //                  3 = 3x3    *    3x5  *      5

      Gaudi::TrackVector delta = delta_zero - H   * delta_zero;
      //                  5    =     5      - 5x5 * 5

      Gaudi::Vector3 lambda_mod =  DVD * lambda ;
      //                   3    =   3x3   *3

      if (msgLevel(MSG::DEBUG))
      {
        debug() << "  delta was " << delta_zero  << " now it is " << delta << endmsg;
        debug() << "  lambda is  " << lambda  << endmsg;
      }

      //       // Then we get the chi2
      double chi2 = ROOT::Math::Dot(lambda ,lambda_mod);
      if (msgLevel(MSG::DEBUG))
        debug() << " Chi2 is " << chi2 << endmsg;

      Matrix5x5 cov_2 = cov - H   * cov;
      //         5x5  = 5x5 - 5x5 * 5x5

      // DeltaR = sqrt [( dy/dz - (dy/dz)' )^2 + ( dx/dz - (dx/dz)' )^2 ]
      double deltaR =  std::sqrt(delta(2)*delta(2) + delta(3)*delta(3));
      double deltaR_err =  std::sqrt(std::pow(delta(2),2)*cov_2(2,2) + std::pow(delta(3),2)*cov_2(3,3)+
                                     delta(2)*delta(3)*cov_2(2,3)) / deltaR;


      // Get largest chi2 variation and its correspondent parameters
      if(chi2 > max_chi2)
      {
        max_chi2 = chi2;
        kink_deltaR = deltaR;
        kink_deltaR_err = deltaR_err;
        kink_z = (upstream ? stateUp.z() : stateDown.z());
      }

      if (msgLevel(MSG::DEBUG))
        debug() << "DeltaR is " << deltaR << " +- " << deltaR_err
                << " chi2 is " << chi2 <<   "     max_chi2 "<< max_chi2 << endmsg;
    }
  }
  else
  {
    return Warning( "Fit result not found, skipping kinkFit calculation" );
  }
  kink_chi2 = max_chi2;

  return StatusCode::SUCCESS;
}
