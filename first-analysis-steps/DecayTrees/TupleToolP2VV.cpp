// Include files

#include "Event/Particle.h"
// kernel
#include "Kernel/IP2VVPartAngleCalculator.h"            // Interface

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
// local
#include "TupleToolP2VV.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolP2VV
//
// 2007-12-18 : Patrick Koppenburg
// 2008-06-02 : Greig Cowan
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolP2VV )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolP2VV::TupleToolP2VV( const std::string& type,
                                const std::string& name,
                                const IInterface* parent )
    : TupleToolBase ( type, name , parent )
    , m_angleTool()
{
  declareInterface<IParticleTupleTool>(this);
  declareProperty( "Calculator", m_calculator = "Bd2KstarMuMuAngleCalculator" );
  declareProperty( "FillTransversity", m_trans = true );
  declareProperty( "FillHelicity", m_helicity = true );
}

//=========================================================================
//  initialize
//=========================================================================
StatusCode TupleToolP2VV::initialize( )
{
  StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  debug() << "Calculator " << m_calculator << endmsg;

  m_angleTool  = tool<IP2VVPartAngleCalculator>(m_calculator,this);
  if( !m_angleTool ){
    Error("Unable to retrieve the IP2VVPartAngleCalculator tool");
    return StatusCode::FAILURE;
  }
  return sc;
}

//=========================================================================
//  Fill
//=========================================================================
StatusCode TupleToolP2VV::fill( const LHCb::Particle*
                                , const LHCb::Particle* P
                                , const std::string& head
                                , Tuples::Tuple& tuple )
{
  std::string prefix = fullName(head);

  bool test = true;
  if( P ){

    if ( m_helicity ){
      double thetaL = -1000.;
      double thetaK = -1000.;
      double phi = -1000.;

      StatusCode sc_hel = m_angleTool->calculateAngles( P, thetaL, thetaK, phi);

      if (msgLevel(MSG::DEBUG)) debug() << "Three helicity angles are theta_L : "
                                        << thetaL
                                        << " K: "<< thetaK
                                        << " phi: " << phi << endmsg ;

      test &= tuple->column( prefix+"_ThetaL", thetaL );
      test &= tuple->column( prefix+"_ThetaK", thetaK );
      test &= tuple->column( prefix+"_Phi",    phi  );
    }

    if ( m_trans )
    {
      double Theta_tr = -1000.;
      double Phi_tr = -1000.;
      double Theta_V = -1000.;

      StatusCode sc_tr = m_angleTool->calculateTransversityAngles( P,
                                                                   Theta_tr,
                                                                   Phi_tr,
                                                                   Theta_V );

      if (msgLevel(MSG::DEBUG)) debug() << "Three transversity angles are Theta_tr : "
                                        << Theta_tr
                                        << " Phi_tr: " << Phi_tr
                                        << " Theta_phi_tr: " << Theta_V
                                        << endmsg ;

      test &= tuple->column( prefix+"_ThetaTr", Theta_tr );
      test &= tuple->column( prefix+"_PhiTr", Phi_tr );
      test &= tuple->column( prefix+"_ThetaVtr", Theta_V  );
    }
  } else {
    return StatusCode::FAILURE;
  }
  return StatusCode(test);
}
