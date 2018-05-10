// $Id: TupleToolIsolationTwoBody.cpp
// from Gaudi
#include "Kernel/Particle2MCLinker.h"
#include "Kernel/IParticle2MCAssociator.h"
#include "LoKi/LoKi.h"
#include "LoKi/PhysTypes.h"
#include "LoKi/Functor.h"
#include "LoKi/CoreCuts.h"
#include "LoKi/Combiner.h"
#include "Event/VertexBase.h"
#include "Event/RecVertex.h"
#include "Event/ProtoParticle.h"
#include "Event/Particle.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <Kernel/IDVAlgorithm.h>
#include <Kernel/GetIDVAlgorithm.h>
#include "TMath.h"
// local
#include "TupleToolIsolationTwoBody.h"
#include "Kernel/IParticleCombiner.h"
// Include files
#include "Event/Particle.h"
// kernel
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
#include "GaudiKernel/PhysicalConstants.h"
#include <Kernel/IDistanceCalculator.h>
#include "Kernel/IPVReFitter.h"
// MC stuff
#include "Event/GenHeader.h"
#include "Event/MCHeader.h"


using namespace LHCb ;
using namespace std;

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolIsolationTwoBody
//
// 2012-03-17 : Fatima Soomro, based on IsolationTwoBody (in DaVinciTools) by Giampiero Mancinelli
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolIsolationTwoBody )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolIsolationTwoBody::TupleToolIsolationTwoBody( const std::string& type,
                                                        const std::string& name,
                                                        const IInterface* parent )
    : TupleToolBase ( type, name, parent )
    , m_Geom(0)
    , m_angle(0)
    , m_fc(0)
    , m_doca(0)
    , m_ips(0)
    , m_svdis(0)
    , m_svdis_h(0)
    , m_pvdis(0)
    , m_pvdis_h(0)
    , m_ghost(0)
    , m_clone(0)
    , m_trchi2(0)
    , m_hltgood(0)
    , m_tracktype(0)
    , m_PVInputLocation(LHCb::RecVertexLocation::Primary)
    , m_p2mcAssoc(0)
    , m_dva(0)
    , m_p2mcAssocType("DaVinciSmartAssociator")
{
  declareInterface<IParticleTupleTool>(this);
  declareProperty("ParticlePath",
                  m_ParticlePath="/Event/Phys/StdAllNoPIDsPions/Particles");
  declareProperty("angle"     , m_angle  = 0.27     ); //
  declareProperty("fc"     , m_fc  = 0.60     ); //
  declareProperty("doca"     , m_doca  = 0.13     ); //
  declareProperty("ips"     , m_ips  = 3.0     ); //
  declareProperty("svdis"     , m_svdis  = -0.15     ); //
  declareProperty("svdis_h"     , m_svdis_h  = 30.     ); //
  declareProperty("pvdis"     , m_pvdis  = 0.5     ); //
  declareProperty("pvdis_h"     , m_pvdis_h  = 40.    ); //
  declareProperty("ghost"     , m_ghost  = 1000000.    ); //
  declareProperty("clone"     , m_clone  = -1000000.    ); //
  declareProperty("trchi2"     , m_trchi2  = 1000000.    ); //
  declareProperty("tracktype"     , m_tracktype  = 3     ); //
  declareProperty("hltgood"     , m_hltgood  = -10     ); //
  declareProperty("PVInputLocation",m_PVInputLocation);
  declareProperty("MuChi2", m_MuChi2 = true);
  declareProperty("isMC", m_isMC = false);

  declareProperty( "IP2MCPAssociatorType", m_p2mcAssocType);
}

//=============================================================================
// Initialize
//=============================================================================
StatusCode TupleToolIsolationTwoBody::initialize()
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  m_dva = Gaudi::Utils::getIDVAlgorithm ( contextSvc(), this ) ;
  if ( !m_dva ) { return Error( "Couldn't get parent DVAlgorithm",
                                StatusCode::FAILURE ); }

  m_p2mcAssoc = tool<IParticle2MCAssociator>( m_p2mcAssocType, this );
  m_combiner  = m_dva->particleCombiner();
  m_Geom      = m_dva->distanceCalculator();

  return sc;
}

//=============================================================================
// Fill
//=============================================================================
StatusCode TupleToolIsolationTwoBody::fill( const LHCb::Particle * /* top */,
                                            const LHCb::Particle *part,
                                            const std::string &   head,
                                            Tuples::Tuple &   tuple   )
{

  const std::string prefix=fullName(head);

  if ( msgLevel(MSG::DEBUG) )
    debug()<<"filling tuple for "<<prefix<< endmsg;

  StatusCode sc;
  if( part->isBasicParticle() && m_MuChi2){ sc = MuChi2(part, prefix, tuple); }

  if( part->isBasicParticle() || part->daughtersVector().size()!=2 ){

    if ( msgLevel(MSG::DEBUG) )
      debug()<<"not filling for "<< fullName(head)<<endmsg;
    return StatusCode::SUCCESS;
  }
  else  {

    bool shdBeTrue=true;
    const LHCb::Particle::ConstVector & parts = part->daughtersVector();

    for (  Particle::ConstVector::const_iterator ip_part = parts.begin(); ip_part != parts.end(); ip_part++) {
      if( !((*ip_part)->isBasicParticle()) )  shdBeTrue=false;
      if ( !shdBeTrue && msgLevel(MSG::DEBUG) )
        debug()<<"Found compositle particle "<< (*ip_part)->particleID().pid()
               <<" in the daughters of"<< fullName(head)<<endmsg;

    }

    if(! shdBeTrue) return StatusCode::SUCCESS;
  }

  //  float* massiso; float* chi2iso; int*iso;

  //   const LHCb::VertexBase* PV = m_dva->bestPV(part);
  //   const LHCb::VertexBase* SV = part->endVertex();
  if ( msgLevel(MSG::DEBUG) ) debug()<<" calling getiso "<<endmsg;
  sc = getIso( part, prefix, tuple);

  //  sc  &= tuple->column( prefix+"_test", massiso[0]);

  return sc;
}


//=============================================================================
// F.Soomro, 9 March 12
// Fill MuonChi2DOF for the track, for Walter
//=============================================================================
StatusCode TupleToolIsolationTwoBody::MuChi2( const LHCb::Particle* P
                                              , const std::string& prefix
                                              , Tuples::Tuple& tuple ){
  //const std::string prefix=fullName(head);

  bool test = true;
  if( !P ) return StatusCode::FAILURE;

  //first just return if the particle isn't supposed to have a track
  if( !P->isBasicParticle() ) return StatusCode::SUCCESS;

  const LHCb::ProtoParticle* protop = P->proto();
  if(!protop) return StatusCode::SUCCESS;
  const LHCb::Track* track = protop->track();
  if(!track) return StatusCode::SUCCESS;

  //  double muchi2 = track->info(LHCb::Track::AdditionalInfo::MuonChi2perDoF , -1);
  //  test += tuple->column( prefix+"_MuonChi2DOF", muchi2);

  double  muchi2 = -1;

  if ( msgLevel(MSG::DEBUG) )
    debug()<<"checking for muonpids at "<<LHCb::MuonPIDLocation::Default<<endmsg;
  LHCb::MuonPIDs* pMuids =NULL;
  if(exist<LHCb::MuonPIDs>(LHCb::MuonPIDLocation::Default)) pMuids=get<LHCb::MuonPIDs>(LHCb::MuonPIDLocation::Default);
  if (!pMuids){
    error() << " Failed to get pMuids" <<  LHCb::MuonPIDLocation::Default << endmsg;
    return StatusCode::FAILURE;
  }

  if ( msgLevel(MSG::DEBUG) )
    debug()<<" found it, the size is "<<pMuids->size()<<endmsg;
  const LHCb::Track *mutrack=NULL;
  LHCb::MuonPIDs::const_iterator ip;
  for( ip = pMuids->begin(); ip != pMuids->end() ; ip++){
    if ((*ip)->idTrack() == track){
      mutrack = (*ip)->muonTrack();
      if (!mutrack) continue;
      if ( msgLevel(MSG::DEBUG) ) debug()<<"found a mutrack "<<endmsg;
    }
  } // end of muonIDs loop

  if(mutrack){
    muchi2 = mutrack->info(LHCb::Track::AdditionalInfo::MuonChi2perDoF , -1);
  }

  else{
    if ( msgLevel(MSG::DEBUG) )
      debug()<<"didnt find any mutrack! "<<endmsg;
  }

  if ( msgLevel(MSG::DEBUG) )
    debug()<<"muchi2 was "<<muchi2<<endmsg;
  test += tuple->column( prefix+"_MuonChi2DOF", muchi2).getCode();

  // here test is always true!!!
  return test ? StatusCode::SUCCESS : StatusCode::FAILURE;
}

//=============================================================================
// End of Fill MuonChi2DOF
//=============================================================================


//=============================================================================
// get the Isolation
//=============================================================================

StatusCode TupleToolIsolationTwoBody::getIso(const LHCb::Particle *P,
                                             const std::string prefix,
                                             Tuples::Tuple& tuple )
{

  if ( msgLevel(MSG::DEBUG) )
    debug() <<"Now get the Isolation level"<< endmsg;
  StatusCode sc = StatusCode::SUCCESS;

  const LHCb::Particle::ConstVector & parts = P->daughtersVector();
  if ( msgLevel(MSG::DEBUG) ) debug() <<"daughters vector "<< endmsg;
  float massiso[2];
  float chi2iso[2];
  //int iso[2];
  if ( msgLevel(MSG::DEBUG) ) debug() <<"the floats "<< endmsg;
  const LHCb::VertexBase *PV = m_dva->bestVertex(P);
  const LHCb::VertexBase *SV = P->endVertex();
  if ( msgLevel(MSG::DEBUG) ) debug() <<" PV and SV "<< endmsg;

  LHCb::Particles* allparts = get<LHCb::Particles>(m_ParticlePath);
  if (!allparts) {
    error() << " Failed to get particles container "
            <<  m_ParticlePath << endmsg;
    return StatusCode::SUCCESS;
  }

  if ( msgLevel(MSG::DEBUG) )
    debug() <<" have all_parts  with size  "<< allparts->size()<< endmsg;

  Assert( m_p2mcAssoc
          , "The DaVinci smart associator hasn't been initialized!");
  /*
    Particle2MCLinker*    m_linkLinks(0);
    if (m_isMC){

    m_linkLinks = new Particle2MCLinker( this,
    //Particle2MCMethod::Chi2,
    Particle2MCMethod::Links,
    m_ParticlePath);
    if( !m_linkLinks ) {
    fatal() << "Unable to retrieve Link Associator tool"<<endmsg;
    return StatusCode::FAILURE;
    }
    }
  */
  const Gaudi::XYZPoint& PosPV = PV->position();
  const Gaudi::XYZPoint& PosSV = SV->position();
  if ( msgLevel(MSG::DEBUG) ) debug() <<" PV and SV position "<< endmsg;

  int i = 0;
  ROOT::Math::SVector<int, 2> iso5;
  iso5[0]=0;
  iso5[1]=0;
  ROOT::Math::SMatrix<double, 3, 2> o_mu;
  ROOT::Math::SMatrix<double, 3, 2> p_mu;
  ROOT::Math::SVector<double, 2> ptmu;
  int storeCharge[2];
  //Loop over input particles, get their simple kinematics
  Particle::ConstVector::const_iterator ip_part;

  if ( msgLevel(MSG::DEBUG) ) debug () <<"=========== 1"<< endmsg;
  //    IParticle2MCAssociator* m_p2mcAssoc;
  Particle::ConstVector iso_parts_0;
  Particle::ConstVector iso_parts_1;
  //const MCParticle* mclink1 = 0;
  //const MCParticle* mclink2 = 0;

  // thats where you decide which daughter is at [0] and [1] of the arrays
  for ( ip_part = parts.begin(); ip_part != parts.end(); ip_part++)
  {
    const LHCb::Particle* part = *(ip_part);
    //if (m_isMC) {
    //if (i==0) mclink1 = m_p2mcAssoc->relatedMCP(part);
    //if (i==1) mclink2 = m_p2mcAssoc->relatedMCP(part);
    //}

    if (i==0) iso_parts_0.push_back(part) ;
    if (i==1) iso_parts_1.push_back(part) ;
    const LHCb::ProtoParticle * proto =  (*ip_part)->proto();
    const LHCb::Track* track = proto->track();
    storeCharge[i] = part->charge();

    o_mu(0,i) = track->position().x();
    o_mu(1,i) = track->position().y();
    o_mu(2,i) = track->position().z();
    p_mu(0,i) = track->momentum().x();
    p_mu(1,i) = track->momentum().y();
    p_mu(2,i) = track->momentum().z();
    ptmu[i]   = std::sqrt(std::pow(p_mu(0,i),2)+std::pow(p_mu(1,i),2));
    ++i;

  }// ip_part
  int j=0;

  float doca2[2];
  doca2[0]=10000.;
  doca2[1]=10000.;
  float bestdoca[2];
  bestdoca[0]=10000.;
  bestdoca[1]=10000.;
  LHCb::Particle* bestpart_0(0) ;
  LHCb::Particle* bestpart_1(0) ;

  if ( msgLevel(MSG::DEBUG) ) debug () <<"=========== 2"<< endmsg;
  //Loop over all particles
  LHCb::Particles::const_iterator ip;
  for ( ip = allparts->begin(); ip != allparts->end() ; ++ip) {

    if ( msgLevel(MSG::DEBUG) ) debug() <<" looping over all parts "<< endmsg;
    j++;
    const LHCb::ProtoParticle * proto =  (*ip)->proto();
    const LHCb::Track* track = proto->track();
    LHCb::Particle*  cand = (*ip);
    Gaudi::XYZPoint o(track->position());
    Gaudi::XYZVector p(track->momentum());
    bool isInList = 0;
    double pt = p.Rho();
    //double ptot = p.R();

    // skip if other particle is in input list
    if ( msgLevel(MSG::DEBUG) ) debug() <<" check isInList "<< endmsg;
    if (ratio(pt, ptmu[0]) < 0.0001 || ratio(pt,ptmu[1]) <0.0001) {
      isInList = 1;
      if ( msgLevel(MSG::DEBUG) ) debug() <<"isInList is found to be "<<isInList<< endmsg;
    }

    if (!isInList) {
      if ( msgLevel(MSG::DEBUG) ) debug() <<"part inNOTinlist"<< endmsg;

      int i2=0;
      //bool fromB = false;
      //bool fromD = false;
      //float MCID(0);
      //float MCmothID(0);
      const MCParticle* mclink = 0;

      if ( msgLevel(MSG::DEBUG) )
      {
        debug() <<"=========== 3"<< endmsg;
        if (m_isMC) {
          debug() <<"=========== 4"<< endmsg;
          debug() <<"=========== 4a"<< endmsg;
          mclink = m_p2mcAssoc->relatedMCP(cand);
          if(mclink) {
            debug() <<"=========== 5"<< endmsg;
            //fromB = ancestor(mclink)->particleID().hasBottom();
            //fromD = ancestor(mclink)->particleID().hasCharm();
            //MCID = mclink->particleID().pid();
            debug() <<"=========== 6"<< endmsg;
            const MCParticle*mc_mom = mclink->mother();
            if(mc_mom) {
              debug() <<"=========== 7"<< endmsg;
              //MCmothID = mc_mom->particleID().pid();
            }
          }
          debug() <<"=========== 4b"<< endmsg;
        }
      }

      //Loop over input particles (2)
      for ( i2 = 0; i2 < 2; i2++) {

        //bool isolating = false;
        //if (m_isMC) {
        //  if (i2 == 0 && mclink && mclink1) isolating = ancestor(mclink)==ancestor(mclink1);
        //  if (i2 == 1 && mclink && mclink2) isolating = ancestor(mclink)==ancestor(mclink2);
        //}

        int hltgood = -1;
        double fc = 0.;
        Gaudi::XYZPoint omu(o_mu(0,i2),o_mu(1,i2),o_mu(2,i2));
        Gaudi::XYZVector pmu(p_mu(0,i2),p_mu(1,i2),p_mu(2,i2));
        // find roadmap isolation (only long tracks)
        IsHltGood(o, p, omu, pmu ,PosPV, hltgood, fc);
        if ( msgLevel(MSG::DEBUG) ) debug() <<"=========== 8"<< endmsg;

        // find doca and angle between input and other tracks
        Gaudi::XYZPoint vtx(0.,0.,0.);
        double doca(-1.);
        double angle(-1.);
        InCone(omu,pmu, o, p, vtx, doca, angle);
        // find impact parameters, distances from secondary and primary vertex
        doca2[i2]=doca;
        if (doca2[i2]<bestdoca[i2]) {
          if (i2==0) bestpart_0 = cand;
          if (i2==1) bestpart_1 = cand;
          bestdoca[i2] = doca2[i2];
        }
        double imp = 0.;
        double impchi2 = 0.;
        double ips,pvdis,svdis;
        ips=100000.;
        RecVertex::Container::const_iterator iv;
        RecVertex::Container* verts = NULL;
        if(exist<LHCb::RecVertex::Container>(m_PVInputLocation)){
          verts = get<LHCb::RecVertex::Container>(m_PVInputLocation);
        }
        for ( iv = verts->begin(); iv != verts->end(); iv++) {
          m_Geom->distance(&(*cand),(*iv),imp,impchi2);
          if (impchi2<ips) ips = impchi2;
        }
        ips=std::sqrt(ips);
        if ( msgLevel(MSG::DEBUG) ) debug() <<"=========== 9"<< endmsg;

        //double deltaZvtx = (vtx.z()-PosPV.z());

        pvdis = (vtx.z()-PosPV.z())/fabs(vtx.z()-PosPV.z())*(vtx-PosPV).R();
        svdis = (vtx.z()-PosSV.z())/fabs(vtx.z()-PosSV.z())*(vtx-PosSV).R();

        //bool is_iso5 = false;

        float clone = proto->track()->info(LHCb::Track::AdditionalInfo::CloneDist,100000);
        float ghost = proto->track()->ghostProbability();
        double trchi2 = proto->track()->chi2()/proto->track()->nDoF();
        // non-isolating criteria #5
        if (angle <m_angle && fc<m_fc && doca<m_doca && ips>m_ips &&
            svdis>m_svdis && svdis<m_svdis_h && pvdis>m_pvdis && pvdis<m_pvdis_h
            && clone>m_clone && ghost<m_ghost && track->type()==m_tracktype
            && trchi2<m_trchi2 && hltgood>m_hltgood) {
          //is_iso5 = true;
          if (i2==0) iso_parts_0.push_back(cand) ;
          if (i2==1) iso_parts_1.push_back(cand) ;
          iso5[i2] += 1;
          if ( msgLevel(MSG::DEBUG) ) debug()<<"isiso "<< i2 << iso5[i2]<< endmsg;
        }


      }// i<2

    } // (!isInList)
  }// allparts

  massiso[0]=0.;
  chi2iso[0]=-100.;
  massiso[1]=0.;
  chi2iso[1]=-100.;

  LHCb::Vertex   vertex0   ;
  LHCb::Particle mother0  ;
  LHCb::Vertex   vertex1   ;
  LHCb::Particle mother1  ;
  if ( msgLevel(MSG::DEBUG) ) debug() <<"=========== 11"<< endmsg;

  if (iso_parts_0.size()>1) {

    StatusCode sc0 = StatusCode::SUCCESS;
    sc0 = m_combiner->combine(iso_parts_0,mother0,vertex0 ) ;
    if(sc0==StatusCode::SUCCESS) {
      massiso[0] = mother0.measuredMass();
      chi2iso[0] = mother0.endVertex()->chi2();
      chi2iso[0] = chi2iso[0]/mother0.endVertex()->nDoF();
    }
  }

  else {
    iso_parts_0.push_back(bestpart_0);
    if ( msgLevel(MSG::DEBUG) ) debug()<<"trying to get to this stage "<<endmsg;
    StatusCode sc0a = StatusCode::SUCCESS;
    sc0a = m_combiner->combine(iso_parts_0,mother0,vertex0 ) ;
    if(sc0a==StatusCode::SUCCESS) {
      chi2iso[0] = mother0.endVertex()->chi2();
      chi2iso[0] = chi2iso[0]/mother0.endVertex()->nDoF();
    }
  }


  if (iso_parts_1.size()>1) {
    StatusCode sc1 = StatusCode::SUCCESS;
    sc1 = m_combiner->combine(iso_parts_1,mother1,vertex1 ) ;
    if(sc1==StatusCode::SUCCESS) {
      massiso[1] = mother1.measuredMass();
      chi2iso[1] = mother1.endVertex()->chi2();
      chi2iso[1] = chi2iso[1]/mother1.endVertex()->nDoF();
    }
  }

  else {
    iso_parts_1.push_back(bestpart_1);
    StatusCode sc1a = StatusCode::SUCCESS;
    sc1a = m_combiner->combine(iso_parts_1,mother1,vertex1 ) ;
    if(sc1a==StatusCode::SUCCESS) {
      chi2iso[1] = mother1.endVertex()->chi2();
      chi2iso[1] = chi2iso[1]/mother1.endVertex()->nDoF();
    }
  }

  if ( msgLevel(MSG::DEBUG) ) debug()<<"NTRACKS "<<j<<endmsg;

  // this is idiotic, but oh well...

  //iso[0] = iso5[0];
  //iso[1] = iso5[1];

  //    fatima, fill here
  if ( msgLevel(MSG::DEBUG) )
  {
    debug()<<"massiso " << massiso[0]<<"  "<<massiso[1]<<" chi2iso " << chi2iso[0]<<"  "<<chi2iso[1]<<endmsg;
    debug()<<" Usual iso5 " << iso5[0]<<"  "<< iso5[1]<<endmsg;
  }

  bool test = tuple->column( prefix+"_P1_massiso", massiso[0]).isSuccess();
  test  &= tuple->column( prefix+"_P2_massiso", massiso[1]);

  test  &= tuple->column( prefix+"_P1_Q", storeCharge[0]);
  test  &= tuple->column( prefix+"_P2_Q", storeCharge[1]);

  test  &= tuple->column( prefix+"_P1_chi2iso", chi2iso[0]);
  test  &= tuple->column( prefix+"_P2_chi2iso", chi2iso[1]);

  test  &= tuple->column( prefix+"_P1_iso5", iso5[0]);
  test  &= tuple->column( prefix+"_P2_iso5", iso5[1]);

  if(test) return StatusCode::SUCCESS;
  else return StatusCode::FAILURE;
}

//=============================================================================
//
//=============================================================================
void TupleToolIsolationTwoBody::IsHltGood(Gaudi::XYZPoint o,Gaudi::XYZVector p,
                                          Gaudi::XYZPoint o_mu,Gaudi::XYZVector
                                          p_mu, Gaudi::XYZPoint PV, int& hltgood,
                                          double& fc) const
{

  Gaudi::XYZVector rv;
  Gaudi::XYZPoint vtx;
  Gaudi::XYZPoint close;
  Gaudi::XYZPoint close_mu;
  bool fail(false);
  closest_point(o,p,o_mu,p_mu,close,close_mu,vtx,fail);
  if (fail) {
    fc = -1.;
    hltgood = -1;
  }
  else {
    double pete = p.Rho();
    rv = vtx - PV;
    double DOCA_b = (close-close_mu).R();
    double ZZ = rv.z();
    fc = pointer(rv,p,p_mu);
    if ((DOCA_b<0.2) && (ZZ>0.) && (ZZ<30.) && (fc<0.4) && (pete>2.)) hltgood = 1;
  }
}


//=============================================================================
//
//=============================================================================
void TupleToolIsolationTwoBody::InCone(Gaudi::XYZPoint o1,
                                       Gaudi::XYZVector p1,Gaudi::XYZPoint o2,
                                       Gaudi::XYZVector p2,
                                       Gaudi::XYZPoint& vtx, double&
                                       doca, double& angle) const
{
  Gaudi::XYZPoint rv;
  Gaudi::XYZPoint close;
  Gaudi::XYZPoint close_mu;
  bool fail(false);
  closest_point(o1,p1,o2,p2,close,close_mu,vtx, fail);
  if (fail) {
    doca =-1.;
    angle=-1.;
  }
  else {
    doca = (close-close_mu).R();
    angle = arcosine(p1,p2);
  }
}

//=============================================================================
//
//=============================================================================
double TupleToolIsolationTwoBody::pointer (Gaudi::XYZVector vertex,
                                           Gaudi::XYZVector p, Gaudi::XYZVector
                                           p_mu) const
{
  double pt=p.Rho()+p_mu.Rho();
  Gaudi::XYZVector ptot(p+p_mu);
  double  num=ptot.R()*sin(arcosine(vertex,ptot));
  double  den=(num+pt);
  double fc = num/den;
  return fc;
}

//=============================================================================
//
//=============================================================================
double TupleToolIsolationTwoBody::ratio( double p1, double p2) const
{
  return TMath::Abs(p1 -p2)*(1./TMath::Abs(p1+p2));
}

//=============================================================================
//
//=============================================================================

double TupleToolIsolationTwoBody::arcosine(Gaudi::XYZVector p1,
                                           Gaudi::XYZVector p2) const
{

  double num=(p1.Cross(p2)).R();
  double den=p1.R()*p2.R();
  double seno = num/den;
  double coseno = p1.Dot(p2)/den;
  double alpha = asin(fabs(seno));
  if (coseno < 0 ) {
    alpha = ROOT::Math::Pi() - alpha;
  }
  return alpha;
}

//=============================================================================
//
//=============================================================================
void TupleToolIsolationTwoBody::closest_point(Gaudi::XYZPoint o,Gaudi::XYZVector p,
                                              Gaudi::XYZPoint o_mu,Gaudi::XYZVector
                                              p_mu, Gaudi::XYZPoint& close1,
                                              Gaudi::XYZPoint& close2,
                                              Gaudi::XYZPoint& vertex, bool&
                                              fail) const
{
  Gaudi::XYZVector v0(o - o_mu);
  Gaudi::XYZVector v1(p.unit());
  Gaudi::XYZVector v2(p_mu.unit());
  Gaudi::XYZPoint temp1(0.,0.,0.);
  Gaudi::XYZPoint temp2(0.,0.,0.);
  fail = false;

  double  d02 = v0.Dot(v2);
  double  d21 = v2.Dot(v1);
  double  d01 = v0.Dot(v1);
  double  d22 = v2.Dot(v2);
  double  d11 = v1.Dot(v1);
  double  denom = d11 * d22 - d21 * d21;
  if (fabs(denom) <= 0.) {
    close1 = temp1;
    close2 = temp2;
    fail = true;
  }
  else {
    double numer = d02 * d21 - d01 * d22;
    double mu1 = numer / denom;
    double mu2 = (d02 + d21 * mu1) / d22;
    close1 = o+v1*mu1;
    close2 = o_mu+v2*mu2;
  }
  vertex = (close1+(close2-close1)*0.5);
}


//=============================================================================
//
//=============================================================================
const LHCb::MCParticle* TupleToolIsolationTwoBody::ancestor(const LHCb::MCParticle*imc) const {
  if (imc->mother()) {
    const LHCb::MCParticle*mc_mom = imc->mother();
    if ( msgLevel(MSG::DEBUG) )
    {
      debug()<<"iso c "<< mc_mom << endmsg;
      debug()<<"iso c "<< mc_mom->momentum() << endmsg;
      debug()<<"iso c "<< mc_mom->particleID() << endmsg;
      debug()<<"iso c "<< mc_mom->mother() << endmsg;
    }
    return ancestor(mc_mom);
  }
  else {
    return imc;
  }
}

//=============================================================================
// Destructor
//=============================================================================
TupleToolIsolationTwoBody::~TupleToolIsolationTwoBody() {
}

//=============================================================================
