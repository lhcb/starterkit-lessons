// $Id: TupleToolGeometry.cpp,v 1.17 2010-05-12 20:01:40 jpalac Exp $
// Include files

// local
#include "TupleToolGeometry.h"

#include "Kernel/IDVAlgorithm.h"
#include <Kernel/GetIDVAlgorithm.h>
#include <Kernel/IDistanceCalculator.h>
#include "Kernel/IPVReFitter.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Event/Particle.h"

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : GeometryTupleTool
//
// 2007-11-07 : Jeremie Borel
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
// actually acts as a using namespace TupleTool
DECLARE_COMPONENT( TupleToolGeometry )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolGeometry::TupleToolGeometry( const std::string& type,
                                        const std::string& name,
                                        const IInterface* parent )
    : TupleToolBase ( type, name , parent )
{
  declareInterface<IParticleTupleTool>(this);
  declareProperty("RefitPVs",m_refitPVs=false,
                  "Refit PVs when doing next best PV checks");
  declareProperty("PVReFitter", m_pvReFitterName = "LoKi::PVReFitter:PUBLIC",
                  "PV refitter algorithm name (':PUBLIC' at end of algo name makes sure a public instance is used)" );
  declareProperty("FillMultiPV",m_fillMultiPV=false,
                  "Fill Multi PV arrays");

  //declareProperty("FillMother",m_fillMother=true,
  //                "Turn false if the mother is expected to be NULL, will not fill mother PV info");
  // replaced by Verbose

  //setProperty( "OutputLevel", 1 );

}

//=============================================================================

StatusCode TupleToolGeometry::initialize()
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  m_dva = Gaudi::Utils::getIDVAlgorithm ( contextSvc(), this ) ;
  if (!m_dva) return Error("Couldn't get parent DVAlgorithm");

  m_dist = m_dva->distanceCalculator();
  if ( !m_dist )
  {
    return Error("Unable to retrieve the IDistanceCalculator tool");
  }

  m_pvReFitter = tool<IPVReFitter>( m_pvReFitterName, this );
  if ( !m_pvReFitter )
  {
    return Error( "Unable to retrieve IPVReFitter instance" );
  }

  return sc;
}

//=============================================================================
StatusCode TupleToolGeometry::fill( const Particle* mother
                                    , const Particle* P
                                    , const std::string& head
                                    , Tuples::Tuple& tuple )
{
  const auto prefix = fullName(head);

  StatusCode sc = StatusCode::SUCCESS;

  Assert( P && m_dist && m_dva
          , "No mother or particle, or tools misconfigured." );

  if ( isPureNeutralCalo(P) )
    return Warning( "Will not fill geometry tuple for neutral Calo particles. No worry",
                    StatusCode::SUCCESS, 10);
  if ( msgLevel(MSG::VERBOSE) )
    verbose() << "TupleToolGeometry::fill " << mother << " " << P << " " << prefix
              << endmsg ;

  //fill min IP
  if ( isVerbose() || m_fillMultiPV )
  {
    sc = fillMinIP(P,prefix,tuple);
    if ( sc.isFailure() )
    {
      return Warning("Could not fill minimum IP", StatusCode::FAILURE, 1);
    }
  }
  //=========================================================================
  //fill end vertex info
  //=========================================================================
  if( P->isBasicParticle() )
  {
    if (msgLevel(MSG::VERBOSE))
      verbose() << "No need to look for endVertex of " << prefix << endmsg ;
  }
  else
  {
    //=========================================================================
    if ( msgLevel(MSG::VERBOSE) && P->endVertex() )
    { // https://savannah.cern.ch/bugs/?92524
      verbose() << "Before cast : " << P->endVertex() << endmsg ;
      verbose() << "Container " << P->endVertex()->parent()->registry()->identifier()
                << " key " << P->endVertex()->key() << endmsg ;
    }

    const auto evtx = P->endVertex();
    if ( !evtx )
    {
      fatal() << "Can't retrieve the end vertex for " << prefix
              << ". TupleToolGeometry cannot act on such a particle. Define a branch" << endmsg ;
      fatal() << "See https://twiki.cern.ch/twiki/bin/view/LHCb/DaVinciTutorial7#Branches" << endmsg ;
      return StatusCode::FAILURE ;
    }

    if ( msgLevel(MSG::VERBOSE) )
    { // https://savannah.cern.ch/bugs/?92524
      verbose() << "End Vertex : " << *evtx << endmsg ;
      verbose() << "Container " << evtx->parent()->registry()->identifier()
                << " key " << evtx->key() << endmsg ;
    }
    // end vertex
    sc = fillVertex(evtx,prefix+"_ENDVERTEX",tuple);
    if ( sc.isFailure() )
    {
      return Warning("Could not fill Endvertex "+prefix, sc, 1);
    }
  }

  const VertexBase* aPV = nullptr;

  //=========================================================================
  // fill IP for Particles's Own BPV.. if it isn't the mother!
  //=========================================================================
  if ( true )
  {
    aPV = m_dva->bestVertex ( P );
    if ( aPV && msgLevel(MSG::VERBOSE) )
      verbose() << "Got best PV of particle : " << *aPV  << endmsg  ;
    sc = fillVertexFull(aPV,P,prefix,"_OWNPV",tuple);
    if ( sc.isFailure() )
    {
      return Warning("Could not fill best PV", sc, 1);
    }
  }
  //=========================================================================
  // fill IP for head of chain's own BPV
  //=========================================================================
  if ( mother && isVerbose() )
  {
    aPV = m_dva->bestVertex ( mother );
    if ( aPV && msgLevel(MSG::VERBOSE))
      verbose() << "Got best PV of mother : " << *aPV  << endmsg  ;
    sc = fillVertexFull(aPV,P,prefix,"_TOPPV",tuple);
    if ( sc.isFailure() )
    {
      return Warning("Could not fill TOP PV", sc, 1);
    }
  }
  //=========================================================================
  // fill IP wrt Mother's decay vertex, if it isn't the mother!
  //=========================================================================
  if ( mother && mother != P)
  {
    aPV = originVertex( mother, P );
    if(aPV && msgLevel(MSG::VERBOSE))
      verbose() << "Got originVertex of mother : " << *aPV << endmsg  ;
    if ( isVerbose() )
    {
      sc = fillVertexFull(aPV,P,prefix,"_ORIVX",tuple);
      if ( sc.isFailure() )
      {
        return Warning("Could not fill VertexFull "+prefix, sc, 1);
      }
    }
    else
    {
      sc = fillVertex(aPV,prefix+"_ORIVX",tuple);
      if ( sc.isFailure() )
      {
        return Warning("Could not fill ORIVX", sc, 1);
      }
      if( !P->isBasicParticle() ) sc = fillFlight(aPV,P,prefix,tuple,"_ORIVX");
      if ( sc.isFailure() )
      {
        return Warning("Could not fill Flight", sc, 1);
      }

    }

    if ( sc.isFailure() ) return sc;
  }
  //=========================================================================

  return sc ;
}

//=========================================================================
//  Fill Everything for this vertex for related PV
//=========================================================================
StatusCode TupleToolGeometry::fillVertexFull(const LHCb::VertexBase* vtx,
                                             const LHCb::Particle* P,
                                             const std::string& prefix,
                                             const std::string& vtx_name,
                                             Tuples::Tuple& tuple) const
{
  if ( !vtx ) ++counter("Can't retrieve the " +vtx_name+ " vertex for " + prefix );
  auto sc = fillVertex(vtx,prefix+vtx_name,tuple);
  if ( sc.isFailure() )
  {
    return Warning("Could not fill Endvertex "+prefix, sc, 1);
  }
  sc = fillBPV(vtx,P,prefix,tuple,vtx_name);
  if ( sc.isFailure() )
  {
    return Warning("Could not fillBPV "+prefix, sc, 1);
  }
  if( !P->isBasicParticle() )
  {
    sc = fillFlight(vtx,P,prefix,tuple,vtx_name);
    if ( sc.isFailure() ) Warning("Error in fillFlight "+prefix, StatusCode::SUCCESS, 1).ignore();
  }
  return sc;
}

//=========================================================================
//  Fill PV for related PV
//=========================================================================
StatusCode TupleToolGeometry::fillBPV( const VertexBase* primVtx
                                       , const Particle* P
                                       , const std::string& prefix
                                       , Tuples::Tuple& tuple
                                       , const std::string& trail) const {
  bool test = true ;

  double ip{-999}, chi2{-999};

  if ( !primVtx )
  {
    ++counter("No BPV for "+prefix);
  }
  else
  {
    test &= m_dist->distance ( P, primVtx, ip, chi2 );
    if ( !test ) { ip = chi2 = -1; }
  }

  test &= tuple->column( prefix + "_IP"+trail,       ip );
  test &= tuple->column( prefix + "_IPCHI2"+trail, chi2 );

  if (!test) Warning("Error in fillBPV "+prefix, StatusCode::SUCCESS, 1).ignore();
  return StatusCode(test) ;
}
//=========================================================================
//  Fill PV for all PV
//=========================================================================
StatusCode TupleToolGeometry::fillMinIP( const Particle* P,
                                         const std::string& prefix,
                                         Tuples::Tuple& tuple ) const
{
  bool test = true ;
  // minimum IP
  double ipmin = -1;
  double minchi2 = -1 ;

  double ipminnextbest = -1;
  double minchi2nextbest = -1;

  if(msgLevel(MSG::VERBOSE)) verbose() << "Looking for Min IP"  << endmsg  ;
  const auto PVS = m_dva->primaryVertices();
  if(msgLevel(MSG::VERBOSE)) verbose() << "PV size: "  << PVS.size() << endmsg  ;

  std::vector<double> ips, ipchi2s, diras;
  if ( !PVS.empty() )
  {
    if(msgLevel(MSG::VERBOSE)) verbose() << "Filling IP " << prefix + "_MINIP : "
                                         << P << " PVs:" << PVS.size() << endmsg ;

    for ( const auto& pv : PVS )
    {
      RecVertex newPV(*pv);
      if (m_refitPVs)
      {

        StatusCode scfit = m_pvReFitter->remove(P, &newPV);
        if(!scfit) { Warning("ReFitter fails!",StatusCode::SUCCESS,10).ignore(); continue; }
      }

      double ip{0}, chi2{0};
      //StatusCode test2 = m_dist->distance ( P, *pv, ip, chi2 );

      auto newPVPtr = (LHCb::VertexBase*)&newPV;
      const auto test2 = m_dist->distance ( P, newPVPtr, ip, chi2 );
      ips.push_back(ip);
      ipchi2s.push_back(chi2);
      if ( P->endVertex()) { diras.push_back(dira(newPVPtr,P)); }
      if ( test2 && isVerbose() )
      {
        if ( ip<ipmin || ipmin<0. )
        {
          ipminnextbest = ipmin;
          ipmin = ip ;
        }
        else
        {
          if( ip < ipminnextbest || ipminnextbest < 0 )
          {
            ipminnextbest = ip;
          }
        }

        if ( chi2<minchi2 || minchi2<0. )
        {
          minchi2nextbest = minchi2;
          minchi2 = chi2 ;
        }
        else
        {
          if( chi2 < minchi2nextbest || minchi2nextbest < 0 )
          {
            minchi2nextbest = chi2;
          }
        }
      }
    }
  }
  if ( isVerbose() )
  {
    if ( msgLevel(MSG::VERBOSE) )
    {
      verbose() << "Filling IP " << prefix + "_MINIP " << ipmin << " at " << minchi2 << endmsg  ;
      verbose() << "Filling IP next best " << prefix + "_MINIP " << ipminnextbest << " at "
                << minchi2nextbest << endmsg  ;
    }
    test &= tuple->column( prefix + "_MINIP", ipmin );
    test &= tuple->column( prefix + "_MINIPCHI2", minchi2 );

    test &= tuple->column( prefix + "_MINIPNEXTBEST", ipminnextbest );
    test &= tuple->column( prefix + "_MINIPCHI2NEXTBEST", minchi2nextbest );
  }
  if (m_fillMultiPV)
  {
    test &= tuple->farray( prefix + "_AllIP", ips, "nPV", m_maxPV );
    test &= tuple->farray( prefix + "_AllIPchi2", ipchi2s, "nPV", m_maxPV );
    if (!diras.empty()) test &= tuple->farray( prefix + "_AllDIRA", diras, "nPV", m_maxPV );
    // --------------------------------------------------
  }

  if(msgLevel(MSG::VERBOSE))
    verbose() << "Return from fillMinIP: " << prefix  << " " << test << endmsg;
  if (!test)  Warning("Error in fillMinIP", StatusCode::FAILURE, 1);
  return StatusCode(test) ;
}
//=========================================================================
// fill vertex stuff
//=========================================================================
StatusCode TupleToolGeometry::fillVertex( const LHCb::VertexBase* vtx,
                                          const std::string& vtx_name,
                                          Tuples::Tuple& tuple ) const
{
  bool test = true ;

  // decay vertex information:
  if ( !vtx )
  {
    Gaudi::XYZPoint pt(-999.,-999.,-999.) ; // arbitrary point
    test &= tuple->column( vtx_name+"_", pt );
    test &= tuple->column( vtx_name + "_XERR", -999. );
    test &= tuple->column( vtx_name + "_YERR", -999. );
    test &= tuple->column( vtx_name + "_ZERR", -999. );
    test &= tuple->column( vtx_name + "_CHI2", -999. );
    test &= tuple->column( vtx_name + "_NDOF", -1 );
    test &= tuple->matrix( vtx_name + "_COV_", Gaudi::SymMatrix3x3()  );
  }
  else
  {
    const auto & m = vtx->covMatrix ();
    test &= tuple->column( vtx_name+"_", vtx->position() );
    test &= tuple->column( vtx_name + "_XERR", std::sqrt( m(0,0) ) );
    test &= tuple->column( vtx_name + "_YERR", std::sqrt( m(1,1) ) );
    test &= tuple->column( vtx_name + "_ZERR", std::sqrt( m(2,2) ) );
    test &= tuple->column( vtx_name + "_CHI2", vtx->chi2() );
    test &= tuple->column( vtx_name + "_NDOF", vtx->nDoF() );
    test &= tuple->matrix( vtx_name + "_COV_", m );
  }

  // --------------------------------------------------
  if (!test) Warning("Error in fillVertex "+vtx_name, StatusCode::SUCCESS, 1).ignore();
  return StatusCode(test) ;

}
//=========================================================================
// fill flight distance, angle...
//=========================================================================
StatusCode TupleToolGeometry::fillFlight( const VertexBase* oriVtx,
                                          const Particle* P,
                                          const std::string& prefix,
                                          Tuples::Tuple& tuple,
                                          const std::string& trail ) const
{
  bool test = true ;
  // --------------------------------------------------
  if ( !oriVtx )
  {
    test &= tuple->column( prefix + "_FD"+trail, -999. );
    test &= tuple->column( prefix + "_FDCHI2"+trail, -999. );
    test &= tuple->column( prefix + "_DIRA"+trail, -999.);
  }
  else
  {

    // flight distance
    double dist{0}, chi2{0};
    const StatusCode sc = m_dist->distance( oriVtx, P->endVertex(), dist, chi2 );

    // always fill variables to make sure branches do not change ...
    test &= tuple->column( prefix + "_FD"+trail,     sc ? dist : -999 );
    test &= tuple->column( prefix + "_FDCHI2"+trail, sc ? chi2 : -999 );
    // --------------------------------------------------
    // cosine of (flight distance) dot (momentum):
    // find the origin vertex. Either the primary or the origin in the
    // decay
    test &= tuple->column( prefix + "_DIRA"+trail, sc ? dira(oriVtx,P) : -999 );

    if ( sc.isFailure() ) return sc ;
  }

  if (!test) Warning("Error in fillFlight "+prefix, StatusCode::SUCCESS, 1).ignore();
  return StatusCode(test);
}
// =====================================================
// find origin vertex in the decay chain
// =====================================================
const VertexBase* TupleToolGeometry::originVertex( const Particle* top,
                                                   const Particle* P ) const
{
  //this used to pass back zero if P was a basic particle.
  //I don't think that's necessary. R Lambert 2009-08-14
  if ( top == P || top->isBasicParticle() ) return nullptr;

  const auto & dau = top->daughters();
  if ( dau.empty() ) return nullptr;

  for ( const auto d : dau )
  {
    if( P == d )
    { // I found the daughter
      if(msgLevel(MSG::VERBOSE))
        verbose() << "It's a daughter, retrning mother's endvertex : "
                  << endmsg;
      return top->endVertex();
    }
  }

  // vertex not yet found, get deeper in the decay:
  for ( const auto d : dau )
  {
    if ( P != d && !d->isBasicParticle() )
    {
      const auto vv = originVertex( d, P );
      if ( msgLevel(MSG::VERBOSE) ) verbose() << "Went up : " << vv  << endmsg  ;
      if ( vv ) return vv;
    }
  }
  return nullptr;
}

double TupleToolGeometry::dira( const LHCb::VertexBase* oriVtx,
                                const LHCb::Particle* P ) const
{
  if ( !P || !oriVtx )
  {
    Exception("Wrong use of dira");
    return -1501. ;
  }
  const auto evtx = P->endVertex();
  if ( !evtx )
  {
    Warning("Cannot find end vertex",StatusCode::SUCCESS,1).ignore();
    return -999.;
  }
  const auto A = P->momentum().Vect();
  const auto B = evtx->position() - oriVtx->position();
  return A.Dot( B ) / std::sqrt( A.Mag2()*B.Mag2() );
}
