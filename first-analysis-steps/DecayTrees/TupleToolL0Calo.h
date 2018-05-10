#ifndef TUPLETOOLL0CALO_H
#define TUPLETOOLL0CALO_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface

struct IPart2Calo;
class DeCalorimeter ;
struct ICaloTriggerAdcsFromRaw ;

/** @class TupleToolL0Calo TupleToolL0Calo.h
 *
 * \brief Calorimeter information to compute L0 efficiencies
 *
 * The particle is projected on the selected calorimeter surface, and the projection information is retrieved.
 * Tuple columns:
 * - realET: ET of the particle when arrived on the calorimeter
 * - xProjection: X dimension of the particle projection on the calorimeter
 * - yProjection: Y dimension of the particle projection on the calorimeter
 * - region: calorimeter region corresponding to the particle projection
 *    -1 = outside of calorimeter,
 *    HCAL: 0 = outer, 1 = inner
 *    ECAL: 0 = outer, 1 = middle, 2 = inner
 * The calorimeter on which to project can be chosen by setting the
 * "WhichCalo" property to "ECAL" or "HCAL".
 * "HCAL" is selected by default.
 *
 *  @author Alexandra Martin Sanchez
 *  @date   2012-10-11
 */

class TupleToolL0Calo : public TupleToolBase,
                        virtual public IParticleTupleTool
{

public:

  /// Standard constructor
  TupleToolL0Calo( const std::string& type,
                   const std::string& name,
                   const IInterface* parent);

  virtual ~TupleToolL0Calo( ){}; ///< Destructor

  StatusCode initialize() override;
  StatusCode fill( const LHCb::Particle* /* mother */,
                   const LHCb::Particle* P,
                   const std::string& head,
                   Tuples::Tuple& tuple) override;

private :

  int isinside_HCAL(double x,
                    double y);
  int isinside_ECAL(double x,
                    double y);
  /// Get the associated L0/LLT cluster
  double getAssociatedCluster( double & hcal_energy , double & xTrigger ,
                               double & yTrigger );

  IPart2Calo* m_part2calo;
  std::string m_calo;
  std::string m_location ;
  bool        m_fillTriggerEt ;
  DeCalorimeter * m_caloDe ;
  ICaloTriggerAdcsFromRaw * m_adcsHcal ; ///< Tool to decode trigger adcs.

};

#endif // TUPLETOOLL0CALO_H
