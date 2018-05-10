#ifndef TUPLETOOLP2VV_H
#define TUPLETOOLP2VV_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface
/** @class TupleToolP2VV TupleToolP2VV.h
 *
 *  \brief Fills three angles as defined in IP2VVPartAngleCalculator.
 *
 * - head_ThetaL : angle of the L+ wrt B in dilepton frame
 * - head_ThetaK : angle of the K+ wrt B in dilepton frame
 * - head_Phi : angle of the K* and LL decay planes in dilepton frame
 *
 * - head_Theta_tr : angle of the L+ wrt z-axis in dilepton frame
 * - head_Phi_tr : azimutal angle of the L+ in dilepton frame
 * - head_Theta_phi_tr : polar angle between the x'-axis and the decaying K+ in the dikaon frame
 *
 *  @author Patrick Koppenburg, Greig Cowan
 *  @date   2007-12-18
 */
struct IP2VVPartAngleCalculator;

class TupleToolP2VV : public TupleToolBase, virtual public IParticleTupleTool {
public:
  /// Standard constructor
  TupleToolP2VV( const std::string& type,
                 const std::string& name,
                 const IInterface* parent);

  StatusCode initialize() override;

  StatusCode fill( const LHCb::Particle*
                   , const LHCb::Particle*
                   , const std::string&
                   , Tuples::Tuple& ) override;

private:

  std::string m_calculator; ///< Name of tool that you want to use to calculate the angles
  bool m_helicity;   ///< Write Helicity angles to tuple
  bool m_trans;      ///< Write Transversity angles to tuple

  IP2VVPartAngleCalculator*   m_angleTool;

};
#endif // TUPLETOOLP2VV_H
