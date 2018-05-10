// Include files

// local
#include "TupleToolGeneration.h"

#include "Event/GenHeader.h"
#include "Kernel/ParticleID.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/ITupleTool.h"

#include "GaudiKernel/IRegistry.h" // IOpaqueAddress

//-----------------------------------------------------------------------------
// Implementation file for class : GenerationTupleTool
//
// 2008-07-01 : Patrick Koppenburg
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolGeneration::TupleToolGeneration( const std::string& type,
                                          const std::string& name,
                                          const IInterface* parent )
  : TupleToolBase ( type, name , parent )
{
  declareInterface<IEventTupleTool>(this);
}

//=============================================================================

StatusCode TupleToolGeneration::fill( Tuples::Tuple& tuple )
{

  const std::string prefix = fullName();

  if (msgLevel(MSG::DEBUG)) debug() << "TupleToolGeneration" << endmsg  ;

  LHCb::GenHeader * mch = getIfExists<LHCb::GenHeader>(LHCb::GenHeaderLocation::Default);
  if ( !mch )
  {
    return Warning("NoGenHeader. You probably don't need this tool.", StatusCode::SUCCESS, 1);
  }
  if (msgLevel(MSG::DEBUG)) debug() << mch->numOfCollisions() << " collisions" << endmsg  ;

  std::vector<unsigned int> heaviestQuark ;
  std::vector<int> processType ;
  unsigned int hqEvent = 0 ;

  // quarks
  processType.reserve( mch->collisions().size() );
  for ( SmartRefVector<LHCb::GenCollision>::const_iterator ic = mch->collisions().begin();
        ic != mch->collisions().end(); ++ic )
  {
    if ( 0 == *ic ) { Exception("NULL LHCb::GenCollision pointer"); }
    const LHCb::HepMCEvent * gene = (*ic)->event() ;
    if ( !gene )
    {
      Warning("NULL LHCb::HepMCEvent pointer -> This tool cannot work on this MC data",
              StatusCode::SUCCESS).ignore();
      continue;
    }

    processType.push_back( (*ic)->processType() );
    unsigned int hq = 2 ;
    for ( HepMC::GenEvent::particle_const_iterator p = gene->pGenEvt()->particles_begin();
          p != gene->pGenEvt()->particles_end(); ++p )
    {
      const LHCb::ParticleID pid( (*p)->pdg_id() ) ;
      if (msgLevel(MSG::VERBOSE)) verbose() << "Gen particle " << (*p)->pdg_id() << " "
                                            << pid.hasQuark(LHCb::ParticleID::bottom)
                                            << " " << pid.hasQuark(LHCb::ParticleID::charm)
                                            << endmsg ;
      if ( (hq<LHCb::ParticleID::top) && pid.hasQuark(LHCb::ParticleID::top) )
      {
        hq = LHCb::ParticleID::top; break ;
      }
      else if (( hq<LHCb::ParticleID::bottom) &&  pid.hasQuark(LHCb::ParticleID::bottom) )
      {
        hq = LHCb::ParticleID::bottom;
      }
      else if ((hq<LHCb::ParticleID::charm) &&  pid.hasQuark(LHCb::ParticleID::charm) )
      {
        hq = LHCb::ParticleID::charm;
      }
      else if ((hq<LHCb::ParticleID::strange) &&  pid.hasQuark(LHCb::ParticleID::strange) )
      {
        hq = LHCb::ParticleID::strange;
      }
    }
    heaviestQuark.push_back( hq );
    if ( hq > hqEvent ) hqEvent = hq ;
    if (msgLevel(MSG::VERBOSE))
      verbose() << "Process type is " << (*ic)->processType() << ". Heaviest quark " << hq
                << endmsg ;
  }

  bool test = true;
  test &= tuple->farray( prefix+"ProcessType" ,  processType , prefix+"Collisions" , 20 );
  test &= tuple->farray( prefix+"HeaviestQuark", heaviestQuark, prefix+"Collisions" , 20 );
  test &= tuple->column( prefix+"HeaviestQuarkInEvent" , hqEvent );

  return StatusCode(test);
}

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolGeneration )
