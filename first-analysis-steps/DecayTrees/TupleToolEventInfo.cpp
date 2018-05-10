// Include files

#include "Kernel/ILHCbMagnetSvc.h"

// local
#include "TupleToolEventInfo.h"

#include "Event/ODIN.h" // event & run number
#include "Event/L0DUReport.h"
#include "Event/HltDecReports.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/ITupleTool.h"
#include "Event/RecVertex.h"
#include "Event/VertexBase.h"
#include "Event/Track.h"

#include "GaudiKernel/IRegistry.h" // IOpaqueAddress

//-----------------------------------------------------------------------------
// Implementation file for class : EventInfoTupleTool
//
// 2007-11-07 : Jeremie Borel
//-----------------------------------------------------------------------------

using namespace Gaudi;
using namespace LHCb;

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolEventInfo )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolEventInfo::TupleToolEventInfo( const std::string& type,
                                          const std::string& name,
                                          const IInterface* parent )
    : TupleToolBase ( type, name , parent )
{
  declareInterface<IEventTupleTool>(this);
  declareProperty("InputLocation",
                  m_pvLocation = LHCb::RecVertexLocation::Primary,
                  "PV location to be used. If empty, take default");
  declareProperty("Mu", m_mu);
}

//=============================================================================

StatusCode TupleToolEventInfo::initialize( )
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;
  m_magSvc = svc<ILHCbMagnetSvc>( "MagneticFieldSvc", true );
  return sc;
}

//=============================================================================
StatusCode TupleToolEventInfo::fill( Tuples::Tuple& tuple )
{
  const std::string prefix = fullName();

  // Load the ODIN
  const LHCb::ODIN* odin = getIfExists<ODIN>(evtSvc(),LHCb::ODINLocation::Default);
  if ( !odin ) { odin = getIfExists<ODIN>(evtSvc(),LHCb::ODINLocation::Default,false); }
  if ( !odin )
  {
    // should always be available ...
    return Error( "Cannot load the ODIN data object", StatusCode::SUCCESS );
  }

  LHCb::L0DUReport* report =
    getIfExists<LHCb::L0DUReport>(evtSvc(),LHCb::L0DUReportLocation::Default);
  if ( !report )
  {
    report =
      getIfExists<LHCb::L0DUReport>(evtSvc(),LHCb::L0DUReportLocation::Default,false);
  }
  if ( !report )
  {
    Warning( "Can't get LHCb::L0DUReportLocation::Default (" +
             LHCb::L0DUReportLocation::Default + ")" ).ignore();
  }

  LHCb::HltDecReports* decreport_hlt1 =
    getIfExists<LHCb::HltDecReports>(evtSvc(),LHCb::HltDecReportsLocation::Hlt1Default);
  if ( !decreport_hlt1 )
  {
    decreport_hlt1 =
      getIfExists<LHCb::HltDecReports>(evtSvc(),LHCb::HltDecReportsLocation::Hlt1Default,false);
  }
  if ( !decreport_hlt1 )
  {
    Warning( "Can't get LHCb::HltDecReportsLocation::DefaultHlt1 (" +
             LHCb::HltDecReportsLocation::Hlt1Default + ")" ).ignore();
  }

  LHCb::HltDecReports* decreport_hlt2 =
    getIfExists<LHCb::HltDecReports>(evtSvc(),LHCb::HltDecReportsLocation::Hlt2Default);
  if ( !decreport_hlt2 )
  {
    decreport_hlt2 =
      getIfExists<LHCb::HltDecReports>(evtSvc(),LHCb::HltDecReportsLocation::Hlt2Default,false);
  }

  if ( !decreport_hlt2 )
  {
    Warning( "Can't get LHCb::HltDecReportsLocation::DefaultHlt2 (" +
             LHCb::HltDecReportsLocation::Hlt2Default + ")" ).ignore();
  }

  bool test = true;

  // Fill the tuple

  if (!m_mu.empty()) test &= tuple->column( prefix+"Mu", m_mu[odin->runNumber()] );

  test &= tuple->column( prefix+"runNumber",    odin->runNumber() );
  test &= tuple->column( prefix+"eventNumber",  odin->eventNumber() );
  test &= tuple->column( prefix+"BCID",         odin->bunchId() );
  test &= tuple->column( prefix+"BCType",       odin->bunchCrossingType() );
  test &= tuple->column( prefix+"OdinTCK",      odin->triggerConfigurationKey() );
  test &= tuple->column( prefix+"L0DUTCK",      report    ? report->tck() : 0 );
  test &= tuple->column( prefix+"HLT1TCK",      decreport_hlt1 ? decreport_hlt1->configuredTCK() : 0 );
  test &= tuple->column( prefix+"HLT2TCK",      decreport_hlt2 ? decreport_hlt2->configuredTCK() : 0 );
  test &= tuple->column( prefix+"GpsTime",      odin->gpsTime() );

  if ( isVerbose() )
  {
    const Gaudi::Time gtime = odin->eventTime();
    test &= tuple->column( prefix+"GpsYear",  gtime.year(false) );
    test &= tuple->column( prefix+"GpsMonth", gtime.month(false) );
    test &= tuple->column( prefix+"GpsDay", gtime.day(false) );
    test &= tuple->column( prefix+"GpsHour", gtime.hour(false) );
    test &= tuple->column( prefix+"GpsMinute", gtime.minute(false) );
    test &= tuple->column( prefix+"GpsSecond", gtime.second(false) + (gtime.nsecond()/1e9) );
    test &= tuple->column( prefix+"TriggerType", odin->triggerType() );
  }

  test &= tuple->column( prefix+"Polarity", (short)(m_magSvc->isDown()?-1:1) );

  if( msgLevel( MSG::VERBOSE ) ) verbose() << "Returns " << test << endmsg;
  return StatusCode(test);
}
