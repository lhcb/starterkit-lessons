#include "TupleToolJetRelations.h"
#include <math.h>
#include <boost/bind.hpp>
#include "boost/lexical_cast.hpp"

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolJetRelations
// Autor: Albert Bursche
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolJetRelations )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolJetRelations::TupleToolJetRelations( const std::string& type,
                                                const std::string& name,
                                                const IInterface* parent )
    : TupleToolJetsBase ( type, name , parent )

{
  declareInterface<IParticleTupleTool>(this);
  declareProperty( "RelationsContainer",m_RelationsContainer = "",
                   "Where to find the table to get information from." );
  declareProperty( "InvertTable",m_Invert = false,
                   "Invert relation table" );
  declareProperty( "ReverseMatching",m_Reverse = false,
                   "Does best matching correspond to highes or lowest weight?" );
  declareProperty( "MaxMatches",m_MaxMatches=1,
                   "How many matches in the tuple?" );
  declareProperty( "ExtraWeights",m_ExtraTableLocations=std::map<std::string,std::string>(),
                   "Write the 'weight' parameter of these tables in the tuple. Uses the value of 'InvertTable'." );
}

//=============================================================================

StatusCode TupleToolJetRelations::fill( const LHCb::Particle* /* top */,
                                        const LHCb::Particle* p,
                                        const std::string& head ,
                                        Tuples::Tuple& tuple )
{
  m_tuple = &tuple;
  bool test = true;
  m_p = p;
  m_head = m_extraName+head;
  //  if(exist<IJets2Jets::Table>(m_RelationsContainer))
  for ( std::map<std::string,std::string>::const_iterator it = m_ExtraTableLocations.begin();
        it != m_ExtraTableLocations.end();++it)
  {
    IJets2Jets::Table * tab = 0;
    getTable(it->second,tab);
    m_ExtraTables[it->first] = tab;
  }
  if (m_ExtraTables.size()!=m_ExtraTableLocations.size())
    return StatusCode::FAILURE;
  if (getTable(m_RelationsContainer,m_Table)==StatusCode::SUCCESS)
  {
    IJets2Jets::Table::Range r = m_Table->i_relations(p);
    (*m_tuple)->column( m_head+"_numMatches",r.size());
    unsigned int numMatches = 0;
    if (r.size())
    {
      if(m_Reverse)
      {
        IJets2Jets::Table::Range::reverse_iterator it = r.rbegin();
        for (;numMatches<m_MaxMatches;numMatches++)
          if(it!=r.rend())
            fill(numMatches,&*it++);
          else
            fill(numMatches,0);
      }
      else
      {
        IJets2Jets::Table::Range::iterator it = r.begin();
        for (;numMatches<m_MaxMatches;numMatches++)
          if(it!=r.end())
            fill(numMatches,&*it++);
          else
            fill(numMatches,0);
      }
    }
    else
      for (;numMatches<m_MaxMatches;numMatches++)
        fill(numMatches,0);
    if (m_Invert)
    {
      delete m_Table;
      for (std::map<std::string,IJets2Jets::Table*>::iterator it = m_ExtraTables.begin();
           it != m_ExtraTables.end();++it)
        if (it->second)
        {
          delete it->second;
          it->second = 0;
        }
    }
  }
  return StatusCode(test);
}

double TupleToolJetRelations::DeltaR(const LHCb::Particle& jet,const LHCb::Particle& jet2)
{  //Delta R
  double dphi = jet.momentum().Phi()- jet2.momentum().Phi();
  while (fabs(dphi)>M_PI)
    dphi+=(dphi>0)?-2*M_PI:2*M_PI;
  return sqrt(pow(jet.momentum().Eta()- jet2.momentum().Eta(),2)+pow(dphi,2));
}
#define SAVEPOINT(POINT,FUN)                    \
  (POINT?POINT->FUN:-1.0)
bool TupleToolJetRelations::fill(unsigned int i,const IJets2Jets::Table::Entry *e)
{
  bool test =  true;
  std::string I = boost::lexical_cast<std::string>(i);
  if ( msgLevel(MSG::DEBUG) )
    debug() << "Tuple: "<< m_tuple << "\t"+ I +" \tRelation" << e << endmsg;

  test &=(*m_tuple)->column(m_head+"Relation"+I+"weight",SAVEPOINT(e,weight()));
  test &=(*m_tuple)->column(m_head+"Relation"+I+"DeltaR",e==0?-1:DeltaR((*e->from()),*(e->to())));
  if (e&&e->to())
    WriteJetToTuple(e->to(),m_head+"Relation"+I+"Jet");
  else
    WriteJetToTuple(0      ,m_head+"Relation"+I+"Jet");
  for (std::map<std::string,IJets2Jets::Table*>::const_iterator it = m_ExtraTables.begin();
       it != m_ExtraTables.end();++it)
    if (it->second)//this should be true for either all or no events
    {
      bool found = false;
      // IJets2Jets::Table::Range FromRes = it->second->i_relations (e->from());
      if(e&&e->to()&&e->from())
      {
        IJets2Jets::Table::Range FromRes = it->second->i_relations (e->from());
        for (IJets2Jets::Table::Range::iterator jt = FromRes.begin(); jt!=FromRes.end();++jt )
          if (jt->to()==e->to())
          {
            if (found)
              Error("Table corrupted!");
            debug() << "Writing value in " << m_head+"Relation"+I+it->first << endmsg;
            test &=(*m_tuple)->column(m_head+"Relation"+I+it->first,jt->weight());
            found = true;
          }
      }
      if (!found)
      {
        debug() << "Writing dummy in " << m_head+"Relation"+I+it->first << endmsg;
        test &=(*m_tuple)->column(m_head+"Relation"+I+it->first,(double)-1);
      }
    }
  return test;
}
StatusCode TupleToolJetRelations::getTable(std::string loc,IJets2Jets::Table*&tab)
{
  if(exist<IJets2Jets::Table>(loc))
  {
    tab = get<IJets2Jets::Table>(loc);
    if (m_Invert)
      tab = new IJets2Jets::Table(*tab,1);
    debug() << "Retrieved  table from :" << loc << endmsg;
    return StatusCode::SUCCESS;
  }
  else
  {
    tab = 0;
    return StatusCode::FAILURE;
  }
}
