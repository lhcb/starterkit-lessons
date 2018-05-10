#ifndef _TupleToolANNPIDTraining_H
#define _TupleToolANNPIDTraining_H 1

// Base class
#include "DecayTreeTupleBase/TupleToolBase.h"

// Interfaces
#include "Kernel/IParticleTupleTool.h"
#include "RecInterfaces/IChargedProtoANNPIDTupleTool.h"

//============================================================================

class TupleToolANNPIDTraining : public TupleToolBase,
                                virtual public IParticleTupleTool
{

public:

  // Standard constructor
  TupleToolANNPIDTraining( const std::string& type,
                           const std::string& name,
                           const IInterface* parent);

  ~TupleToolANNPIDTraining() {}

  StatusCode initialize() override; ///< Initialise

public:

  StatusCode fill(const LHCb::Particle*,
                  const LHCb::Particle*,
                  const std::string& ,
                  Tuples::Tuple& ) override;

private:

  /// Pointer to the ANNPID tuple tool
  const ANNGlobalPID::IChargedProtoANNPIDTupleTool * m_tuple;

};

#endif // _TupleToolANNPIDTraining_H
