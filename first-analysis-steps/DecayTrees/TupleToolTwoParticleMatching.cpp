// $Id: TupleToolTwoParticleMatching.cpp,v 1.0 2015-02-27 08:59:11 ikomarov $

#include "TupleToolTwoParticleMatching.h"
#include "Kernel/HashIDs.h"
#include "Kernel/ParticleProperty.h"
#include "Kernel/IParticlePropertySvc.h"
#include <cstdlib>
//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolTwoParticleMatching
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolTwoParticleMatching )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolTwoParticleMatching::TupleToolTwoParticleMatching( const std::string& type,
                                                  const std::string& name,
                                                  const IInterface* parent )
: TupleToolBase ( type, name, parent )
  //,m_tuple       ( NULL )
{
  std::vector<std::string> toolNames_default;

  toolNames_default.push_back  (  "TupleToolKinematic"  );
  toolNames_default.push_back  (  "TupleToolPid"        );
  //toolNames_default.push_back  (  "TupleToolGeometry"   );
  toolNames_default.push_back  (  "TupleToolMCTruth"   );
  toolNames_default.push_back  (  "TupleToolMCBackgroundInfo"   );

  declareProperty ( "MatchLocations" , m_matching_locations );
  declareProperty( "ToolList", m_toolNames = toolNames_default );
  declareProperty( "Prefix", m_headPrefix = "Matched_" );
  declareProperty( "Suffix", m_headSuffix = "");
  declareProperty( "MatcherTool", m_teslaMatcherName = "TeslaMatcher:PUBLIC" );
  declareInterface<IParticleTupleTool>(this);

// PROPERTY INCLUDED TO PRESERVE BACKWARD COMPATIBILITY
  declareProperty( "MatchWith",  m_matching_location = "OBSOLETE" );
  declareProperty( "TupleTools", m_toolNames);

}

//=============================================================================

StatusCode TupleToolTwoParticleMatching::initialize()
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  for(std::vector<std::string>::iterator iName = m_toolNames.begin(); iName != m_toolNames.end(); ++iName)
  {
    if ((*iName)=="TupleToolGeometry")
    {
      warning()<<"Geometry tuple tool can not be used in matching (yet). Please find a way how to include it by yourself."<<endmsg;
    }
    else
    {
      m_tuple.push_back(tool<IParticleTupleTool>( *iName, *iName+":PUBLIC", this ));
    }
  }

  if (m_matching_location != "OBSOLETE")
  {
    error() << "Option MatchWith is deprecated. Use MatchLocations instead." << endmsg;
    m_matching_locations ["J/psi(1S)"] = m_matching_location;
  }

  // Gets the ParticleProperty service
  LHCb::IParticlePropertySvc*  ppSvc =
    this -> template svc<LHCb::IParticlePropertySvc>
                            ( "LHCb::ParticlePropertySvc" , true) ;

  std::map<std::string, std::string>::const_iterator matchLocation;

  for (matchLocation = m_matching_locations.begin();
       matchLocation != m_matching_locations.end();
       matchLocation++)
  {
    const LHCb::ParticleProperty* property = ppSvc->find(matchLocation->first);
    if (property)
      m_parsed_locations [ abs(property->pdgID().pid()) ] = matchLocation->second;
  }

  m_matcher = tool<ITeslaMatcher>(m_teslaMatcherName, this);

  return sc;
}

//=============================================================================

StatusCode TupleToolTwoParticleMatching::fill( const LHCb::Particle*
                                               , const LHCb::Particle* P
                                               , const std::string& head
                                               , Tuples::Tuple& tuple )
{
  StatusCode sc = StatusCode::SUCCESS;

  const std::string prefix = m_headPrefix + fullName(head) + m_headSuffix;

  if (!P)
    return StatusCode ( true );

  const int pid = abs(P->particleID().pid());
  const int apid = abs(pid);

  if (m_parsed_locations.count(apid))
  {
    const LHCb::Particle* best_particle = P;
    const std::string matching_location ( m_parsed_locations [ apid ] );

    double o1(-9999), o2(-9999), counter(-9999);

    if ( sc ) sc = m_matcher->findBestMatch (P, best_particle, matching_location);
    if (sc && best_particle)
    {
      std::pair<double,double> o = LHCb::HashIDs::overlap(best_particle, P);

      o1 = o.first; o2 = o.second;
      counter = 1;
    }
    else
    {
      best_particle = P;  //reset to P
      Error("Matching failed.", StatusCode::SUCCESS);
    }

    if ( sc ) sc = tuple->column("NMatchingCand_"+fullName(head)+m_headSuffix, counter);
    if ( sc ) sc = tuple->column("Overlap_loaded_"+fullName(head)+m_headSuffix, o1);
    if ( sc ) sc = tuple->column("Overlap_original_"+fullName(head)+m_headSuffix, o2);


    for(std::vector<IParticleTupleTool*>::const_iterator iTool = m_tuple.begin(); iTool != m_tuple.end(); ++iTool)
      if ( sc ) sc = (*iTool)->fill( best_particle, best_particle, prefix, tuple );
  }
  return sc;
}
