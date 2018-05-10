// $Id: TupleToolMassHypo.cpp,v 1.12 2010-04-23 12:01:26 jhe Exp $
// Include files

// local
#include "TupleToolMassHypo.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleProperty.h"
#include "Kernel/IParticleTransporter.h"
#include "Kernel/Escape.h"
//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolMassHypo
//
// 2011-02-10 Patrick Koppenburg
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
// actually acts as a using namespace TupleTool
DECLARE_COMPONENT( TupleToolMassHypo )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolMassHypo::TupleToolMassHypo( const std::string& type,
                                      const std::string& name,
                                      const IInterface* parent )
  : TupleToolBase ( type, name , parent )
  , m_first(true)
  , m_transporter()
{
  declareInterface<IParticleTupleTool>(this);
  declareProperty( "PIDReplacements", m_replace, "List of PID replacements as dictionary of strings");
  declareProperty( "CC", m_revert = true, "Do cc by default");
}

//=============================================================================

StatusCode TupleToolMassHypo::initialize()
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  LHCb::IParticlePropertySvc* ppsvc =
    svc<LHCb::IParticlePropertySvc>("LHCb::ParticlePropertySvc",true) ;

  if (m_revert) info() << "Will apply cc on all replacement pairs" << endmsg ;

  for (std::map<std::string,std::string>::const_iterator iterS = m_replace.begin();
       iterS != m_replace.end(); ++iterS ){
    const LHCb::ParticleProperty* prop1 = ppsvc->find( iterS->first );
    const LHCb::ParticleProperty* prop2 = ppsvc->find( iterS->second );
    if (!prop1 || !prop2 ) {
      err() << "one of " << iterS->first << " " <<  iterS->second << " does nor exist" << endmsg ;
      return StatusCode::FAILURE ;
    }
    int pid = prop1->pdgID().pid() ;
    if ( m_revert) pid = abs(pid);
    m_massMap.insert(std::pair<int,double>(pid,prop1->mass()));
    m_massMap.insert(std::pair<int,double>(prop2->pdgID().pid(),prop2->mass()));
    m_nameMap.insert(std::pair<int,std::string>(pid,Decays::escape(prop1->name())));
    if (msgLevel(MSG::VERBOSE)) verbose() << "Inserted name ``" << Decays::escape(prop1->name()) << "''" << endmsg ;
    m_nameMap.insert(std::pair<int,std::string>(prop2->pdgID().pid(),Decays::escape(prop2->name())));
    if (msgLevel(MSG::VERBOSE)) verbose() << "Inserted name ``" << Decays::escape(prop2->name()) << "''" << endmsg ;
    m_replacePDG.insert( std::pair<int,int>( pid,prop2->pdgID().pid() ));
    if (msgLevel(MSG::VERBOSE)) verbose() << "Inserted PID pair ``" << pid << ","
                                          << prop2->pdgID().pid() << "''" << endmsg ;
  } // iterS
  m_transporter = tool<IParticleTransporter>("ParticleTransporter:PUBLIC", this);

  return sc ;
}

//=============================================================================
StatusCode TupleToolMassHypo::fill( const LHCb::Particle*
                                    , const LHCb::Particle* P
                                    , const std::string& head
                                    , Tuples::Tuple& tuple )
{
  if( !P ) return StatusCode::FAILURE;
  if( P->isBasicParticle() ) return StatusCode::SUCCESS;
  if (msgLevel(MSG::VERBOSE)) verbose() << "Entering with a " << P->particleID().pid() << endmsg ;
  const std::string prefix=fullName(head);
  bool test = true;

  //first just return if the particle is a final state
  LHCb::Particle::ConstVector dv = P->daughtersVector();

  possDecayVec possibilities = getPossibilities(dv);
  if (possibilities.size()==1) {
    Warning("The decay of this particle does not contain the declared PIDs.",
            StatusCode::SUCCESS,1);
    return StatusCode::SUCCESS ; // ignore cases where nothing is to be done
  }

  for (possDecayVec::const_iterator p = possibilities.begin() ; p!=possibilities.end() ; ++p){
    std::string combname ;
    if (msgLevel(MSG::VERBOSE)) verbose() << "Old mass " << P->measuredMass()  << endmsg ;
    double mass = recalculateMass(*p,P->referencePoint(),combname);
    test &= tuple->column( prefix+"_M_with"+combname, mass );
    if (msgLevel(MSG::VERBOSE)) verbose() << "Filled column ``" << prefix+"_M_with"+combname
                                          << "'' with mass " << mass << endmsg ;
  }//

  return StatusCode(test);
}
//=============================================================================
double TupleToolMassHypo::recalculateMass(const possDecay& pd, const Gaudi::XYZPoint& pt,
                                          std::string& combname){
  Gaudi::LorentzVector newM ;
  for ( possDecay::const_iterator p = pd.begin() ; p != pd.end() ; ++p ){
    if (msgLevel(MSG::VERBOSE)) verbose() << "Loop ``" << combname << "'' pid : ``" << p->second << "''"<< endmsg ;
    const LHCb::Particle* P  = p->first;
    if ( m_nameMap.find(p->second)!=m_nameMap.end()) combname += "_"+m_nameMap[p->second] ;
    LHCb::Particle transParticle;
    m_transporter->transport(P, pt.z(), transParticle);
    Gaudi::LorentzVector mom = transParticle.momentum();
    if (P->particleID().pid()!=p->second || (m_revert && P->particleID().pid()!=-p->second)){
      double mm = m_massMap[p->second];
      if (msgLevel(MSG::VERBOSE)) verbose() << "Momentum is " << mom << " new mass = " << mm << endmsg ;
      mom.SetE(sqrt(mom.P2()+(mm*mm)));
      if (msgLevel(MSG::VERBOSE)) verbose() << "New Momentum is " << mom << endmsg ;
    }
    newM += mom ;
    if (msgLevel(MSG::VERBOSE)) verbose() << "Total Momentum is now " << newM << endmsg ;
  }
  if (msgLevel(MSG::VERBOSE)) verbose() << "New mass with " << combname << " is " << newM.M()  << endmsg ;
  return newM.M();
}
//=============================================================================
possDecayVec TupleToolMassHypo::getPossibilities(const LHCb::Particle::ConstVector& dv){
  possDecayVec possibilities;
  // loop over daghters
  for ( LHCb::Particle::ConstVector::const_iterator d1 = dv.begin() ; d1!=dv.end() ; ++d1){
    if (msgLevel(MSG::VERBOSE)) verbose() << "getPossibilities Looping over " << (*d1)->particleID().pid() << endmsg ;
    possibilities = increaseVector(*d1,possibilities);
    if (msgLevel(MSG::VERBOSE)) verbose() << "getPossibilities Size of possibilities " << possibilities.size() << endmsg ;
  }
  if ((possibilities.size()>1) && (msgLevel(MSG::DEBUG) || m_first)) printVec(possibilities);
  if (m_first) m_first = false ;
  return possibilities;
}
//=============================================================================
void TupleToolMassHypo::addPossPair(const LHCb::Particle* d, int pid, possDecay& pm){
  pm.push_back(possPair(d,pid));
  if (msgLevel(MSG::VERBOSE)) verbose() << "addPossPair Added " << pid << " size: " << pm.size() << endmsg ;
}

//=============================================================================
// multiplies the vector
possDecayVec TupleToolMassHypo::increaseVector(const LHCb::Particle* d, possDecayVec& poss){
  int pid = d->particleID().pid();
  if (m_revert) pid = abs(pid);
  if (msgLevel(MSG::VERBOSE)) verbose() << "increaseVector seeing a "
                                        << d->particleID().pid() << " treated as " << pid << endmsg ;
  possDecayVec tmp  ;
  if (poss.empty()){
    possDecay pm;
    addPossPair(d,pid,pm);
    tmp.push_back(pm);
    if (m_replacePDG.find(pid)!=m_replacePDG.end()) {
      possDecay pm2;
      addPossPair(d,m_replacePDG[pid],pm2);
      tmp.push_back(pm2);
    }
  } else {
    if (msgLevel(MSG::VERBOSE)) verbose() << "increaseVector with poss size " << poss.size() << endmsg ;
    for (possDecayVec::const_iterator i = poss.begin() ; i!=poss.end() ; ++i){
      possDecay pm = *i;
      addPossPair(d,pid,pm);
      tmp.push_back(pm);
      if (m_replacePDG.find(pid)!=m_replacePDG.end()){
        int pid2 = m_replacePDG[pid];
        possDecay pm2 = *i;
        addPossPair(d,pid2,pm2);
        tmp.push_back(pm2);
      }
    }
  }
  if (msgLevel(MSG::VERBOSE)) printVec(tmp);
  return tmp ;
}

//=============================================================================
void TupleToolMassHypo::printVec(const possDecayVec& possibilities){
  for ( possDecayVec::const_iterator i = possibilities.begin() ; i!=possibilities.end() ; ++i){
    info() << "Combination: " ;
    for ( possDecay::const_iterator j = i->begin() ; j!=i->end() ; ++j){
      info() << j->first->particleID().pid() << ": " << j->second << ", " ;
    }
    info() << endmsg ;
  }
}
