// Include files

// local
#include "TupleToolMCBackgroundInfo.h"

#include "Kernel/IBackgroundCategory.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Event/Particle.h"

//-----------------------------------------------------------------------------
// Implementation file for class : MCBackgroundInfoTupleTool
//
// 2007-11-07 : Jeremie Borel
//-----------------------------------------------------------------------------

using namespace LHCb;

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolMCBackgroundInfo::TupleToolMCBackgroundInfo( const std::string& type,
                                                      const std::string& name,
                                                      const IInterface* parent )
  : TupleToolBase ( type, name, parent )
{
  declareInterface<IParticleTupleTool>(this);
  m_backCatTypes.push_back( "BackgroundCategoryViaRelations" );
  m_backCatTypes.push_back( "BackgroundCategory" );
  declareProperty( "IBackgroundCategoryTypes", m_backCatTypes );
}

//=============================================================================

StatusCode TupleToolMCBackgroundInfo::initialize()
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  if (msgLevel(MSG::DEBUG))
    debug() << "Background Category tools " << m_backCatTypes << endmsg;

  for ( std::vector<std::string>::const_iterator iT = m_backCatTypes.begin();
        iT != m_backCatTypes.end(); ++iT )
  {
    m_bkgs.push_back( tool<IBackgroundCategory>( *iT, *iT, this ) );
  }

  return sc;
}

//=============================================================================

StatusCode TupleToolMCBackgroundInfo::fill( const Particle* headP,
                                            const Particle* P,
                                            const std::string& head,
                                            Tuples::Tuple& tuple )
{
  StatusCode sc = StatusCode::SUCCESS;

  const std::string prefix = fullName(head);

  Assert( P && !m_bkgs.empty() , "This should not happen :(" );

  if ( !P->isBasicParticle() )
  {
    IBackgroundCategory::categories cat = IBackgroundCategory::Undefined;

    for ( std::vector<IBackgroundCategory*>::const_iterator iT = m_bkgs.begin();
          iT != m_bkgs.end(); ++iT )
    {
      cat = (*iT)->category( P, headP );
      if ( cat != IBackgroundCategory::Undefined ) break;
    }

    if (msgLevel(MSG::DEBUG))
      debug() << "BackgroundCategory decision for "
              << prefix << " : " << cat << endmsg;

    sc = tuple->column( prefix+"_BKGCAT", (int)cat );
  }

  return sc;
}

//=============================================================================

// Declaration of the Tool Factory
// actually acts as a using namespace TupleTool
DECLARE_COMPONENT( TupleToolMCBackgroundInfo )

//=============================================================================
