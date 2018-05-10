// $Id: TupleToolCaloHypo.cpp,v 1.1 2010-05-21 17:03:11 odescham Exp $
// Include files

// local
#include "TupleToolCaloHypo.h"
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
#include "Event/Particle.h"
#include "GaudiKernel/IRegistry.h" //

#include "CaloUtils/CaloAlgUtils.h"
#include "CaloUtils/CaloMomentum.h"
#include "CaloUtils/Calo2Track.h"
#include "Relations/IRelationWeighted2D.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolCaloHypo
//
// 2008-10-31 :(Happy Halloween)
// Yasmine Amhis
//-----------------------------------------------------------------------------

using namespace Gaudi;
using namespace LHCb;

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolCaloHypo )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolCaloHypo::TupleToolCaloHypo( const std::string& type,
                                        const std::string& name,
                                        const IInterface* parent )
    : TupleToolBase ( type, name , parent ){
    declareInterface<IParticleTupleTool>(this);
    declareProperty("DataList",m_dataList);
    declareProperty("AllowBremHypo",m_brem = false);
    declareProperty("AllowChargedHypo",m_charged=false);
    declareProperty("AllowDaughtersHypo",m_daugh=false); // SplitPhotons from merged
    m_dataList.push_back( "All" );
  }



StatusCode TupleToolCaloHypo::initialize() {
  if( ! TupleToolBase::initialize() ) return StatusCode::FAILURE;

  m_estimator = tool<ICaloHypoEstimator>("CaloHypoEstimator","CaloHypoEstimator",this);
  m_tables = tool<ICaloRelationsGetter>("CaloRelationsGetter","CaloRelationsGetter",this);
  m_caloElectron = tool<ICaloElectron>("CaloElectron", this);
  m_pileup = tool<ICaloDigitFilterTool>("CaloDigitFilterTool","FilterTool");

  info() << "Caution : this TupleTool may trigger part of Calo reconstruction - ONLY WORK ON DST" << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
StatusCode TupleToolCaloHypo::fill(const Particle* , const Particle* P
                                   ,const std::string& head
                                   ,Tuples::Tuple& tuple ){

  const std::string prefix=fullName(head);


  if( NULL == P )return StatusCode::SUCCESS;
  if( NULL == P->proto() )return StatusCode::SUCCESS;
  if( 0 != P->charge() && !m_charged)return StatusCode::SUCCESS;

  bool filltuple = true;
  const SmartRefVector<LHCb::CaloHypo>& hypos = P->proto()->calo();
  bool hasCalo = true;
  bool hasBremCalo = false;
  if( 0 == hypos.size() )hasCalo = false;
  if( 1 < hypos.size() )hasBremCalo = true;
  filltuple &= tuple->column( prefix+"_hasCaloHypo", (int) hasCalo );
  if( !isPureNeutralCalo( P ) && m_brem )
    filltuple &= tuple->column( prefix+"_hasBremCaloHypo", (int) hasBremCalo );



  using namespace CaloDataType;
  // select hypos 
  const LHCb::CaloHypo* hypo   = NULL;
  const LHCb::CaloHypo* hypo2  = NULL;
  for( SmartRefVector<CaloHypo>::const_iterator ih = hypos.begin() ; hypos.end() != ih ; ++ih){
    if( !isPureNeutralCalo( P ) ){
      if( (*ih)->hypothesis() == LHCb::CaloHypo::EmCharged)hypo = *ih;
      else hypo2 = *ih;
    }else hypo = *ih;
  }

  //info() << "coucou 0 " << P->pid() << endmsg;
  const LHCb::CaloHypo* daugh1 = NULL;
  const LHCb::CaloHypo* daugh2 = NULL;
  if( hypo != NULL && hypo->hypothesis() == LHCb::CaloHypo::Pi0Merged && m_daugh){
    // == extract SplitPhotons hypos
    const SmartRefVector<LHCb::CaloHypo>& hypos = hypo->hypos();
    daugh1 = *(hypos.begin() );
    daugh2 = *(hypos.begin()+1 );
  }
  //info() << "coucou 1 " << hypo << " " << hypo2 << " " << daugh1 << " " << daugh2 << endmsg;

  double he =0.;
  double hx =0.;
  double hy =0.;
  double hz =0.;
  double dT =-1.;
  
  if(hypo != NULL ){
    const LHCb::CaloPosition* position=hypo->position();
    if( position != NULL){
      he = position->e() ;
      hx = position->x() ;
      hy = position->y() ;
      hz = position->z() ;
      double th1 = incidence( hypo ) ;
      double th2 = incidence(hypo, true);
      if(th1 != -1. && th2 != -1)dT = th1-th2;

    }
    //info() << "coucou 2 " << hypo->hypothesis() << endmsg;
  }
  filltuple &= tuple->column( prefix+"_CaloHypo_E", he );
  filltuple &= tuple->column( prefix+"_CaloHypo_X", hx );
  filltuple &= tuple->column( prefix+"_CaloHypo_Y", hy );
  filltuple &= tuple->column( prefix+"_CaloHypo_Z", hz );



  if(  m_charged )filltuple &= tuple->column( prefix+"_CaloHypo_DeltaTheta", dT );
  if( hypo != NULL && hypo->hypothesis() == LHCb::CaloHypo::Pi0Merged && m_daugh && daugh1 != NULL && daugh2 != NULL){
    const LHCb::CaloPosition* pos1=daugh1->position();
    const LHCb::CaloPosition* pos2=daugh2->position();
    if( pos1 != NULL && pos2 != NULL){
      filltuple &= tuple->column( prefix+"_CaloHypoDaugh0_E", pos1->e() );
      filltuple &= tuple->column( prefix+"_CaloHypoDaugh0_X", pos1->x() );
      filltuple &= tuple->column( prefix+"_CaloHypoDaugh0_Y", pos1->y() );
      filltuple &= tuple->column( prefix+"_CaloHypoDaugh0_Z", pos1->z() );
      filltuple &= tuple->column( prefix+"_CaloHypoDaugh1_E", pos2->e() );
      filltuple &= tuple->column( prefix+"_CaloHypoDaugh1_X", pos2->x() );
      filltuple &= tuple->column( prefix+"_CaloHypoDaugh1_Y", pos2->y() );
      filltuple &= tuple->column( prefix+"_CaloHypoDaugh1_Z", pos2->z() );
    }
  }
  
  // PileUp subtracted energy
  const LHCb::CaloCluster* cl = LHCb::CaloAlgUtils::ClusterFromHypo(hypo, false);
  const LHCb::CaloCellID seed = (cl !=NULL) ? cl->seed() : LHCb::CaloCellID();
  double offset = (cl != NULL) ? m_pileup->offset( seed , 0 ) : -1.;
  filltuple &= tuple->column(prefix+"_CaloHypoPileUpEnergy", offset);
  
  //fill tuple
  for( int id = 0 ; id < Last ; ++id){
    int mask = ( P->charge() == 0 ) ? 0x1 : 0x2;
    if( useData( id , mask ) ){
      double val  =  (NULL != hypo && hasCalo)    ? m_estimator->data(hypo, (DataType) id ,0.) : 0.;      
      //info() << "coucou loop 1 " << Name[id] << " " << val << endmsg;
      filltuple &= tuple->column( prefix+"_CaloHypo_"+Name[id], val );
    }
    if( P->charge() != 0 && m_brem && useData( id , 0x1 ) ){
      double bval =  (NULL != hypo2 && hasBremCalo) ? m_estimator->data(hypo2,(DataType) id ,0.) : 0.;
      //info() << "coucou loop 2 " << Name[id] << " " << bval << endmsg;
      filltuple &= tuple->column( prefix+"_BremCaloHypo_"+Name[id], bval );
    }
    if( hypo != NULL && hypo->hypothesis() == LHCb::CaloHypo::Pi0Merged && m_daugh && useData(id,0x1) ){
      double val0 =( NULL != daugh1 ) ? m_estimator->data(daugh1,(DataType) id, 0.) : 0.;
      double val1 =( NULL != daugh2 ) ? m_estimator->data(daugh2,(DataType) id, 0.) : 0.;
      //info() << "coucou loop 3 " << Name[id] << " " << val0 << " " << val1 << endmsg;
      filltuple &= tuple->column( prefix+"_CaloHypoDaugh0_"+Name[id], val0 );
      filltuple &= tuple->column( prefix+"_CaloHypoDaugh1_"+Name[id], val1 );
    }
  }
  return StatusCode(filltuple);
}

bool TupleToolCaloHypo::useData( int id , int mask){
  using namespace CaloDataType;
  std::string name = Name[id];
  int type = TypeMask[id];
  for(std::vector<std::string>::iterator i = m_dataList.begin() ; m_dataList.end() !=i ; ++i){
    if( *i == "All" && (type & mask) !=0 )return true;
    if( *i == name )return true;
  }
  return false;
}

double TupleToolCaloHypo::incidence(const LHCb::CaloHypo* hypo, bool straight)const {

  const LHCb::CaloCluster* cluster = LHCb::CaloAlgUtils::ClusterFromHypo(hypo,true) ;
  std::string m_cmLoc= LHCb::CaloAlgUtils::CaloIdLocation("ClusterMatch", context());

  double incidence = -1;
  if(  LHCb::CaloHypo::EmCharged == hypo->hypothesis() && !straight ){
    // for electron hypothesis : get the matching track
    if (exist<LHCb::Calo2Track::IClusTrTable> (m_cmLoc)) {
      LHCb::Calo2Track::IClusTrTable* ctable = m_tables->getClusTrTable( m_cmLoc );
      const LHCb::Calo2Track::IClusTrTable::Range range = ctable -> relations(cluster);
      if ( !range.empty() ){
        const LHCb::Track* ctrack = range.front();
        // temporary protoParticle
        LHCb::ProtoParticle* proto = new LHCb::ProtoParticle();
        proto->setTrack( ctrack );
        proto->addToCalo( hypo );
        if( m_caloElectron->set(proto))incidence = m_caloElectron->caloState().momentum().Theta();
        delete proto;
      }
    } 
  }else{
    // for neutrals :
    LHCb::CaloMomentum cMomentum = LHCb::CaloMomentum( hypo );
    incidence = cMomentum.momentum().Theta();    
  }
  return incidence;
}      


