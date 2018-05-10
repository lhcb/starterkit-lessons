#ifndef TUPLETOOLHOP_H
#define TUPLETOOLHOP_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleProperty.h"

struct IDVAlgorithm;

namespace LHCb
{
  class Particle;
  class VertexBase;
}


/** @class TupleToolHOP TupleToolHOP.h
 *
 * \brief Fill geometry related information for DecayTreeTuple
 *
 * - mother_HOP : HOP correction, the ratio between the Pt (with respecto to the mothers direction of flight) of electrons and non-electrons
 * - mother_HOP_MASS : Mother mass obtained by scalling the electrons P by the hop factor
 * - mother_ELECTRON_MASS : Mass obtained by summing the four-momenta of all electrons
 *
 *
 *
 * \sa DecayTreeTuple
 *
 *  @author Vinicius Franco, Carla Marin Benito
 *  @date   2016-29-06
 */


class TupleToolHOP : public extends<TupleToolBase, IParticleTupleTool>
{

public:

  /// Standard constructor
  using base_class::base_class;

  StatusCode initialize() override;

  StatusCode fill( const LHCb::Particle*
                   , const LHCb::Particle*
                   , const std::string&
                   , Tuples::Tuple& ) override;

private:
  float HOPProjectMomentum(const LHCb::Particle* top,
                           const Gaudi::LorentzVector* part_four_mom) const;

  int encodeTuple(int isElectron, int hasSameChildren) const { return (isElectron << 2) | hasSameChildren; }
  int isElectron(int tuple) const { return tuple >> 2 ; } ;
  int hasSameChildren(int tuple) const { return tuple & 3 ; } ;


  int ClassifyParticles(
      const LHCb::Particle& top,
      SmartRefVector<LHCb::Particle>& electronContainer,
      SmartRefVector<LHCb::Particle>& nonElectronContainer) const;

  int ClassifyParticles_Merge(
      const SmartRefVector<LHCb::Particle>& dau,
      std::vector<int>& t_list,
      bool are_equal,
      SmartRefVector<LHCb::Particle>& electronContainer,
      SmartRefVector<LHCb::Particle>& nonElectronContainer) const;

private:
  //bool m_fillMother;
  IDVAlgorithm* m_dva = nullptr;
  LHCb::IParticlePropertySvc * m_ppSvc = nullptr;
  float m_electronMassSquared = 0.0;
};
#endif // TUPLETOOLHOP_H
