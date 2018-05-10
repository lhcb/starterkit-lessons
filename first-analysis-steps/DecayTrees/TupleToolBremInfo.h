// $Id: TupleToolBremInfo.h
#ifndef _TUPLETOOLBREMINFO_H
#define _TUPLETOOLBREMINFO_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
// Interfaces
#include "Kernel/IParticleTupleTool.h"
#include "Kernel/IBremAdder.h"
#include <memory>
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleProperty.h"


//============================================================================
class TupleToolBremInfo : public TupleToolBase, virtual public IParticleTupleTool {
  //==========================================================================

public:

  // Standard constructor
  TupleToolBremInfo( const std::string& type,
                     const std::string& name,
                     const IInterface* parent);

  StatusCode initialize() override;
  virtual ~TupleToolBremInfo( ) {}; ///< Destructor

  StatusCode fill( const LHCb::Particle*,
                   const LHCb::Particle*,
                   const std::string&,
                   Tuples::Tuple& ) override;
private :
  const LHCb::ParticleProperty* property(const LHCb::ParticleID pid){return (m_ppsvc) ? m_ppsvc->find( pid ) : NULL;};
  LHCb::IParticlePropertySvc* m_ppsvc;
  IBremAdder* m_adder;
  std::vector<std::string> m_parts;
  std::vector<unsigned int> m_pids;
  bool m_dst;
};

#endif // _TUPLETOOLBREMINFO_H
