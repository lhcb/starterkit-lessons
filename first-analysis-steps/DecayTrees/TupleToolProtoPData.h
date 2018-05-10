// $Id: TupleToolProtopinfo.h,v 1.1 2010-05-21 17:03:11 odescham Exp $
#ifndef _TUPLETOOLEPROTOPDATA_H
#define _TUPLETOOLEPROTOPDATA_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
// Interface
#include "Kernel/IParticleTupleTool.h"
#include "Event/ProtoParticle.h"

//============================================================================
class TupleToolProtoPData : public TupleToolBase, virtual public IParticleTupleTool {
  //==========================================================================
public:
  // Standard constructor
  TupleToolProtoPData( const std::string& type,
                       const std::string& name,
                       const IInterface* parent);

  virtual ~TupleToolProtoPData(){};
  StatusCode fill(const LHCb::Particle*,
                  const LHCb::Particle*,
                  const std::string& ,
                  Tuples::Tuple& ) override;

private:
  std::vector<std::string> m_list;
  int m_last;
};
#endif // _TUPLETOOLEPROTOPDATA_H
