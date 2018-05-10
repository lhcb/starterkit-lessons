#ifndef TUPLETOOL_MC_ASSOCIATED_CLUSTERS_
#define TUPLETOOL_MC_ASSOCIATED_CLUSTERS_ 1

#include "DecayTreeTupleBase/TupleToolBase.h"
#include "Kernel/IParticleTupleTool.h"            // Interface

class IParticle2MCAssociator;

namespace LHCb
{
  class Particle;
}

/**
 * The TupleToolMCAssociatedClusters stores information on the number of TT/IT/OT/Velo
 * clusters which are associated with the MCParticle (which again is associated with the
 * Particle currently looked at).
 *
 * The information that is stored:
 *   - How many clusters are there associated to this MCParticle
 *   - Out of the clusters used by the track reconstructed, how many are linked to this MCP
 *
 * Combining this with the LoKi track functors to count the total number of clusters
 * used by the track, one then also can deduce the number of clusters used in the track
 * which were not associated with the MCParticle.
 *
 * In order to use this, the linker tables must be there. For modern MC,
 * this requires the DaVinci input file to be at least "LDST".
 *
 * A somewhat related Brunel-tool is the TrackCheckerNT, which runs over all tracks
 * in the event. The goal of this tool is to get the information for a signal decay
 * only.
 *
 * @author Laurent Dufour <laurent.dufour@cern.ch>
 */
class TupleToolMCAssociatedClusters: public TupleToolBase, virtual public IParticleTupleTool
{
	public:
		TupleToolMCAssociatedClusters(const std::string& type, const std::string& name, const IInterface* parent);

		StatusCode initialize() override;

		StatusCode fill(const LHCb::Particle*, const LHCb::Particle*, const std::string&, Tuples::Tuple&) override;
	private:
		std::vector<std::string> m_p2mcAssocTypes;
		std::vector<IParticle2MCAssociator*> m_p2mcAssocs;

		bool m_checkUniqueness;

};

#endif

