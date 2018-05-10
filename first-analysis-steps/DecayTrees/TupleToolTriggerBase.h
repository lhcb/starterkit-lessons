#ifndef TUPLETOOLTRIGGERBASE_H
#define TUPLETOOLTRIGGERBASE_H 1

// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface

/** @class TupleToolTriggerBase TupleToolTriggerBase.h
 *
 *  The baseclass for all tupletools interacting with the trigger
 *  This allows for code sharing between TupleToolTISTOS and TupleToolTrigger
 *
 *  For real data the triggers will change run by run. Eventually this means
 *  your stripped data may have a different set of trigger lines per event.
 *  To fins out all the trigger lines which ran, you can set CollateTriggerList
 *  to true, and then all triggers will be printed at the end of the event.
 *
 *  Options:
 *
 *  - FillL0:      Fill L0 decisions
 *  - VerboseL0:   Fill verbose L0 information
 *  - FillHlt1:    Fill Hlt1 decisions
 *  - VerboseHlt1: Fill verbose Hlt1 information
 *  - FillHlt2:    Fill Hlt2 decisions
 *  - VerboseHlt2: Fill verbose Hlt2 information
 *  - FillStripping:    Fill Stripping decisions
 *  - VerboseStripping: Fill verbose Stripping information
 *
 *  - Verbose:     Sets all the verbose flags to true
 *
 *
 *  - TriggerList: Supply a list of triggers you are interested in (all of them?)
 *                 Necessary if the triggers change in the middle of your job!!
 *
 *  @author R. Lambert
 *  @date   2010-01-25
 */

class TupleToolTriggerBase : public TupleToolBase,
                             virtual public IParticleTupleTool
{

public:

  /// Standard constructor
  TupleToolTriggerBase( const std::string& type,
                        const std::string& name,
                        const IInterface* parent);

  ~TupleToolTriggerBase( ); ///< Destructor

  StatusCode initialize() override;

  StatusCode fill( const LHCb::Particle*
                   , const LHCb::Particle*
                   , const std::string&
                   , Tuples::Tuple& ) override;

  virtual StatusCode fill( Tuples::Tuple& );

protected:

  bool m_verboseL0;     ///< get details on L0
  bool m_verboseHlt1;   ///< get details on Hlt1
  bool m_verboseHlt2;   ///< get details on Hlt2
  bool m_verboseStripping;   ///< get details on Stripping


  bool m_doL0;     ///< fill L0 global
  bool m_doHlt1;   ///< fill Hlt1 global
  bool m_doHlt2;   ///< fill hlt2 global
  bool m_doStripping;   ///< fill stripping global


  // do not allow trigger names to mutate from event-to-event
  std::vector<std::string> m_l0;
  std::vector<std::string> m_hlt1;
  std::vector<std::string> m_hlt2;
  std::vector<std::string> m_stripping;


  std::vector<std::string> m_triggerList;//property: list of triggers to specifically look at

  bool compileMyList(const std::vector<std::string>& list);
  bool appendToList();

  std::vector< std::string > l0TriggersFromL0DU() const;

protected:

  virtual StatusCode fillBasic( const LHCb::Particle*
                                , const LHCb::Particle*
                                , const std::string&
                                , Tuples::Tuple& );

  virtual StatusCode fillBasic( Tuples::Tuple& );

  virtual StatusCode fillVerbose( const LHCb::Particle*
                                  , const LHCb::Particle*
                                  , const std::string&
                                  , Tuples::Tuple& );

  virtual StatusCode fillVerbose( Tuples::Tuple& );

};

#endif // TUPLETOOLTRIGGERBASE_H
