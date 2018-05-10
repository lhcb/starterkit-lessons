// $Id: TupleToolTagging.cpp,v 1.9 2010-01-27 11:43:50 rlambert Exp $
// Include files

// from Gaudi
#include "GaudiKernel/SmartIF.h"

#include <Kernel/GetIDVAlgorithm.h>
#include <Kernel/IBTaggingTool.h>
#include <Kernel/IDVAlgorithm.h>
#include <Kernel/IDistanceCalculator.h>

// local
#include "TupleToolTagging.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Event/Particle.h"
#include "Event/Vertex.h"
#include "Event/RecVertex.h"
#include <utility>
#include <boost/algorithm/string.hpp>

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : EventInfoTupleTool
//
// 2007-11-07 : Jeremie Borel
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
// actually acts as a using namespace TupleTool
DECLARE_COMPONENT( TupleToolTagging )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolTagging::TupleToolTagging( const std::string& type,
                                    const std::string& name,
                                    const IInterface* parent )
  : TupleToolBase ( type, name , parent )
  , m_dva(0)
  , m_dist(0)
  , m_fitter(0)
  , m_tagging(0)
  , m_tagger_map()
  , m_tagger_rmap()
  , m_activeTaggers(0)
{
  declareInterface<IParticleTupleTool>(this);

  //there is a long list of taggers defined in the enum
  m_tagger_map[(int)Tagger::none]="none";
  m_tagger_map[(int)Tagger::unknown]="unknown";
  m_tagger_map[(int)Tagger::OS_Muon]="OS_Muon";
  m_tagger_map[(int)Tagger::OS_Electron]="OS_Electron";
  m_tagger_map[(int)Tagger::OS_Kaon]="OS_Kaon";
  m_tagger_map[(int)Tagger::SS_Kaon]="SS_Kaon";
  m_tagger_map[(int)Tagger::SS_Pion]="SS_Pion";  
  m_tagger_map[(int)Tagger::SS_PionBDT]="SS_PionBDT";
  m_tagger_map[(int)Tagger::VtxCharge]="VtxCharge";
  m_tagger_map[(int)Tagger::Topology]="Topology";
  m_tagger_map[(int)Tagger::jetCharge]="jetCharge";
  m_tagger_map[(int)Tagger::OS_nnetKaon]="OS_nnetKaon";
  m_tagger_map[(int)Tagger::SS_nnetKaon]="SS_nnetKaon";
  m_tagger_map[(int)Tagger::SS_Proton]="SS_Proton";
  m_tagger_map[(int)Tagger::OS_Charm]="OS_Charm";

  for(std::map<int, std::string>::const_iterator t=m_tagger_map.begin();
      t!=m_tagger_map.end(); t++)
  {
    m_tagger_rmap[t->second]=t->first;
  }

  //but only these ones really need to be filled, and only these are really used
  m_activeTaggers.push_back("OS_Muon");
  m_activeTaggers.push_back("OS_Electron");
  m_activeTaggers.push_back("OS_Kaon");
  m_activeTaggers.push_back("SS_Kaon");
  m_activeTaggers.push_back("SS_Pion");
  m_activeTaggers.push_back("SS_PionBDT");
  m_activeTaggers.push_back("VtxCharge");
  m_activeTaggers.push_back("OS_nnetKaon");
  m_activeTaggers.push_back("SS_nnetKaon");
  m_activeTaggers.push_back("SS_Proton");
  m_activeTaggers.push_back("OS_Charm");

  declareProperty("TaggingToolName", m_toolName = "",
                  "The Tagging Tool, if empty string, the tool will be retrieved from the parent DVAlg");
  declareProperty("StoreTaggersInfo", m_extendedTagging = false);

  declareProperty("ActiveTaggers", m_activeTaggers );
  declareProperty("useFTonDST", m_useFTonDST = false );

  declareProperty("TagBeautyOnly", m_tagBeautyOnly = true);

}//=============================================================================

StatusCode TupleToolTagging::initialize() {
  if( ! TupleToolBase::initialize() ) return StatusCode::FAILURE;
  if( msgLevel( MSG::DEBUG ) ){
    if( m_useFTonDST ) debug() << " Going to write Tagging informations written on DST " << endmsg;
    else debug() << " Going to rerun FT !!!! (Ignore informations written on DST) " << endmsg;
  }
  
  m_dva = Gaudi::Utils::getIDVAlgorithm ( contextSvc(), this ) ;
  //if (m_dva==NULL) return Error("Couldn't get parent DVAlgorithm",
  //                           StatusCode::FAILURE);
  m_dist = m_dva->distanceCalculator();

  //m_fitter = tool<IVertexFit>("OfflineVertexFitter");
  m_fitter = tool<IVertexFit>("LoKi::VertexFitter");

  //if null string, get parent DVA, else use own private tool
  if(m_toolName == "" && m_dva!=NULL) m_tagging = m_dva->flavourTagging();
  else if (m_toolName != "") m_tagging = tool<IBTaggingTool>( m_toolName, this );

  if( !m_tagging && m_dva==NULL  )
  {
    return Error("Unable to retrieve the IBTaggingTool tool, unable to retrieve parent DVAlg",StatusCode::FAILURE);
  }
  if( !m_tagging )
  {
    return Error("Unable to retrieve the IBTaggingTool tool",StatusCode::FAILURE);
  }

  if( m_tagBeautyOnly ) debug() << "Running Flavour Tagging only on b-hadron particles" << endmsg;
  else debug() << "Running Flavour Tagging on all particles (not only b-hadrons)" << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
VerboseData TupleToolTagging::getVerboseData(const LHCb::Particle *particle, const LHCb::Particle *B) {
        const ProtoParticle *proto = particle->proto();
        VerboseData data;

        data.id = particle->particleID().pid();
        data.p = particle->momentum().P();
        data.px = particle->momentum().Px();
        data.py = particle->momentum().Py();
        data.pz = particle->momentum().Pz();
        data.pt = particle->momentum().Pt();
        data.theta = particle->momentum().theta();
        data.phi = particle->momentum().phi();
        if(proto!=NULL) 
        {
          
          data.pid_e = proto->info(LHCb::ProtoParticle::CombDLLe, -1000);
          data.pid_mu = proto->info(LHCb::ProtoParticle::CombDLLmu, -1000);
          data.pid_k = proto->info(LHCb::ProtoParticle::CombDLLk, -1000);
          data.pid_p = proto->info(LHCb::ProtoParticle::CombDLLp, -1000);
        }
        else
        {
          data.pid_e  = -999;
          data.pid_mu = -999;
          data.pid_k  = -999;
          data.pid_p  = -999;
        }
        // ip wrt the B primary vertex
        const VertexBase *primVtxB = m_dva->bestVertex(B);
        StatusCode test = StatusCode::FAILURE;
        if ( primVtxB!=NULL) test = m_dist->distance(particle, primVtxB, data.ip, data.chi2);
        if ( !test )
        {
          data.ip   = -1;
          data.chi2 = -1;
        }

        // ip wrt the B decay vertex
        const VertexBase *decayVtxB = B->endVertex();
        test = StatusCode::FAILURE;

        if ( decayVtxB!=NULL) test = m_dist->distance (particle, decayVtxB, data.bip, data.bchi2);
        if ( !test )
        {
          data.bip   = -1;
          data.bchi2 = -1;
        }




        Vertex vtx;
        test = m_fitter->fit(vtx,*B,*particle);    
        if( !test  ) data.bp_chi2 = -1.;
        else data.bp_chi2 = vtx.chi2()/(float)vtx.nDoF();

        return data;
}


StatusCode TupleToolTagging::fill( const Particle* mother
                                   , const Particle* P
                                   , const std::string& head
                                   , Tuples::Tuple& tuple )
{
  const std::string prefix=fullName(head);

  Assert( P && mother && m_dva && m_tagging,
          "Should not happen, you are inside TupleToolTagging.cpp" );

  std::string loc = objectLocation( P->parent() );

  // nothing to tag on something which is not a B (unless explicitly required)
  if( m_tagBeautyOnly )
  {
    if( !P->particleID().hasBottom()) return StatusCode::SUCCESS;
  }
  
  if( msgLevel( MSG::DEBUG ) ){
    debug() << " Going to Save Tagging information for candidate "
            << endmsg;
  }

  FlavourTag theTag;
  FlavourTags* tags = NULL;
  bool check = false;

  StatusCode sc=StatusCode::SUCCESS;

  boost::replace_all( loc, "/Particles", "/FlavourTags" );
  if( m_useFTonDST ) {
    if( exist < LHCb::FlavourTags > (loc,IgnoreRootInTES))
      tags = get< LHCb::FlavourTags > (loc,IgnoreRootInTES );
  }
  
  if (tags) {
    for(FlavourTags::const_iterator it = tags->begin(); it != tags->end(); ++it) {
      if( P != (**it).taggedB()) continue;
      theTag =  **it;
      check = true;
    }
    if (!check) sc = StatusCode::FAILURE;
  } else {
    const VertexBase* v = m_dva->bestVertex( mother );
    const RecVertex* vtx = dynamic_cast<const RecVertex*>(v);
    if( !vtx ){
      sc = m_tagging->tag( theTag, P );
    } else {
      sc = m_tagging->tag( theTag, P, vtx );
    }
  }
  // try to find unphysical defaults
  int dec = 0;
  double omega = 0.5;
  int decOS = 0;
  double omegaOS = 0.5;

  if( sc ){
    dec = theTag.decision();
    omega = theTag.omega(); // predicted wrong tag fraction.
    decOS = theTag.decisionOS();
    omegaOS = theTag.omegaOS(); // predicted wrong tag fraction.
  } else {
    Warning("The tagging algorithm failed");
  }

  bool test = true;
  test &= tuple->column( prefix+"_TAGDECISION" , dec );
  test &= tuple->column( prefix+"_TAGOMEGA" , omega );
  test &= tuple->column( prefix+"_TAGDECISION_OS" , decOS );
  test &= tuple->column( prefix+"_TAGOMEGA_OS" , omegaOS );

  int taggers_code = 0;
  // intialize tagger by tagger W :

  std::vector<Tagger> taggers = theTag.taggers();

  for(size_t i=0; i<taggers.size(); ++i) {
    int tdec = taggers[i].decision();

    if(tdec) switch ( taggers[i].type() ) {
    case Tagger::OS_Charm    : taggers_code +=1000000000 *(tdec+2); break;
    case Tagger::SS_Proton   : taggers_code += 100000000 *(tdec+2); break;
    case Tagger::OS_nnetKaon : taggers_code +=  10000000 *(tdec+2); break;
    case Tagger::SS_nnetKaon : taggers_code +=   1000000 *(tdec+2); break;
    case Tagger::OS_Muon     : taggers_code +=    100000 *(tdec+2); break;
    case Tagger::OS_Electron : taggers_code +=     10000 *(tdec+2); break;
    case Tagger::OS_Kaon     : taggers_code +=      1000 *(tdec+2); break;
    case Tagger::SS_Kaon     : taggers_code +=       100 *(tdec+2); break;
    case Tagger::SS_Pion     : taggers_code +=        10 *(tdec+2); break;
    case Tagger::VtxCharge   : taggers_code +=         1 *(tdec+2); break;

    }
  }
  
  test &= tuple->column( prefix+"_TAGGER" , taggers_code);
 
  if(isVerbose())  
  {    
    // Initialize all columns to default values
    for(size_t i=0; i<m_activeTaggers.size(); i++) {
        std::string active = m_activeTaggers[i];

        test &= tuple->column( prefix+"_"+active+"_DEC",  (short int)0 );
        test &= tuple->column( prefix+"_"+active+"_PROB", (float)0.5 );
        
        if(m_extendedTagging){
          if( true ) {
            std::vector<double> id, p, px, py, pz, pt, theta, phi;
            std::vector<double> pid_e, pid_mu, pid_k, pid_p;
            std::vector<double> ip, chi2, bip, bchi2, bp_chi2;
            
            const std::string num_name = prefix+"_"+active+"_PARTICLES_NUM";
            test &= tuple->farray( prefix+"_"+active+"_PARTICLES_ID", id.begin(), id.end(), num_name, 20 );
            test &= tuple->farray( prefix+"_"+active+"_PARTICLES_P", p.begin(), p.end(), num_name, 20 );
            test &= tuple->farray( prefix+"_"+active+"_PARTICLES_PX", px.begin(), px.end(), num_name, 20 );
            test &= tuple->farray( prefix+"_"+active+"_PARTICLES_PY", py.begin(), py.end(), num_name, 20 );
            test &= tuple->farray( prefix+"_"+active+"_PARTICLES_PZ", pz.begin(), pz.end(), num_name, 20 );
            test &= tuple->farray( prefix+"_"+active+"_PARTICLES_PT", pt.begin(), pt.end(), num_name, 20 );
            test &= tuple->farray( prefix+"_"+active+"_PARTICLES_THETA", theta.begin(), theta.end(), num_name, 20 );
            test &= tuple->farray( prefix+"_"+active+"_PARTICLES_PHI", phi.begin(), phi.end(), num_name, 20 );
            test &= tuple->farray( prefix+"_"+active+"_PARTICLES_PIDe", pid_e.begin(), pid_e.end(), num_name, 20 );
            test &= tuple->farray( prefix+"_"+active+"_PARTICLES_PIDmu", pid_mu.begin(), pid_mu.end(), num_name, 20 );
            test &= tuple->farray( prefix+"_"+active+"_PARTICLES_PIDk", pid_k.begin(), pid_k.end(), num_name, 20 );
            test &= tuple->farray( prefix+"_"+active+"_PARTICLES_PIDp", pid_p.begin(), pid_p.end(), num_name, 20 );
            test &= tuple->farray( prefix+"_"+active+"_PARTICLES_IP_OWNPV", ip.begin(), ip.end(), num_name, 20 );
            test &= tuple->farray( prefix+"_"+active+"_PARTICLES_IPCHI2_OWNPV", chi2.begin(), chi2.end(), num_name, 20 );
            test &= tuple->farray( prefix+"_"+active+"_PARTICLES_IP_BVertex", bip.begin(), bip.end(), num_name, 20 );
            test &= tuple->farray( prefix+"_"+active+"_PARTICLES_IPCHI2_BVertex", bchi2.begin(), bchi2.end(), num_name, 20 );
            test &= tuple->farray( prefix+"_"+active+"_PARTICLES_CHI2_BpVertex", bp_chi2.begin(), bp_chi2.end(), num_name, 20 );
            }
         }
                
    }  

    for(size_t i=0; i<taggers.size(); i++) {
      std::string name = m_tagger_map[(int)taggers[i].type()];

      //loop over active taggers only
      if(std::find(m_activeTaggers.begin(), m_activeTaggers.end(), name) != m_activeTaggers.end()) {
        Tagger tagger = taggers[i];

        test &= tuple->column( prefix+"_"+name+"_DEC",  tagger.decision() );
        test &= tuple->column( prefix+"_"+name+"_PROB", tagger.omega()    );

        if(m_extendedTagging){
          // Save interesting tagging data
          std::vector<double> id, p, px, py, pz, pt, theta, phi;
          std::vector<double> pid_e, pid_mu, pid_k, pid_p;
          std::vector<double> ip, chi2, bip, bchi2, bp_chi2;
          
          SmartRefVector<LHCb::Particle> parts = tagger.taggerParts();
          for(SmartRefVector<LHCb::Particle>::const_iterator it=parts.begin();
              it != parts.end(); it++) {
            
            VerboseData data = getVerboseData(*it, P);
            
            id.push_back(data.id);
            p.push_back(data.p);
            px.push_back(data.px);
            py.push_back(data.py);
            pz.push_back(data.pz);
            pt.push_back(data.pt);
            theta.push_back(data.theta);
            phi.push_back(data.phi);
            pid_e.push_back(data.pid_e);
            pid_mu.push_back(data.pid_mu);
            pid_k.push_back(data.pid_k);
            pid_p.push_back(data.pid_p);
            ip.push_back(data.ip);
            chi2.push_back(data.chi2);
            bip.push_back(data.bip);
            bchi2.push_back(data.bchi2);
            bp_chi2.push_back(data.bp_chi2);
          }
          
          const std::string num_name = prefix+"_"+name+"_PARTICLES_NUM";
          test &= tuple->farray( prefix+"_"+name+"_PARTICLES_ID", id.begin(), id.end(), num_name, 20 );
          test &= tuple->farray( prefix+"_"+name+"_PARTICLES_P", p.begin(), p.end(), num_name, 20 );
          test &= tuple->farray( prefix+"_"+name+"_PARTICLES_PX", px.begin(), px.end(), num_name, 20 );
          test &= tuple->farray( prefix+"_"+name+"_PARTICLES_PY", py.begin(), py.end(), num_name, 20 );
          test &= tuple->farray( prefix+"_"+name+"_PARTICLES_PZ", pz.begin(), pz.end(), num_name, 20 );
          test &= tuple->farray( prefix+"_"+name+"_PARTICLES_PT", pt.begin(), pt.end(), num_name, 20 );
          test &= tuple->farray( prefix+"_"+name+"_PARTICLES_THETA", theta.begin(), theta.end(), num_name, 20 );
          test &= tuple->farray( prefix+"_"+name+"_PARTICLES_PHI", phi.begin(), phi.end(), num_name, 20 );
          test &= tuple->farray( prefix+"_"+name+"_PARTICLES_PIDe", pid_e.begin(), pid_e.end(), num_name, 20 );
          test &= tuple->farray( prefix+"_"+name+"_PARTICLES_PIDmu", pid_mu.begin(), pid_mu.end(), num_name, 20 );
          test &= tuple->farray( prefix+"_"+name+"_PARTICLES_PIDk", pid_k.begin(), pid_k.end(), num_name, 20 );
          test &= tuple->farray( prefix+"_"+name+"_PARTICLES_PIDp", pid_p.begin(), pid_p.end(), num_name, 20 );
          test &= tuple->farray( prefix+"_"+name+"_PARTICLES_IP_OWNPV", ip.begin(), ip.end(), num_name, 20 );
          test &= tuple->farray( prefix+"_"+name+"_PARTICLES_IPCHI2_OWNPV", chi2.begin(), chi2.end(), num_name, 20 );
          test &= tuple->farray( prefix+"_"+name+"_PARTICLES_IP_BVertex", bip.begin(), bip.end(), num_name, 20 );
          test &= tuple->farray( prefix+"_"+name+"_PARTICLES_IPCHI2_BVertex", bchi2.begin(), bchi2.end(), num_name, 20 );
          test &= tuple->farray( prefix+"_"+name+"_PARTICLES_CHI2_BpVertex", bp_chi2.begin(), bp_chi2.end(), num_name, 20 );
        }        
      }      
    }    
  }
  

  if( msgLevel( MSG::DEBUG ) ){
    debug() << "Tagging summary: decision: " << dec
            << ", omega=" << omega << endmsg;
  }

  return StatusCode(test);

}

// CRJ : Not used
// const DVAlgorithm* TupleToolTagging::getParent() const {

//   // look for desktop
//   const IAlgTool* atool = this ;
//   // get last tool in chain
//   debug() << "Looking for parents of " << atool->name() << endmsg ;
//   while ( NULL!=dynamic_cast<const IAlgTool*>(atool->parent())){
//     atool = dynamic_cast<const IAlgTool*>(atool->parent());
//     debug() << "... tool is owned by tool " << atool->name() << endmsg ;
//   }
//   // check it's not the ToolSvc
//   const IToolSvc* tsvc = dynamic_cast<const IToolSvc*>( atool->parent() );
//   if ( NULL!=tsvc ){
//     warning() << "Parent of " << atool->name() << " is the ToolSvc." << endmsg ;
//     return NULL;
//   }

//   // check if it is an algorithm
//   const DVAlgorithm* dvalgo = dynamic_cast<const DVAlgorithm*>( atool->parent() );
//   if ( NULL==dvalgo ){
//     warning() << "Parent of " << atool->name() << " is not a DVAlgorithm." << endmsg ;
//     return NULL;
//   }

//   debug() << atool->name() << " is owned by " << dvalgo->name() << endmsg ;
//   return dvalgo;
// }

// std::string TupleToolTagging::getCatName( const LHCb::Tagger& tag ) const{
//   std::string tts;
//   switch ( tag.type() ) {
//   case Tagger::none        : tts="none";        break;
//   case Tagger::unknown     : tts="unknown";     break;
//   case Tagger::OS_Muon     : tts="OS_Muon";     break;
//   case Tagger::OS_Electron : tts="OS_Electron"; break;
//   case Tagger::OS_Kaon     : tts="OS_Kaon";     break;
//   case Tagger::SS_Kaon     : tts="SS_Kaon";     break;
//   case Tagger::SS_Pion     : tts="SS_Pion";     break;
//   case Tagger::jetCharge   : tts="jetCharge";   break;
//   case Tagger::OS_jetCharge: tts="OS_jetCharge";break;
//   case Tagger::SS_jetCharge: tts="SS_jetCharge";break;
//   case Tagger::VtxCharge   : tts="VtxCharge";   break;
//   case Tagger::Topology    : tts="Topology";    break;
//   default: tts="Unknown tag category";
//   }
//   return tts;
// }

