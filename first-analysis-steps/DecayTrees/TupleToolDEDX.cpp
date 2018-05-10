#include "TupleToolDEDX.h"
#include "map"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Event/Particle.h"
#define MAXCLUSTERS 45
//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolDEDX
//
// 2009-06-09 : Rob Lambert
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
// actually acts as a using namespace TupleTool
DECLARE_COMPONENT( TupleToolDEDX )

namespace {
  using std::pow;
  using std::sqrt;
}
//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolDEDX::TupleToolDEDX( const std::string& type,
                                const std::string& name,
                                const IInterface* parent )
    : GaudiTool ( type, name , parent )
{
  declareInterface<IParticleTupleTool>(this);
  declareProperty("absCharge", m_abs=false);
}

//=============================================================================

StatusCode TupleToolDEDX::fill( const LHCb::Particle*
                                , const LHCb::Particle* P
                                , const std::string& head
                                , Tuples::Tuple& tuple ){
  //fill some information about the particle here!
  bool test = true;
  if( P ){
    //fill some information here!
    //std::cout<<"INSIDE TUPLETOOL HERE!!!!!!" << std::endl;
    if(m_abs) test &= tuple->column( head+"_Q", std::abs(P->charge()) ).isSuccess();
    else test &= tuple->column( head+"_Q", P->charge() ).isSuccess();
    test &= tuple->column( head+"_eta", P->momentum().Rapidity() ).isSuccess();

    if ( !exist<LHCb::VeloClusters>( LHCb::VeloClusterLocation::Default ) ) {
      debug() << "No VeloClusters container found for this event !" << endmsg;
      return StatusCode::FAILURE;
    }
    else {
      m_clusters = get<LHCb::VeloClusters>( LHCb::VeloClusterLocation::Default );
      debug() << "  -> number of clusters found in TES: "
              << m_clusters->size() <<endmsg;
    }

    //NEW CODE:
    //Loop on tracks starts here:
    //int nTracks=tracks->size();
    //int nProtoParticles = protoParticles->size();
    int TrackCounter=0;
    int ClusterCounter=0;
    //int PID=0;
    std::string dirName;

    const LHCb::ProtoParticle* proto = P->proto();
    if(!proto) return StatusCode::SUCCESS;
    const LHCb::Track* track = proto->track();
    if(!track) return StatusCode::SUCCESS;
    //Skip the tracks without VELO(Velo, VeloR, Upstream and Long)hits
    if ( !track->hasVelo() ) {
      debug() <<"Track has no VELO hits, continuing with next track."<< endmsg;
      // continue;
    }

    //Get track IDs:
    const std::vector< LHCb::LHCbID > & trackID = track->lhcbIDs() ;
    // int nClusters=trackID.size();

    //Get track angle vector:
    Gaudi::XYZVector slope= track->slopes();
    Gaudi::XYZVector momentum = track->momentum();
    debug() <<"Starting linker" << endmsg;

    int trackType = track->type();
    double theta = slope.Theta();
    //double phi = slope.Phi();
    // double trackp = sqrt(momentum.Mag2() )/Gaudi::Units::GeV;
    double trackpt = momentum.rho();
    double trackpx = momentum.x();
    double trackpy = momentum.y();
    double trackpz = momentum.z();
    double prap = track->pseudoRapidity();
    double trackChi2 = track->chi2();
    int    trackKey  = track->key();
    int    trackVeloKey  = -10 ;
    const SmartRefVector<LHCb::Track>& ancestors = track->ancestors();
    for (SmartRefVector<LHCb::Track>::const_iterator it4 = ancestors.begin();  it4 != ancestors.end();  ++it4){
      if((*it4)->type() == 1) trackVeloKey = (*it4)->key();
      //else trackVeloKey = -1;
    }

    double trackProbChi2 = track->probChi2();
    double trackLikelihood = track->likelihood();
    double trackGhostProb = track->ghostProbability();
    double trackChi2PerDoF = track->chi2PerDoF();
    int tracknDoF = track->nDoF();

    //Muon PID:
    double protoMuLL(-999);
    double protoMuBkgLL(-999);
    if(proto->hasInfo(200) ){
      const LHCb::MuonPID* protoMuPID = proto->muonPID();
      protoMuLL= protoMuPID->MuonLLMu();
      debug() << "Muon LL: " << protoMuLL << endmsg;
      protoMuBkgLL = protoMuPID->MuonLLBg();
    }

    //Rich PID:
    float protoRichDLLe(-999);
    float protoRichDLLmu(-999);
    float protoRichDLLpi(-999);
    float protoRichDLLk(-999);
    float protoRichDLLp(-999);
    int protoRichBestPID(-1);
    if(proto->hasInfo(100) ) protoRichDLLe = proto->richPID()->particleDeltaLL( (Rich::Electron) );
    if(proto->hasInfo(101) ) protoRichDLLmu = proto->richPID()->particleDeltaLL((Rich::Muon) );
    if(proto->hasInfo(102) ) protoRichDLLpi = proto->richPID()->particleDeltaLL((Rich::Pion) );
    if(proto->hasInfo(103) ) protoRichDLLk = proto->richPID()->particleDeltaLL((Rich::Kaon) );
    if(proto->hasInfo(104) ) protoRichDLLp = proto->richPID()->particleDeltaLL((Rich::Proton) );

    if(proto->richPID() ) {
      protoRichBestPID = proto->richPID()->bestParticleID();
    }

    //Try to find Calo PID:
    const SmartRefVector<LHCb::CaloHypo> &caloVec = proto->calo();
    std::vector<int> protoCaloHypothesis;
    std::vector<int> protoCaloLh;
    SmartRefVector< LHCb::CaloHypo >::const_iterator iCalo;
    for (iCalo=caloVec.begin(); iCalo!= caloVec.end(); iCalo++){
      const LHCb::CaloHypo* protoCaloData = iCalo->target();
      if(protoCaloData){
        protoCaloHypothesis.push_back(protoCaloData->hypothesis());
        protoCaloLh.push_back(protoCaloData->lh());
      }
    }
    //tracks travel at an angle relative to sensors, so have a factor to account for
    // the excess material traversed. This will need to be refined later...
    // dE/dx = constant \times ADC counts/ path. path = 300 um /cos(theta);
    // dE/dx = constant' \times cos(theta)
    double AngleFactor=1;
    //if(m_AngleCorrection){
    if(cos(theta) !=0)AngleFactor = cos(theta);
    //}
    std::vector<LHCb::LHCbID>::const_iterator iID;
    std::vector<double> ADCValues;
    std::vector<double> CorrectedADCValues;
    std::list<double> listOfADCValues;
    debug()<<"==> Begin loop over track IDs: " << trackID.size() << " clusters" <<endmsg;
    ClusterCounter=0;
    debug() << "Track number " << TrackCounter << endmsg;
    double ClusterArray[MAXCLUSTERS]={0.0};//store up to MAXCLUSTERS of info...
    double CorrectedClusterArray[MAXCLUSTERS]={0.0};//store up to MAXCLUSTERS of info...
    for(iID=trackID.begin(); iID!=trackID.end(); ++iID) {
      if((*iID).isVelo()) {
        const LHCb::VeloChannelID vcID = (*iID).veloID();
        //Veloclusters:
        m_clusters = get<LHCb::VeloClusters>( LHCb::VeloClusterLocation::Default );
        debug() << "  -> number of clusters found in TES: "
                << m_clusters->size() <<endmsg;

        debug() << "Getting cluster "  << endmsg;
        LHCb::VeloCluster *cluster = m_clusters->object( vcID );
        if(!cluster) return StatusCode::SUCCESS;

        debug() << "After cluster " << TrackCounter << endmsg;

        if ( 0 == cluster ) {
          error() << "Missing cluster with ID " << vcID << endmsg;
          continue;
        }
        LHCb::VeloChannelID idfilter = cluster->channelID();

        //Separate info for R and Phi sensors...
        if(idfilter.isRType()){
          //Can do something here...
        }
        else if (idfilter.isPhiType() ){
          // do something with the phis.
        }
        double adc = cluster->totalCharge();
        if(ClusterCounter<MAXCLUSTERS){
          ClusterArray[ClusterCounter]=adc;
          ADCValues.push_back(adc);
        }
        adc = cluster->totalCharge()*AngleFactor;
        if(ClusterCounter<MAXCLUSTERS){
          CorrectedClusterArray[ClusterCounter]=adc;
          CorrectedADCValues.push_back(adc);
          //listOfADCValues.push_back(adc);
        }
        else {
          error() << "More clusters than MAXCLUSTERS..." << endmsg;
          continue;
        }

        //Make some plots of the first 2 tracks of the first 3 events to check
        //everything is okay...
        // if(TrackCounter<2 && m_event<3){
        //     boost::format fmtEvt ( "Landau/Event%03d/Track%03d" ) ;
        //     fmtEvt % m_event % TrackCounter ;
        //     dirName = fmtEvt.str() ;
        //     plot1D(adc,dirName, "Landau", 0, 200, 100);
        // }
        ClusterCounter++;
      }
    }
    quickSort(ClusterArray, 0, ClusterCounter-1);
    quickSort(CorrectedClusterArray, 0, ClusterCounter-1);



    float MPV_Median=0;
    float MPV_Corrected_Median=0;
    float MPV_Trunc20=0;
    float MPV_Trunc40=0;
    float MPV_GenMean_0_5=0;
    float MPV_GenMean_2=0;
    float MPV_GenMean_4=0;
    float MPV_GenMean_6=0;
    float MPV_Mean=0;
    int nToInclude20=0;
    int nToInclude40=0;
    double tempMean=0.0;
    if(ClusterCounter>=2){
      //Median:
      debug()<<"==> Calculating Median " << endmsg;
      if( ClusterCounter%2!=0){
        MPV_Corrected_Median=(float)CorrectedClusterArray[ClusterCounter/2];
        MPV_Median=(float)ClusterArray[ClusterCounter/2];
      }
      else{
        MPV_Corrected_Median = 0.5*((float)CorrectedClusterArray[ClusterCounter/2] +
                                    (float)CorrectedClusterArray[(ClusterCounter/2) -1]);
        MPV_Median = 0.5*((float)ClusterArray[ClusterCounter/2] +
                          (float)ClusterArray[(ClusterCounter/2) -1]);
      }

      debug()<<"==> Calculating Mean " << endmsg;
      tempMean=0.0;
      for(int i=0; i<ClusterCounter ; i++)       tempMean += ClusterArray[i];
      MPV_Mean = tempMean/ClusterCounter;
      tempMean=0.0;
      //Trunc20:
      debug()<<"==> Calculating Truncated Mean " << endmsg;
      if(ClusterCounter<5) MPV_Trunc20=MPV_Median;
      else{
        nToInclude20 = (int)round(0.8*ClusterCounter);
        tempMean=0.0;
        for(int i=0; i<nToInclude20 ; i++)          tempMean += ClusterArray[i];
        MPV_Trunc20 = tempMean/nToInclude20;
      }
      //Trunc40:
      if(ClusterCounter<5)         MPV_Trunc40=MPV_Median;
      else{
        nToInclude40 = (int)round(0.6*ClusterCounter);
        tempMean=0.0;
        for(int i=0; i<nToInclude40 ; i++)           tempMean += ClusterArray[i];
        MPV_Trunc40 = tempMean/nToInclude40;
      }

      //Generalized mean, k=-2:
      debug()<<"==> Calculating Generalized Mean " << endmsg;
      tempMean=0.0;

      float PowerK = -2.;
      for(int i=0; i<ClusterCounter ; i++){
        tempMean += pow( ClusterArray[i], PowerK);
      }
      MPV_GenMean_2 = pow( (tempMean/ClusterCounter), 1./PowerK);

      //Generalized mean, k=-0.5:
      tempMean=0.0;
      PowerK = -0.5;
      for(int i=0; i<ClusterCounter ; i++){
        tempMean += pow( ClusterArray[i], PowerK);
      }
      MPV_GenMean_0_5 = pow( (tempMean/ClusterCounter), 1./PowerK);

      //Generalized mean, k=-4:
      tempMean=0.0;
      PowerK = -4.;
      for(int i=0; i<ClusterCounter ; i++){
        tempMean += pow( ClusterArray[i], PowerK);
      }
      MPV_GenMean_4 = pow( (tempMean/ClusterCounter), 1./PowerK);

      //Generalized mean, k=-6:
      tempMean=0.0;
      PowerK = -6.;
      for(int i=0; i<ClusterCounter ; i++){
        tempMean += pow( ClusterArray[i], PowerK);
      }
      MPV_GenMean_6 = pow( (tempMean/ClusterCounter), 1./PowerK);
    }
    //Fill ntuple:
    //tuple->column("EventNumber", m_event);
    //tuple->column("TrackNumber",TrackCounter );
    //tuple->column("phi", phi);
    //tuple->column("TrackID", PID);
    tuple->column(head+"_DEDX_theta", theta);
    //tuple->column("momentum", trackp);
    //tuple->column("momentumTRUE", trackpTRUE/GeV);
    tuple->column( head+"_DEDX_pt", trackpt);
    tuple->column( head+"_DEDX_px", trackpx);
    tuple->column( head+"_DEDX_py", trackpy);
    tuple->column( head+"_DEDX_pz", trackpz);
    tuple->column( head+"_DEDX_pseudoRapidity", prap);
    tuple->column( head+"_DEDX_trackType", trackType);
    tuple->column( head+"_DEDX_trackKey", trackKey);
    tuple->column( head+"_DEDX_trackVeloKey", trackVeloKey);
    tuple->column( head+"_DEDX_trackChi2", trackChi2);
    tuple->column( head+"_DEDX_trackProbChi2", trackProbChi2);
    tuple->column( head+"_DEDX_trackLikelihood", trackLikelihood);
    tuple->column( head+"_DEDX_trackGhostProb", trackGhostProb);
    tuple->column( head+"_DEDX_trackChi2PerDoF", trackChi2PerDoF);
    tuple->column( head+"_DEDX_tracknDoF", tracknDoF);
    //PID:
    tuple->column( head+"_DEDX_protoMuBkgLL", protoMuBkgLL);
    tuple->column( head+"_DEDX_protoMuLL", protoMuLL);
    tuple->column( head+"_DEDX_protoRichDLLe", protoRichDLLe);
    tuple->column( head+"_DEDX_protoRichDLLmu", protoRichDLLmu);
    tuple->column( head+"_DEDX_protoRichDLLpi", protoRichDLLpi);
    tuple->column( head+"_DEDX_protoRichDLLk", protoRichDLLk);
    tuple->column( head+"_DEDX_protoRichDLLp", protoRichDLLp);
    tuple->column( head+"_DEDX_protoRichBestPID", protoRichBestPID);
    tuple->farray( head+"_DEDX_protoCaloHypothesis", protoCaloHypothesis.begin(), protoCaloHypothesis.end(), "CaloHypoLength", 100);
    tuple->farray( head+"_DEDX_protoCaloLh", protoCaloLh.begin(), protoCaloLh.end(), "CaloHypoLength", 100);
    tuple->column( head+"_DEDX_MPV_Mean", MPV_Mean);
    tuple->column( head+"_DEDX_MPV_Median", MPV_Median);
    tuple->column( head+"_DEDX_MPV_Corrected_Median", MPV_Corrected_Median);
    tuple->column( head+"_DEDX_MPV_Trunc20", MPV_Trunc20);
    tuple->column( head+"_DEDX_MPV_Trunc40", MPV_Trunc40);
    tuple->column( head+"_DEDX_MPV_GenMean_2", MPV_GenMean_2);
    tuple->column( head+"_DEDX_MPV_GenMean_4", MPV_GenMean_4);
    tuple->column( head+"_DEDX_MPV_GenMean_6", MPV_GenMean_6);
    tuple->column( head+"_DEDX_MPV_GenMean_0_5", MPV_GenMean_0_5);
    tuple->column( head+"_DEDX_nClustersPerTrack", ClusterCounter);
    tuple->farray( head+"_DEDX_ADCValues", ADCValues.begin(), ADCValues.end(), head+"_DEDX_ArrayLength", MAXCLUSTERS);
    tuple->farray( head+"_DEDX_CorrectedADCValues", CorrectedADCValues.begin(),
                   CorrectedADCValues.end(),
                   head+"_DEDX_ArrayLength", MAXCLUSTERS);

  }else
  {
    return StatusCode::FAILURE;
  }
  return StatusCode(test);
}

//============================================================================
// QuickSort, for finding median...
//============================================================================
void TupleToolDEDX::quickSort(double arr[], int left, int right)
{
  int i = left, j = right;
  double tmp;
  double pivot = arr[(left + right) / 2];
  /* partition */
  while (i <= j) {
    while (arr[i] < pivot)
      i++;
    while (arr[j] > pivot)
      j--;
    if (i <= j) {
      tmp = arr[i];
      arr[i] = arr[j];
      arr[j] = tmp;
      i++;
      j--;
    }
  };
  /* recursion */
  if (left < j)
    quickSort(arr, left, j);
  if (i < right)
    quickSort(arr, i, right);
}
