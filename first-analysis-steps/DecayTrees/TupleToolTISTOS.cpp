// Include files
#include "boost/regex.hpp"

#include "Event/HltDecReports.h"
#include "Event/HltSelReports.h"
#include "Event/Particle.h"
// kernel
#include "Kernel/IANNSvc.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
// local
#include "TupleToolTISTOS.h"

//#include <sstream>

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolTISTOS
//
// 2008-04-09 : V. Gligorov
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolTISTOS )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolTISTOS::TupleToolTISTOS(const std::string &type,
                                 const std::string &name,
                                 const IInterface *parent)
    : TupleToolTriggerBase(type, name, parent),
      m_Hlt1TriggerTisTosName("Hlt1TriggerTisTos"),
      m_Hlt2TriggerTisTosName("Hlt2TriggerTisTos"),
      m_L0TriggerTisTosName("L0TriggerTisTos"), m_Hlt1TriggerTisTosTool(NULL),
      m_Hlt2TriggerTisTosTool(NULL), m_L0TriggerTisTosTool(NULL),
      m_pidList(0) // 1, 22)
{
  declareInterface<IParticleTupleTool>(this);
  declareProperty("Hlt1TriggerTisTosName", m_Hlt1TriggerTisTosName);
  declareProperty("Hlt2TriggerTisTosName", m_Hlt2TriggerTisTosName);
  declareProperty("L0TriggerTisTosName", m_L0TriggerTisTosName);
  declareProperty("PIDList", m_pidList);
  declareProperty("TopParticleOnly", m_onlyTop = false);
  declareProperty("Hlt1Phys", m_hlt1Phys = "Hlt1(?!ODIN)(?!L0)(?!Lumi)(?!Tell1)(?!MB)(?!NZS)(?!Velo)(?!BeamGas)(?!Incident).*Decision");
  declareProperty("Hlt2Phys", m_hlt2Phys = "Hlt2(?!Forward)(?!DebugEvent)(?!Express)(?!Lumi)(?!Transparent)(?!PassThrough).*Decision");
  declareProperty("TIS", m_TIS = true);
  declareProperty("TOS", m_TOS = true);
  declareProperty("TUS", m_TUS = false);
  declareProperty("TPS", m_TPS = false);
}

//=============================================================================
// Destructor
//=============================================================================
TupleToolTISTOS::~TupleToolTISTOS() {}

//=============================================================================

//=========================================================================
//  initialize
//=========================================================================
StatusCode TupleToolTISTOS::initialize()
{
  const StatusCode sc = TupleToolTriggerBase::initialize();
  if (sc.isFailure())
  {
    return sc;
  }

  m_Hlt1TriggerTisTosTool = tool<ITriggerTisTos>(m_Hlt1TriggerTisTosName, "Hlt1TriggerTisTos", this);
  m_Hlt2TriggerTisTosTool = tool<ITriggerTisTos>(m_Hlt2TriggerTisTosName, "Hlt2TriggerTisTos", this);
  m_L0TriggerTisTosTool = tool<ITriggerTisTos>(m_L0TriggerTisTosName, "L0TriggerTisTos", this);

  return sc;
}

void TupleToolTISTOS::fillDecision(ITriggerTisTos *triggerTisTosTool,
                                   const std::string &decision,
                                   const std::string &prefix,
                                   const std::string &decisionName,
                                   Tuples::Tuple &tuple)
{
  triggerTisTosTool->setTriggerInput(decision);
  const auto classifiedDec = triggerTisTosTool->tisTosTobTrigger();
  tuple->column(prefix + "_" + decisionName + "_Dec", classifiedDec.decision());
  if (m_TIS)
    tuple->column(prefix + "_" + decisionName + "_TIS", classifiedDec.tis());
  if (m_TOS)
    tuple->column(prefix + "_" + decisionName + "_TOS", classifiedDec.tos());
  if (m_TUS)
    tuple->column(prefix + "_" + decisionName + "_TUS", classifiedDec.tus());
  if (m_TPS)
    tuple->column(prefix + "_" + decisionName + "_TPS", classifiedDec.tps());
}

//=========================================================================
//  Fill
//=========================================================================
StatusCode TupleToolTISTOS::fillBasic(const LHCb::Particle *top,
                                      const LHCb::Particle *P,
                                      const std::string &head,
                                      Tuples::Tuple &tuple)
{
  if (m_onlyTop)
  {
    if (P != top)
    {
      return StatusCode::SUCCESS;
    }
  }

  m_checkPID = false;

  // check if the particle matched any of the PIDs given in list IF there is a list
  if (!m_pidList.empty())
  {
    for (std::vector<int>::const_iterator it = m_pidList.begin();
         m_pidList.end() != it; ++it)
    {
      if (!m_checkPID && abs(P->particleID().pid()) == *it)
      {
        m_checkPID = true;
      }
    }
    if (!m_checkPID)
    {
      return StatusCode::SUCCESS;
    }
  }
  else
  {
    m_checkPID = true;
  }

  if (msgLevel(MSG::VERBOSE))
  {
    verbose() << "Filling TISTOS information for particle with pid "
              << P->particleID().pid() << endmsg;
  }
  const std::string prefix = fullName(head);

  // Since the tools can be configured differently, it is safer to do it for each tool.
  m_L0TriggerTisTosTool->setOfflineInput(*P);
  m_Hlt1TriggerTisTosTool->setOfflineInput(*P);
  m_Hlt2TriggerTisTosTool->setOfflineInput(*P);

  if (m_doL0)
  {
    fillDecision(m_L0TriggerTisTosTool, "L0.*Decision", prefix, "L0Global", tuple);
  }

  if (m_doHlt1)
  {
    // Fill the decision, tis and tos parametres for the Hlt1 as a whole
    fillDecision(m_Hlt1TriggerTisTosTool, "Hlt1.*Decision", prefix, "Hlt1Global", tuple);
    // Hlt1Phys
    fillDecision(m_Hlt1TriggerTisTosTool, m_hlt1Phys, prefix, "Hlt1Phys", tuple);
  }

  if (m_doHlt2)
  {
    // In Turbo03 Hlt2SelReports are missing.
    if (!exist<LHCb::HltSelReports>("Hlt2/SelReports", false))
    {
      if (!exist<LHCb::HltSelReports>("Hlt2/SelReports"))
      {
        return StatusCode::SUCCESS;
      }
    }
    // Do the Hlt2
    fillDecision(m_Hlt2TriggerTisTosTool, "Hlt2.*Decision", prefix,
                 "Hlt2Global", tuple);
    // Hlt2Phys
    fillDecision(m_Hlt2TriggerTisTosTool, m_hlt2Phys, prefix, "Hlt2Phys",
                 tuple);
  }

  return StatusCode::SUCCESS;
}

StatusCode TupleToolTISTOS::fillVerbose(const LHCb::Particle *top,
                                        const LHCb::Particle *P,
                                        const std::string &head,
                                        Tuples::Tuple &tuple)
{
  if (m_onlyTop)
  {
    if (P != top)
    {
      return StatusCode::SUCCESS;
    }
  }

  if (!m_checkPID)
  {
    return StatusCode::SUCCESS;
  }
  const std::string prefix = fullName(head);

  ITriggerTisTos::TisTosTob classifiedDec;

  // I think this is not needed as it has to run through fillBasic
  // m_L0TriggerTisTosTool->setOfflineInput(*P);
  // m_Hlt1TriggerTisTosTool->setOfflineInput(*P);
  // m_Hlt2TriggerTisTosTool->setOfflineInput(*P);

  if (m_verboseL0)
  {
    for (const auto &s : m_l0)
    {
      const std::string name = getName(s);
      if (msgLevel(MSG::VERBOSE))
      {
        verbose() << "Selection " << s << endmsg;
      }
      fillDecision(m_L0TriggerTisTosTool, s, prefix, name, tuple);
    }
  }

  if (m_verboseHlt1)
  {
    for (const auto &s : m_hlt1)
    {
      const std::string name = getName(s);
      if (msgLevel(MSG::VERBOSE))
      {
        verbose() << "Selection " << s << endmsg;
      }
      if (s.find("Hlt1L0") != 0)
      {
        fillDecision(m_Hlt1TriggerTisTosTool, s, prefix, name, tuple);
      }
      else
      {
        // assume that we want to look at L0 rather than Hlt1L0 decisions
        const auto classifiedDec = m_Hlt1TriggerTisTosTool->tisTosTobSelection(s);
        bool decision = m_Hlt1TriggerTisTosTool->hltSelectionObjectSummaries(s).size() != 0;
        tuple->column(prefix + "_" + name + "_Dec", decision);
        if (m_TIS)
          tuple->column(prefix + "_" + name + "_TIS", classifiedDec.tis());
        if (m_TOS)
          tuple->column(prefix + "_" + name + "_TOS", classifiedDec.tos());
        if (m_TUS)
          tuple->column(prefix + "_" + name + "_TUS", classifiedDec.tus());
        if (m_TPS)
          tuple->column(prefix + "_" + name + "_TPS", classifiedDec.tps());
      }
    }
  }

  if (m_verboseHlt2)
  {
    // In Turbo03 Hlt2SelReports are missing.
    if (!exist<LHCb::HltSelReports>("Hlt2/SelReports", false))
    {
      if (!exist<LHCb::HltSelReports>("Hlt2/SelReports"))
      {
        return StatusCode::SUCCESS;
      }
    }
    for (const auto &s : m_hlt2)
    {
      const std::string name = getName(s);
      if (msgLevel(MSG::VERBOSE))
      {
        verbose() << "Selection " << s << endmsg;
      }
      fillDecision(m_Hlt2TriggerTisTosTool, s, prefix, name, tuple);
    }
  }
  return StatusCode::SUCCESS;
}

std::string TupleToolTISTOS::getName(const std::string &mystr)
{
  std::string name = mystr;
  const int found = name.find(".*");
  if (found >= 0)
  {
    name.replace(found, 2, "__");
    if (msgLevel(MSG::VERBOSE))
    {
      verbose() << "From the function... Replaced   " << mystr << "  with  "
                << name << endmsg;
    }
  }
  return name;
}

StatusCode TupleToolTISTOS::fillBasic(Tuples::Tuple &T)
{
  return TupleToolTriggerBase::fillBasic(T);
}

StatusCode TupleToolTISTOS::fillVerbose(Tuples::Tuple &T)
{
  return TupleToolTriggerBase::fillVerbose(T);
}
