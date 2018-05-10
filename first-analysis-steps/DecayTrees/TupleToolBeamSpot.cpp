//-----------------------------------------------------------------------------
// Implementation for class: TupleToolBeamSpot
//
// 2015-07-10 : TupleToolBeamSpot
//-----------------------------------------------------------------------------

#include "TupleToolBeamSpot.h"

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( TupleToolBeamSpot )

TupleToolBeamSpot::TupleToolBeamSpot( const std::string& type, const std::string& name, const IInterface* parent )
  : TupleToolBase(type, name, parent)
{
  declareInterface<IEventTupleTool>(this);

  declareProperty("Bound", m_bound = 10., "Bound to pass to beamspot constructor");
}

TupleToolBeamSpot::~TupleToolBeamSpot() { }

StatusCode TupleToolBeamSpot::initialize()
{
  StatusCode sc = TupleToolBase::initialize();
  if ( ! sc.isSuccess() ) { return sc; }

  if (msgLevel(MSG::DEBUG)) { debug() << "=> Initialize" << endmsg; }

  svc<IService>("LoKiSvc", true);

  m_beamspot = std::unique_ptr<LoKi::BeamSpot>{new LoKi::BeamSpot(m_bound)};
  if ( ! m_beamspot ) {
    return Error("Could not get the beamspot");
  }

  return StatusCode::SUCCESS;
}

StatusCode TupleToolBeamSpot::finalize()
{
  if (msgLevel(MSG::DEBUG)) { debug() << "=> Finalize" << endmsg; }

  m_beamspot.reset();

  return TupleToolBase::finalize();
}

StatusCode TupleToolBeamSpot::fill( Tuples::Tuple& tuple )
{
  std::string prefix = fullName();

  tuple->column(prefix+"BeamX", m_beamspot->x());
  tuple->column(prefix+"BeamY", m_beamspot->y());

  return StatusCode::SUCCESS;
}
