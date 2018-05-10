#ifndef TUPLETOOLJETSFORB_H
#define TUPLETOOLJETSFORB_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface

#include "Kernel/IDistanceCalculator.h"
#include "Event/HltObjectSummary.h"

#include "Kernel/ITriggerTisTos.h"

struct IDVAlgorithm;

typedef std::pair<double,LHCb::Particle*> PtParticlePair;

/** @class TupleToolJetsForB TupleToolJetsForB.h
 *
 *
 *  @author Xabier Cid Vidal
 *  @date   2014-06-30
 */
class TupleToolJetsForB : public TupleToolBase,
                          virtual public IParticleTupleTool{

public:
  /// Standard constructor
  TupleToolJetsForB( const std::string& type,
                        const std::string& name,
                        const IInterface* parent);

  StatusCode initialize() override;


  StatusCode fill( const LHCb::Particle*
                   , const LHCb::Particle*
                   , const std::string&
                   , Tuples::Tuple& ) override;

protected:

private:

  std::string m_loc_jetsnomu; //location of jets with no muons
  std::string m_loc_jetsinc; //location of jets with no veto
  std::string m_loc_jetsb; //locations of jets with B
  bool m_use_jetsnomu; //look at jets with no muons
  bool m_use_jetsinc; // look at jets with no veto
  bool m_use_jetsb;  // look at jets with B
  bool m_onemu; // one muon only (for Bmunu)
  int m_ind_part; // pid of the individual particle (13 for Bmunu)

  IDVAlgorithm *m_dva; // parent DVA Algorithm
  const IDistanceCalculator* m_dist;  // for obtaining the best PV
  //IJetTagTool*  m_nnjettag; //Jet Tag NN
    ITriggerTisTos* m_TriggerTisTosTool;
  // maps for all the extra info for all the jets
  std::map <std::string,double> m_JetNoMu1;
  std::map <std::string,double> m_JetNoMu2;
  std::map <std::string,double> m_JetNoMu3;
  std::map <std::string,double> m_JetMu1;
  std::map <std::string,double> m_JetMu2;
  std::map <std::string,double> m_JetB;

  bool m_forcePV;// force same PV as top candidate or not!

  double jetNNTag(const LHCb::Particle* jet);
  StatusCode getJetLHCbIDs(const LHCb::Particle* p,  std::vector< LHCb::LHCbID > & AllIDs) const;
  StatusCode getHltObjLHCbIDs(const LHCb::HltObjectSummary * sum, std::vector< LHCb::LHCbID > & AllIDs) const;

      void pt_sorted_samePV(const LHCb::Particles & jets_list , const int pvkey, std::vector<LHCb::Particle*> & out_list);

  StatusCode find_jet_b(const LHCb::Particles* list_of_jets,
                        LHCb::Particle& myjet);
  StatusCode find_jet_mu(const LHCb::Particles* list_of_jets,
                         const LHCb::Particle mu, LHCb::Particle& myjet);

  void fillInfo(const std::string& prefix, Tuples::Tuple& tuple);
  std::map <std::string,double> fillProperties(LHCb::Particle* jet);
  std::map <std::string,double> emptyProperties(void);

};

#endif // TUPLETOOLJETSFORB_H
