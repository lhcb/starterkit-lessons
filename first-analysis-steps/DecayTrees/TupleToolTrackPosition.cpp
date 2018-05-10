// Include files


#include "Event/Particle.h"
//#include "Event/Track.h"
// kernel
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
//#include "GaudiKernel/PhysicalConstants.h"

// local
#include "TrackInterfaces/ITrackExtrapolator.h"
#include "TrackInterfaces/ITrackStateProvider.h"
#include "TupleToolTrackPosition.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolTrackPosition
//
// 2009-05-06 : Michel De Cian
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( TupleToolTrackPosition )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolTrackPosition::TupleToolTrackPosition( const std::string& type,
                                                  const std::string& name,
                                                  const IInterface* parent)
    : TupleToolBase ( type, name , parent ),
      m_Z(2500.),
      m_extrapolator(NULL),
      m_extrapolatorName("")
{
  declareInterface<IParticleTupleTool>(this);
  declareProperty( "Z", m_Z = 2500.,
                   "Which Z-position to propagate the track to, defaults to 2500, the position of the TT-stations");
  declareProperty( "Extrapolator", m_extrapolatorName = "TrackStateProvider",
                   "Which extrapolator to use");

}
//=============================================================================
// Destructor
//=============================================================================
TupleToolTrackPosition::~TupleToolTrackPosition() {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode TupleToolTrackPosition::initialize()
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  m_extrapolator=tool<ITrackStateProvider>(m_extrapolatorName,this);

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;

  return sc;
}

//=============================================================================
// Fill the tuple
//=============================================================================
StatusCode TupleToolTrackPosition::fill( const LHCb::Particle *, const LHCb::Particle *   part,
                                         const std::string &   head, Tuples::Tuple &   tuple )
{

  const std::string prefix=fullName(head);
  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Fill" << endmsg;

  bool test=true;

  double X=-999999;
  double Y=-999999;


  if( part && part->proto() && part->proto()->track() )
  {

    if ( msgLevel(MSG::VERBOSE) ) verbose() << "extrapolating track to Z="<< m_Z << endmsg;

    LHCb::State aState=LHCb::State();
    //const LHCb::Track aTrack= *(part->proto()->track());
    StatusCode sc=m_extrapolator->stateFromTrajectory(aState,*(part->proto()->track()),m_Z);
    if(sc.isSuccess())
    {
      X=aState.x();
      Y=aState.y();
    }

  }

  test &= tuple->column( prefix+"_X", X );
  test &= tuple->column( prefix+"_Y", Y );

  return StatusCode(test);
}
