// $Id: TupleToolSwimmingInfo.cpp,v 1.6 2010-03-04 14:02:03 xieyu Exp $
// Include files

// boost
#include <boost/foreach.hpp>
#include <boost/algorithm/string/replace.hpp>

// local
#include "TupleToolSwimmingInfo.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Event/Particle.h"

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolSwimmingInfo
//
// 2011-10-15 : V Gligorov & R Aaij
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
// actually acts as a using namespace TupleTool
DECLARE_COMPONENT( TupleToolSwimmingInfo )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolSwimmingInfo::TupleToolSwimmingInfo( const std::string& type,
                                                const std::string& name,
                                                const IInterface* parent )
    : TupleToolBase ( type, name , parent )
{
  declareInterface<IParticleTupleTool>(this);
  declareProperty("RelationsLocation", m_swimRelTableLoc);
  declareProperty("ReportStage"      , m_swimRepsStage    = "Trigger");
}

bool TupleToolSwimmingInfo::hasDescendant(const LHCb::Particle *mother, const LHCb::Particle *desc)
{
  if(!mother || !desc)
  {
    error() <<  "TupleToolSwimmingInfo::hasDescendant(" << mother << ", " << desc << ")" << endmsg;
    return false;
  }

  LHCb::Particle::ConstVector daughters(mother->daughtersVector());
  for(LHCb::Particle::ConstVector::iterator diter = daughters.begin(); diter != daughters.end(); diter++)
  { 
    const LHCb::Particle *daug(*diter);
    if(daug == desc or hasDescendant(daug, desc))
      return true;
  }
  return false;
}

bool TupleToolSwimmingInfo::getRelationsFromDescendant(P2TPRelation::Range &range, const P2TPRelation *relatePart, const LHCb::Particle* Phead, const LHCb::Particle *P)
{
  if(!relatePart || !Phead || !P)
  {
    error() << "TupleToolSwimmingInfo::getRelationsFromDescendant(range, " << relatePart << ", " << Phead << ", " << P << ")" << endmsg;
    return false;
  }

  LHCb::Particle::ConstVector daughters(Phead->daughtersVector());
  for(LHCb::Particle::ConstVector::iterator diter = daughters.begin(); diter != daughters.end(); diter++)
  {
    const LHCb::Particle *daug(*diter);
    range = relatePart->relations(daug);
    debug() << "Got range.size() = " << range.size() << endmsg;
    if(range.size() == 1)
    {
      // This could be the swum B/D
      if(hasDescendant(daug, P))
      {
        // It is the correct swum B/D
        debug() << "getRelationsFromDescendant() found the correct B/D from " << relatePart->relations(P).size() << " choices" << endmsg;
        debug() << "swum descendant is " << *daug << " which has " << *P << " as a descendant" << endmsg;
        return true;
      }
      else
      {
        debug() << "Found particle " << daug << " with exactly one related swimming report, but it didn't have our track as a descentant" << endmsg;
        if(daug)
          debug() << *daug << endmsg;
      }
    }
    else
    {
      // Recurse down before we move onto the next 'Phead' daughter.
      if(getRelationsFromDescendant(range, relatePart, daug, P))
      {
        debug() << "Succeeded calling getRelationsFromDescendant(range, relatePart, " << daug << ", " << P << ")" << endmsg;
        return true;
      }
    }
  }

  return false;
}

//=============================================================================
StatusCode TupleToolSwimmingInfo::fill( const Particle* Phead
                                        , const Particle* P
                                        , const std::string& head
                                        , Tuples::Tuple& tuple )
{
  const std::string prefix(fullName(head) + "_" + m_swimRepsStage);
  const unsigned int maxTurns(1000);

  typedef std::vector<double> doubleVec;
  typedef std::set<std::string> stringSet;
  typedef std::map<std::string, std::vector<double> > MapType;

  doubleVec raw, dec, tau, ip;
  P2TPRelation *relatePart(NULL);
  if(exist<P2TPRelation>(m_swimRelTableLoc))
  {
    relatePart = get<P2TPRelation>(m_swimRelTableLoc);
    debug() << "Found Particle2TurningPointRelations at " << m_swimRelTableLoc << endmsg;
    if(!relatePart)
      return Error("Retrieved NULL Particle2TurningPointRelations");
  }
  else
  {
    error() << "Couldn't get Particle2TurningPointRelations from " << m_swimRelTableLoc << endmsg;
    return StatusCode::FAILURE;
  }

  // We need to find some reports. Any reports. Just so we can get a list of names, so we always consistently
  // set some value for all of the branches.
  // It's not (/no longer) acceptable to just return some error code without tidying up when we find a candidate
  // which wasn't swum
  stringSet decisions, infonames, extranames;
  P2TPRelation::Range allreports(relatePart->relations());
  for(P2TPRelation::Range::const_iterator report_iter = allreports.begin(); report_iter != allreports.end(); report_iter++)
  {
    const SwimmingReport *report(report_iter->to());
    const tPoints& turns = report->turningPoints(m_swimRepsStage);
    BOOST_FOREACH(const LHCb::TurningPoint& tp, turns)
    {               
      BOOST_FOREACH(const std::string& name, tp.decisions())
      {
        decisions.insert(name); // things like Hlt2CharmHadD02HHXDst_hhXDecision, /Event/Phys/StripBlah/Particles
      }
      BOOST_FOREACH(const std::string& name, tp.infoNames())
      {
        infonames.insert(name); // things like trRec_VELO_OFF, trRec_HLT1
      }
      BOOST_FOREACH(const std::string &name, tp.extraNames())
      {
        extranames.insert(name); // things like 'mipchi2', 'fdchi2'
      }
    }
  }

  if(decisions.empty())
    return Error("Failed to populate decisions");

  // Now we should have the information to know what all of our branches are called
  debug() << "Found decisions:";
  for(stringSet::const_iterator dec_iter = decisions.begin(); dec_iter != decisions.end(); dec_iter++)
    debug() << " " << *dec_iter;
  debug() << endmsg;
  debug() << "Found infonames:";
  for(stringSet::const_iterator inf_iter = infonames.begin(); inf_iter != infonames.end(); inf_iter++)
    debug() << " " << *inf_iter;
  debug() << endmsg;
  debug() << "Found extranames:";
  for(stringSet::const_iterator xtra_iter = extranames.begin(); xtra_iter != extranames.end(); xtra_iter++)
    debug() << " " << *xtra_iter;
  debug() << endmsg;

  if(P)
  {
    P2TPRelation::Range range(relatePart->relations(P));
    debug() << "Got range.size() = " << range.size() << endmsg;
    // range.size() will be zero if we failed to find any associated TPs -- in this case we don't fill anything and nTP = 0.
    // range.size() will be one if everything is simple and there is only one swum candidate 'P' could possibly be associated with
    // range.size() will be larger if there are multiple swum candidates and 'P' is part of more than one of them.
    LHCb::SwimmingReport *report(range.size() == 1 ? range.begin()->to() : NULL);
    if(range.size() == 1 and !report)
      return Error("Got NULL LHCb::SwimmingReport");

    if(range.size() > 1)
    {
      // There can be multiple B/D candidates swum in one event. This means there will be multiple SwimmingReports.
      // A particular B/D descendant track/particle may be part of more than one B/D candidate, and therefore we may
      // find more than one related SwimmingReport in the relations table.
      // In this case we need to find which SwimmingReport corresponds to the B/D candidate which we are currently
      // processing.
      // Start searching from 'Phead' downwards, until we find a particle with exactly one SwimmingReport and which
      // has 'P' as a descendant.
      debug() << "Phead = " << Phead << endmsg;
      if(Phead)
        debug() << "*Phead = " << *Phead << endmsg;
      if(getRelationsFromDescendant(range, relatePart, Phead, P) && range.size() == 1)
      {
        debug() << "Successfully found relations despite range.size() initially being > 1" << endmsg;
        report = range.begin()->to();
      }
      else
      {
        debug() << "About to fail, range.size() == " << range.size() << endmsg;
        return Error("Failed to find the correct swimming report");
      }
    }

    bool test(true);
    const std::string
      triggerPrefix("Trigger"),
      strippingPrefix("Stripping");
    if( !P->isBasicParticle() )
    {
      // Loop over turning points to fill decision names and insert vectors
      MapType line_decisions, extra_info;

      // If there is only one decision then the next part would just be duplicating ..._TP_DEC above.
      bool doDecisions(decisions.size() > 1);

      // This is just setting up some structures rather than actually filling turning point values
      // Even if we didn't swim this event, we still need to fill in the right branches in the tree
      // This is why 'decisions' was populated earlier...
      for(stringSet::const_iterator dec_iter = decisions.begin(); doDecisions && dec_iter != decisions.end(); dec_iter++)
        line_decisions.insert(make_pair(*dec_iter, doubleVec()));
      for(stringSet::const_iterator xtra_iter = extranames.begin(); xtra_iter != extranames.end(); xtra_iter++)
        extra_info.insert(make_pair(*xtra_iter, doubleVec()));
     
      if(report)
      {
        // After all that, we do have swimming reports to associate with this event
        const tPoints& turns = report->turningPoints(m_swimRepsStage);
        debug() << "Retrieved " << turns.size() << " turning points" << endmsg;
        // Loop over turning points to fill vectors
        BOOST_FOREACH(const LHCb::TurningPoint& tp, turns)
        {
          // Fill variables
          raw.push_back(tp.raw());
          dec.push_back(tp.dec());
          tau.push_back(tp.tau());
          ip.push_back (tp.ip() );

          if(doDecisions)
          {
            // Decisions must not change between turning points
            BOOST_FOREACH(MapType::value_type& entry, line_decisions)
            {
              if (!tp.dec())
              {
                entry.second.push_back(false);
              }
              else
              {
                entry.second.push_back(tp.decision(entry.first));
              }
            }
          }

          BOOST_FOREACH(MapType::value_type& entry, extra_info)
          {
            entry.second.push_back(tp.extra(entry.first));
          }
        }
      }

      if(raw.size() > maxTurns)
        warning() << "Have " << raw.size() << " turning points, which is > maxTurns = " << maxTurns << endmsg;

      // Fill tuple from vectors
      test &= tuple->farray( prefix+"_TP_RAW", raw ,prefix+"_nTP",maxTurns );
      test &= tuple->farray( prefix+"_TP_DEC", dec ,prefix+"_nTP",maxTurns );
      test &= tuple->farray( prefix+"_TP_TAU", tau ,prefix+"_nTP",maxTurns );
      test &= tuple->farray( prefix+"_TP_IP" , ip  ,prefix+"_nTP",maxTurns );
      
      if(doDecisions)
      {
        debug() << "Populating line_decisions stuff" << endmsg;
        BOOST_FOREACH(const MapType::value_type& entry, line_decisions)
        {
          std::string branch(entry.first);
          boost::algorithm::replace_all(branch, "/", "_");
          test &= tuple->farray(prefix + "_TP_DEC_" + branch, entry.second, prefix + "_nTP", maxTurns);
        }
      }

      debug() << "Populating extra_info..." << endmsg;
      BOOST_FOREACH(const MapType::value_type& entry, extra_info)
      {
        test &= tuple->farray(prefix + "_TP_" + entry.first, entry.second, prefix + "_nTP", maxTurns);
      }
    }
    else if (m_swimRepsStage.size() >= triggerPrefix.size() and m_swimRepsStage.substr(0, triggerPrefix.size()) == triggerPrefix)
    {
      //The each track part is only for the trigger
      // Loop over turning points to fill info names and insert vectors
      MapType track_infos;

      debug() << "Doing per-track trigger stuff" << endmsg;

      for(stringSet::const_iterator infoname_iter = infonames.begin(); infoname_iter != infonames.end(); infoname_iter++)
        track_infos.insert(make_pair(*infoname_iter, doubleVec()));

      if(report)
      {
        const tPoints& turns = report->turningPoints(m_swimRepsStage);
        debug() << "Got " << turns.size() << " per-track TPs..." << endmsg;
        BOOST_FOREACH(const LHCb::TurningPoint& tp, turns)
        {
          // Fill infos
          BOOST_FOREACH(MapType::value_type& entry, track_infos)
          {
            if (!tp.dec()) {
              entry.second.push_back(false);
            } else {
              entry.second.push_back(tp.participated(entry.first, *P));
            }
          }
        }
      }

      // Fill tuple with infos
      BOOST_FOREACH(const MapType::value_type& entry, track_infos)
      {
        test &= tuple->farray(prefix + "_TP_DEC_" + entry.first, entry.second,
                              prefix + "_nTP", maxTurns);
      }

      debug() << "Finished doing track stuff" << endmsg;
    }
  }
  return StatusCode::SUCCESS;
}
