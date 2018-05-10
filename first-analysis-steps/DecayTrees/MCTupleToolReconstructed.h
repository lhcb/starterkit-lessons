#ifndef MCTUPLETOOLMCRECONSTRUCTED_H
#define MCTUPLETOOLMCRECONSTRUCTED_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IMCParticleTupleTool.h"            // Interface
#include "Kernel/Particle2MCLinker.h"
#include "Event/ProtoParticle.h"

/** @class MCTupleToolReconstructed MCTupleToolReconstructed.h
 *
 *  Fill MC Particle
 *
 * See the properties inline documentation to toggle on/off some of
 * the columns
 *
 * - head_Reconstructible : IMCReconstructible category
 * - head_Reconstructed : IMCReconstructed  category
 * - head_PP_PX,Y,Z : ProtoParticle PX, PY, PZ
 * - head_PP_Weight : ProtoParticle association weight
 *
 * \sa MCDecayTreeTuple, IMCReconstructible, IMCReconstructed
 *
 *  @author Patrick Koppenburg
 *  @date   2009-01-22
 */
struct IMCReconstructible ;
struct IMCReconstructed ;

class MCTupleToolReconstructed : public TupleToolBase, virtual public IMCParticleTupleTool {
public:
  /// Standard constructor
  MCTupleToolReconstructed( const std::string& type,
                            const std::string& name,
                            const IInterface* parent);

  StatusCode initialize() override;

public:

  StatusCode fill( const LHCb::MCParticle*
                   , const LHCb::MCParticle*
                   , const std::string&
                   , Tuples::Tuple& ) override;


private:

  std::vector<std::pair<const LHCb::ProtoParticle*,double> > getProtos(const LHCb::MCParticle*) const;

#include "DecayTreeTupleBase/isStable.h"

private:

  IMCReconstructible* m_recible ; ///< Reconstructible tool
  IMCReconstructed* m_rected ; ///< Reconstructed too
  ProtoParticle2MCLinker* m_pCPPAsct ; ///< Charged protoparticle associator
  ProtoParticle2MCLinker* m_pNPPAsct ; ///< Neutral protoparticle associator
  bool m_associate ; ///< Do association
  bool m_pid ; ///< Do PID

};
#endif // MCTUPLETOOLMCRECONSTRUCTED_H
