#ifndef TUPLETOOLJETS_H
#define TUPLETOOLJETS_H 1

#include "TupleToolJetsBase.h"
#include "Kernel/IParticleTupleTool.h"

//autor: Albert Bursche
class TupleToolJets : public TupleToolJetsBase,
                      virtual public IParticleTupleTool
{

public:

  /// Standard constructor
  TupleToolJets( const std::string& type,
                 const std::string& name,
                 const IInterface* parent);

  virtual ~TupleToolJets(){} ///< Destructor

  StatusCode fill( const LHCb::Particle*
                   , const LHCb::Particle*
                   , const std::string&
                   , Tuples::Tuple& ) override;

private:

  const LHCb::Particle * m_p;
  std::string m_head;

};

#endif // TUPLETOOLJETS_H
