// Include files

// local
#include "TupleToolSelResults.h"
#include "Kernel/ICheckSelResults.h"            // Interface

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolSelResults
//
// 2009-02-11 : Patrick Koppenburg
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolSelResults )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolSelResults::TupleToolSelResults( const std::string& type,
                                            const std::string& name,
                                            const IInterface* parent )
    : TupleToolBase ( type, name , parent ),
      m_selTool(0),
      m_selections(0)
{
  declareInterface<IEventTupleTool>(this);
  declareProperty("Selections", m_selections, "List of algorithm names");
  //deprecated, use ExtraName instead
  //declareProperty("Head", m_head = "", "This will be appended before any tuple entry");
}

//=============================================================================
// init
//=============================================================================
StatusCode TupleToolSelResults::initialize()
{
  StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;
  m_selTool = tool<ICheckSelResults>("CheckSelResultsTool",this);
  return sc ;
}

//=============================================================================
// Fill
//=============================================================================
StatusCode TupleToolSelResults::fill( Tuples::Tuple& tup)
{
  const std::string prefix = fullName();
  bool test = true;
  for ( std::vector<std::string>::const_iterator s = m_selections.begin() ;
        s != m_selections.end(); ++s )
  {
    test &= tup->column(prefix+(*s),m_selTool->isSelected(*s));
    if (!test)
    {
      err() << "Cannot fill variable name " << prefix+(*s) << endmsg ;
      break;
    }
  }
  return StatusCode(test) ;
}
