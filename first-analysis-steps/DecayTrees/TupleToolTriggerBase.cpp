// Include files
#include "boost/regex.hpp"

#include "Event/Particle.h"
// kernel
#include "Kernel/IANNSvc.h"
#include "Event/L0DUReport.h"
#include "Event/HltDecReports.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
// local
#include "TupleToolTriggerBase.h"

//#include <sstream>

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolTriggerBase
//
// 2008-04-09 : V. Gligorov
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolTriggerBase::TupleToolTriggerBase( const std::string& type,
                                            const std::string& name,
                                            const IInterface* parent )
  : TupleToolBase ( type, name , parent ),
    m_l0(0),
    m_hlt1(0),
    m_hlt2(0),
    m_stripping(0),
    m_triggerList(0)
{
  //declareInterface<IParticleTupleTool>(this);

  //to turn all verbosity on, set Verbose=true
  declareProperty( "VerboseL0",   m_verboseL0=false );
  declareProperty( "VerboseHlt1", m_verboseHlt1=false );
  declareProperty( "VerboseHlt2", m_verboseHlt2=false );
  declareProperty( "VerboseStripping", m_verboseStripping=false );
  declareProperty( "FillL0",   m_doL0=true );
  declareProperty( "FillHlt1", m_doHlt1=true );
  declareProperty( "FillHlt2", m_doHlt2=true );
  declareProperty( "FillStripping", m_doStripping=false );

  //List of triggers to look at
  declareProperty( "TriggerList",   m_triggerList=std::vector<std::string>(0) );
}

//=============================================================================
// Destructor
//=============================================================================
TupleToolTriggerBase::~TupleToolTriggerBase() {}

//=============================================================================

//=========================================================================
//  initialize
//=========================================================================
StatusCode TupleToolTriggerBase::initialize( )
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  if (isVerbose()) m_verboseL0=m_verboseHlt1=m_verboseHlt2=m_verboseStripping=true;
  const bool iv = (m_verboseL0||m_verboseHlt1||m_verboseHlt2||m_verboseStripping);

  if ( !m_triggerList.empty() && !iv )
  {
    warning() << "You have set a list of triggers to look for, but not asked for verbose mode ... OK, but this is weird! "
              << endmsg;
  }

  if(m_triggerList.size() == 0 && iv )
  {
    warning() << "You have not set a list of triggers to look for, but have asked for verbose output. "
              << endmsg;
    m_verboseL0 = m_verboseHlt1 = m_verboseHlt2 = m_verboseStripping =false;
  }

  //bug, missing this line
  if ( !m_triggerList.empty() ) compileMyList(m_triggerList);

  return sc;
}

//=========================================================================
//  Fill
//=========================================================================

StatusCode TupleToolTriggerBase::fill( const LHCb::Particle* M
                                       , const LHCb::Particle* P
                                       , const std::string& head
                                       , Tuples::Tuple& tuple )
{
  if(msgLevel(MSG::DEBUG)) debug() << "fill particle " << P
                                   << endmsg;

  bool test=true;
  test &= fillBasic(M, P, head, tuple);

  //Fill details about the requested triggers
  if(m_verboseL0 || m_verboseHlt1 || m_verboseHlt2 || m_verboseStripping || isVerbose())
    test &=fillVerbose(M, P, head, tuple);

  return StatusCode(test);
}

StatusCode TupleToolTriggerBase::fill(Tuples::Tuple& tuple )
{
  if(msgLevel(MSG::DEBUG)) debug() << "fill tuple "
                                   << endmsg;
  bool test=true;
  test &= fillBasic(tuple);

  //Fill details about the requested triggers
  if(m_verboseL0 || m_verboseHlt1 || m_verboseHlt2 || m_verboseStripping || isVerbose()) test &=fillVerbose(tuple);

  return StatusCode(test);
}


bool TupleToolTriggerBase::compileMyList(const std::vector<std::string>& list)
{

  if(msgLevel(MSG::DEBUG)) debug() << "compiling List "
                                   << endmsg;

  //boost::regex l0("Hlt1L0.*Decision");
  //boost::regex hlt1("Hlt1[^L0].*Decision");
  boost::regex l0("L0.*Decision");
  boost::regex hlt1("Hlt1.*Decision");
  boost::regex hlt2("Hlt2.*Decision");
  boost::regex strip("Stripping.*Decision");

  for ( std::vector< std::string >::const_iterator s = list.begin();
        s != list.end(); ++s )
  {
    if( boost::regex_match( *s, l0 ) )
    {
      m_l0.push_back(*s);
    }
    else if( boost::regex_match( *s, hlt1 ) )
    {
      m_hlt1.push_back(*s);
    }
    else if( boost::regex_match( *s,  hlt2 ) )
    {
      m_hlt2.push_back(*s);
    }
    else if( boost::regex_match( *s,  strip ) )
    {
      m_stripping.push_back(*s);
    }
    else
    {
      error() << "List member ``" << *s
              << "'' does not match any known pattern. Have you forgotten the trailing 'Decision' or the starting 'Stripping' ?"
              << endmsg ;
    }
  }


  if(msgLevel(MSG::DEBUG))
  {
    debug() << " ==== L0 ==== " << endmsg;
    for (std::vector<std::string>::const_iterator s=m_l0.begin();s != m_l0.end();++s) debug() << " " << (*s);
    debug() <<endmsg;
    debug() << " ==== HLT1 ==== " << endmsg;
    for (std::vector<std::string>::const_iterator s=m_hlt1.begin();s != m_hlt1.end();++s) debug() << " " << (*s);
    debug() <<endmsg;
    debug() << " ==== HLT2 ==== " << endmsg;
    for (std::vector<std::string>::const_iterator s=m_hlt2.begin();s != m_hlt2.end();++s) debug() << " " << (*s);
    debug() <<endmsg;
    debug() << " ==== STRIPPING ==== " << endmsg;
    for (std::vector<std::string>::const_iterator s=m_stripping.begin();s != m_stripping.end();++s) debug() << " " << (*s);
    debug() <<endmsg;
    debug() << " ==== Compiled list ====" << endmsg;
  }

  return true;

}

StatusCode TupleToolTriggerBase::fillBasic( const LHCb::Particle*
                                            , const LHCb::Particle*
                                            , const std::string&
                                            , Tuples::Tuple& )
{
  return Error("Don't call the fill of the baseclass!!");
}

StatusCode TupleToolTriggerBase::fillBasic(Tuples::Tuple& )
{
  return Error("Don't call the fill of the baseclass!!");
}

StatusCode TupleToolTriggerBase::fillVerbose( const LHCb::Particle*
                                              , const LHCb::Particle*
                                              , const std::string&
                                              , Tuples::Tuple& )
{
  return Error("Don't call the fill of the baseclass!!");
}

StatusCode TupleToolTriggerBase::fillVerbose( Tuples::Tuple& )
{
  return Error("Don't call the fill of the baseclass!!");
}
