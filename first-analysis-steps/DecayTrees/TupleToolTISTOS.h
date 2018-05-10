#ifndef TUPLETOOLTISTOS_H
#define TUPLETOOLTISTOS_H 1

// Include files
// from Gaudi
#include "TupleToolTriggerBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface
#include "Kernel/ITriggerTisTos.h"


/** @class TupleToolTISTOS TupleToolTISTOS.h
 *
 *  This is an adaptation of the TISTOS example by Tomasz
 *  Skwarnicki for the DecayTreeTuple framework
 *
 *  It saves the trigger TIS/TOS decisions for each
 *  particle for the L0, and each HLT Alley and HltSelection
 *
 *  Saved values for each trigger:
 *
 *  Dec      : the trigger decision, 1 is pass, 0 is fail
 *  TIS      : was the event tis? 1 is yes, 0 is no.
 *  TOS      : was the event tos? 1 is yes, 0 is no.
 *
 * If verbose is true the tool needs somehow to find a list of triggers to fill.
 * In this case it uses the base class TupleToolTriggerBase to sort everything out.
 *
 * If \b VerboseL0 = true
 * L0Decision_xxx : LHCb::L0DUReport->channelDecisionByName(xxx)
 *
 * If \b VerboseHlt1 = true
 * Hlt1_xxx_Decision : filled
 *
 * If \b VerboseHlt2 = true
 * Hlt2_xxx_Decision : filled
 *
 * Verbose flag is a shortcut to turn all verbosity on.
 *
 *  @author V. Gligorov
 *  @date   2008-04-09
 */

class TupleToolTISTOS : public TupleToolTriggerBase,
                        virtual public IParticleTupleTool
{

public:

  /// Standard constructor
  TupleToolTISTOS( const std::string& type,
                   const std::string& name,
                   const IInterface* parent);

  ~TupleToolTISTOS( ); ///< Destructor

  StatusCode initialize() override;

protected:

  /// to make it possible to replace standard TisTos tools.
  std::string m_Hlt1TriggerTisTosName;
  std::string m_Hlt2TriggerTisTosName;
  std::string m_L0TriggerTisTosName;

private:

  ITriggerTisTos* m_Hlt1TriggerTisTosTool;
  ITriggerTisTos* m_Hlt2TriggerTisTosTool;
  ITriggerTisTos* m_L0TriggerTisTosTool;

private:

  StatusCode fillBasic( const LHCb::Particle* top
                        , const LHCb::Particle* P
                        , const std::string& head
                        , Tuples::Tuple& tuple ) override;

  StatusCode fillVerbose( const LHCb::Particle* top
                          , const LHCb::Particle* P
                          , const std::string& head
                          , Tuples::Tuple& tuple ) override;

  void fillDecision( ITriggerTisTos* triggerTisTosTool
                          , const std::string& decision
                          , const std::string& prefix
                          , const std::string& decisionName
                          , Tuples::Tuple &tuple);

  std::string getName(const std::string& mystr);

private:

  StatusCode fillBasic( Tuples::Tuple& T ) override;

  StatusCode fillVerbose( Tuples::Tuple& T ) override;

private:

  // copied from TisTosTobbing/TisTos/TisTos.h to avoid dependence
  enum HitType {kVelo=0,kAllHits=kVelo,kOTIT,kTT,kMuon,kEcal,kHcal,nHitTypes,kNotUsed=nHitTypes};

  std::vector<int> m_pidList;
  bool m_checkPID = false;
  std::string m_hlt1Phys;
  std::string m_hlt2Phys;

  //  must be true to fill this info
  bool m_TIS;
  bool m_TOS;
  bool m_TUS;
  bool m_TPS;

  // set to true to fill only top particle in decay descriptor
  bool m_onlyTop;
};

#endif // TUPLETOOLTISTOS_H
