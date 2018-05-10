// $Id: TupleToolVeto.cpp
// Include files

// local
#include "TupleToolVeto.h"
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
#include "Event/Particle.h"
#include "GaudiKernel/IRegistry.h" //
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleProperty.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolVeto
//
//  Flag particle P when it overlaps with any particle from given container(s)
//
//  usage :
//
//  mytuple.Particle="name"       # the specific particle to be checked (default : all maked DecayTreeTuple::DecayDescriptor particles)
//  mytuple.TupleToolVeto[VetoName].["Container1","Container2",...]  # list of containers to be checked
//
//
//  e.g.:
//   - pi0-Veto for single photon  : flag all photons which are already used to build a pi0 as defined in StdLooseResolvedPi0
//      mytuple.TupleToolVeto.Particle="gamma"
//      mytuple.TupleToolVeto.Veto[Pi0R]=["/Event/Phys/StdLooseResolvedPi0/Particles"]
//
//   - pi0-veto for pi0 :  flag all pi0 which are not exclusive with respect to **other** pi0's
//      mytuple.TupleToolVeto.Particle="pi0"
//      mytuple.TupleToolVeto.VetoOther[Pi0R]=["/Event/Phys/StdLooseResolvedPi0/Particles"]
//
//      -> the overlap of the pi0 with itself is not considered (property VetoOther) - only partial overlap are checked
//
//   - works for any (composite or basic) particle. Relies on tool DaVinciTools/ParticleVeto  (see doxygen for details)
//
//
//-----------------------------------------------------------------------------

using namespace Gaudi;
using namespace LHCb;

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolVeto )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolVeto::TupleToolVeto( const std::string& type,
                                const std::string& name,
                                const IInterface* parent )
    : TupleToolBase ( type, name , parent ),
      m_pid(0)
{
  declareInterface<IParticleTupleTool>(this);
  declareProperty("Particle",m_part="");
  declareProperty("Veto", m_veto);
  declareProperty("VetoOther", m_vetoOther);
}

StatusCode TupleToolVeto::initialize()
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;
  m_check = tool<IParticleVeto>("ParticleVeto","ParticleVeto" ,this );
  if ( !m_part.empty() )
  {
    LHCb::IParticlePropertySvc* ppsvc =
      svc<LHCb::IParticlePropertySvc>("LHCb::ParticlePropertySvc",true) ;
    const LHCb::ParticleProperty* pp = ppsvc->find( m_part );
    m_pid = (pp) ? pp->pdgID().abspid() : 0;
  }
  return sc;
}

//=============================================================================

StatusCode TupleToolVeto::fill( const Particle* , const Particle* P,
                                const std::string& head,
                                Tuples::Tuple& tuple )
{
  const std::string prefix = fullName(head);

  bool filltuple = true;
  if ( P )
  {

    if ( !m_part.empty() && m_pid != P->particleID().abspid() )
    {
      return StatusCode::SUCCESS;
    }

    for ( std::map<std::string,std::vector<std::string> >::const_iterator i = m_veto.begin();
          m_veto.end() != i; ++i )
    {
      const std::string&              flag = i->first;
      const std::vector<std::string>& cont = i->second;
      const bool veto = m_check->foundOverlap( P, cont );
      filltuple &= tuple->column( prefix+"_Veto"+flag, veto );
    }

    for( std::map<std::string,std::vector<std::string> >::const_iterator i = m_vetoOther.begin();
         m_vetoOther.end() != i; ++i )
    {
      const std::string& flag              = i->first;
      const std::vector<std::string>& cont = i->second;
      const bool veto = m_check->foundOverlap( P, cont , 0x3); // mode == 3
      filltuple &= tuple->column( prefix+"_VetoOther"+flag, veto );
    }

  }

  return StatusCode(filltuple);
}
