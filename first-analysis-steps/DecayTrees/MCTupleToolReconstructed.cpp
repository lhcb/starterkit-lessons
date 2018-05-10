// Include files
#include "gsl/gsl_sys.h"

// from Gaudi
#include "GaudiKernel/PhysicalConstants.h"
#include "GaudiKernel/Vector3DTypes.h"

// local
#include "MCTupleToolReconstructed.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Event/MCParticle.h"
#include "MCInterfaces/IMCReconstructible.h"
#include "MCInterfaces/IMCReconstructed.h"

using namespace LHCb;
//-----------------------------------------------------------------------------
// Implementation file for class : MCTupleToolReconstructed
//
// 2009-01-19 : Patrick Koppenburg
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( MCTupleToolReconstructed )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  MCTupleToolReconstructed::MCTupleToolReconstructed( const std::string& type,
                                                      const std::string& name,
                                                      const IInterface* parent )
    : TupleToolBase ( type, name , parent )
    , m_recible(0)
    , m_rected(0)
    , m_pCPPAsct(0)
    , m_pNPPAsct(0)
{
  declareInterface<IMCParticleTupleTool>(this);
  declareProperty("Associate",m_associate=true,"Fill associated protoparticle");
  declareProperty("FillPID",m_pid=true,"Fill PID, also set by Verbose");
}

//=============================================================================
// initialize
//=============================================================================

StatusCode MCTupleToolReconstructed::initialize()
{
  if( ! TupleToolBase::initialize() ) return StatusCode::FAILURE;

  m_recible = tool<IMCReconstructible>("MCReconstructible");
  m_rected  = tool<IMCReconstructed>("MCReconstructed");

  if (m_associate)
  {
    m_pCPPAsct = new ProtoParticle2MCLinker( this, Particle2MCMethod::ChargedPP, LHCb::ProtoParticleLocation::Charged);
    m_pNPPAsct = new ProtoParticle2MCLinker( this, Particle2MCMethod::NeutralPP, LHCb::ProtoParticleLocation::Neutrals);
  }

  if(isVerbose()) m_pid=true;

  return StatusCode::SUCCESS ;
}
//=============================================================================
// Fill
//=============================================================================
StatusCode MCTupleToolReconstructed::fill( const LHCb::MCParticle*
                                           , const LHCb::MCParticle* mcp
                                           , const std::string& head
                                           , Tuples::Tuple& tuple )
{
  const std::string prefix=fullName(head);

  bool test = true;
  int catted = -10 ;
  int catible = -10 ;

  // pointer is ready, prepare the values:
  if( 0!=mcp )
  {
    catted  = m_rected->reconstructed(mcp);
    catible = m_recible->reconstructible(mcp);
  }

  // fill the tuple:
  test &= tuple->column( prefix+"_Reconstructible", catible );
  test &= tuple->column( prefix+"_Reconstructed", catted );
  std::vector<double> PX, PY, PZ, Weights, dlle, dllmu, dllk, dllp, pchi2;

  if ( (0!=mcp) && m_associate && isStable(mcp))
  {
    std::vector<std::pair<const LHCb::ProtoParticle*,double> > ppv = getProtos(mcp);
    for ( std::vector<std::pair<const LHCb::ProtoParticle*,double> >::const_iterator ppp = ppv.begin() ;
          ppp != ppv.end() ; ++ppp)
    {
      const LHCb::ProtoParticle* proto = ppp->first;
      double w = ppp->second;
      /// @todo There's plenty more that can be added here. Like PID for instance.
      if (0!=proto->track())
      {
        Gaudi::XYZVector mom = proto->track()->momentum();
        PX.push_back(mom.X());
        PY.push_back(mom.Y());
        PZ.push_back(mom.Z());
        pchi2.push_back(proto->track()->probChi2());
      }
      Weights.push_back(w);
      if (m_pid){
        dlle.push_back(proto->info( ProtoParticle::CombDLLe,  -999.0 ));
        dllmu.push_back(proto->info( ProtoParticle::CombDLLmu,  -999.0 ));
        dllk.push_back(proto->info( ProtoParticle::CombDLLk,  -999.0 ));
        dllp.push_back(proto->info( ProtoParticle::CombDLLp,  -999.0 ));
      }
    }
  }
  const unsigned int maxPP = 20 ;
  test &= tuple->farray(  prefix+"_PP_PX", PX,  prefix+"_ProtoParticles" , maxPP );
  test &= tuple->farray(  prefix+"_PP_PY", PY,  prefix+"_ProtoParticles" , maxPP );
  test &= tuple->farray(  prefix+"_PP_PZ", PZ,  prefix+"_ProtoParticles" , maxPP );
  test &= tuple->farray(  prefix+"_PP_Weight", Weights,  prefix+"_ProtoParticles" , maxPP );
  test &= tuple->farray(  prefix+"_PP_tr_pchi2", pchi2,  prefix+"_ProtoParticles" , maxPP );
  if (m_pid)
  {
    test &= tuple->farray(  prefix+"_PP_DLLe", dlle,  prefix+"_ProtoParticles" , maxPP );
    test &= tuple->farray(  prefix+"_PP_DLLk", dllk,  prefix+"_ProtoParticles" , maxPP );
    test &= tuple->farray(  prefix+"_PP_DLLp", dllp,  prefix+"_ProtoParticles" , maxPP );
    test &= tuple->farray(  prefix+"_PP_DLLmu", dllmu,  prefix+"_ProtoParticles" , maxPP );
  }


  return StatusCode(test);
}

//=========================================================================
///  Protoparticles list @todo return weight as well
//========================================================================
std::vector<std::pair<const LHCb::ProtoParticle*,double> >
MCTupleToolReconstructed::getProtos(const LHCb::MCParticle* mcp) const
{
  ProtoParticle2MCLinker* asct = (mcp->particleID().threeCharge()==0)?m_pNPPAsct:m_pCPPAsct;
  if (0==asct) Exception("Null PP asociator");
  std::vector<std::pair<const LHCb::ProtoParticle*,double> > ppv ;
  double w=0;
  const LHCb::ProtoParticle* pp = asct->firstP(mcp,w);
  while( pp )
  {
    ppv.push_back(std::pair<const LHCb::ProtoParticle*,double>(pp,w));
    pp = asct->nextP(w);
  }
  return ppv;
}
