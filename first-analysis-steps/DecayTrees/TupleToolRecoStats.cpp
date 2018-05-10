#include "TupleToolRecoStats.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolRecoStats
//
// 2009-02-11 : Patrick Koppenburg
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolRecoStats::TupleToolRecoStats( const std::string& type,
                                        const std::string& name,
                                        const IInterface* parent )
  : TupleToolBase ( type, name, parent )
,m_conv(NULL)
{
  declareInterface<IEventTupleTool>(this);
}

//=============================================================================
// Destructor
//=============================================================================
TupleToolRecoStats::~TupleToolRecoStats() {}

//=============================================================================
StatusCode TupleToolRecoStats::fill( Tuples::Tuple& tup )
{
  // Initialise the converter tool
  m_conv = tool<IReportConvert>("ReportConvertTool", this );
  if ( !m_conv ){
    return Error("Unable to retrieve the Report converter tool");
  }
  m_conv->setReportVersionLatest();
  bool toDelete=false;

  const std::string prefix = fullName();

  // Load the RecSummary object
  LHCb::RecSummary * rS = getIfExists<LHCb::RecSummary>(evtSvc(),LHCb::RecSummaryLocation::Default);
  if ( !rS )
  {
    rS = getIfExists<LHCb::RecSummary>(evtSvc(),LHCb::RecSummaryLocation::Default,false);
  }
  if ( !rS )
  {
    rS = getIfExists<LHCb::RecSummary>(evtSvc(),"/Event/Turbo/Rec/Summary",false);
  }
  // If not there then try to recreate from selection reports
  if ( !rS )
  {
    toDelete=true;
    rS = new LHCb::RecSummary();
    const LHCb::HltSelReports* selReports = get<LHCb::HltSelReports>( "Hlt2/SelReports" );
    if(selReports){
      const LHCb::HltObjectSummary* recsummaryObj = selReports->selReport("Hlt2RecSummary");
      if(recsummaryObj){
        const LHCb::HltObjectSummary* Rec_subobj = recsummaryObj->substructure()[0].target();
        if(Rec_subobj){
          const LHCb::HltObjectSummary::Info& Rec_info = Rec_subobj->numericalInfo();
          m_conv->RecSummaryObjectFromSummary(&Rec_info,rS);
        }
      }
    }
  }

  // Fill the tuple
  bool test = true;

  test &= tup->column( prefix+"nPVs",              rS ? rS->info(LHCb::RecSummary::nPVs,-1) : -2 );

  test &= tup->column( prefix+"nTracks",           rS ? rS->info(LHCb::RecSummary::nTracks,-1)           : -2 );
  test &= tup->column( prefix+"nLongTracks",       rS ? rS->info(LHCb::RecSummary::nLongTracks,-1)       : -2 );
  test &= tup->column( prefix+"nDownstreamTracks", rS ? rS->info(LHCb::RecSummary::nDownstreamTracks,-1) : -2 );
  test &= tup->column( prefix+"nUpstreamTracks",   rS ? rS->info(LHCb::RecSummary::nUpstreamTracks,-1)   : -2 );
  test &= tup->column( prefix+"nVeloTracks",       rS ? rS->info(LHCb::RecSummary::nVeloTracks,-1)       : -2 );
  test &= tup->column( prefix+"nTTracks",          rS ? rS->info(LHCb::RecSummary::nTTracks,-1)          : -2 );
  test &= tup->column( prefix+"nBackTracks",       rS ? rS->info(LHCb::RecSummary::nBackTracks,-1)       : -2 );

  test &= tup->column( prefix+"nRich1Hits",        rS ? rS->info(LHCb::RecSummary::nRich1Hits,-1) : -2 );
  test &= tup->column( prefix+"nRich2Hits",        rS ? rS->info(LHCb::RecSummary::nRich2Hits,-1) : -2 );

  test &= tup->column( prefix+"nVeloClusters",     rS ? rS->info(LHCb::RecSummary::nVeloClusters,-1) : -2 );

  test &= tup->column( prefix+"nITClusters",       rS ? rS->info(LHCb::RecSummary::nITClusters,-1) : -2 );
  test &= tup->column( prefix+"nTTClusters",       rS ? rS->info(LHCb::RecSummary::nTTClusters,-1) : -2 );
  test &= tup->column( prefix+"nOTClusters",       rS ? rS->info(LHCb::RecSummary::nOTClusters,-1) : -2 );

  test &= tup->column( prefix+"nSPDHits",          rS ? rS->info(LHCb::RecSummary::nSPDhits,-1) : -2 );

  test &= tup->column( prefix+"nMuonCoordsS0",     rS ? rS->info(LHCb::RecSummary::nMuonCoordsS0,-1) : -2 );
  test &= tup->column( prefix+"nMuonCoordsS1",     rS ? rS->info(LHCb::RecSummary::nMuonCoordsS1,-1) : -2 );
  test &= tup->column( prefix+"nMuonCoordsS2",     rS ? rS->info(LHCb::RecSummary::nMuonCoordsS2,-1) : -2 );
  test &= tup->column( prefix+"nMuonCoordsS3",     rS ? rS->info(LHCb::RecSummary::nMuonCoordsS3,-1) : -2 );
  test &= tup->column( prefix+"nMuonCoordsS4",     rS ? rS->info(LHCb::RecSummary::nMuonCoordsS4,-1) : -2 );
  test &= tup->column( prefix+"nMuonTracks",       rS ? rS->info(LHCb::RecSummary::nMuonTracks,-1)   : -2 );

  if(toDelete) delete rS;
  return StatusCode(test);
}

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolRecoStats )
