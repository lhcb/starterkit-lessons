// local
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
#include "TupleToolMCAssociatedClusters.h"

#include "Event/Particle.h"
#include "Event/MCParticle.h"

// from Event/LinkerEvent
#include "Linker/LinkedTo.h"
#include "Linker/LinkedFrom.h"
#include "Linker/LinkedFromKey.h"

// from Event/TrackEvent
#include "Event/Track.h"
#include "Kernel/OTChannelID.h"
#include "Event/OTTime.h"
#include "Event/STCluster.h"
#include "Event/VeloCluster.h"

// from Event/MCEvent
#include "Event/MCParticle.h"

// kernel
#include "Kernel/IParticle2MCAssociator.h"

TupleToolMCAssociatedClusters::TupleToolMCAssociatedClusters(const std::string& type, const std::string& name, const IInterface* parent) :
		TupleToolBase ( type, name, parent )
{
	// interface
	declareInterface<IParticleTupleTool> ( this );

	// MC associators to try, in order
	m_p2mcAssocTypes.push_back ( "DaVinciSmartAssociator" );
	m_p2mcAssocTypes.push_back ( "MCMatchObjP2MCRelator" );
	declareProperty ( "IP2MCPAssociatorTypes", m_p2mcAssocTypes );

	declareProperty ( "CheckUniqueness", m_checkUniqueness=true );
}


//=============================================================================
// Initialization. Check parameters
//=============================================================================
StatusCode TupleToolMCAssociatedClusters::initialize()
{
	const StatusCode sc = TupleToolBase::initialize ();
	if (sc.isFailure ())
		return sc;

	// the MC associators
	m_p2mcAssocs.clear ();
	for (std::vector<std::string>::const_iterator iMCAss = m_p2mcAssocTypes.begin (); iMCAss != m_p2mcAssocTypes.end (); ++iMCAss)
	{
		m_p2mcAssocs.push_back ( tool<IParticle2MCAssociator> ( *iMCAss, this ) );
	}
	if (m_p2mcAssocs.empty ())
	{
		return Error ( "No MC associators configured" );
	}


	return StatusCode::SUCCESS;
}

StatusCode TupleToolMCAssociatedClusters::fill(const LHCb::Particle*,
		const LHCb::Particle* P, const std::string& head, Tuples::Tuple& tuple)
{
	// retrieve the currently linked MCP, this will be our working base
	const LHCb::MCParticle* mcp ( NULL );
	const std::string prefix = fullName ( head );

	unsigned int nFoundOT = 0;
	unsigned int nFoundTT = 0;
	unsigned int nFoundIT = 0;
	unsigned int nFoundVelo = 0;

	unsigned int nFoundUniqueOT = 0;
	unsigned int nFoundUniqueTT = 0;
	unsigned int nFoundUniqueIT = 0;
	unsigned int nFoundUniqueVelo = 0;

	size_t nMCHitsOT = 0;
	size_t nMCHitsTT = 0;
	size_t nMCHitsIT = 0;
	size_t nMCHitsVelo = 0;

	if (!P)
		return StatusCode::FAILURE;

	//first just return if the particle isn't supposed to have a track
	if (!P->isBasicParticle ())
		return StatusCode::SUCCESS;

	const auto& protop = P->proto ();
	if (!protop)
		return StatusCode::SUCCESS;

	const auto& track = protop->track ();
	if (!track)
		return StatusCode::SUCCESS;

	const auto& lhcbIDs = track->lhcbIDs();

	if (msgLevel ( MSG::VERBOSE ))
		verbose() << "Number of lhcbIDs: " << lhcbIDs.size() << endmsg;

	if (msgLevel ( MSG::VERBOSE ))
		verbose () << "Getting related MCP to " << P << endmsg;

	for (std::vector<IParticle2MCAssociator*>::const_iterator iMCAss = m_p2mcAssocs.begin (); iMCAss != m_p2mcAssocs.end (); ++iMCAss)
	{
		mcp = (*iMCAss)->relatedMCP ( P );
		if (mcp)
			break;
	}

	if (msgLevel ( MSG::VERBOSE ))
		verbose () << "Got mcp " << mcp << endmsg;

	if (mcp != NULL)
	{
		// get VeloClusters and count correct and total number of clusters
		// Get the linker table MCParticle => VeloCluster
		if (msgLevel ( MSG::VERBOSE ))
			verbose() << "Trying to retrieve linker tables." << endmsg;

		LinkedFromKey<LHCb::MCParticle, LHCb::OTChannelID> otLink ( evtSvc (), msgSvc (), LHCb::OTTimeLocation::Default );

		LinkedTo<LHCb::MCParticle, LHCb::STCluster> ttLink ( evtSvc (), msgSvc (), LHCb::STClusterLocation::TTClusters );
		LinkedFrom<LHCb::STCluster, LHCb::MCParticle> ttLinkRev ( evtSvc (), msgSvc (), LHCb::STClusterLocation::TTClusters );

		LinkedTo<LHCb::MCParticle, LHCb::STCluster> itLink ( evtSvc (), msgSvc (), LHCb::STClusterLocation::ITClusters );
		LinkedFrom<LHCb::STCluster, LHCb::MCParticle> itLinkRev ( evtSvc (), msgSvc (), LHCb::STClusterLocation::ITClusters );

		LinkedFrom<LHCb::VeloCluster, LHCb::MCParticle> velo2MCPLink ( evtSvc (), msgSvc (), LHCb::VeloClusterLocation::Default  );
		LinkedTo<LHCb::MCParticle, LHCb::VeloCluster> MCP2VeloLink ( evtSvc (), msgSvc (), LHCb::VeloClusterLocation::Default  );

		if (otLink.notFound ())
			return Error ( "Unable to retrieve MCParticle-Cluster linker table for OT" );

		if (itLink.notFound ())
			return Error ( "Unable to retrieve MCParticle-Cluster linker table for IT" );

		if (ttLink.notFound ())
			return Error ( "Unable to retrieve MCParticle-Cluster linker table for TT" );

		if (velo2MCPLink.notFound ())
			return Error ( "Unable to retrieve MCParticle-VeloCluster linker table" );

		if (msgLevel ( MSG::VERBOSE ))
			verbose() << "Building cluster lists for this MCP." << endmsg;

		const auto& otChannelIdList = otLink.keyRange( mcp );
		const auto& itClusterList = itLinkRev.range( mcp );
		const auto& ttClusterList = ttLinkRev.range( mcp );
		const auto& veloClusterList = velo2MCPLink.range( mcp );

		if (msgLevel ( MSG::VERBOSE ))
			verbose() << "Counting cluster lists." << endmsg;
		nMCHitsOT = otChannelIdList.size();
		nMCHitsIT = itClusterList.size();
		nMCHitsTT = ttClusterList.size();
		nMCHitsVelo = veloClusterList.size();

		/**
		 * Comparing here the number of LHCb IDs v.s. the number of MCParticle clusters.
		 */
		std::vector<int> usedVeloIDs;
		std::vector<int> usedOTIDs;
		std::vector<int> usedITIDs;
		std::vector<int> usedTTIDs;

		for (const auto& lhcbID : lhcbIDs)
		{
			if (lhcbID.isOT())
			{
				const LHCb::OTChannelID& measuredOTChannelID = lhcbID.otID();
				const auto& it = std::find_if(otChannelIdList.begin(), otChannelIdList.end(), [&measuredOTChannelID] (const LHCb::OTChannelID& otchannelid) { return otchannelid == measuredOTChannelID; } );

				if (it != otChannelIdList.end())
				{
					if (m_checkUniqueness && std::find(usedOTIDs.begin(), usedOTIDs.end(), measuredOTChannelID) == usedOTIDs.end())
					{
						usedOTIDs.push_back( measuredOTChannelID );
						nFoundUniqueOT++;
					}

					nFoundOT++;
				}
			}

			if (lhcbID.isIT())
			{
				const auto& measuredITChannelID = lhcbID.stID();
				const auto& it = std::find_if(itClusterList.begin(), itClusterList.end(), [&measuredITChannelID] (const LHCb::STCluster* const& cluster) { return cluster->channelID() == measuredITChannelID; } );

				if (it != itClusterList.end())
				{
					if (m_checkUniqueness && std::find(usedITIDs.begin(), usedITIDs.end(), measuredITChannelID) == usedITIDs.end())
					{
						usedITIDs.push_back( measuredITChannelID );
						nFoundUniqueIT++;
					}

					nFoundIT++;
				}
			}


			if (lhcbID.isTT())
			{
				const auto& measuredTTChannelID = lhcbID.stID();
				const auto& it = std::find_if(ttClusterList.begin(), ttClusterList.end(), [&measuredTTChannelID] (const LHCb::STCluster* const& cluster) { return cluster->channelID() == measuredTTChannelID; } );

				if (it != itClusterList.end())
				{
					if (m_checkUniqueness && std::find(usedTTIDs.begin(), usedTTIDs.end(), measuredTTChannelID) == usedTTIDs.end())
					{
						usedTTIDs.push_back( measuredTTChannelID );
						nFoundUniqueTT++;
					}

					nFoundTT++;
				}
			}

			if (lhcbID.isVelo())
			{
				const auto& measuredVeloChannelID = lhcbID.veloID();
				const auto& it = std::find_if(veloClusterList.begin(), veloClusterList.end(), [&measuredVeloChannelID] (const LHCb::VeloCluster* const& cluster) { return cluster->channelID() == measuredVeloChannelID; } );

				if (it != veloClusterList.end())
				{
					if (m_checkUniqueness && std::find(usedVeloIDs.begin(), usedVeloIDs.end(), measuredVeloChannelID) == usedVeloIDs.end())
					{
						usedVeloIDs.push_back( measuredVeloChannelID );
						nFoundUniqueVelo++;
					}

					nFoundVelo++;
				}
			}
		}
	}

	tuple->column( prefix+"_MC_nUsedOTClusters", nFoundOT );
	tuple->column( prefix+"_MC_nUsedITClusters", nFoundIT );
	tuple->column( prefix+"_MC_nUsedTTClusters", nFoundTT );
	tuple->column( prefix+"_MC_nUsedVeloClusters", nFoundVelo );

	if (m_checkUniqueness)
	{
		tuple->column( prefix+"_MC_nUsedUniqueOTClusters", nFoundUniqueOT );
		tuple->column( prefix+"_MC_nUsedUniqueITClusters", nFoundUniqueIT );
		tuple->column( prefix+"_MC_nUsedUniqueTTClusters", nFoundUniqueTT );
		tuple->column( prefix+"_MC_nUsedUniqueVeloClusters", nFoundUniqueVelo );
	}

	tuple->column( prefix+"_MC_nOTClusters", nMCHitsOT );
	tuple->column( prefix+"_MC_nITClusters", nMCHitsIT );
	tuple->column( prefix+"_MC_nTTClusters", nMCHitsTT );
	tuple->column( prefix+"_MC_nVeloClusters", nMCHitsVelo );

	return StatusCode::SUCCESS;
}

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolMCAssociatedClusters )
