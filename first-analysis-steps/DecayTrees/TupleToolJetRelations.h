#ifndef TUPLETOOLJETRELATIONS_H
#define TUPLETOOLJETRELATIONS_H 1

#include "TupleToolJetsBase.h"
#include "Kernel/IParticleTupleTool.h"
#include <Kernel/IJets2Jets.h>

//autor: Albert Bursche
class TupleToolJetRelations : public TupleToolJetsBase,
                              virtual public IParticleTupleTool
{

public:

  /// Standard constructor
  TupleToolJetRelations( const std::string& type,
                         const std::string& name,
                         const IInterface* parent);

  virtual ~TupleToolJetRelations(){} ///< Destructor

  StatusCode fill( const LHCb::Particle*
                   , const LHCb::Particle*
                   , const std::string&
                   , Tuples::Tuple& ) override;
private:

  std::string m_RelationsContainer;
  bool m_Invert; // invert relation table
  bool m_Reverse;// reverse order in relation table (true highest weight is closest)
  IJets2Jets::Table * m_Table;
  const LHCb::Particle * m_p;
  //  Tuples::Tuple* m_tuple;
  unsigned int m_MaxMatches;
  std::string m_head;
  static double DeltaR(const LHCb::Particle&,const LHCb::Particle&); //can be made virtual to allow using the same code with different distance measure
  bool furtherAway (LHCb::Particle& a,LHCb::Particle& b)
  {return DeltaR(*m_p,a)>DeltaR(*m_p,b);}
  bool fill(unsigned int i,const IJets2Jets::Table::Entry *e);
  std::map<std::string,std::string> m_ExtraTableLocations;
  std::map<std::string,IJets2Jets::Table*> m_ExtraTables;
  StatusCode getTable(std::string loc,IJets2Jets::Table*&tab);
};

#endif // TUPLETOOLJETRELATIONS_H
