#ifndef JBOREL_TUPLETOOLMASSHYPO_H
#define JBOREL_TUPLETOOLMASSHYPO_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface
#include "Event/Particle.h"

typedef std::pair<const LHCb::Particle*,int> possPair ;
typedef std::vector< possPair > possDecay ;  // not a std::map
typedef std::vector< possDecay > possDecayVec ;

/** @class TupleToolMassHypo TupleToolMassHypo.h jborel/TupleToolMassHypo.h
 *
 * \brief Return mass of composite under another PID hypothesis
 *
 * An example for Ks0:
 * \verbatim
 from configurables import TupleToolMassHypo
 Tuple.addTool(TupleToolMassHypo)
 Tuple.TupleToolMassHypo.PIDReplacements = { "pi+" : "p+" }
 Tuple.TupleToolMassHypo.CC = True
 \endverbatim
 *
 * will create 4 new entries in the nTuple called KS0_M_with_piplus_piminus,
 * KS0_M_with_piplus_p~minus, KS0_M_with_pplus_piminus, KS0_M_with_pplus_p~minus.
 *
 * Cases with 3 or more daughters are possible but not tested.
 *
 * Warnings:
 *
 * 1) PIDReplacement is a python dictionary. To test two mass hypthesis replacements
 *   for the same daughter you must used 2 tool instances.
 *
 * 2) When CC = True (default), PIDReplacements implies cc.
 *
 * \sa DecayTreeTuple
 *
 *  @author Patrick Koppenburg
 *  @date   2011-02-10
 */
struct IParticleTransporter;
class TupleToolMassHypo : public TupleToolBase, virtual public IParticleTupleTool {
public:
  /// Standard constructor
  TupleToolMassHypo( const std::string& type,
                     const std::string& name,
                     const IInterface* parent);

  StatusCode initialize() override; ///< initialisation

  StatusCode fill( const LHCb::Particle*
                   , const LHCb::Particle*
                   , const std::string&
                   , Tuples::Tuple& ) override;

private :
  double recalculateMass(const possDecay&, const Gaudi::XYZPoint&, std::string& combname);
  possDecayVec getPossibilities(const LHCb::Particle::ConstVector&);
  possDecayVec increaseVector(const LHCb::Particle* , possDecayVec& );
  void addPossPair(const LHCb::Particle* , int pid, possDecay& );
  void printVec(const possDecayVec&);
  std::map<std::string,std::string> m_replace ; ///< the PIDs to replace
  std::map<int,int> m_replacePDG ; ///< the PIDs to replace (as PDG codes)
  bool m_revert ; ///< revert to positive PDG codes (default)
  bool m_first ;
  IParticleTransporter* m_transporter;
  std::map<int,double> m_massMap ;
  std::map<int,std::string> m_nameMap ;
};

#endif // JBOREL_TUPLETOOLMASSHYPO_H
