// Include files
// from Gaudi
#include "GaudiKernel/PhysicalConstants.h"
// local
#include "MCTupleToolEventType.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Event/Particle.h"

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : MCTupleToolEventType
//
// 2009-03-04 : R Lambert
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
// actually acts as a using namespace TupleTool
DECLARE_COMPONENT( MCTupleToolEventType )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  MCTupleToolEventType::MCTupleToolEventType( const std::string& type,
                                              const std::string& name,
                                              const IInterface* parent )
    : TupleToolBase ( type, name , parent )
    , m_fillGenEvent(0)
    , m_findGenEvent(0)
    , m_fillWholeEvent(0)
    , m_fillSlowFind(0)
    , m_fillPseudoFind(0)
    , m_findEventTypes(0)
    , m_hasEventType(0)
    , m_hasMCDecay("")
    , m_mcEventType(0)
    , m_mcDecay(0)
{
  declareInterface<IEventTupleTool>(this);

  // should I write out the gen event header?
  declareProperty( "fillGenEvent", m_fillGenEvent=true );

  // should I look for the generated type in the event?
  declareProperty( "findGenEvent", m_findGenEvent=false );

  // should I examine every MC particle in the event?
  declareProperty( "fillWholeEvent", m_fillWholeEvent=true );

  // search through using decay strings, very slow method
  declareProperty( "fillSlowFind", m_fillSlowFind=false );

  // construct the event types logically, much faster but less accurate
  declareProperty( "fillPseudoFind", m_fillPseudoFind=true );

  // the full list of all event types to consider.for the slow find. This is ignored by the fast/Pseudo find.
  declareProperty( "allEventTypes", m_findEventTypes=std::vector<long unsigned int>(0) );

  // list of event types, the number of these types in the event will appear as an entry in the ntuple
  declareProperty( "hasEventTypes", m_hasEventType=std::vector<long unsigned int>(0) );

  // look for this decay string aswell
  declareProperty( "hasMCDecay", m_hasMCDecay="" );

  //extra name. EVT in this case.
  setProperty("ExtraName", "EVT");
}

//=============================================================================

StatusCode MCTupleToolEventType::initialize()
{
  if( ! TupleToolBase::initialize() ) return StatusCode::FAILURE;
  StatusCode sc=StatusCode::SUCCESS;

  m_mcEventType = tool<IMCEventTypeFinder>("MCEventTypeFinder","Event_Type",this);
  m_mcDecay = tool<IMCDecayFinder>("MCDecayFinder","Decay_Type",this);

  vec2set(m_findEventTypes,m_findEventTypeSet);
  vec2set(m_hasEventType,m_hasEventTypeSet);

  if(!m_findEventTypeSet.empty()) sc=m_mcEventType->setEventTypes(m_findEventTypeSet);
  if(!m_findEventTypeSet.empty() && sc.isFailure())
  {
    warning()<<"Error setting the event types starting with " << m_findEventTypes[0] << endmsg;
    m_findEventTypes.clear();
    m_findEventTypeSet.clear();
  }

  if (m_mcDecay && m_hasMCDecay!="") sc=m_mcDecay->setDecay(m_hasMCDecay);
  if(sc.isFailure())
  {
    warning()<<"Error setting this decay string " << m_hasMCDecay << endmsg;
    m_mcDecay=NULL;
    m_hasMCDecay="";

  }
  if ( msgLevel(MSG::DEBUG) )
  {
    //output all the options
    debug() << "MCTupleToolEventType initialised. Values are set as follows:" << endmsg;
    debug() << "-m_fillGenEvent " << m_fillGenEvent << endmsg;
    debug() << "-m_findGenEvent " << m_findGenEvent << endmsg;
    debug() << "-m_fillWholeEvent " << m_fillWholeEvent << endmsg;
    debug() << "-m_fillSlowFind " << m_fillSlowFind << endmsg;
    debug() << "-m_fillPseudoFind " << m_fillPseudoFind << endmsg;
    debug() << "-m_hasMCDecay " << m_hasMCDecay << endmsg;
    debug() << "-m_findEventTypeSet.size() " << m_findEventTypeSet.size() << endmsg;
    debug() << "-m_hasEventTypeSet.size() " << m_hasEventTypeSet.size() << endmsg;
    debug() << "-m_mcEventType " << (m_mcEventType!=NULL) << endmsg;
    debug() << "-m_mcDecay " << (m_mcDecay!=NULL) << endmsg;

  }
  if(!m_fillGenEvent && !m_fillWholeEvent) warning() << "Nothing to fill, choose at least one of"
                                                     << " fillGenEvent or fillWholeEvent" << endmsg;


  return StatusCode::SUCCESS;
}

StatusCode MCTupleToolEventType::fill( Tuples::Tuple& tuple )/*( const LHCb::Particle*
                                                               , const LHCb::Particle* P
                                                               , const std::string&
                                                               , Tuples::Tuple& tuple )*/{

  const std::string prefix=fullName();

  if ( msgLevel(MSG::DEBUG) ) debug() << "Filling MCTupleToolEventType" << endmsg;


  //The fill method is is two stages and is steered by the options.
  //Firstly, if asked, (fillWholeEvent) the full event is searched for all types

  StatusCode sc=StatusCode::SUCCESS;

  bool test=true;

  LHCb::EventTypeSet foundfull, foundfast;

  //1) Find event types from full event

  //fast or slow?
  if(m_findGenEvent || m_fillWholeEvent)
  {
    if ( msgLevel(MSG::DEBUG) ) debug() << "Running event type finder on full event" << endmsg;
    //slow or fast method?
    if(m_fillSlowFind) sc=m_mcEventType->findEventTypes(foundfull);
    if(sc.isFailure())
    {
      warning() << "Could not perform the fill using the slow method, reverting to the fast method" << endmsg;
      m_fillSlowFind=false;
      m_fillPseudoFind=true;
    }

    if(m_fillPseudoFind && msgLevel(MSG::DEBUG) )
      debug() << "Just about to call construct event types" << endmsg;
    if(m_fillPseudoFind) sc=m_mcEventType->constructEventTypes(foundfast);
    if(m_fillPseudoFind && msgLevel(MSG::DEBUG) )
      debug() << "Finished call to construct event types" << endmsg;
    if(sc.isFailure())
    {
      warning() << "Could not perform the fill using the fast method, I therefore cannot find the overall event type" << endmsg;
      m_fillWholeEvent=m_findGenEvent=false;
    }
    else if(msgLevel(MSG::DEBUG) || msgLevel(MSG::VERBOSE)) debug()
      << "Found types on full event. All OK. fast:" << foundfast.size() << " slow:" << foundfull.size() << endmsg;

  }

  //add the generated event type, if it exists.

  if(m_fillGenEvent)
  {

    if(msgLevel(MSG::DEBUG) || msgLevel(MSG::VERBOSE)) debug() << "filling gen event" << endmsg;
    LHCb::GenHeader* header = getIfExists<LHCb::GenHeader>( evtSvc(), LHCb::GenHeaderLocation::Default, false );
    if(!header)
    {
      warning() << "The GenHeader could not be found, therefore it will not be filled" <<endmsg;
      m_fillGenEvent=false;
    }
    else
    {

      long unsigned int genType=header->evType();

      //add this to the tuple
      test &= tuple->column( prefix+"GenEvent", genType );

      if(m_findGenEvent) //find the event of this type :)
      {
        if(m_fillSlowFind)
        {

          bool foundGenType = (foundfull.count(genType));
          test &= tuple->column( prefix+"hasGenEvent", foundGenType );
          if(msgLevel(MSG::DEBUG) || msgLevel(MSG::VERBOSE)) debug() << "Booked slow event find" << endmsg;
        }
        if(m_fillPseudoFind)
        {

          bool foundGenType = (foundfast.count(genType));
          test &= tuple->column( prefix+"hasPseudoGenEvent", foundGenType );
          if(msgLevel(MSG::DEBUG) || msgLevel(MSG::VERBOSE)) debug() << "Booked fast event find" << endmsg;
        }

      }
    }


  }

  //add the event types for the whole event

  if(m_fillWholeEvent)
  {
    if ( msgLevel(MSG::DEBUG) ) debug() << "filling info for whole event" << endmsg;
    if(m_fillSlowFind)
    {

      test &= tuple->column( prefix+"numFoundTypes", foundfull.size() );

      //insert "found" as an farray
      std::vector<long unsigned int> foundvec(0);
      set2vec(foundfull,foundvec);
      if ( msgLevel(MSG::DEBUG) ) debug() << "ready to book vector of size:"
                                                                 << foundvec.size() << endmsg;
      test &= tuple->farray( "FoundTypes", foundvec, "FoundLen", 20 );
      if ( msgLevel(MSG::DEBUG) ) debug() << "booked vector of size:" << foundvec.size() << endmsg;

      if(!m_hasEventTypeSet.empty()) //book the matching types
      {

        LHCb::EventTypeSet intersection;
        std::set_intersection(foundfull.begin(),foundfull.end(),
                              m_hasEventTypeSet.begin(), m_hasEventTypeSet.end(),
                              std::insert_iterator<LHCb::EventTypeSet>(intersection,intersection.begin()),
                              LHCb::EventTypeComp() );

        test &= tuple->column( prefix+"numMatchingTypes", intersection.size() );
        if ( msgLevel(MSG::DEBUG) ) debug() << "booked intersection of size:"
                                                                   << intersection.size() << endmsg;
        //insert "found" as an farray
        std::vector<long unsigned int> _foundvec(0);
        set2vec(intersection,_foundvec);
        test &= tuple->farray( prefix+"MatchingTypes", _foundvec, prefix+"MatchLen", m_hasEventTypeSet.size() );
      }
    }
    if(m_fillPseudoFind)
    {

      test &= tuple->column( prefix+"numPseudoTypes", foundfast.size() );
      //insert "found" as an farray
      std::vector<long unsigned int> foundvec(0);
      set2vec(foundfast,foundvec);
      if ( msgLevel(MSG::DEBUG) ) debug() << "ready to book vector of size:"
                                                                 << foundvec.size() << endmsg;
      test &= tuple->farray( prefix+"PseudoTypes", foundvec, prefix+"PseudoLen", 20 );
      if ( msgLevel(MSG::DEBUG) ) debug() << "booked vector of size:" << foundvec.size() << endmsg;


      if(!m_hasEventTypeSet.empty()) //book the matching types
      {

        LHCb::EventTypeSet intersection;
        std::set_intersection(foundfast.begin(),foundfast.end(),
                              m_hasEventTypeSet.begin(), m_hasEventTypeSet.end(),
                              std::insert_iterator<LHCb::EventTypeSet>(intersection,intersection.begin()),
                              LHCb::EventTypeComp() );

        test &= tuple->column( prefix+"numMatchingPseudoTypes", intersection.size() );
        if ( msgLevel(MSG::DEBUG) ) debug() << "booked intersection of size:"
                                                                   << intersection.size() << endmsg;
        //insert "found" as an farray
        std::vector<long unsigned int> _foundvec(0);
        set2vec(intersection,_foundvec);
        test &= tuple->farray( prefix+"MatchingPseudoTypes", _foundvec, prefix+"MatchPseudoLen", m_hasEventTypeSet.size() );
      }

    }
    if(m_hasMCDecay!="" && m_mcDecay )
    {
      bool hasMCDecay = m_mcDecay->hasDecay();
      test &= tuple->column( prefix+"hasGivenDecay", hasMCDecay );
      //use the standard decay finding method
    }


  }


  if(msgLevel(MSG::DEBUG) || msgLevel(MSG::VERBOSE)) debug() << "done and returning" << endmsg;
  return StatusCode(test);
}

bool MCTupleToolEventType::vec2set(std::vector<long unsigned int>& avec, LHCb::EventTypeSet& aset)
{
  aset.clear();
  for(std::vector<long unsigned int>::iterator n=avec.begin(); n!=avec.end(); n++)
  {
    aset.insert(*n);
  }
  return (aset.size() > 0);

}

bool MCTupleToolEventType::set2vec(LHCb::EventTypeSet& aset,std::vector<long unsigned int>& avec)
{
  avec.clear();
  avec.reserve(aset.size());
  for(LHCb::EventTypeSet::iterator n=aset.begin(); n!=aset.end(); n++)
  {
    avec.push_back(*n);
  }
  return (avec.size() > 0);

}
