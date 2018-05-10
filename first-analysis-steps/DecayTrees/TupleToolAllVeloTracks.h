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

/** @class TupleToolAllVeloTracks TupleToolAllVeloTracks.h
 *
 *  This tuple tool fills information about every VELO track best-tracks container.
 *
 *  The primary target of this tool is analysis in a low multiplicity environment, particularly CEP studies.
 *  The default case is therefore to limit the stored information to 50 tracks in the container, in order
 *  to minimise the impact on ntuple size. However, this limit is left configurable.
 *
 *  @author Dan Johnson, Victor Coco, Kazu Akiba
 *  @date   2017-05-25
 */
class TupleToolAllVeloTracks : public TupleToolBase,
                               virtual public IEventTupleTool {
 public:
  /// Standard constructor
  TupleToolAllVeloTracks(const std::string& type, const std::string& name,
                         const IInterface* parent);

  /// Declare properties
  Gaudi::Property< std::string > m_location{this, "Location", LHCb::TrackLocation::Default, "Location of particles"};
  Gaudi::Property< unsigned int > m_max    {this, "Max"     , 50  						  , "Maximum n photons to store information about"};

  virtual StatusCode fill(Tuples::Tuple&) override;  ///< Fill tuple
  StatusCode initialize() override;                  ///< init

};

#endif  // TUPLETOOLALLTRACKS_H
