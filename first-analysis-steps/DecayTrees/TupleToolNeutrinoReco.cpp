// Include files

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include <Kernel/IDVAlgorithm.h>
#include <Kernel/GetIDVAlgorithm.h>

// local
#include "TupleToolNeutrinoReco.h"

#include "Event/Particle.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolNeutrinoReco
//
// 2010-11-24 : Rob Lambert
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
// actually acts as a using namespace TupleTool
DECLARE_COMPONENT( TupleToolNeutrinoReco )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolNeutrinoReco::TupleToolNeutrinoReco( const std::string& type,
                                                const std::string& name,
                                                const IInterface* parent )
    : TupleToolBase ( type, name , parent )
{
  declareInterface<IParticleTupleTool>(this);

  declareProperty("MotherMass",m_motherMass=0.);
}

//=============================================================================

StatusCode TupleToolNeutrinoReco::initialize()
{
  if( ! TupleToolBase::initialize() ) return StatusCode::FAILURE;

  m_dva = Gaudi::Utils::getIDVAlgorithm ( contextSvc(), this ) ;

  if (!m_dva) return Error("Couldn't get parent DVAlgorithm",
                           StatusCode::FAILURE);

  return StatusCode::SUCCESS;
}

//=============================================================================

StatusCode TupleToolNeutrinoReco::fill( const LHCb::Particle*
                                        , const LHCb::Particle* P
                                        , const std::string& head
                                        , Tuples::Tuple& tuple )
{
  const std::string prefix=fullName(head);

  bool test = true;
  if( P )
  {
    const LHCb::VertexBase* aPV = m_dva->bestVertex( P );
    // Parallel and Perpendicular Momentum of the particle
    // releative to the flight direction
    double Nu_Parl = MomentumParallel(aPV,P,&P->momentum());
    double Nu_Perp = MomentumPerpendicular(aPV,P,&P->momentum());


    if(isVerbose() || m_motherMass==0.)
    {

      test &= tuple->column( prefix+"_Nu_Parl", Nu_Parl );
      test &= tuple->column( prefix+"_Nu_Perp", Nu_Perp );
    }

    if(isVerbose())
    {
      //intermediate steps,
      //all that do not require the mother mass

      double a=4.*(Nu_Perp*Nu_Perp+P->measuredMass()*P->measuredMass());
      double X=4.*Nu_Parl*(2*Nu_Perp*Nu_Perp+P->measuredMass()*P->measuredMass());
      double Y=4.*Nu_Perp*Nu_Perp*Nu_Parl*Nu_Parl;
      test &= tuple->column( prefix+"_Nu_a", a );
      test &= tuple->column( prefix+"_Nu_X", X );
      test &= tuple->column( prefix+"_Nu_Y", Y );
    }

    if(m_motherMass!=0.)
    {
      double pb=MomentumQuadratic(Nu_Parl,Nu_Perp,P->measuredMass());
      double pd=MomentumSqrt(Nu_Parl,Nu_Perp,P->measuredMass());

      if(isVerbose())
      {
        test &= tuple->column( prefix+"_Nu_pb", pb );
        test &= tuple->column( prefix+"_Nu_pd", pd );
      }

      double P_Hi=Nu_Parl+pb+pd;
      double P_Lo=Nu_Parl+pb-pd;

      test &= tuple->column( prefix+"_Nu_Hi", P_Hi );
      test &= tuple->column( prefix+"_Nu_Lo", P_Lo );

    }


  }
  else
  {
    return StatusCode::FAILURE;
  }
  return StatusCode(test);
}
double TupleToolNeutrinoReco::MomentumParallel(const LHCb::VertexBase* pv,
                                               const LHCb::Particle* b,
                                               const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >* mv)
{

  // fd means flight distance
  if (b->endVertex()==NULL || pv==NULL) return 0.;

  double fd_x = b->endVertex()->position().X()-pv->position().X();
  double fd_y = b->endVertex()->position().Y()-pv->position().Y();
  double fd_z = b->endVertex()->position().Z()-pv->position().Z();

  double fd = sqrt(fd_x*fd_x + fd_y*fd_y + fd_z*fd_z);

  fd_x=fd_x/fd;
  fd_y=fd_y/fd;
  fd_z=fd_z/fd;

  // return the dot product of the flight distance unit vector
  // with the momentum
  return mv->Px()*fd_x + mv->Py()*fd_y + mv->Pz()*fd_z;
}

double TupleToolNeutrinoReco::MomentumPerpendicular(const LHCb::VertexBase* pv,
                                                    const LHCb::Particle* b,
                                                    const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >* mv)
{
  if (b->endVertex()==NULL || pv==NULL) return 0.;
  double fd_x = b->endVertex()->position().X()-pv->position().X();
  double fd_y = b->endVertex()->position().Y()-pv->position().Y();
  double fd_z = b->endVertex()->position().Z()-pv->position().Z();

  double fd = sqrt(fd_x*fd_x + fd_y*fd_y + fd_z*fd_z);

  fd_x=fd_x/fd;
  fd_y=fd_y/fd;
  fd_z=fd_z/fd;

  double P_Pa = mv->Px()*fd_x + mv->Py()*fd_y + mv->Pz()*fd_z;

  double P_Pa_x = fd_x*P_Pa;
  double P_Pa_y = fd_y*P_Pa;
  double P_Pa_z = fd_z*P_Pa;

  double P_Pe_x = mv->Px()-P_Pa_x;
  double P_Pe_y = mv->Py()-P_Pa_y;
  double P_Pe_z = mv->Pz()-P_Pa_z;

  return sqrt(P_Pe_x*P_Pe_x + P_Pe_y*P_Pe_y + P_Pe_z*P_Pe_z);
}


double TupleToolNeutrinoReco::MomentumQuadratic(const double Parl,
                                                const double Perp,
                                                const double Mass)
{
  return -0.5*Parl*(1.-(m_motherMass*m_motherMass-Perp*Perp)/(Perp*Perp+Mass*Mass));
}

double TupleToolNeutrinoReco::MomentumSqrt(const double Parl,
                                           const double Perp,
                                           const double Mass)
{
  double a=4.*(Perp*Perp+Mass*Mass);
  double b=4.*Parl*(2.*Perp*Perp-m_motherMass*m_motherMass+Mass*Mass);
  double c=
    4.*Perp*Perp*(Parl*Parl+m_motherMass*m_motherMass)
    -
    (m_motherMass*m_motherMass-Mass*Mass)*(m_motherMass*m_motherMass-Mass*Mass);

  double d=b*b-4.*a*c;
  if (d>0.) return std::sqrt(d)/(2.*a);
  else return 0;

}

