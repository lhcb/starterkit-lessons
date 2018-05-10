// $Id: TupleToolBremInfo.cpp
// Include files

// local
#include "TupleToolBremInfo.h"
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
#include "Event/Particle.h"
#include "GaudiKernel/IRegistry.h" //
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleProperty.h"
#include "CaloUtils/CaloMomentum.h"
#include <boost/algorithm/string.hpp>
//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolBremInfo
//
//
//-----------------------------------------------------------------------------

using namespace Gaudi;
using namespace LHCb;

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolBremInfo::TupleToolBremInfo( const std::string& type,
                                      const std::string& name,
                                      const IInterface* parent )
  : TupleToolBase ( type, name , parent ),
    m_parts(),
    m_pids()
{
  declareInterface<IParticleTupleTool>(this);
  declareProperty("Particle",m_parts);
  declareProperty("fullDST",m_dst=true);// to switch off/on information not available on microDST
  m_parts.push_back("e+");
  m_parts.push_back("gamma"); // for gamma->e+e-
}

StatusCode TupleToolBremInfo::initialize()
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  m_adder = tool<IBremAdder>("BremAdder","BremAdder" ,this );

  m_ppsvc =      svc<LHCb::IParticlePropertySvc>("LHCb::ParticlePropertySvc",true) ;
  if ( !m_parts.empty() ){
    for ( std::vector<std::string>::const_iterator p = m_parts.begin();m_parts.end() != p; ++p ){
      const LHCb::ParticleProperty* pp = m_ppsvc->find( *p );
      if (pp) { m_pids.push_back( pp->pdgID().abspid() ); }
    }
  }

  return sc;
}

//=============================================================================
StatusCode TupleToolBremInfo::fill(const Particle*,
                                   const Particle* P,
                                   const std::string& head,
                                   Tuples::Tuple& tuple )
{
  if( !P ) return StatusCode::SUCCESS;

  const std::string prefix = fullName(head);

  bool ok=false;
  for ( std::vector<unsigned int>::const_iterator pid = m_pids.begin();
        m_pids.end() != pid; ++pid )  {
    if (  P->particleID().abspid() == *pid ) { ok=true; break; }
  }
  if ( !ok ) return StatusCode::SUCCESS;


  
  bool filltuple = true;
  if( P->isBasicParticle() && P->proto() != NULL && P->proto()->track() != NULL ){
    LHCb::CaloMomentum brem =m_adder->bremMomentum( P, Gaudi::Utils::toString(P->particleID().pid()) );
    if ( msgLevel(MSG::VERBOSE) ){ // https://savannah.cern.ch/bugs/?92524
      verbose() << "Multi : " << brem.multiplicity() << endmsg ;
      verbose() << " Brem P : " << brem.momentum() << endmsg ;
      verbose() << " Origin : " << brem.referencePoint() << endmsg ;
    }
    
    //brem info from the particle (from creation)
    filltuple &= tuple->column( prefix+"_HasBremAdded"    , P->info(LHCb::Particle::HasBremAdded,0.)> 0 ) ; 
    filltuple &= tuple->column( prefix+"_BremMultiplicity",  P->info(LHCb::Particle::HasBremAdded,0.)); // replace the multiplicity() method that need fullDST
    //brem info from rerunning the BremAdder, looking at the photons
    //This is not correct when running on an mDST - you look at a subset of the total photons in the event.
    if((int)  P->info(LHCb::Particle::HasBremAdded,0.) != (int)brem.multiplicity() && m_dst)
      Warning("TupleToolBremInfo requires fullDST -  BremP and BremOrigin might not be reliable (Multiplicity is OK)",StatusCode::SUCCESS,5);
    if(m_dst)filltuple &= tuple->column( prefix+"_BremP"           , brem.momentum());  // need fullDST 
    if(m_dst)filltuple &= tuple->column( prefix+"_BremOrigin"      , brem.referencePoint()); // need fullDST

  }else{
    // e.g. diElectron
    LHCb::Particle::ConstVector  daughters = P->daughtersVector();
    if( daughters.size() != 2)return StatusCode(filltuple);
    
    const LHCb::Particle* p1 = NULL;
    const LHCb::Particle* p2 = NULL;
    int k=0;
    for( LHCb::Particle::ConstVector::const_iterator itmp = daughters.begin(); itmp!=daughters.end(); itmp++){
      const LHCb::Particle* p= *itmp;
      if( p1 == NULL)p1=p;
      else p2=p;
      for ( std::vector<unsigned int>::const_iterator pid = m_pids.begin();m_pids.end() != pid; ++pid )  {
        if (  p->particleID().abspid() == *pid ) { k++;break;}
      }
    }
    if( k!=2 || p1 == NULL || p2 == NULL)return StatusCode(filltuple);
    // OK let's evaluate the brem for (p1,p2) system

    double mult = p1->info(LHCb::Particle::HasBremAdded,0.) + p2->info(LHCb::Particle::HasBremAdded,0.);
    const std::pair<LHCb::CaloMomentum,LHCb::CaloMomentum> brems=m_adder->bremMomenta(p1,p2);
    LHCb::CaloMomentum b1 = brems.first;
    LHCb::CaloMomentum b2 = brems.second;
    filltuple &= tuple->column( prefix+"_DiEBremMultiplicity",  mult );
    if(m_dst)filltuple &= tuple->column( prefix+"_DiEBremP",  b1.momentum()+b2.momentum() );
    std::string name1 = property( p1->particleID())->name() ;
    std::string name2 = property( p2->particleID())->name() ;
    boost::replace_all(name1,"+","plus"); 
    boost::replace_all(name1,"-","minus"); 
    boost::replace_all(name2,"+","plus"); 
    boost::replace_all(name2,"-","minus");
    filltuple &= tuple->column( prefix+"_"+name1+"_DiEBremMultiplicity",   p1->info(LHCb::Particle::HasBremAdded,0.) );
    if(m_dst)filltuple &= tuple->column( prefix+"_"+name1+"_DiEBremP",  b1.momentum() );
    filltuple &= tuple->column( prefix+"_"+name2+"_DiEBremMultiplicity",   p2->info(LHCb::Particle::HasBremAdded,0.) );
    if(m_dst)filltuple &= tuple->column( prefix+"_"+name2+"_DiEBremP",  b2.momentum() );

    // standalone evaluation
    if( m_dst ){
      LHCb::CaloMomentum sb1=m_adder->bremMomentum(p1);
      LHCb::CaloMomentum sb2=m_adder->bremMomentum(p2);
      filltuple &= tuple->column( prefix+"_"+name1+"_StandaloneBremMultiplicity",  sb1.multiplicity()  );
      filltuple &= tuple->column( prefix+"_"+name2+"_StandaloneBremMultiplicity",  sb2.multiplicity()  );
    }
    
  }

  
  return StatusCode(filltuple);
}

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolBremInfo )
