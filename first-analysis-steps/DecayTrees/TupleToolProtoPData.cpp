#include "TupleToolProtoPData.h"
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
#include "CaloUtils/CaloAlgUtils.h"
//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolProtoPData
//
// 2011-06-08 : Olivier Deschamps
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolProtoPData )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolProtoPData::TupleToolProtoPData( const std::string& type,
                                            const std::string& name,
                                            const IInterface* parent ):
    TupleToolBase ( type, name , parent ){
    declareInterface<IParticleTupleTool>(this);
    declareProperty( "DataList", m_list );
    declareProperty( "LastData", m_last=1000);
    // default : all Comb and Prob data
    m_list.push_back("*Comb*");
    m_list.push_back("*Prob*");
  }

//=============================================================================



StatusCode TupleToolProtoPData::fill( const LHCb::Particle*
                                      , const LHCb::Particle* P
                                      , const std::string& head
                                      , Tuples::Tuple& tuple ){
  const std::string prefix=fullName(head);

  if( !P->isBasicParticle() )return StatusCode::SUCCESS;
  const LHCb::ProtoParticle* pp = P->proto();
  if( NULL ==  pp )return StatusCode::SUCCESS;

  bool fill=true;

  double def = -999999.;
  int last =  (m_last < 0) ?  LHCb::ProtoParticle::LastGlobal : m_last;
  for(int i=0; i < last ; ++i){
    std::ostringstream var("");
    var << (LHCb::ProtoParticle::additionalInfo) i;
    if( std::string::npos != var.str().find("ERROR") )continue;
    if( LHCb::CaloAlgUtils::StringMatcher( m_list , var.str() ) ){
      double val = pp->info( (LHCb::ProtoParticle::additionalInfo) i , def);
      fill &= tuple->column( prefix+"_PP_"+var.str(), val );
    }
  }

  return StatusCode::SUCCESS;
}
