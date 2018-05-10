// Include files
#include "gsl/gsl_sys.h"
// from Gaudi
#include "GaudiKernel/PhysicalConstants.h"
// local
#include "MCTupleToolDecayType.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Event/Particle.h"
#include "Event/MCParticle.h"

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : MCTupleToolDecayType
//
// 2009-05-01 : Rob Lambert
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
// actually acts as a using namespace TupleTool
DECLARE_COMPONENT( MCTupleToolDecayType )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
MCTupleToolDecayType::MCTupleToolDecayType( const std::string& type,
                                            const std::string& name,
                                            const IInterface* parent )
: TupleToolBase ( type, name , parent )
  , m_mother(0)
  , m_fillSlowFind(0)
  , m_fillPseudoFind(0)
  , m_findEventTypes(0)
  , m_hasEventType(0)
  , m_hasMCDecay("")
  , m_mcEventType(0)
  , m_mcDecay(0)
{
  declareInterface<IMCParticleTupleTool>(this);

  // categorise the mcmother of this mcparticle, not the associate itself
  declareProperty( "mother", m_mother = false );

  // categorise the ultimate mcmother of this mcparticle, not the associate itself
  declareProperty( "top", m_mother = false );

  // search through using decay strings, very slow method
  declareProperty( "fillSlowFind", m_fillSlowFind=false );

  // construct the event types logically, much faster but less accurate
  declareProperty( "fillPseudoFind", m_fillPseudoFind=true );

  // the full list of all event types to consider.
  declareProperty( "allEventTypes", m_findEventTypes=std::vector<long unsigned int>(0) );

  // list of event types, the number of these types in the event will appear as an entry in the ntuple
  declareProperty( "hasEventTypes", m_hasEventType=std::vector<long unsigned int>(0) );

  // look for this decay string aswell
  declareProperty( "hasMCDecay", m_hasMCDecay="" );

  //extra name. MCP in this case.
  setProperty("ExtraName", "MCP");
}

//=============================================================================

StatusCode MCTupleToolDecayType::initialize()
{
  StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  m_mcEventType = tool<IMCEventTypeFinder>("MCEventTypeFinder","Event_Type",this);
  m_mcDecay = tool<IMCDecayFinder>("MCDecayFinder","Decay_Type",this);

  vec2set(m_findEventTypes,m_findEventTypeSet);
  vec2set(m_hasEventType,m_hasEventTypeSet);

  if(!m_findEventTypes.empty()) sc=m_mcEventType->setEventTypes(m_findEventTypeSet);
  if(!m_findEventTypes.empty() && sc.isFailure())
  {
    warning()<<"Error setting the event types starting with " << m_findEventTypes[0] << endmsg;
    m_findEventTypes.clear();
  }

  if (m_mcDecay && m_hasMCDecay!="") sc=m_mcDecay->setDecay(m_hasMCDecay);
  if(sc.isFailure())
  {
    warning()<<"Error setting this decay string " << m_hasMCDecay << endmsg;
    m_mcDecay=NULL;
    m_hasMCDecay="";

  }
  if(msgLevel(MSG::DEBUG))
  {
    //output all the options
    debug() << "MCTupleToolDecayType initialised. Values are set as follows:" << endmsg;
    debug() << "-m_mother " << m_mother << endmsg;
    debug() << "-m_fillSlowFind " << m_fillSlowFind << endmsg;
    debug() << "-m_fillPseudoFind " << m_fillPseudoFind << endmsg;
    debug() << "-m_hasMCDecay " << m_hasMCDecay << endmsg;
    debug() << "-m_findEventTypeSet.size() " << m_findEventTypeSet.size() << endmsg;
    debug() << "-m_hasEventTypeSet.size() " << m_hasEventTypeSet.size() << endmsg;
    debug() << "-m_mcEventType " << (m_mcEventType!=NULL) << endmsg;
    debug() << "-m_mcDecay " << (m_mcDecay!=NULL) << endmsg;

  }


  return StatusCode::SUCCESS;
}

StatusCode MCTupleToolDecayType::fill( const LHCb::MCParticle*
                                       , const LHCb::MCParticle* mcp
                                       , const std::string& head
                                       , Tuples::Tuple& tuple )
{
  const std::string prefix=fullName(head);

  if(msgLevel(MSG::DEBUG)) debug() << "Filling MCTupleToolDecayType" << endmsg;


  //The fill method is is two stages and is steered by the options.
  //Firstly, if asked, (fillWholeEvent) the full event is searched for all types
  //Secondly, if asked, (fillAssociated) the MC associated particle is searched for all types

  StatusCode sc=StatusCode::SUCCESS;

  bool test=true;


  if(msgLevel(MSG::DEBUG)) debug() << "finding event types for MCAssociate" << endmsg;
  LHCb::EventTypeSet foundfull;
  LHCb::EventTypeSet foundfast;

  // pointer is ready, prepare the sets:
  if( mcp )
  {
    verbose() << "found MC Particle" << endmsg;
    //fast or slow?
    if(m_fillSlowFind) sc=m_mcEventType->findDecayType(foundfull,mcp);
    if(sc.isFailure())
    {
      warning() << "Could not perform the fill using the slow method, reverting to the fast method" << endmsg;
      m_fillSlowFind=false;
      m_fillPseudoFind=true;
    }

    if(m_fillPseudoFind) sc=m_mcEventType->constructDecayType(foundfast,mcp);
    if(sc.isFailure())
    {
      warning() << "Could not perform the fill using the fast method,"
                << " I therefore cannot find the overall event type" << endmsg;
      m_fillPseudoFind=false;
    }
  }
  if(msgLevel(MSG::DEBUG)) debug() << "filling info for MCAssociate" << endmsg;
  if(m_fillSlowFind)
  {
    test &= tuple->column( prefix+"_numFoundTypes", foundfull.size() );
    //insert "found" as an farray
    std::vector<long unsigned int> foundvec(0);
    set2vec(foundfull,foundvec);
    test &= tuple->farray( prefix+"_MCP_FoundTypes", foundvec, prefix+"_FoundLen", 20 );

    if(!m_hasEventTypeSet.empty()) //book the matching types
    {

      LHCb::EventTypeSet intersection;
      std::set_intersection(foundfull.begin(),foundfull.end(),
                            m_hasEventTypeSet.begin(), m_hasEventTypeSet.end(),
                            std::inserter(intersection,intersection.begin()),
                            LHCb::EventTypeComp() );

      //bool foundGiven=(intersection.size()>0);
      test &= tuple->column( prefix+"_numMatchingTypes", intersection.size() );
      if(msgLevel(MSG::DEBUG)) debug() << "booked intersection of size:"
                                       << intersection.size() << endmsg;
      //insert "found" as an farray
      std::vector<long unsigned int> _foundvec(0);
      set2vec(intersection,_foundvec);
      test &= tuple->farray( prefix+"_MatchingTypes", _foundvec, prefix+"_MatchLen", m_hasEventTypeSet.size() );
    }
  }
  if(m_fillPseudoFind)
  {
    test &= tuple->column( prefix+"_numPseudoTypes", foundfast.size() );
    //insert "found" as an farray
    std::vector<long unsigned int> foundvec(0);
    set2vec(foundfast,foundvec);
    test &= tuple->farray( prefix+"_PseudoTypes", foundvec, prefix+"_MCP_PseudoLen", 20 );

    if(!m_hasEventTypeSet.empty()) //book the matching types
    {

      LHCb::EventTypeSet intersection;
      std::set_intersection(foundfast.begin(),foundfast.end(),
                            m_hasEventTypeSet.begin(), m_hasEventTypeSet.end(),
                            std::inserter(intersection,intersection.begin()),
                            LHCb::EventTypeComp() );

      //bool foundGiven=(intersection.size()>0);
      test &= tuple->column( prefix+"_numMatchingPseudoTypes", intersection.size() );
      if(msgLevel(MSG::DEBUG)) debug() << "booked intersection of size:"
                                       << intersection.size() << endmsg;
      //insert "found" as an farray
      std::vector<long unsigned int> _foundvec(0);
      set2vec(intersection,_foundvec);
      test &= tuple->farray( prefix+"_MatchingPseudoTypes",
                             _foundvec, prefix+"_MCP_MatchPseudoLen",
                             m_hasEventTypeSet.size() );
    }
  }

  if(m_hasMCDecay!="" && m_mcDecay )
  {
    bool hasMCDecay =false;
    LHCb::MCParticle::ConstVector dummyvec(0); //stupid way, but is the only way!
    if (mcp)
    {
      dummyvec.push_back(mcp);
      hasMCDecay = m_mcDecay->hasDecay(dummyvec);
    }

    test &= tuple->column( prefix+"_hasGivenDecay", hasMCDecay );
    //use the standard decay finding method
  }

  if(msgLevel(MSG::DEBUG)) debug() << "done and returning" << endmsg;

  return StatusCode(test);
}

bool MCTupleToolDecayType::vec2set(std::vector<long unsigned int>& avec, LHCb::EventTypeSet& aset)
{
  aset.clear();
  for(std::vector<long unsigned int>::iterator n=avec.begin(); n!=avec.end(); n++)
  {
    aset.insert(*n);
  }
  return (aset.size() > 0);
}

bool MCTupleToolDecayType::set2vec(LHCb::EventTypeSet& aset,std::vector<long unsigned int>& avec)
{
  avec.clear();
  avec.reserve(aset.size());
  for(LHCb::EventTypeSet::iterator n=aset.begin(); n!=aset.end(); n++)
  {
    avec.push_back(*n);
  }
  return (avec.size() > 0);
}
