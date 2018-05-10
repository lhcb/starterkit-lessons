#include "TupleToolPrimaries.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
#include "Event/VertexBase.h"
#include "Event/Track.h"

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolPrimaries
//
// 2007-11-07 : Jeremie Borel
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
// actually acts as a using namespace TupleTool
DECLARE_COMPONENT( TupleToolPrimaries )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolPrimaries::TupleToolPrimaries( const std::string& type,
                                          const std::string& name,
                                          const IInterface* parent )
    : TupleToolBase ( type, name, parent )
{
  declareInterface<IEventTupleTool>(this);
  declareProperty("InputLocation",
                  m_pvLocation = LHCb::RecVertexLocation::Primary,
                  "PV location to be used.");
}

//=============================================================================
//=============================================================================

StatusCode TupleToolPrimaries::initialize()
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  debug() << "Will be looking for PVs at " << m_pvLocation << endmsg ;

  return sc;
}

//=============================================================================
//=============================================================================

StatusCode TupleToolPrimaries::fill( Tuples::Tuple& tuple )
{
  const std::string prefix=fullName();

  if (msgLevel(MSG::VERBOSE))
    verbose() << "Storing PVs with prefix ``" << prefix << "''" << endmsg ;

  std::vector<double>  pvx, pvy, pvz;
  std::vector<double>  epvx, epvy, epvz;
  std::vector<double>  pvchi2, pvndof, pvntracks, pvsumpt;

  const RecVertex::Container* PV = getIfExists<RecVertex::Container>(m_pvLocation);
  if ( PV )
  {

    if( PV->size() > m_maxPV )
    {
      Warning("Too many primaries, no PVs will be stored.").ignore();
    }
    else
    {
      for(RecVertex::Container::const_iterator i = PV->begin() ; PV->end()!=i ; ++i )
      {
        if (msgLevel(MSG::VERBOSE)) verbose() << "PV: " <<  (*i)->position() << endmsg ;
        pvx.push_back( (*i)->position().X() );
        pvy.push_back( (*i)->position().Y() );
        pvz.push_back( (*i)->position().Z() );
        if (msgLevel(MSG::VERBOSE)) verbose() << "PV matrix: " << (*i)->covMatrix()(0,0) << " "
                                              << (*i)->covMatrix()(1,1)  << " "
                                              << (*i)->covMatrix()(2,2)  << endmsg ;
        epvx.push_back( std::sqrt((*i)->covMatrix()(0,0)) );
        epvy.push_back( std::sqrt((*i)->covMatrix()(1,1)) );
        epvz.push_back( std::sqrt((*i)->covMatrix()(2,2)) );
        pvchi2.push_back((*i)->chi2());
        pvndof.push_back((double)(*i)->nDoF());
        pvntracks.push_back((double)(*i)->tracks().size());
        if (isVerbose()) pvsumpt.push_back(sumPT(*i)) ;
        if (msgLevel(MSG::VERBOSE)) verbose() << "Tracks: "  << (*i)->tracks().size() << endmsg ;
      }
    }
    if (msgLevel(MSG::DEBUG))
      debug() << "There are " << PV->size() << " PVs at " <<  pvz << endmsg ;
  }
  else if (msgLevel(MSG::DEBUG))
  {
    debug() << "PV container is empty" << endmsg ;
  }

  bool test=true;
  test &= tuple->farray( prefix+"PVX", pvx, prefix+"nPV",  m_maxPV );
  test &= tuple->farray( prefix+"PVY", pvy, prefix+"nPV",  m_maxPV );
  test &= tuple->farray( prefix+"PVZ", pvz, prefix+"nPV",  m_maxPV );
  test &= tuple->farray( prefix+"PVXERR", epvx, prefix+"nPV",  m_maxPV );
  test &= tuple->farray( prefix+"PVYERR", epvy, prefix+"nPV",  m_maxPV );
  test &= tuple->farray( prefix+"PVZERR", epvz, prefix+"nPV",  m_maxPV );

  test &= tuple->farray( prefix+"PVCHI2", pvchi2, prefix+"nPV",  m_maxPV );
  test &= tuple->farray( prefix+"PVNDOF", pvndof, prefix+"nPV",  m_maxPV );
  test &= tuple->farray( prefix+"PVNTRACKS", pvntracks, prefix+"nPV",  m_maxPV );
  if (isVerbose()) test &= tuple->farray( prefix+"PVsumPT", pvsumpt, prefix+"nPV",  m_maxPV );

  return StatusCode(test);
  //  return StatusCode::SUCCESS;
}
//=============================================================================
// Sum PT
//=============================================================================
double TupleToolPrimaries::sumPT(const LHCb::RecVertex* pv) const {
  if (!pv) Exception("Not a RecVertex?");
  double spt = 0 ;
  for (SmartRefVector< LHCb::Track >::const_iterator t = pv->tracks().begin() ;
       t!= pv->tracks().end() ; ++t) {
    if (0==*t) {
      Warning("Cannot resolve pointer to Track. Probably a microDST. Set Verbose to false.",
              StatusCode::FAILURE,10);
      return -1;
    }
    spt += (*t)->pt();
  }
  return spt ;
}
