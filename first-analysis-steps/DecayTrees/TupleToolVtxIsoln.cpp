// Include files

// local
#include "TupleToolVtxIsoln.h"

#include <Kernel/GetIDVAlgorithm.h>
#include <Kernel/IDVAlgorithm.h>
#include <Kernel/IDistanceCalculator.h>
#include <Kernel/IVertexFit.h>
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
#include "CaloUtils/CaloAlgUtils.h"

#include "Event/Particle.h"
using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolVtxIsoln
//
// @author Mitesh Patel, Patrick Koppenburg
// @date   2008-04-15
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
// actually acts as a using namespace TupleTool
DECLARE_COMPONENT( TupleToolVtxIsoln )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolVtxIsoln::TupleToolVtxIsoln( const std::string& type,
                                        const std::string& name,
                                        const IInterface* parent )
    : TupleToolBase ( type, name , parent )
    , m_isolationTool(0)
{
  declareInterface<IParticleTupleTool>(this);
}

//=============================================================================

StatusCode TupleToolVtxIsoln::initialize()
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  m_isolationTool = tool<IExtraInfoTool>("VertexIsolation", "VertexIsolation", this) ;
  if ( !m_isolationTool )
  {
    Error("Unable to retrieve the isolation tool");
    return StatusCode::FAILURE;
  }
  // Get var names
  m_indices.first = m_isolationTool->getFirstIndex() ;
  m_indices.last  = m_isolationTool->getNumberOfParameters() + m_indices.first ;
  double dummy ;
  std::string varName ;
  for (int index = m_indices.first; index != m_indices.last; index++)
  {
    m_isolationTool->getInfo(index, dummy, varName) ;
    m_varNames[index] = varName ;
  }

  return sc;
}

//=============================================================================

StatusCode TupleToolVtxIsoln::fill( const Particle* mother
                                    , const Particle* P
                                    , const std::string& head
                                    , Tuples::Tuple& tuple ){

  const std::string prefix = fullName(head);
  Assert( P && mother, "This should not happen, you are inside TupleToolVtxIsoln.cpp :(" );
  // Check the ExtraInfo
  bool test = true ;
  if ( P->isBasicParticle() ) return StatusCode::SUCCESS ;
  // Fill tuple
  if ( P->hasInfo(m_indices.first) )
  {
    double def = -999999. ;
    double val ;
    for (int index = m_indices.first; index != m_indices.last; ++index)
    {
      val = P->info(index, def) ;
      test &= tuple->column(prefix+"_"+m_varNames[index], val);
    }
  }
  else
  {
    m_isolationTool->calculateExtraInfo(P, P) ;
    double val ;
    int result ;
    for (int index = m_indices.first; index != m_indices.last; ++index)
    {
      std::string name ;
      result = m_isolationTool->getInfo(index, val, name) ;
      if ( !result ) continue ;
      test &= tuple->column(prefix+"_"+name, val);
    }
  }
  return StatusCode(test) ;
}

