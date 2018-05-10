#ifndef JBOREL_TUPLETOOLPROPERTIME_H
#define JBOREL_TUPLETOOLPROPERTIME_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface

struct IDVAlgorithm;
struct ILifetimeFitter;

namespace LHCb
{
  class Particle;
  class Vertex;
}

/** @class TupleToolPropertime TupleToolPropertime.h jborel/TupleToolPropertime.h
 * \brief Fills the propertime for DecayTreeTuple
 *
 * Columns filled:
 * - head_TAU
 * - head_TAUERR
 * - head_TAUCHI2
 *
 * \sa DecayTreeTuple
 *
 *  @author Jeremie Borel
 *  @date   2007-11-07
 */
class TupleToolPropertime : public TupleToolBase,
                            virtual public IParticleTupleTool
{

public:

  /// Standard constructor
  TupleToolPropertime( const std::string& type,
                       const std::string& name,
                       const IInterface* parent);

  StatusCode initialize() override;

  StatusCode fill( const LHCb::Particle*, const LHCb::Particle*
                   , const std::string&, Tuples::Tuple& ) override;

private:

  const LHCb::Vertex* originVertex( const  LHCb::Particle*
                                    , const LHCb::Particle* ) const;

private:

  IDVAlgorithm* m_dva;
  const ILifetimeFitter* m_fit;
  bool        m_fitToPV;

};

#endif // JBOREL_TUPLETOOLPROPERTIME_H
