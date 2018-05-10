#ifndef MCTUPLETOOLPROMPT_H
#define MCTUPLETOOLPROMPT_H 1

// from Gaudi
#include "GaudiKernel/PhysicalConstants.h"

// from Kernel
#include "Kernel/IMCParticleTupleTool.h"
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleProperty.h"

#include "DecayTreeTupleBase/TupleToolBase.h"

// from GaudiAlg
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

// from Event
#include "Event/MCParticle.h"


/** @class MCTupleToolPrompt MCTupleToolPrompt.h
 *
 * \brief Tool to determine whether a decay originates from prompt charm based on the true lifetime of its ancestors.
 * Based on code written by Patrick Spradlin.
 *
 * Tuple columns:
 * - MC_ISPROMPT, MC_LONGLIVED_ID, MC_LONGLIVED_KEY
 *
 *  @author Philip John Hunt
 *  @date   2011-06-13
 */


class MCTupleToolPrompt : public TupleToolBase,
                          virtual public IMCParticleTupleTool
{

public:

  /// Standard constructor
  MCTupleToolPrompt( const std::string& type,
                     const std::string& name,
                     const IInterface* parent);

  virtual ~MCTupleToolPrompt( ); ///< Destructor

  StatusCode initialize() override;

  StatusCode fill( const LHCb::MCParticle*
                   , const LHCb::MCParticle*
                   , const std::string&
                   , Tuples::Tuple& ) override;

protected:

  double m_maxLifetime;
  bool m_storeLongLivedPid;

private:

  LHCb::IParticlePropertySvc *m_ppSvc;

};

#endif // MCTUPLETOOLPROMPT_H
