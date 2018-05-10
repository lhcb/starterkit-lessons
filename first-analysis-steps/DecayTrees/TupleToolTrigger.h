// $Id: TupleToolTrigger.h,v 1.12 2010-03-16 01:11:28 rlambert Exp $
#ifndef JBOREL_TUPLETOOLTRIGGER_H
#define JBOREL_TUPLETOOLTRIGGER_H 1

#include <vector>

#include "TupleToolTriggerBase.h"
#include "Kernel/IEventTupleTool.h"            // Interface

/** @class TupleToolTrigger TupleToolTrigger.h jborel/TupleToolTrigger.h
 *
 * \brief Fill the trigger informations for the DecayTreeTuple.
 *
 * - L0Decision : LHCb::L0DUReport->decision()
 * - Hlt1Global : Global HLT1 decision
 * - Hlt2Global : Global HLT2 decision
 * - StrippingGlobal : Global Stripping decision
 *
 * If verbose is true the tool needs somehow to find a list of triggers to fill.
 * In this case it uses the base class TupleToolTriggerBase to sort everything out.
 *
 * If \b VerboseL0 = true
 * L0Decision_xxx : LHCb::L0DUReport->channelDecisionByName(xxx)
 *
 * If \b VerboseHlt1 = true
 * Hlt1_xxx_Decision : filled ... [ -1, 0, 1 ] = [not run, decision false, decision true]
 *
 * If \b VerboseHlt2 = true
 * Hlt2_xxx_Decision : filled ... [ -1, 0, 1 ] = [not run, decision false, decision true]
 *
 * If \b VerboseStripping = true
 * Stripping_xxx_Decision : filled ... [ -1, 0, 1 ] = [not run, decision false, decision true]
 *
 * Verbose flag is a shortcut to turn all verbosity on.
 *
 *
 *  @author Jeremie Borel
 *  @date   2007-11-07
 */
class TupleToolTrigger : public TupleToolTriggerBase,
                         virtual public IEventTupleTool
{

public:

  /// Standard constructor
  TupleToolTrigger( const std::string& type,
                    const std::string& name,
                    const IInterface* parent );

  ~TupleToolTrigger( ) {} ///< Destructor

  StatusCode initialize() override;

public:

  StatusCode fill( Tuples::Tuple& tuple ) override;

  StatusCode fill( const LHCb::Particle* top
                   , const LHCb::Particle* part
                   , const std::string& head
                   , Tuples::Tuple& tuple ) override;

private:

  ///fill verbose information for the L0
  StatusCode fillL0( Tuples::Tuple& );
  ///fill verbose information for the HLT
  StatusCode fillHlt( Tuples::Tuple&, const std::string &);
  StatusCode fillRoutingBits( Tuples::Tuple& );

  StatusCode fillBasic(Tuples::Tuple& tuple ) override;

  StatusCode fillVerbose(Tuples::Tuple& tuple ) override;

private:

  StatusCode fillBasic( const LHCb::Particle*
                        , const LHCb::Particle*
                        , const std::string&
                        , Tuples::Tuple& ) override;

  StatusCode fillVerbose( const LHCb::Particle*
                          , const LHCb::Particle*
                          , const std::string&
                          , Tuples::Tuple& ) override;

private:

  std::vector<unsigned int> m_routingBits ; ///< Routing bits to fill

  // RawEvent Locations to search
  std::vector<std::string> m_rawEventLocs;

};

#endif // JBOREL_TUPLETOOLTRIGGER_H
