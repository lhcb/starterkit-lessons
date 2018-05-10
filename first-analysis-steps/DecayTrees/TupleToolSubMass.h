#ifndef TUPLETOOLSUBMASS_H
#define TUPLETOOLSUBMASS_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface
#include "Event/Particle.h"
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleProperty.h"
struct IDVAlgorithm;


/** @class TupleToolSubMass TupleToolSubMass.h jborel/TupleToolSubMass.h
 *
 * \brief Return all n-bodies mass combinations from the decay tree
 *
 *
 * \verbatim
from configurables import TupleToolSubMass
Tuple.addTool(TupleToolSubMass)
Tuple.TupleToolSubMass.SetMax = 3  # look to 2- and 3-bodies  (defaut all possible n-bodies)
\endverbatim
 *
 * - will create Sum_{p=2,max) C(N,p)  new entries in the nTuple called [head]_M[ij...]
 *
 * - Particle daughters are sorted by PID at each branch of the tree (cc-independant)
 *
 * **** Substitution property
 *
 * - usage :
 *  TupleTool.Substitution += ["pi+ => K+"]
 *  TupleTool.Substitution += ["K+ => pi+"]
 *
 * - produce alternative mass with substituted PID pi<->K (cc is assumed)
 *
 * -change only one pion (K) at once in case of several pion (K) in the decay tree (producing separate output par pion (K) )
 *
 * **** DoubleSubstitution property
 *
 * - usage :
 *  TupleTool.DoubleSubstitution += ["K+/pi- => pi+/K-"]
 *  TupleTool.DoubleSubstitution += ["K+/K-" => pi+/pi-"]
 *
 * - change all [K+pi-]cc ([K+K-]cc) pairs to (pi+K-)cc ([pi+pi-]cc)
 * - change only one pair at once in case of several pairs in the decay tree (producing separate output per pair)
 * - "/" separator is not mandatory : K+pi- syntax is allowed (just a bit slower to parse)
 *
 *  @author Olivier Deschamps
 *  @date   2011-06-01
 */

class SortDaughtersByPID {
public:
  SortDaughtersByPID(const LHCb::Particle* P,LHCb::IParticlePropertySvc* ppsvc){
    m_sign=1;
    m_ppsvc=ppsvc;
    if ( P )m_sign = ( P->particleID().pid() > 0) ? +1 : -1;
  }
  bool operator()(const LHCb::Particle* c1, const LHCb::Particle* c2) {
    int p1 = ( property( c1->particleID())->selfcc() ) ? c1->particleID().pid() : m_sign*c1->particleID().pid();
    int p2 = ( property( c2->particleID())->selfcc() ) ? c2->particleID().pid() : m_sign*c2->particleID().pid();
    // special case  c1 == c2 but with different decay structure (e.g.  pi0->g(g->ee) : check daughters size
    if( p1 == p2 && c1->daughters().size() != c2->daughters().size())return c1->daughters().size() > c2->daughters().size() ;
    // add a complete test of the decay structure (above test won't work for e.g. Bs->phi(KsKs)phi(K+K-) decay
    // ..
    // else
    return p1 > p2 ;
  };
private :
  const LHCb::ParticleProperty* property(const LHCb::ParticleID pid){return (m_ppsvc) ? m_ppsvc->find( pid ) : NULL;};
  LHCb::IParticlePropertySvc* m_ppsvc;
  int m_sign;
};


class TupleToolSubMass : public TupleToolBase, virtual public IParticleTupleTool {
public:
  /// Standard constructor
  TupleToolSubMass( const std::string& type,
		    const std::string& name,
		    const IInterface* parent);

  StatusCode initialize() override; ///< initialisation

  StatusCode fill( const LHCb::Particle*
			   , const LHCb::Particle*
			   , const std::string&
			   , Tuples::Tuple& ) override;

private :
  std::vector<std::vector<int> >     combine(std::vector<std::vector<int> >  vec, unsigned int level, int nfs);
  std::vector<const LHCb::Particle*> getTree(const LHCb::Particle* P );
  bool isEndTree(const LHCb::Particle* p);
  bool fillVertexInfo(const LHCb::Particle* P, std::vector<const LHCb::Particle*> tree, const std::vector<int> map,
                      Tuples::Tuple& tuple,const std::string prefix);
  std::pair<std::string,double>      getMass(std::vector<const LHCb::Particle*> tree , const std::vector<int> map,
                                             int sPos=-1, double sMass=0.,int sPos2=-1,double sMass2=0.);
  std::string                        getFlag(std::string from,std::string to,int pos,
                                             std::string from2="",std::string to2="",int pos2=-1);
  Gaudi::LorentzVector               sMomentum(const LHCb::Particle* part , double sMass);
  std::pair<std::string,std::string> parseID(std::string PID);
  std::pair<std::string,std::string> parseSubst(std::string subst);


  unsigned int m_max;
  std::vector<std::string> m_subst;
  std::vector<std::string> m_subst2;
  LHCb::IParticlePropertySvc* m_ppsvc;
  const LHCb::ParticleProperty* property(std::string name){return (m_ppsvc) ? m_ppsvc->find( name ) : NULL;};
  const LHCb::ParticleProperty* property(const LHCb::ParticleID pid){return (m_ppsvc) ? m_ppsvc->find( pid ) : NULL;};
  const LHCb::Particle* m_ancestor;
  IDVAlgorithm* m_dva;
  bool m_vertex;
  std::vector<int> m_endTree;
};

#endif // TUPLETOOLSUBMASS_H
