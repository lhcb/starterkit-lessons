#ifndef JBOREL_TUPLETOOLTAGGING_H
#define JBOREL_TUPLETOOLTAGGING_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface
#include "Kernel/IVertexFit.h"

struct IDVAlgorithm;
struct IDistanceCalculator;
struct IBTaggingTool;
struct IVertexFit;

struct VerboseData {
  double id, p, px, py, pz, pt, theta, phi;
  double pid_e, pid_mu, pid_k, pid_p;
  double ip, chi2, bip, bchi2, bp_chi2 ;
  VerboseData(): id(0), p(0), px(0), pz(0), pt(0), theta(0), phi(0),
                 pid_e(0), pid_mu(0), pid_k(0), pid_p(0),
                 ip(0), chi2(0), bip(0), bchi2(0), bp_chi2(0) {}
};

/** @class TupleToolTagging TupleToolTagging.h jborel/TupleToolTagging.h
 *
 * \brief Fill the tagging information in the tuple.
 *
 * There are many taggers for whom there is an enum, and maybe even code,
 * but are not active at the moment.
 * The list of active taggers can be defined here with the property ActiveTaggers
 *
 * Typically however these are
 * - ("OS_Muon");
 * - ("OS_Electron");
 * - ("OS_Kaon");
 * - ("SS_Kaon");
 * - ("SS_Pion");
 * - ("VtxCharge");
 * - ("OS_nnetKaon");
 * - ("SS_nnetKaon");
 * - ("SS_Proton");
 * - ("OS_Charm");
 *
 * Since only one of SS_Kaon and SS_Pion will fire at any on time, these typical
 * cases are packed into a single variable: head+_TAGGER
 *
 *  @code
 *   if(tdec) switch ( taggers[i].type() ) {
 *   case Tagger::OS_Charm    : taggers_code +=1000000000 *(tdec+2); break;
 *   case Tagger::SS_Proton   : taggers_code += 100000000 *(tdec+2); break;
 *   case Tagger::OS_nnetKaon : taggers_code +=  10000000 *(tdec+2); break;
 *   case Tagger::SS_nnetKaon : taggers_code +=   1000000 *(tdec+2); break;
 *   case Tagger::OS_Muon     : taggers_code +=    100000 *(tdec+2); break;
 *   case Tagger::OS_Electron : taggers_code +=     10000 *(tdec+2); break;
 *   case Tagger::OS_Kaon     : taggers_code +=      1000 *(tdec+2); break;
 *   case Tagger::SS_Kaon     : taggers_code +=       100 *(tdec+2); break;
 *   case Tagger::SS_Pion     : taggers_code +=        10 *(tdec+2); break;
 *   case Tagger::VtxCharge   : taggers_code +=         1 *(tdec+2); break;
 *
 *   }
 *  @endcode
 *
 * If the list of active taggers is different, you will need to switch to Verbose mode
 *
 * if Verbose is set the probability and decision for each tagger will be filled.
 * - head+tagger+DEC
 * - head+tagger+PROB
 *
 * @see  BTaggingTool
 * @see  Tagger
 *
 * @author Jeremie Borel
 * @date   2007-11-07
 */
class TupleToolTagging : public TupleToolBase, virtual public IParticleTupleTool {
public:
  /// Standard constructor
  TupleToolTagging( const std::string& type,
		     const std::string& name,
		     const IInterface* parent);

  virtual ~TupleToolTagging(){}; ///< Destructor

  StatusCode initialize() override;

  StatusCode fill( const LHCb::Particle*, const LHCb::Particle*
			   , const std::string&, Tuples::Tuple& ) override;


private:

  std::string m_inputLocation, m_toolName;
  IDVAlgorithm* m_dva;
  const IDistanceCalculator* m_dist;
  IVertexFit *m_fitter;
  IBTaggingTool* m_tagging;

  std::map<int, std::string> m_tagger_map; ///<map of enum to string
  std::map<std::string, int> m_tagger_rmap;///<reverse, map of string to enum

  std::vector<std::string> m_activeTaggers; ///<List of active taggers
  bool m_extendedTagging, m_useFTonDST, m_tagBeautyOnly;
  VerboseData getVerboseData(const LHCb::Particle *p, const LHCb::Particle *B);
};

#endif // JBOREL_TUPLETOOLTAGGING_H
