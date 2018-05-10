// Include files 
// from Gaudi 
//#include "GaudiKernel/ToolFactory.h"
// local 
#include "TupleToolCorrectedMass.h"
#include <Kernel/GetIDVAlgorithm.h>
#include <Kernel/IDVAlgorithm.h>
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
#include "Event/Particle.h"

using namespace LHCb;
using namespace Gaudi::Units;
using namespace ROOT::Math;

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolCorrectedMass
// @author Michel De Cian
// @date   2018-03-21 
//----------------------------------------------------------------------------- 
DECLARE_COMPONENT( TupleToolCorrectedMass )
//============================================================================= 
// Standard constructor, initializes variables
//============================================================================= 
TupleToolCorrectedMass::TupleToolCorrectedMass( const std::string& type,
                                      const std::string& name,
                                        const IInterface* parent )
: TupleToolBase ( type, name , parent ),
  m_dva(nullptr)
{
  declareInterface<IParticleTupleTool>(this);
}
//============================================================================= 
// -- Initialize
//============================================================================= 
StatusCode TupleToolCorrectedMass::initialize(){
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  m_dva = Gaudi::Utils::getIDVAlgorithm ( contextSvc(), this ) ; 
  if (!m_dva) return Error("Couldn't get parent DVAlgorithm", 
                           StatusCode::FAILURE); 
   
  return sc; 
} 
//============================================================================= 
// -- The main method
//============================================================================= 
StatusCode TupleToolCorrectedMass::fill( const Particle* mother 
					 , const Particle* P 
					 , const std::string& head 
					 , Tuples::Tuple& tuple ) { 
  
  const std::string prefix=fullName(head); 
  Assert( P && mother
          , "This should not happen, you are inside TupleToolCorrectedMass.cpp :(" ); 
  if( !P ) return StatusCode::FAILURE;
  if( !( P->particleID().hasBottom() )) return StatusCode::SUCCESS;
  
  // -------------------------------------------------- 
  const LHCb::Vertex* BEndVertex        = P->endVertex();
  const LHCb::VertexBase* primaryVertex = m_dva->bestVertex ( P );

  const Gaudi::LorentzVector pMother = P->momentum();
  
  const TVector3 PV(primaryVertex->position().X(),primaryVertex->position().Y(), primaryVertex->position().Z());
  const TVector3 SV(BEndVertex->position().X(),BEndVertex->position().Y(), BEndVertex->position().Z());
  const TVector3 flightDir = (SV - PV).Unit();
  
  const double MCorr = mCorr(pMother, flightDir);
  const std::pair<double, double> MCorrErrors = mCorrErrors(SV, PV, pMother, P->covMatrix(), primaryVertex->covMatrix());

  bool test=true; 
  test &= tuple->column(  prefix + "_MCORR", MCorr);
  test &= tuple->column(  prefix + "_MCORRERR", MCorrErrors.second);
  test &= tuple->column(  prefix + "_MCORRVERTEXERR", MCorrErrors.first); // only errors on vertices
  
  return StatusCode(test); 
} 
//========================================================================= 
// -- Calculate the corrected mass: m_corr = sqrt( m_vis² + pT² ) + pT
//=========================================================================
double TupleToolCorrectedMass::mCorr(const Gaudi::LorentzVector p, const TVector3 flightDir) const {
  const  TVector3 pVec(p.Px(), p.Py(), p.Pz());
  const  double PT = pVec.Perp(flightDir);
  return std::sqrt(p.M2() + PT * PT) + PT;
}
//========================================================================= 
// -- Helper calculation for mCorrErrors
//=========================================================================
double TupleToolCorrectedMass::dPTdx(const double dAdx, const double dBdx, const double PT, const double A, const double B) const {
  
  return 1/PT * -0.5 * (2*A*B*dAdx - A*A*dBdx)/(B*B);

}
//========================================================================= 
// -- Calculate the corrected mass error
//=========================================================================
std::pair<double, double> TupleToolCorrectedMass::mCorrErrors(const TVector3 sv, const TVector3 pv, const Gaudi::LorentzVector p,  
							const Gaudi::SymMatrix7x7 covP, const Gaudi::SymMatrix3x3 covPV ) const {
  // --
  // -- m_corr = sqrt( m_vis² + pT² ) + pT
  // --
  // -- To transform the errors on the vertices and the momentum to the corrected mass, one essentially needs to do
  // -- mcorr_err² = Sum_i,j( d(m_corr)/dx_i d(m_corr)/dx_j M_ij )_PV + Sum_n,m( d(m_corr)/dx_n d(m_corr)/dx_m M_nm )_SV
  // -- where M_ij is the covariance matrix of the PV, and M_nm the covariance matrix of the SV, including uncertainties of the momenta
  // -- of the particles that formed the SV.
  // --
  // -- For the vertex coordinates:
  // -- d(m_corr) / dx = d(m_corr)/dpT * dpT/dx
  // -- d(m_corr)/dpT =  1/2 * 1/std::sqrt( m_vis² + pT² ) * 2 * pT + 1; 
  // -- pT = sqrt( (p_vec - (x_SV_vec - x_PV_vec) * A/B)² )
  // -- with A = px(x_SV - x_PV) +  py(y_SV - y_PV) +  pz(z_SV - z_PV)
  // -- and  B = (x_SV - x_PV)² + (y_SV - y_PV)² + (z_SV - z_PV)², or the magnitude squared of x_SV_vec - x_PV_vec
  // --
  // -- For the momentum coordinates:
  // -- m_vis² = E² - (px² + py² + pz²)
  // -- d(m_corr) / dpx = 1/2 * 1 / sqrt( m_vis² + pT²) * ( -2A/B(x_SV-x_PV) ) + 1/pT * ( px - 2A/B(x_SV - x_PV) )
  // -- d(m_corr) / dE  = 1/2 * / sqrt( m_vis² + pT²) * 2E

  const double x_SV  = sv.Px(); //SV
  const double y_SV  = sv.Py(); //SV
  const double z_SV  = sv.Pz(); //SV
  const double x_PV  = pv.Px(); //PV
  const double y_PV  = pv.Py(); //PV
  const double z_PV  = pv.Pz(); //PV
  const double px = p.Px();
  const double py = p.Py();
  const double pz = p.Pz();
  const double E = p.E();
  
  const TVector3 pVec(p.Px(), p.Py(), p.Pz());
  const double PT = pVec.Perp((sv - pv));
  const double A = px*(x_SV - x_PV) + py*(y_SV - y_PV) + pz*(z_SV - z_PV);
  const double B = (sv-pv).Mag2();
  
  const double invSqrtMassPT =  1/std::sqrt( p.M2() + PT*PT );
  const double dMcorrdPT  = 0.5 * invSqrtMassPT * 2 * PT + 1; 
  
  // -- First let's calculate the derivates of 'A' and 'B'
  // -- A
  const double dAdx_SV =  px;
  const double dAdy_SV =  py;
  const double dAdz_SV =  pz;
  
  const double dAdx_PV = -px;
  const double dAdy_PV = -py;
  const double dAdz_PV = -pz;
  
  const double dAdpx = x_SV - x_PV;
  const double dAdpy = y_SV - y_PV;
  const double dAdpz = z_SV - z_PV;

  // -- B
  const double dBdx_SV =  2*(x_SV - x_PV);
  const double dBdy_SV =  2*(y_SV - y_PV);
  const double dBdz_SV =  2*(z_SV - z_PV);
  
  const double dBdx_PV = -2*(x_SV - x_PV);
  const double dBdy_PV = -2*(y_SV - y_PV);
  const double dBdz_PV = -2*(z_SV - z_PV);

  
  // -- the vertices
  const double dMcdx_SV = dMcorrdPT * dPTdx( dAdx_SV, dBdx_SV, PT, A, B);
  const double dMcdy_SV = dMcorrdPT * dPTdx( dAdy_SV, dBdy_SV, PT, A, B );
  const double dMcdz_SV = dMcorrdPT * dPTdx( dAdz_SV, dBdz_SV, PT, A, B );

  const double dMcdx_PV = dMcorrdPT * dPTdx( dAdx_PV, dBdx_PV, PT, A, B );
  const double dMcdy_PV = dMcorrdPT * dPTdx( dAdy_PV, dBdy_PV, PT, A, B );
  const double dMcdz_PV = dMcorrdPT * dPTdx( dAdz_PV, dBdz_PV, PT, A, B );
  

  // -- the momentum
  const double dMcdpx = -1 * invSqrtMassPT * A/B*dAdpx + 1/PT * (px - A/B*dAdpx); 
  const double dMcdpy = -1 * invSqrtMassPT * A/B*dAdpy + 1/PT * (py - A/B*dAdpy); 
  const double dMcdpz = -1 * invSqrtMassPT * A/B*dAdpz + 1/PT * (pz - A/B*dAdpz); 
  const double dMcdE  =      invSqrtMassPT * E;

  
  // -- the errors on the vertices
  const double errsqVertex = 
    // -- the diagonal for SV
    covP(0,0) * dMcdx_SV * dMcdx_SV + 
    covP(1,1) * dMcdy_SV * dMcdy_SV + 
    covP(2,2) * dMcdz_SV * dMcdz_SV + 
    // -- the diagonal for PV
    covPV(0,0) * dMcdx_PV * dMcdx_PV + 
    covPV(1,1) * dMcdy_PV * dMcdy_PV + 
    covPV(2,2) * dMcdz_PV * dMcdz_PV + 
    // -- the cross terms for SV
    covP(0,1) * 2. * dMcdx_SV * dMcdy_SV + 
    covP(0,2) * 2. * dMcdx_SV * dMcdz_SV + 
    covP(1,2) * 2. * dMcdy_SV * dMcdz_SV + 
    // -- the cross terms for PV
    covPV(0,1) * 2. * dMcdx_PV * dMcdy_PV + 
    covPV(0,2) * 2. * dMcdx_PV * dMcdz_PV + 
    covPV(1,2) * 2. * dMcdy_PV * dMcdz_PV ;


  // -- the errors on the momentum x vertex
  const double errsqMom =
    // -- the diagonal for the momentum
    covP(3,3) * dMcdpx * dMcdpx +   
    covP(4,4) * dMcdpy * dMcdpy + 
    covP(5,5) * dMcdpz * dMcdpz + 
    covP(6,6) * dMcdE * dMcdE   + 
    // -- momentum x momomentum cross terms
    covP(3,4) * 2. * dMcdpx * dMcdpy +   
    covP(3,5) * 2. * dMcdpx * dMcdpz + 
    covP(3,6) * 2. * dMcdpx * dMcdE  + 
    covP(4,5) * 2. * dMcdpy * dMcdpz + 
    covP(4,6) * 2. * dMcdpy * dMcdE  +
    covP(5,6) * 2. * dMcdpz * dMcdE  +  
    // -- momentum x position terms
    covP(0,3) * 2. * dMcdx_SV * dMcdpx + 
    covP(1,3) * 2. * dMcdy_SV * dMcdpx + 
    covP(2,3) * 2. * dMcdz_SV * dMcdpx +
    covP(0,4) * 2. * dMcdx_SV * dMcdpy + 
    covP(1,4) * 2. * dMcdy_SV * dMcdpy + 
    covP(2,4) * 2. * dMcdz_SV * dMcdpy +
    covP(0,5) * 2. * dMcdx_SV * dMcdpz + 
    covP(1,5) * 2. * dMcdy_SV * dMcdpz + 
    covP(2,5) * 2. * dMcdz_SV * dMcdpz +
    covP(0,6) * 2. * dMcdx_SV * dMcdE  + 
    covP(1,6) * 2. * dMcdy_SV * dMcdE  + 
    covP(2,6) * 2. * dMcdz_SV * dMcdE;
  
 return std::make_pair( std::sqrt( errsqVertex ), std::sqrt( errsqVertex + errsqMom ) );
 
}

