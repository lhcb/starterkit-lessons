#pragma once

/**@class TupleToolDecayTreeFitter
 * -  History : this tool was written during the study of the b->JpsiX lifetimes. It was not very generic.
 *    This re-implmentation is hopefully more generic, although it requires several instances of the tool
 *    to achive the same result.
 * -  Advice  : Most of the output of this code could be saved via the LoKi style lines.

 * -  Usage   : One can plug this Tool in DecayTreeTuple. It is advised to give it to a branch and not to try to run
 *    it on all the tree. To get full functionality use several instances.
 *    Always give it an instance name which will be used as prefix. See example.
 *  - Configuration :
 @code
 constrainToOriginVertex = False          # save a fit to compute the lifetime.
 particlesToConstrain = ["J/psi(1S)"]     # chose the particles to mass to constrain.
 ExtraName = ""                           # Additional Prefix (will replace instance name) (inherited from TupleToolBase)
 Verbose = False                          # Fill Lifetime for daughters. (inherited from TupleToolBase)
 @endcode
 *
 * -  Outputs: for each fit is saved:
 - The status, the chi2, the number of degres of freedom of the fit.
 - The fitted mass, mass error for example : Lambda_b0_massConAllDaughters_M ...
 - The fitted lifetime, and error : Lambda_b0_taufit_ctau (ctau is given in mm).
 *
 * -  Example:
 @code
 decay = "B0 -> (^J/psi(1S) -> ^mu+ ^mu-) (^KS0 -> ^pi+ ^pi-)"
 bh = 'B0'
 tuple.Decay = decay
 tuple = DecayTreeTuple("MyTuple")
 tuple.ToolList +=  ["TupleToolGeometry",  "TupleToolKinematic", "TupleToolPrimaries", "TupleToolEventInfo" ]
 tuple.Branches = { "B"  : "["+bh+"]cc : "+decay.replace("^","") }
 from Configurables import TupleToolDecayTreeFitter
 tuple.B.ToolList +=  [ "TupleToolDecayTreeFitter/Fit",            # just a refit
 "TupleToolDecayTreeFitter/MassFit",        # fit with J/psi mass constraint
 "TupleToolDecayTreeFitter/PVFit" ]         # fit with all constraints I can think of

 tuple.B.addTool(TupleToolDecayTreeFitter("MassFit"))
 tuple.B.MassFit.constrainToOriginVertex = False
 tuple.B.MassFit.daughtersToConstrain = [ "J/psi(1S)" ]

 tuple.B.addTool(TupleToolDecayTreeFitter("PVFit"))
 tuple.B.PVFit.Verbose = True
 tuple.B.PVFit.constrainToOriginVertex = True
 tuple.B.PVFit.daughtersToConstrain = [ "J/psi(1S)", "KS0" ]

 # now two that check for reflections
 tuple.B.ToolList +=  ["TupleToolDecayTreeFitter/SubLambda", "TupleToolDecayTreeFitter/SubLambdabar" ]
 subDTF = TupleToolDecayTreeFitter("SubLambda", Verbose=True,
 daughtersToConstrain = [ "J/psi(1S)" ],
 constrainToOriginVertex=True,
 Substitutions={ 'Beauty -> Meson (Strange -> ^pi+ pi-)': 'p+' })
 tuple.B.addTool(subDTF)
 tuple.B.addTool(subDTF.clone("SubLambdabar",Substitutions={ 'Beauty -> Meson (Strange -> pi+ ^pi-)': 'p-' }))

 @endcode
 *
 * - This will produce the following columns for the B (from this tool):
 *     - B_Fit_status B_Fit_nDOF B_Fit_chi2_B B_Fit_nIter B_Fit_M B_Fit_MERR B_Fit_P B_Fit_PERR
 *     - B_MassFit_status B_MassFit_nDOF B_MassFit_chi2_B B_MassFit_nIter B_MassFit_M B_MassFit_MERR B_MassFit_P
 *       B_MassFit_PERR B_PVFit_status B_PVFit_nDOF B_PVFit_chi2_B B_PVFit_nIter B_PVFit_M B_PVFit_MERR B_PVFit_P
 *     - B_PVFit_PERR B_PVFit_ctau B_PVFit_ctauErr B_PVFit_decayLength B_PVFit_decayLengthErr B_PVFit_J_psi_1S_ctau
 *       B_PVFit_J_psi_1S_ctauErr B_PVFit_J_psi_1S_decayLength B_PVFit_J_psi_1S_decayLengthErr B_PVFit_KS0_ctau
 *       B_PVFit_KS0_ctauErr B_PVFit_KS0_decayLength B_PVFit_KS0_decayLengthErr
 *
 * - One can also hange the PID of some particles in the chain, using the same syntax as SubstitutePID
 *
 * \sa SubstitutePID
 *
 * \sa DecayTreeTuple
 *
 *  @author Yasmine Amhis, Matt Needham (original authors), Patrick Koppenburg (re-implementation)
 *  @date   2010-10-30, 2011-04-01
 *
 **/

// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/ToolHandle.h"
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "DecayTreeTupleBase/TupleToolBase.h"

#include "Kernel/IParticleTupleTool.h"
#include "Kernel/ISubstitutePID.h"
#include "Kernel/GetIDVAlgorithm.h"
#include "Kernel/IDVAlgorithm.h"
#include "Kernel/ParticleProperty.h"
#include "Kernel/IParticleDescendants.h"
#include "Kernel/Escape.h"
#include "Kernel/IParticlePropertySvc.h"

#include "TrackInterfaces/ITrackStateProvider.h"

#include "DecayTreeFitter/Fitter.h"

#include "Event/RecVertex.h"
#include "Event/Particle.h"

#include "LoKi/ParticleProperties.h"

// boost
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

// STL
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

struct IParticleDescendants;

namespace LHCb
{
  class ParticleID;
  class IParticlePropertySvc;
  class VertexBase;
  class Particle;
  class RecVertex ;
  class DecayTree ;
}
namespace DecayTreeFitter
{
  class Fitter;
}

struct IDVAlgorithm;
/** @class TupleToolDecayTreeFitter TupleToolDecayTreeFitter.h
 *
 * Yasmine Amhis and Matt Needham
 * 30-10-10
 */
class TupleToolDecayTreeFitter : public TupleToolBase, virtual public IParticleTupleTool
{

private:

  typedef std::map<std::string,std::string> SubstitutionMap ;
  typedef std::map< std::string, std::vector<double> > TupleMap ; ///< temporary storage of tuple data

public:

  /// Standard constructor
  TupleToolDecayTreeFitter( const std::string& type,
                            const std::string& name,
                            const IInterface* parent);

  ~TupleToolDecayTreeFitter( ) = default; ///< Destructor

  StatusCode initialize() override;

  StatusCode finalize() override;

  StatusCode fill( const LHCb::Particle*
                   , const LHCb::Particle*
                   , const std::string&
                   , Tuples::Tuple& ) override;

private:

  ///  Fill inforation for a given origin vertex
  StatusCode fit(DecayTreeFitter::Fitter& fitter,
                 const LHCb::Particle* P,
                 const LHCb::VertexBase* pv,
                 const std::string& prefix,
                 TupleMap& tMap) const;

  ///  Fill lifetime information
  ///  Fill fit inforation for top decay
  StatusCode fillDecay(const DecayTreeFitter::Fitter& fitter,
                       const std::string& prefix,
                       TupleMap& tMap) const;

  ///  Fill lifetime information
  StatusCode fillLT(const DecayTreeFitter::Fitter& fitter,
                    const LHCb::Particle* P,
                    const std::string& prefix,
                    TupleMap& tMap) const;

  ///  Fill momentum and mass information
  StatusCode fillMomentum(const DecayTreeFitter::Fitter& fitter,
                          const LHCb::Particle* P,
                          const std::string& prefix,
                          TupleMap& tMap) const;

  ///  Fill PV information
  StatusCode fillPV(const LHCb::VertexBase* pv,
                    const std::string& prefix,
                    TupleMap& tMap) const;

  ///  Fill lifetime information for non stable daughters
  StatusCode fillDaughters( const DecayTreeFitter::Fitter& fitter,
                            const LHCb::Particle* P,
                            const std::string& prefix,
                            TupleMap& tMap )const;

  ///  Fill lifetime information for non stable daughters
  StatusCode fillStableDaughters( const DecayTreeFitter::Fitter& fitter,
                                  const LHCb::Particle* P,
                                  const std::string& prefix,
                                  TupleMap& tMap )const;

  ///  Fill updated tracks momentum
  StatusCode fillTracksMomentum(const DecayTreeFitter::Fitter& fitter,
                                const LHCb::Particle* P,
                                const std::string& prefix,
                                TupleMap& tMap) const;

  /// actual filling of tuple
  StatusCode fillTuple(TupleMap& tMap,
                       Tuples::Tuple& tuple,
                       const std::string& prefix ) ;

  /// insert helper method
  StatusCode insert( const std::string& leaf,
                     const double val,
                     TupleMap& tMap ) const ;

  std::string getName(const int id) const;  ///< name of particle

  ///  origin vertex
  std::vector<const LHCb::VertexBase*> originVertex( const LHCb::Particle*,
                                                     const LHCb::Particle* ) const;

  /// same PV?
  bool samePV(const LHCb::VertexBase*, const LHCb::VertexBase*) const ;

  /// sort tracks into a set
  std::set<const LHCb::Track*> sortedTracks(const LHCb::VertexBase*) const ;

  /// substitutions
  StatusCode substitute( LHCb::DecayTree& tree);

  /// checkMassConstraints
  StatusCode checkMassConstraints(const LHCb::DecayTree& tree);

  /// Get the TES location for a data object
  template<class TYPE>
  inline std::string tesLocation( const TYPE * obj ) const
  {
    return ( obj && obj->parent() && obj->parent()->registry() ?
             obj->parent()->registry()->identifier() : "NotInTES" );
  }

  /// Test if a given PID code is veto'ed
  inline bool isVetoed( const int id ) const
  {
    return ( !m_vetoPIDs.empty() &&
             std::find( m_vetoPIDs.begin(), m_vetoPIDs.end(), id ) != m_vetoPIDs.end() );
  }

  /// Compare two maps
  inline bool checkTupleKeys( const TupleMap& r )
  {
    bool OK = true;
    if ( UNLIKELY( m_firstTupleKeys.size() != r.size() ) )
    {
      Error( "Maps have different sizes" ).ignore();
      OK = false;
    }
    auto i = m_firstTupleKeys.begin();
    auto j = r.begin();
    for ( ; j != r.end(); ++i, ++j )
    {
      // compare key names
      if ( UNLIKELY( *i != j->first ) )
      {
        std::ostringstream mess;
        mess << "Tuple entry error : " << *i << " != " << j->first;
        Error( mess.str() ).ignore();
        OK = false;
      }
    } 
    return OK;
  }

private:

  std::string m_pvLocation ; ///<  PV location to be used. If empty, take context-dependent default

  IDVAlgorithm* m_dva = nullptr;

  std::vector<std::string> m_massConstraints;
  std::vector<LHCb::ParticleID> m_massConstraintsPids;

  bool m_constrainToOriginVertex;   ///< Constrain to Origin Vertex for lifetime

  LHCb::IParticlePropertySvc* m_ppSvc = nullptr;
  IParticleDescendants* m_particleDescendants = nullptr;

  /// mapping : { 'decay-component' : "new-pid" } (property)
  ISubstitutePID::SubstitutionMap  m_map  ; // mapping : { 'decay-component' : "new-pid" }
  /// Substitute Tool
  ISubstitutePID* m_substitute = nullptr; 
  bool m_storeAnyway ;///< Store PV even if a refitted version is already the best PV (i.e store twice)
  bool m_updateDaughters; ///< Store updated momenta of tracks in the decay tree
  ToolHandle<ITrackStateProvider> m_stateprovider{ "TrackStateProvider" };
  bool m_first{true} ; ///< This is the first candidate
  bool m_firstTupleFill{true}; ///< Flag for the first tuple filling

  std::vector<std::string> m_firstTupleKeys; ///< cache the keys for the first tuple map

  /// List of PID codes to veto filling for
  std::vector<int> m_vetoPIDs;

  /// Use full decay tree in branch names
  bool m_useFullTreeInName;

};
