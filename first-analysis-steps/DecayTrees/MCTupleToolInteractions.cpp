// $Id: MCTupleToolInteractions.cpp,v 1.2 2010-01-26 15:39:25 rlambert Exp $

// local
#include "MCTupleToolInteractions.h"

//-----------------------------------------------------------------------------
// Implementation file for class : GenerationTupleTool
//
// 2008-07-01 : Patrick Koppenburg
//-----------------------------------------------------------------------------

using namespace Gaudi;
using namespace LHCb;

// Declaration of the Tool Factory
DECLARE_COMPONENT( MCTupleToolInteractions )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
MCTupleToolInteractions::MCTupleToolInteractions( const std::string& type,
                                                  const std::string& name,
                                                  const IInterface* parent )
: TupleToolBase ( type, name , parent )
  ,m_mean(0.)
  ,m_adjustmean(0.)
  ,m_normaliseAt(0)
  ,m_useRecPV(false)
{
  declareInterface<IEventTupleTool>(this);

  // overwrite the mean value of interactions for this event
  declareProperty( "Mean", m_mean=0. );

  // calculate a scaling factor to approximate this number of interactions per event
  declareProperty( "AdjustMean", m_adjustmean=0. );

  // normalise the scaling factor such that it is equal to 1 at this value of I
  declareProperty( "NormaliseAt", m_normaliseAt=0 );
  
  // use the #of reconstructed PVs, rather than the MC Collisions.
  declareProperty( "UseRecPV", m_useRecPV=false );
  
  //deprecated, use Verbose
  // fill extra information on MCPV, MC Collisions and Reconstructed PVs
  //declareProperty( "FillDetails", m_fillDetails=false );
  
  // change the default PV location
  declareProperty( "RecPVLocation", m_RecPVLocation=LHCb::RecVertexLocation::Primary );
  
  // prefix to give to the variables, in case you want to use two copies of this tool
  setProperty( "ExtraName", "EVT_Int" );
}

//=============================================================================

StatusCode MCTupleToolInteractions::fill( Tuples::Tuple& tuple )
{
  const std::string prefix = fullName();
  unsigned int n = 1;
  int MCI   = -1;
  int MCPV  = -1;
  int RecPV = -1;
  const LHCb::GenHeader* gh = get<LHCb::GenHeader>(LHCb::GenHeaderLocation::Default);

  if ( gh && !gh->collisions().empty() )
  {
    MCI = gh->collisions().size() ;
    if ( msgLevel(MSG::VERBOSE) ) verbose() << "retrieved I from genHeader" << endmsg;
  }

  if ( MCI<=0 || isVerbose() )
  {
    const LHCb::MCHeader* mch = get<LHCb::MCHeader>(LHCb::MCHeaderLocation::Default);
    if(mch)
    {
      MCPV = mch->primaryVertices().size() ;
    }
  }

  if(isVerbose() || m_useRecPV)
  {
    const RecVertex::Container* PV = get<LHCb::RecVertex::Container>(m_RecPVLocation);
    if(PV)
    {
      RecPV = PV->size() ;
    }
  }
  if      (  m_useRecPV && RecPV>=0 ) { n=RecPV; }
  else if ( !m_useRecPV && MCI  > 0 ) { n=MCI;   }
  else if ( !m_useRecPV && MCPV > 0 ) { n=MCPV;  }
  else
  {
    Warning( "Could not retrieve number of interactions, filling as if n=1",
             StatusCode::SUCCESS ).ignore();
  }


  double mean=m_mean;
  if ( mean == 0. )
  {
    if ( msgLevel(MSG::VERBOSE) ) verbose() << "calculating mean" << endmsg;
    //extract it from the GenHeader

    //as in the IPileUpTool for FixedLuminosity.{h,cpp}
    if(gh && gh->crossingFreq() && gh->luminosity() && gh->totCrossSection())
    {
      if ( msgLevel(MSG::VERBOSE) ) verbose() << "using genheader " << mean << endmsg;
      mean=gh->luminosity()*gh->totCrossSection()/gh->crossingFreq();
    }

    //if it isn't in the GenHeader, then make a default, the DC06 mean
    else
    {
      if ( msgLevel(MSG::VERBOSE) ) verbose() << "using default for DC06 data " << mean << endmsg;
      mean=(2.e32 /Gaudi::Units::cm2/Gaudi::Units::s)*(102.4 * Gaudi::Units::millibarn)/(30.0 * Gaudi::Units::megahertz);
    }


    if ( msgLevel(MSG::VERBOSE) ) verbose() << "calculated mean " << mean << endmsg;
    //if it isn't in the GenHeader, then make a default
  }

  if ( msgLevel(MSG::VERBOSE) ) verbose() << "Filling tuples" << endmsg;
  bool test = true;
  test &= tuple->column( prefix + "_I" , n );
  test &= tuple->column( prefix + "_Mean" , mean );
  test &= tuple->column( prefix + "_Prob" , poisson(mean,n) );

  if(m_adjustmean!=0.)
  {
    if ( msgLevel(MSG::VERBOSE) ) verbose() << "Filling adjusted tuples" << endmsg;
    double normalise=1;
    if(m_normaliseAt) normalise=weight(mean,m_adjustmean,m_normaliseAt);
    test &= tuple->column( prefix + "_AdjustMean" , m_adjustmean );
    test &= tuple->column( prefix + "_AdjustProb" , poisson(m_adjustmean,n) );
    test &= tuple->column( prefix + "_AdjustWeight" , weight(mean,m_adjustmean,n)/normalise );
    if(isVerbose()) test &= tuple->column( prefix + "_NormalisedAt" , m_normaliseAt );
    if(isVerbose()) test &= tuple->column( prefix + "_NormalisedTo" , normalise );
  }

  if(isVerbose())
  {
    if ( msgLevel(MSG::VERBOSE) ) verbose() << "Filling detailed tuples" << endmsg;
    test &= tuple->column( prefix + "_nMCI" , MCI );
    test &= tuple->column( prefix + "_nMCPV" ,MCPV );
    test &= tuple->column( prefix + "_nRecPV" , RecPV );
  }

  return StatusCode(test);
}

//=============================================================================
