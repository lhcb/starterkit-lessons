#ifndef TUPLETOOLJETSBASE_H
#define TUPLETOOLJETSBASE_H 1

#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"
#include "LoKi/LoKi.h"
#include "Kernel/JetEnums.h"

//autor: Albert Bursche
class TupleToolJetsBase : public TupleToolBase, virtual public IParticleTupleTool {
public:
  TupleToolJetsBase( const std::string& type,
                     const std::string& name,
                     const IInterface* parent);

  StatusCode initialize() override;
protected:
  Tuples::Tuple* m_tuple;
  LoKi::Types::Fun charge;
  LoKi::Types::Fun positiveParticles;
  LoKi::Types::Fun negativeParticles;
  LoKi::Types::Fun neutralParticles;
  LoKi::Types::Fun maxPT;
  LoKi::Types::Fun m_M;
  LoKi::Types::Fun m_MM;

  bool WriteJetToTuple(const LHCb::Particle*jet,std::string prefix);
  double MaxSumNPart(const LHCb::Particle* jet,unsigned int n,const LoKi::Types::Fun& fun,SmartRefVector< LHCb::Particle >* SortedDaughters);
  template <class T1,class T2>
  class Comperator
  {
    const LoKi::Types::Fun & m_fun;
  public:
    Comperator(const LoKi::Types::Fun &fun)
      :m_fun(fun)
    {}
    bool operator()(T1 t1,T2 t2)
    {return m_fun(t1)>m_fun(t2);}
  };
};

#endif //TUPLETOOLJETSBASE_H
