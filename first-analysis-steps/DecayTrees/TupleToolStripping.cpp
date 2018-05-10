#include "TupleToolStripping.h"
#include "Event/HltDecReports.h"
#include "Event/HltDecReport.h"
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

// gaudirun.py ~/cmtuser/Saved-options/DVTestStripping-ReadDST.py ~/cmtuser/Saved-options/DiMuon_100419.py | tee test

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolStripping
//
// 2010-04-23 : Patrick Koppenburg
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolStripping )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolStripping::TupleToolStripping( const std::string& type,
                                          const std::string& name,
                                          const IInterface* parent )
    : TupleToolTriggerBase ( type, name , parent )
{
  declareInterface<IEventTupleTool>(this);
  /// @todochange with rootontes
  declareProperty("StrippingReportsLocations", m_location = "/Event/Strip/Phys/DecReports" );
  declareProperty("StrippingList", m_strippingList );// act as TriggerList in base class
  m_doL0=false;
  m_doHlt1=false;
  m_doHlt2=false;
  m_doStripping=true;
  m_verboseStripping=true;
}

//=============================================================================
// Destructor
//=============================================================================
TupleToolStripping::~TupleToolStripping() {}

//=============================================================================

StatusCode TupleToolStripping::initialize ( )
{

  // before initializing the base class
  if(  m_triggerList.empty() &&  m_strippingList.empty())
    return Warning("You MUST explicitely configure the list of stripping lines",
                   StatusCode::FAILURE);
  if( !m_triggerList.empty() && !m_strippingList.empty())
    return Warning("You must configure either StrippingList or TriggerList (equivalent) but not both",
                   StatusCode::FAILURE);
  if( !m_strippingList.empty()) { m_triggerList = m_strippingList; }
  m_strippingList.clear();

  const StatusCode sc = TupleToolTriggerBase::initialize();
  if ( sc.isFailure() ) return sc;

  if ( m_doL0 || !m_l0.empty() )
    Warning("L0 line(s) requested : You should use TupleToolStripping for that",
            StatusCode::SUCCESS);
  if ( m_doHlt1 || !m_hlt1.empty() )
    Warning("HLT1 line(s) requested : You should use TupleToolStripping for that",
            StatusCode::SUCCESS);
  if ( m_doHlt2 || !m_hlt2.empty() )
    Warning("HLT1 line(s) requested : You should use TupleToolStripping for that",
            StatusCode::SUCCESS);

  if ( !m_doStripping || m_stripping.empty() )
    return Error("You MUST explicitly configure the list of stripping lines",
                 StatusCode::FAILURE);

  //
  info() << "Requested information for stripping lines : " << m_stripping << endmsg;

  return sc ;
}

//=============================================================================

StatusCode TupleToolStripping::fill( Tuples::Tuple& tuple )
{

  bool fillTup=true;
  const LHCb::HltDecReports* dr = getIfExists<LHCb::HltDecReports>(evtSvc(),m_location,false);
  if ( !dr ) { dr = getIfExists<LHCb::HltDecReports>(evtSvc(),m_location); }

  if ( dr )
  {
    if ( msgLevel(MSG::DEBUG) )
    {
      debug() << "There are " << dr->size() << " DecReports at " << m_location << endmsg ;
    }
    if ( msgLevel(MSG::VERBOSE) )
    {
      const std::vector<std::string> & names = dr->decisionNames() ;
      verbose() << "NAMES: " << names << endmsg ;
    }
  }
  else
  {
    Warning( "No DecReports at "+m_location, StatusCode::SUCCESS, 1).ignore();
  }

  unsigned int i = 0 ;
  for ( std::vector<std::string>::const_iterator s = m_stripping.begin() ;
        s != m_stripping.end(); ++s, ++i )
  {
    if ( msgLevel(MSG::VERBOSE) )
      verbose() << "Trying " << i << " " << *s << " in " << m_stripping << endmsg ;

    if ( dr && dr->hasDecisionName(*s) )
    {
      const LHCb::HltDecReport* report = dr->decReport(*s);
      if ( !report ) Exception("Cannot find report "+*s);
      if(msgLevel(MSG::VERBOSE)) verbose() << *s << " says " << report->decision() << endmsg ;
      fillTup &= tuple->column( *s, report->decision() ) ;
    }
    else
    {
      if(msgLevel(MSG::VERBOSE))
        verbose() <<" no report for " << *s << " decision is 0" << endmsg ;
      const unsigned int dec = 0;
      fillTup &= tuple->column( *s, dec ) ;
    }
  }

  return StatusCode(fillTup);
}

//=============================================================================
