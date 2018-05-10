#ifndef TUPLETOOLNEUTRINORECO_H
#define TUPLETOOLNEUTRINORECO_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface

/** @class TupleToolNeutrinoReco TupleToolNeutrinoReco.h
 *
 * \brief Fill neutrino reconstruction variables
 *
 * Assuming a particle of zero-mass has gone undetected, we can calculate
 * the corrected momentum for the true moteh particle, if we know its mass.
 *
 * Firstly PT and P|| are the momenta perp and parl to the direction of flight.
 *
 * 0 = aP(neutrino)^2+bP(neutrino)+c
 * a = 4(PT^2+M^2)
 * b = 4P||(2PT^2 - mB^2 + M^2)             = X - 4P||*mB^2
 * c = 4PT^2(P||^2 + mB^2) -(mB^2 - M^2)^2 = Y + 4PT ^2*mB*2 - (mB^2 - M^2)^2
 *
 * P*(neutrino)= [-b +/- sqrt(b^2 - 4ac)]/(2a)
 *
 * With two ambiguous solutions
 *
 * PB=P||+P(neutrino)
 *
 * Option:
 * - MotherMass: double: PDG Mass of the mother particle
 *
 * If no mother particle mass is set the following are filled:
 *
 * - head_Nu_Parl : momentum of the input particle paralell to the direction of flight
 * - head_Nu_Perp : momentum of the input particle perpendicular to the direction of flight
 *
 * If a non-zero mother mass is set, the two solutions will be calculated
 * - head_Nu_Hi   : the B-momentum with the positive sqrt solution
 * - head_Nu_Lo   : the B-momentum with the negative sqrt solution
 *
 * If the verbose flag is set, the intemediate working will also be stored
 * - head_Nu_a    : a in the above equation
 * - head_Nu_X    : X in the above equation, 4P||(2PT^2 + M^2)
 * - head_Nu_Y    : Y in the above equation, 4PT^2*P||^2
 *
 * It can be useful to store these variables, since they are the part of the
 * equation which does not depend on the mass of the mother particle, in case an
 * overall mass rescaling is required the remaining calculation can be done as follows:
 *
 * Note that the solution is given by the quadratic formula,
 * Note also that -b/2a simplifies to:
 *
 * -b/2a = - P|| [ 1 - (mB^2 - PT^2)/(PT^2+M^2)    ] / 2
 *
 * So, with 'head' replaced by something like 'B0', and mB being the PDG mass:
 *
 * b = B0_Nu_X - 4*B0_Nu_Parl*mB*mB
 * c = B0_Nu_Y + 4*B0_Nu_Perp*B0_Nu_Perp*mB*mB - (mB*mB - B0_MM*B0_MM)*(mB*mB - B0_MM*B0_MM)
 *
 * pb = -B0_Nu_Parl*( 1 - (mB*mB-B0_Nu_Perp*B0_Nu_Perp)/(B0_Nu_Perp*B0_Nu_Perp + B0_MM*B0_MM)  )/2
 * pd = sqrt(b*b - 4*B0_Nu_a*c)/(2*B0_Nu_a)
 *
 * B0_Nu_Lo = B0_Nu_Parl+pb-pd
 * B0_Nu_Hi = B0_Nu_Parl+pb+pd
 *
 * Are the two B-momentum solutions
 *
 * \sa DecayTreeTuple
 *
 *  @author Rob Lambert
 *  @date   2010-11-30
 */
namespace LHCb
{
  class VertexBase;
  class Particle;
}
struct IDVAlgorithm;

class TupleToolNeutrinoReco : public TupleToolBase,
                              virtual public IParticleTupleTool
{

public:

  /// Standard constructor
  TupleToolNeutrinoReco( const std::string& type,
                         const std::string& name,
                         const IInterface* parent);

public:

  StatusCode initialize() override;

  StatusCode fill( const LHCb::Particle*
                   , const LHCb::Particle*
                   , const std::string&
                   , Tuples::Tuple& ) override;

  double MomentumParallel(const LHCb::VertexBase* ,
                          const LHCb::Particle* ,
                          const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >* );

  double MomentumPerpendicular(const LHCb::VertexBase* ,
                               const LHCb::Particle* ,
                               const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >* ) ;

  double MomentumQuadratic(const double,
                           const double,
                           const double);

  double MomentumSqrt(const double,
                      const double,
                      const double);

private:

  IDVAlgorithm* m_dva = nullptr;
  double m_motherMass;

};

#endif // TUPLETOOLNEUTRINORECO_H
