// $Id: TupleToolCPU.h,v 1.1 2010-08-19 13:50:45 pkoppenb Exp $
#ifndef JBOREL_TUPLETOOLCPU_H
#define JBOREL_TUPLETOOLCPU_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IEventTupleTool.h"            // Interface

class ITupleTool;
class ISequencerTimerTool ;

/** @class TupleToolCPU TupleToolCPU.h jborel/TupleToolCPU.h
 *
 * \brief Event and Run number for DecayTreeTuple
 *
 * Tuple columns:
 * - CPU of event
 * - Memory used

 * \sa DecayTreeTuple
 *  @author Patrick Koppenburg
 *  @date   2010-08-19
 */
class TupleToolCPU : public TupleToolBase, virtual public IEventTupleTool {
public:
  /// Standard constructor
  TupleToolCPU( const std::string& type,
                const std::string& name,
                const IInterface* parent);

  ~TupleToolCPU( ){}; ///< Destructor

  StatusCode fill( Tuples::Tuple& ) override;
  StatusCode initialize( ) override;

private :
  ISequencerTimerTool* m_timerTool ; ///< timer tool
  int m_timer ; ///< timer index
};
#endif // JBOREL_TUPLETOOLCPU_H
