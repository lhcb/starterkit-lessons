#ifndef TUPLETOOLPARTICLEREFIT_H
#define TUPLETOOLPARTICLEREFIT_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface

struct IDVAlgorithm;
struct ILifetimeFitter;
struct IVertexFit;

namespace LHCb
{
  class Particle;
  class Vertex;
}

/** @class TupleToolParticleReFit TupleToolParticleReFit.h jborel/TupleToolParticleReFit.h
 * \brief Use a standard vertex fitter to refit head of a decay chain and fill DecayTreeTuple
 *
 * Columns filled:
 * - head_OVrefit_status
 * - head_OVrefit_ENDVERTEX_NDOF
 * - head_OVrefit_ENDVERTEX_CHI2
 * - head_OVrefit_MM
 * - head_OVrefit_MMERR
 * - head_OVrefit_P
 * - head_OVrefit_PT
 * - head_OVrefit_P[E|X|Y|Z]
 * - head_OVrefit_TAU
 * - head_OVrefit_TAUERR
 * - head_OVrefit_TAUCHI2
 *
 * The middle name "OV" standard for "OfflineVertexFitter" and can be changed
 * by setting the "ExtraName" property.
 *
 * The main purpose of the tool is to get the MM and TAU varibales after
 * refitting the B particle with or without a certain daughter mass cosntraint.
 *
 * Example 1: refit in order to apply daughter mass cosntraint
 * tuple.addTool(TupleToolParticleReFit())
 * tuple.TupleToolParticleReFit.addTool(OfflineVertexFitter())
 * tuple.TupleToolParticleReFit.OfflineVertexFitter.useResonanceVertex = True
 * tuple.TupleToolParticleReFit.OfflineVertexFitter.applyDauMassConstraint = True
 *
 * Example 2: refit in order to get rid of already applied daughter mass cosntraint
 * tuple.TupleToolParticleReFit.OfflineVertexFitter.useResonanceVertex = False
 *
 * \sa DecayTreeTuple
 *
 *  @author Yuehong Xie
 *  @date   2010-11-2
 */
class TupleToolParticleReFit : public TupleToolBase,
                               virtual public IParticleTupleTool
{

public:

  /// Standard constructor
  TupleToolParticleReFit( const std::string& type,
                          const std::string& name,
                          const IInterface* parent);

  StatusCode initialize() override;

  StatusCode fill( const LHCb::Particle*,
                   const LHCb::Particle*,
                   const std::string&,
                   Tuples::Tuple& ) override;

private:

  IDVAlgorithm* m_dva = nullptr;
  const ILifetimeFitter* m_timefitter = nullptr;
  const IVertexFit* m_vtxfitter = nullptr;

};

#endif // TUPLETOOLPARTICLEREFIT_H
