// Include files
#include "gsl/gsl_sys.h"
// from Gaudi
#include "GaudiKernel/PhysicalConstants.h"
// local
#include "TupleToolMCTruth.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Event/Particle.h"
#include "Event/MCParticle.h"

// kernel
#include "Kernel/IParticle2MCAssociator.h"
//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolMCTruth
//
// 2007-11-07 : Jeremie Borel
//-----------------------------------------------------------------------------

using namespace LHCb;

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolMCTruth::TupleToolMCTruth( const std::string& type,
                                    const std::string& name,
                                    const IInterface* parent )
  : TupleToolBase ( type, name , parent )
  , m_toolList(1,"MCTupleToolKinematic")
{
  // interface
  declareInterface<IParticleTupleTool>(this);
  // The names of MCTupleTools to use on the associated mcp
  declareProperty( "ToolList", m_toolList  );
  // MC associators to try, in order
  m_p2mcAssocTypes.push_back( "DaVinciSmartAssociator" );
  m_p2mcAssocTypes.push_back( "MCMatchObjP2MCRelator"  );
  declareProperty( "IP2MCPAssociatorTypes", m_p2mcAssocTypes );
}

//=============================================================================

StatusCode TupleToolMCTruth::initialize()
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  // the MC associators
  m_p2mcAssocs.clear();
  for ( std::vector<std::string>::const_iterator iMCAss = m_p2mcAssocTypes.begin();
        iMCAss != m_p2mcAssocTypes.end(); ++iMCAss )
  {
    m_p2mcAssocs.push_back( tool<IParticle2MCAssociator>(*iMCAss,this) );
  }
  if ( m_p2mcAssocs.empty() ) { return Error("No MC associators configured"); }

  // remove duplicate tools from the list
  std::sort( m_toolList.begin(), m_toolList.end() );
  m_toolList.erase( std::unique(m_toolList.begin(),m_toolList.end()), m_toolList.end() );

  // initialise the tuple tools
  for ( std::vector<std::string>::const_iterator it = m_toolList.begin();
        m_toolList.end()!=it ; ++it )
  {
    if (msgLevel(MSG::VERBOSE)) verbose() << "Adding the tool " << *it << endmsg ;
    IMCParticleTupleTool* aTool = tool<IMCParticleTupleTool>(*it,this);
    if ( aTool )
    {
      m_mcTools.push_back(aTool);
    }
    else
    {
      Warning("There was a problem retrieving " + *it +" , this tool will be ignored").ignore();
    }
  }

  if ( msgLevel(MSG::VERBOSE) )
  {
    verbose() << "Completed TupleTool intialisation, "
              << m_mcTools.size()
              << " tools added " << endmsg ;
  }

  return sc;
}

//=============================================================================

StatusCode TupleToolMCTruth::fill( const LHCb::Particle*,
                                   const LHCb::Particle* P,
                                   const std::string& head,
                                   Tuples::Tuple& tuple )
{
  const std::string prefix = fullName(head);

  bool test = true;

  const LHCb::MCParticle* mcp(NULL);
  if ( P )
  {
    //assignedPid = P->particleID().pid();
    if (msgLevel(MSG::VERBOSE)) verbose() << "Getting related MCP to " << P << endmsg ;
    for ( std::vector<IParticle2MCAssociator*>::const_iterator iMCAss = m_p2mcAssocs.begin();
          iMCAss != m_p2mcAssocs.end(); ++iMCAss )
    {
      mcp = (*iMCAss)->relatedMCP(P);
      if ( mcp ) break;
    }
    if (msgLevel(MSG::VERBOSE))
      verbose() << "Got mcp " << mcp << endmsg ;
  }

  // pointer is ready, prepare the values
  const int mcPid = ( mcp ? mcp->particleID().pid() : 0 );

  // fill the tuple:
  test &= tuple->column( prefix+"_TRUEID", mcPid );

  // fill all requested MCTools
  for ( std::vector< IMCParticleTupleTool* >::const_iterator it = m_mcTools.begin();
        it != m_mcTools.end(); ++it )
  {
    test &= (*it)->fill(NULL,mcp,prefix,tuple);
  }

  return StatusCode(test);
}

//=============================================================================

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolMCTruth )

//=============================================================================
