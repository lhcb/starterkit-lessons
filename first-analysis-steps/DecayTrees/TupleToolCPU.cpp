#include "TupleToolCPU.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/ITupleTool.h"
#include "GaudiKernel/Memory.h"
#include "GaudiAlg/ISequencerTimerTool.h"

#include "GaudiKernel/IRegistry.h" // IOpaqueAddress

//-----------------------------------------------------------------------------
// Implementation file for class : CPUTupleTool
//
// 2010-08-19 Patrick Koppenburg
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolCPU )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolCPU::TupleToolCPU( const std::string& type,
                            const std::string& name,
                            const IInterface* parent )
  : TupleToolBase ( type, name , parent )
  , m_timerTool(0)
  , m_timer(0)
{
  declareInterface<IEventTupleTool>(this);
}
//=============================================================================

StatusCode TupleToolCPU::initialize()
{
  const StatusCode sc = TupleToolBase::initialize();
  m_timerTool = tool<ISequencerTimerTool>( "SequencerTimerTool" ); //global tool
  m_timer = m_timerTool->addTimer( name() );
  m_timerTool->start(m_timer) ; /// start it now
  return sc ;
}

//=============================================================================

StatusCode TupleToolCPU::fill( Tuples::Tuple& tuple )
{
  const std::string prefix=fullName();

  double t =  m_timerTool->stop(m_timer) ; /// stop
  if (msgLevel(MSG::DEBUG)) debug() << "Time is " << t  << endmsg ;
  m_timerTool->start(m_timer) ; /// start again

  bool test = true;
  test &= tuple->column( prefix+"Memory", (double)System::virtualMemory());
  test &= tuple->column( prefix+"CPUTime", t);
  return StatusCode(test);
}
