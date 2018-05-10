#include "TupleToolTriggerRecoStats.h"
#include "Event/Particle.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolTriggerRecoStats
//
// 2009-07-30 : Patrick Koppenburg
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolTriggerRecoStats )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolTriggerRecoStats::TupleToolTriggerRecoStats( const std::string& type,
                                                      const std::string& name,
                                                      const IInterface* parent )
  : TupleToolBase ( type, name , parent ),
    m_locations()
{
  declareInterface<IEventTupleTool>(this);
  m_locations.push_back("Hlt2NoCutsPions");
  m_locations.push_back("Hlt2GoodPions");
  m_locations.push_back("Hlt2Muons");
  m_locations.push_back("Hlt2Electrons");
  m_locations.push_back("Hlt2Photons");
  m_locations.push_back("Hlt2BiKalmanFittedRichKaons");
  declareProperty("InputLocations",m_locations,"Locations to look at");
}

//=============================================================================
// Destructor
//=============================================================================
TupleToolTriggerRecoStats::~TupleToolTriggerRecoStats() { }

//=============================================================================
//=============================================================================
// Fill
//=============================================================================
StatusCode TupleToolTriggerRecoStats::fill( Tuples::Tuple& tup)
{
  const std::string prefix=fullName();
  bool test = true;
  for ( std::vector<std::string>::const_iterator l = m_locations.begin() ;
        l != m_locations.end() ; ++l)
  {
    test &= tup->column(prefix+"NumberOf"+*l,number<LHCb::Particles>("/Event/Hlt2/"+*l+"/Particles"));
  }
  return StatusCode(test) ;
}
