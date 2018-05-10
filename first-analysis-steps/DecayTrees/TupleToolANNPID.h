#ifndef _TupleToolANNPID_H
#define _TupleToolANNPID_H 1

// STL
#include <string>
#include <vector>

// Base class
#include "DecayTreeTupleBase/TupleToolBase.h"

// Interfaces
#include "Kernel/IParticleTupleTool.h"
#include "RecInterfaces/IChargedProtoANNPIDTool.h"

//============================================================================

class TupleToolANNPID : public TupleToolBase,
                        virtual public IParticleTupleTool
{

public:

  // Standard constructor
  TupleToolANNPID( const std::string& type,
                   const std::string& name,
                   const IInterface* parent);

  ~TupleToolANNPID() {}

  StatusCode initialize() override; ///< Initialise

public:

  StatusCode fill( const LHCb::Particle*,
                   const LHCb::Particle*,
                   const std::string& ,
                   Tuples::Tuple& ) override;

private:

  // Local PID type enum
  enum PID { El = 0, Mu, Pi, Ka, Pr, De, Gh };
  // total PID types possible
  static const std::size_t NPIDs = 7;
  
private:

  /// Activation flag for each type
  std::array<bool,NPIDs> m_pidIsOn;

  /// Pointer to the ANNPID tool
  const ANNGlobalPID::IChargedProtoANNPIDTool * m_pidTool = nullptr;

  /// The ANNPID tunes to fill
  std::vector<std::string> m_pidTunes;

  /// The PID types to fill
  std::vector<std::string> m_pidTypes;

};

#endif // _TupleToolANNPID_H
