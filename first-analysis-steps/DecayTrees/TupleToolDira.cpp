// Include files

// local
#include "TupleToolDira.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : DiraTupleTool
//
// 2010-09-06 : Fatima Soomro
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolDira::TupleToolDira( const std::string& type,
                                const std::string& name,
                                const IInterface* parent )
    : TupleToolBase ( type, name , parent )
    
{
  declareInterface<IParticleTupleTool>(this);
}

//=============================================================================

StatusCode TupleToolDira::initialize()
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  m_context = Gaudi::Utils::getIDVAlgorithm ( contextSvc(), this ) ;
  if ( !m_context ) return Error("Could not get parent DVAlgorithm",
                                 StatusCode::FAILURE);

  return sc;
}

//=============================================================================

StatusCode TupleToolDira::fill( const Particle* mother
                                , const Particle* P
                                , const std::string& head
                                , Tuples::Tuple& tuple )
{
  Assert( P && mother && m_context
          , "This should not happen, you are inside TupleToolDira.cpp :(" );

  if ( isPureNeutralCalo(P) )
    return Warning( "Will not fill geometry tuple for calo neutrals. No worry.", 
                    StatusCode::SUCCESS, 10 );
  if ( mother != P || P->isBasicParticle() ) return StatusCode::SUCCESS;

  const VertexBase* motherPV = m_context->bestVertex ( mother );
  if ( !motherPV ) 
  { 
    return Warning( "Could not find related PV" ); 
  }

  return fillDIRAError( motherPV, mother, head, tuple );
}

//=========================================================================
// fill vertex stuff
//=========================================================================
StatusCode TupleToolDira::fillDIRAError( const VertexBase* primVtx
                                         ,const Particle* P
                                         , const std::string head
                                         , Tuples::Tuple& tuple ) const 
{

  const VertexBase* evtx = P->endVertex();

  if( !evtx )
  {
    return Error( "Cannot retrieve the end vertex for " + head );
  }


  double diraerr(0), dira(0);
  calculateDIRAError( P ,primVtx , dira, diraerr );

  bool test = true;
  test &= tuple->column( head + "_DiraAngleError", diraerr/(std::sqrt(1-dira*dira))) ;
  test &= tuple->column( head + "_DiraCosError", diraerr );
  test &= tuple->column( head + "_DiraAngle", std::acos(dira)) ;
  test &= tuple->column( head + "_DiraCos", dira );

  return StatusCode(test);
}

//=========================================================================

void TupleToolDira::calculateDIRAError( const Particle* P, 
                                        const VertexBase* primVtx, 
                                        double & dira, 
                                        double & diraerr  ) const
{


  const Gaudi::SymMatrix7x7 & covMother = P->covMatrix ();
  const Gaudi::SymMatrix3x3 & covPrim= primVtx->covMatrix ();
  SymMatrix10x10 Cov;

  for(int i=0; i<7; ++i)
  {
    for(int j=0; j<7; ++j)
    {
      Cov(i,j) = covMother(i,j);
    }
  }

  for(int i=7; i<10; ++i)
  {
    for(int j=7; j<10; ++j)
    {
      Cov(i,j) = covPrim(i-7,j-7);
    }
  }

  for(int i=7; i<10; ++i)
  {
    for(int j=0; j<7; ++j) 
    {
      Cov(i,j) = 0.0;
      Cov(j,i) = 0.0;
    }
  }

  const Gaudi::XYZVector  vectord = P->endVertex()->position() -  primVtx->position();
  const Gaudi::XYZVector& vectorp = P->momentum().Vect() ;
  
  double vecd[3] = { vectord.x(), vectord.y(), vectord.z()};
  double vecp[3] = { vectorp.x(), vectorp.y(), vectorp.z()};
  
  const double magd = std::sqrt( vectord.Mag2() ) ;
  const double magp = std::sqrt( vectorp.Mag2() );

  ROOT::Math::SMatrix<double,10, 1> gradVec;
  gradVec(6,0) = 0.0;

  for ( int i = 0; i < 3; ++i )
  {
    double gp = vecd[i]*magp*magp;
    double gd = vecp[i]*magd*magd;

    for ( int j = 0; j < 3; ++j )
    {
      gp -= vecd[j]*vecp[j]*vecp[i];
      gd -= vecp[j]*vecd[j]*vecd[i];
    }
    gradVec(i+3,0) = gp/(magd*magp*magp*magp);
    gradVec(i,0) = gd/(magp*magd*magd*magd);
    gradVec(i+7,0) = -gd/(magp*magd*magd*magd);
  }

  const Gaudi::SymMatrix1x1 diraErrSq = 
    ROOT::Math::SimilarityT<double, 10, 1>(gradVec, Cov);

  diraerr = std::sqrt(diraErrSq(0,0));
  
  dira = (vectord.Dot(vectorp))/ (magp*magd);

  return;
}

//=========================================================================

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolDira )

//=========================================================================
