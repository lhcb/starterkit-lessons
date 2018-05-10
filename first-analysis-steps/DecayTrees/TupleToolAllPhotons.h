#ifndef TUPLETOOLALLTRACKS_H
#define TUPLETOOLALLTRACKS_H 1

// Include files
// from Gaudi
#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IEventTupleTool.h"  // Interface
#include "Kernel/IRelatedPVFinder.h"
#include <Kernel/IDistanceCalculator.h>

struct ITrackExtrapolator;
struct ITrackStateProvider;
class IParticle2MCAssociator;

/** @class TupleToolAllPhotons TupleToolAllPhotons.h
 *
 *  This tuple tool fills information about every calorimeter object in the StdLooseAllPhotons container.
 *
 *  The primary target of this tool is analysis in a low multiplicity environment, particularly CEP studies.
 *  The default case is therefore to limit the stored information to 12 objects in the container, in order
 *  to minimise the impact on ntuple size. However, this limit is left configurable.
 *
 *  @author Dan Johnson, Victor Coco, Kazu Akiba
 *  @date   2017-05-25
 */
class TupleToolAllPhotons : public TupleToolBase,
                            virtual public IEventTupleTool {
 public:
  /// Standard constructor
  TupleToolAllPhotons(const std::string& type, const std::string& name,
                      const IInterface* parent);

  /// Declare properties
  Gaudi::Property< std::string > m_location{this, "Location", "/Event/Phys/StdLooseAllPhotons/Particles", "Location of particles"};
  Gaudi::Property< unsigned int > m_max    {this, "Max"     , 12										, "Maximum n photons to store information about"};
  Gaudi::Property< bool > m_allChi2		   {this, "AllChi2" , false										, "Fill all chi2?"};
  Gaudi::Property< std::vector<std::string> > m_p2mcAssocTypes {this, "IP2MCPAssociatorTypes" , {"DaVinciSmartAssociator","MCMatchObjP2MCRelator"} , "MC associators to try, in order"};

  virtual StatusCode fill(Tuples::Tuple&) override;  ///< Fill tuple
  StatusCode initialize() override;                  ///< init

 private:
  int TID(const LHCb::Particle* p);
  const IDistanceCalculator* m_dist=0;
  IRelatedPVFinder* m_pvf=0;
  const ITrackStateProvider* m_extrapolator=0;  ///<pointer to the track extrapolator
  std::vector<IParticle2MCAssociator*> m_p2mcAssocs;
};

#endif  // TUPLETOOLALLTRACKS_H
