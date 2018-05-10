// Include files
#include "Event/Particle.h"
// kernel
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
#include "GaudiKernel/PhysicalConstants.h"
#include "Kernel/IParticle2MCAssociator.h"
//#include "Kernel/Particle2MCLinker.h"
#include <Kernel/IDistanceCalculator.h>
#include "Kernel/IPVReFitter.h"
#include "Kernel/IBIntegrator.h"

//
#include "TrackInterfaces/ITrackExtrapolator.h"
// MC stuff
#include "Event/GenHeader.h"
#include "Event/MCHeader.h"

// from GSL
#include "gsl/gsl_math.h"
// Boost
#include <boost/assign/list_of.hpp>

// From the MuonDetector:
//#include "MuonAlgs/MuonBasicGeometry.h"
#include "MuonDet/MuonBasicGeometry.h"
#include "MuonDet/DeMuonDetector.h"
#include "Event/MuonDigit.h"
#include "Event/MCMuonDigit.h"
#include "Event/MCMuonDigitInfo.h"
#include "Event/IntLink.h"

#include "TupleToolMuonIDCalib.h"
//#include "TMath.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolMuonIDCalib
//
// 2011-09-23 : Fatima Soomro
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( TupleToolMuonIDCalib )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolMuonIDCalib::TupleToolMuonIDCalib( const std::string& type,
                                              const std::string& name,
                                              const IInterface* parent)
    : TupleToolBase ( type, name , parent )
{
  declareInterface<IParticleTupleTool>(this);
  declareProperty ( "NFoi"          ,m_NFoi  = 2 ) ;
  declareProperty( "XFOIParameter1", m_xfoiParam1 );
  declareProperty( "XFOIParameter2", m_xfoiParam2 );
  declareProperty( "XFOIParameter3", m_xfoiParam3 );
  declareProperty( "YFOIParameter1", m_yfoiParam1 );
  declareProperty( "YFOIParameter2", m_yfoiParam2 );
  declareProperty( "YFOIParameter3", m_yfoiParam3 );
  declareProperty( "isVerbose", m_doVerbose = false);
  declareProperty( "MomentumEstimate", m_estimate = true);

  std::vector<double> tmp1 = boost::assign::list_of(0.015)(0.29);
  declareProperty( "resParams", m_resParams = tmp1);

  std::vector<double> tmp2 = boost::assign::list_of(1.04)(0.14);
  declareProperty( "ParabolicCorrection", m_ParabolicCorrection = tmp2);

  declareProperty( "ConstantCorrection",  m_Constant = 0.*Gaudi::Units::MeV );
  declareProperty( "Extrapolator"  ,m_extrapolatorName = "TrackMasterExtrapolator" ) ;

}

//=============================================================================
// Fill the tuple
//=============================================================================
StatusCode TupleToolMuonIDCalib::fill( const LHCb::Particle * /* top */,
                                       const LHCb::Particle *part,
                                       const std::string &   head,
                                       Tuples::Tuple &   tuple )
{
  // nothing for particles that are not Bs
  if( !part->isBasicParticle() || NULL==part->proto() )
    return StatusCode::SUCCESS;

  LoadMuonGeometry();
  if ( msgLevel(MSG::DEBUG) )
    debug () <<"Fatima: Loaded Geometry in TupleToolMuIDCalib"<< endmsg;

  m_bIntegrator = tool<IBIntegrator>( "BIntegrator" );
  if (!m_bIntegrator) return StatusCode::FAILURE;

  m_extrapolator = tool<ITrackExtrapolator>( m_extrapolatorName, "MuEffExtrap",this );
  if (!m_extrapolator) return StatusCode::FAILURE;

  StatusCode sc = fillCoordVectors();
  if(!sc) error()<<"couldnt fillCoorVectors!!!"<<endmsg;
  debug() <<"Fatima: fillCoordVectors in TupleToolMuIDCalib"<< endmsg;

  m_FieldPolarity = 1;
  StatusCode sc2 = FindFieldPolarity();
  if (!sc2) error()<<"couldnt determine field polarity !!"<<endmsg;

  const std::string prefix=fullName(head);
  StatusCode sc3 = fillVars( part, prefix, tuple );
  return sc3;
  //return StatusCode(fillVars( part, prefix, tuple ));
}


//==========================================================================
// Load the geometry
//==========================================================================

void TupleToolMuonIDCalib::LoadMuonGeometry()
{
  m_NStation = 0;
  m_NRegion = 0;
  MuonBasicGeometry basegeometry( detSvc(),msgSvc());
  m_NStation= basegeometry.getStations();
  m_NRegion = basegeometry.getRegions();
  debug () << "stations and regions from MuonGeometry are "<< m_NStation<<" "<<m_NRegion<<endmsg;

  int i=0;
  while(i<m_NStation){
    m_stationNames.push_back(basegeometry.getStationName(i));
    debug()   <<" station "<<i<<" "<<m_stationNames[i]<<endmsg;
    i++;
  }
  m_mudet=getDet<DeMuonDetector>("/dd/Structure/LHCb/DownstreamRegion/Muon");
  // set the size of the local vectors
  m_regionInnerX.resize(m_NStation);
  m_regionOuterX.resize(m_NStation);
  m_regionInnerY.resize(m_NStation);
  m_regionOuterY.resize(m_NStation);
  for(int station = 0 ; station < m_NStation ; station++ ){
    m_stationZ.push_back(m_mudet->getStationZ(station));
    m_regionInnerX[station] = m_mudet->getInnerX(station);
    m_regionOuterX[station] = m_mudet->getOuterX(station);
    m_regionInnerY[station] = m_mudet->getInnerY(station);
    m_regionOuterY[station] = m_mudet->getOuterY(station);
  }

}

//==========================================================================
// fill vectors of x,y,z positions for the MuonCoords
//==========================================================================

StatusCode TupleToolMuonIDCalib::fillCoordVectors(){

  //  m_HitInTrk.clear();
  m_coordPos.clear();
  m_coordPos.resize(m_NStation * m_NRegion);
  m_hitInFOIx.clear();
  m_hitInFOIdx.clear();
  m_hitInFOIy.clear();
  m_hitInFOIdy.clear();
  m_hitInFOIz.clear();
  m_hitInFOIdz.clear();
  m_hitInFOIuncrossed.clear();
  m_hitInFOIID.clear();
  m_hitInFOITDC1.clear();
  m_hitInFOITDC2.clear();
  m_allMuonHitsX.clear();
  m_allMuonHitsDX.clear();
  m_allMuonHitsY.clear();
  m_allMuonHitsDY.clear();
  m_allMuonHitsZ.clear();
  m_allMuonHitsDZ.clear();
  m_allMuonHitsUncrossed.clear();
  m_allMuonHitsID.clear();
  m_allMuonHitsTDC1.clear();
  m_allMuonHitsTDC2.clear();

  // get the MuonCoords for each station in turn
  LHCb::MuonCoords* coords = NULL;
  if(exist<LHCb::MuonCoords>(LHCb::MuonCoordLocation::MuonCoords))
    coords = get<LHCb::MuonCoords>(LHCb::MuonCoordLocation::MuonCoords);
  if ( !coords )
  {
    err() << "Cannot retrieve MuonCoords " << endmsg;
    return StatusCode::FAILURE;
  }

  // loop over the coords
  LHCb::MuonCoords::const_iterator iCoord;
  for ( iCoord = coords->begin() ; iCoord != coords->end() ; iCoord++ ){
    int region = (*iCoord)->key().region();
    int station = (*iCoord)->key().station();
    bool uncrossed = (*iCoord)->uncrossed();
    double x,dx,y,dy,z,dz;
    int digitTDC1 = (*iCoord)->digitTDC1();
    int digitTDC2 = (*iCoord)->digitTDC2();
    LHCb::MuonTileID tile=(*iCoord)->key();
    StatusCode sc = m_mudet->Tile2XYZ(tile,x,dx,y,dy,z,dz);
    if (sc.isFailure()){
      warning() << "Failed to get x,y,z of tile " << tile << endmsg;
      continue;
    }
    m_coordPos[station*m_NRegion+region].
      push_back(coordExtent_(x,dx,y,dy,z,dz,uncrossed,digitTDC1,digitTDC2,*iCoord));
    //      m_HitInTrk.push_back(false);
  }

  return StatusCode::SUCCESS;

}

// return the FOI in x in a station and region for momentum (in MeV/c)
// this is a simpler version of the parameterization:
// foi = par0 + par2*exp(-par3*p)
//=============================================================================
double TupleToolMuonIDCalib::foiX(const int &station, const int &region, const double &p,
                                  const double &dx){
  //=============================================================================
  const double temp_x = ( m_xfoiParam1[ station * m_NRegion + region ] +
                          m_xfoiParam2[ station * m_NRegion + region ]*
                          std::exp(-m_xfoiParam3[ station * m_NRegion + region ]*p/Gaudi::Units::GeV ) )*dx;
  //in the future optimize this checking that 2*dx =m_padSizeX[station * m_NRegion + region]
  //then eliminates dx from function

  if ( msgLevel(MSG::VERBOSE) )
    verbose()<< "from FOIX "<<temp_x<< endmsg;

  return temp_x;
}


// return the FOI in y in a station and region for momentum (in MeV/c)
//=============================================================================
double TupleToolMuonIDCalib::foiY(const int &station,
                                  const int &region,
                                  const double &p,
                                  const double &dy)
{
  //=============================================================================
  const double temp_y = ( m_yfoiParam1[ station * m_NRegion + region ] +
                          m_yfoiParam2[ station * m_NRegion + region ]*
                          std::exp(-m_yfoiParam3[ station * m_NRegion + region ]*p/Gaudi::Units::GeV ) )*dy;

  if ( msgLevel(MSG::VERBOSE) )
    verbose() <<"from FOIY "<<temp_y<< endmsg;

  return temp_y;
}


//=============================================================================
//  fill Xs, Ys and DistAve
//=============================================================================
StatusCode TupleToolMuonIDCalib::fillVars(  const LHCb::Particle *part,
                                            const std::string prefix, Tuples::Tuple &tuple) {


  if( (!part->isBasicParticle() ) ||  NULL==part->proto()   ){
    error () << "!! Tried to fill MuonIDCalib quantities for particle " <<part->particleID().pid()<< "!!!  This should not happen"<< endmsg;
    return StatusCode::SUCCESS;
  }

  const LHCb::Track*  track= part->proto()->track();
  if((track->checkFlag(LHCb::Track::Flags::Clone))  ||
     !( track->checkType(LHCb::Track::Types::Long) || track->checkType(LHCb::Track::Types::Downstream)
        || track->checkType(LHCb::Track::Types::Ttrack) )){
    return StatusCode::SUCCESS ;
  }

  ///-------------------------------------------------------------------------------------
  // 6 March 2012, Add the muon detector hit positions related to the track, to the ntuple
  // one has to get the muon part of the track (from muonPIDs)and then get the lhcbIDs related to it
  ///-------------------------------------------------------------------------------------

  if(m_doVerbose){

    std::vector<float> m_x;
    std::vector<float> m_y;
    std::vector<float> m_z;

    LHCb::MuonPIDs* pMuids =NULL;
    if(exist<LHCb::MuonPIDs>(LHCb::MuonPIDLocation::Default)) pMuids=get<LHCb::MuonPIDs>(LHCb::MuonPIDLocation::Default);

    if (!pMuids){
      error() << " Failed to get pMuids" <<  LHCb::MuonPIDLocation::Default << endmsg;
      return StatusCode::FAILURE;
    }
    if ( msgLevel(MSG::DEBUG) )
      debug() << " >> number of Mu tracks in the event " << pMuids->size() << endmsg;
    const LHCb::Track *mutrack=NULL;
    LHCb::MuonPIDs::const_iterator ip;
    for( ip = pMuids->begin(); ip != pMuids->end() ; ip++){
      if ((*ip)->idTrack() == track){
        mutrack = (*ip)->muonTrack();
        if (!mutrack) continue;
        debug()<<"found a mutrack "<<endmsg;
      }
    } // end of muonIDs loop
    if ( msgLevel(MSG::DEBUG) )
      debug()<<"Fatima, I out of mutrack loop"<<endmsg;

    if(mutrack) {
      const std::vector< LHCb::LHCbID > lhcbIDs = mutrack->lhcbIDs();
      if ( msgLevel(MSG::DEBUG) )
        debug() <<" Starting to find the muon hit info, have   " <<lhcbIDs.size()<<" to loop over "<<endmsg;

      int countVelo(0), countIT(0), countOT(0), countTT(0), countMuon(0), countCALO(0), countRICH(0);
      std::vector< LHCb::LHCbID >::const_iterator myID = lhcbIDs.begin();
      for ( ; myID != lhcbIDs.end(); myID++ )
      {
        if ( ((*myID).isVelo()) ) countVelo++;
        if ( ((*myID).isIT()) ) countIT++;
        if ( ((*myID).isOT()) ) countOT++;
        if ( ((*myID).isTT()) ) countTT++;
        if ( ((*myID).isCalo()) ) countCALO++;
        if ( ((*myID).isRich()) ) countRICH++;

        if ( ((*myID).isMuon()) )
        {
          countMuon++;
          const LHCb::MuonTileID mutile = (*myID).muonID();
          double x,dx,y,dy,z,dz;
          StatusCode sc = m_mudet->Tile2XYZ(mutile,x,dx,y,dy,z,dz);
          if ( msgLevel(MSG::DEBUG) )
            debug()<<"Muon pad hit: "<<x<< " "<<dx<<" "<<y<<" "<<dy<<" "<<z<<" "<<dz<<". Stn: "<<mutile.station()<<" regn: "<<mutile.region()<<" qrtr: "<<mutile.quarter()<<" nX and nY "<<mutile.nX()<<" "<<mutile.nY()<<endmsg;
          m_x.push_back(x);
          m_y.push_back(y);
          m_z.push_back(z);
        }
      }
      if ( msgLevel(MSG::DEBUG) )
      {
        debug()<< "track history is " <<track->checkHistory(LHCb::Track::History::MuonID)  <<endmsg;
        debug()<<"=============== velo "<<countVelo<<" IT "<< countIT <<" OT "<< countOT
               <<" TT "<< countTT<<" calo "<< countCALO<<" rich "<<countRICH<<" muon "<<countMuon
               <<endmsg;
      }
    }

    else
    {
      if ( msgLevel(MSG::DEBUG) )
        debug()<<"While trying to get lhcbIDs, I didn't find a mutrack! "<<endmsg;
    }

    tuple->farray(prefix+"_LHCbID_X", m_x.begin(), m_x.end(), prefix+"_n", 100);
    tuple->farray(prefix+"_LHCbID_Y", m_y.begin(), m_y.end(), prefix+"_n", 100);
    tuple->farray(prefix+"_LHCbID_Z", m_z.begin(), m_z.end(), prefix+"_n", 100);

  }// m_doVerbose

  ///-------------------------------------------------------------------------------
  /// Project the state at M1 into the rest of the muon stations and store the x, y
  ///-------------------------------------------------------------------------------

  m_stateP = &(track->closestState(m_stationZ[0]));

  for(int station = 0; station < m_NStation ; station++){

    m_trackX[station] = m_stateP->x() + ( m_stateP->tx() * (m_stationZ[station] - m_stateP->z()) );
    m_trackY[station] = m_stateP->y() + ( m_stateP->ty() * (m_stationZ[station] - m_stateP->z()) );
    if ( msgLevel(MSG::DEBUG) )
      debug () <<"Station: "<<station<<".  "<< "m_trackX[station] "
               << m_trackX[station] << " = "<<m_stateP->x() << " + "
               << "  (" << m_stateP->tx() <<" * ("<< m_stationZ[station]
               <<" - "<<m_stateP->z()<<" ) )"<< endmsg;
  } // station

  // extrapolation through the stations to get expected positions with
  // errors due to multiple scattering

  bool ecode = estrapola();
  if ( !ecode ) debug() << "estrapola: track extrapolation failed " << endmsg;

  bool test = true;
  for(int station = 0; station < m_NStation ; station++)
  {
    std::stringstream ss;
    ss << station+1;
    test &= tuple->column(prefix+"_Xs"+ss.str(), m_trackX[station] );
    test &= tuple->column(prefix+"_Ys"+ss.str(), m_trackY[station] );

    test &= tuple->column(prefix+"_extraXs"+ss.str(), m_TextraX[station] );
    test &= tuple->column(prefix+"_extraYs"+ss.str(), m_TextraY[station] );
    test &= tuple->column(prefix+"_ms2Xs"+ss.str(), m_ms2X[station] );
    test &= tuple->column(prefix+"_ms2Ys"+ss.str(), m_ms2Y[station] );
    test &= tuple->column(prefix+"_TRegs"+ss.str(), m_TextraReg[station] );
  }

  // ======================================
  // Calculate and fill DistAve and Xs, Ys
  // ======================================

  double m_momentum = m_stateP -> p();
  int nHits=0;
  int ic_hit = 0;
  //int ic_closesthit = 0;
  float dist_ave = 0;
  std::vector<double> small_dist(m_NStation);

  for ( int i = 0; i < m_NStation; ++i )
  {
    small_dist[i] = 100000000.;
    m_smalldist_X[i] = 100000000.;
    m_smalldist_Y[i] = 100000000.;
    m_smalldist_Z[i] = 100000000.;
    m_smalldist_dX[i] = 0.;
    m_smalldist_dY[i] = 0.;
    m_smalldist_dZ[i] = 0.;
  }

  // store hit info if its in the FOI of the track


  for(int station = 0 ; station < m_NStation ; station++){

    for(int region = 0 ; region < m_NRegion ; region++){

      if( !m_coordPos[station*m_NRegion + region].empty() ){

        std::vector<coordExtent_>::const_iterator itPos;

        for(itPos = m_coordPos[station*m_NRegion + region].begin(); itPos != m_coordPos[station*m_NRegion + region].end(); itPos++){

	      // all muon hits
          if(m_doVerbose) {
            m_allMuonHitsX.push_back(itPos->m_x);
            m_allMuonHitsDX.push_back(itPos->m_dx);
            m_allMuonHitsY.push_back(itPos->m_y);
            m_allMuonHitsDY.push_back(itPos->m_dy);
            m_allMuonHitsZ.push_back(itPos->m_z);
            m_allMuonHitsDZ.push_back(itPos->m_dz);
            m_allMuonHitsUncrossed.push_back(itPos->m_uncrossed);
	          m_allMuonHitsID.push_back(station*m_NRegion + region);
            m_allMuonHitsTDC1.push_back(itPos->m_digitTDC1);
            m_allMuonHitsTDC2.push_back(itPos->m_digitTDC2);
          }

          // hits in foi
          double x = itPos->m_x;
          double dx = itPos->m_dx;
          double y = itPos->m_y;
          double dy = itPos->m_dy;

          if ( msgLevel(MSG::VERBOSE) )
            verbose() << "Computing DistAve: x, dx, y, dy are   "<<x<<"  "<<dx<<"  "<<y<<"  "<<dy<< endmsg;

          double xdist = fabs(x-m_trackX[station])/dx;
          double ydist = fabs(y-m_trackY[station])/dy;
          double dist = std::sqrt( xdist*xdist + ydist*ydist  );

          if ( msgLevel(MSG::VERBOSE) )
            verbose() <<"xdist, ydist and dist are  "<<xdist<<" "<<ydist<<" "<<dist<< endmsg;

          // Look for the Closest hit to the track in any station:
          if (dist < small_dist[station])
          {
            small_dist[station] = dist;
            m_smalldist_X[station] = itPos->m_x;
            m_smalldist_Y[station] = itPos->m_y;
            m_smalldist_Z[station] = itPos->m_z;
            m_smalldist_dX[station] = itPos->m_dx;
            m_smalldist_dY[station] = itPos->m_dy;
            m_smalldist_dZ[station] = itPos->m_dz;
            //ic_closesthit = ic_hit;
          }

          // define the FOI in which we have to evaluate the average
          double foiXDim = m_NFoi*foiX( station, region, m_momentum, dx);
          double foiYDim = m_NFoi*foiY( station, region, m_momentum, dy);

          if ( msgLevel(MSG::VERBOSE) )
            verbose() << "Checking for FOI: "<< m_NFoi<<" "<<station<<" "<<region
                      <<" "<<m_momentum<<" "<<dx<< endmsg;

          // check if the hit is in the window
          if(  ( fabs( x - m_trackX[station] ) < foiXDim ) &&
               ( fabs( y - m_trackY[station] ) < foiYDim )  )
          {
            ++nHits;
            dist_ave = dist_ave + (xdist*xdist+ydist*ydist);

            if(m_doVerbose){
              m_hitInFOIx.push_back(x);
              m_hitInFOIdx.push_back(dx);
              m_hitInFOIy.push_back(y);
              m_hitInFOIdy.push_back(dy);
              m_hitInFOIz.push_back(itPos->m_z);
              m_hitInFOIdz.push_back(itPos->m_dz);
              m_hitInFOIuncrossed.push_back(itPos->m_uncrossed);
              m_hitInFOIID.push_back(station*m_NRegion + region);
              m_hitInFOITDC1.push_back(itPos->m_digitTDC1);
              m_hitInFOITDC2.push_back(itPos->m_digitTDC2);
              m_hitInFOIID.push_back(station*m_NRegion + region);
            }

          }

          ++ic_hit;
        }// for itPos

      }// region not empty

    }// region

  } // station

  if(m_doVerbose){
    tuple->farray(prefix+"_hitInFOI_X",  m_hitInFOIx.begin(),  m_hitInFOIx.end(),  prefix+"_n_InFOI", 500);
    tuple->farray(prefix+"_hitInFOI_dX", m_hitInFOIdx.begin(), m_hitInFOIdx.end(), prefix+"_n_InFOI", 500);
    tuple->farray(prefix+"_hitInFOI_Y",  m_hitInFOIy.begin(),  m_hitInFOIy.end(),  prefix+"_n_InFOI", 500);
    tuple->farray(prefix+"_hitInFOI_dY", m_hitInFOIdy.begin(), m_hitInFOIdy.end(), prefix+"_n_InFOI", 500);
    tuple->farray(prefix+"_hitInFOI_Z",  m_hitInFOIz.begin(),  m_hitInFOIz.end(),  prefix+"_n_InFOI", 500);
    tuple->farray(prefix+"_hitInFOI_dZ", m_hitInFOIdz.begin(), m_hitInFOIdz.end(), prefix+"_n_InFOI", 500);
    tuple->farray(prefix+"_hitInFOI_ID", m_hitInFOIID.begin(), m_hitInFOIID.end(), prefix+"_n_InFOI", 500);
    tuple->farray(prefix+"_hitInFOI_uncrossed", m_hitInFOIuncrossed.begin(), m_hitInFOIuncrossed.end(), prefix+"_n_InFOI", 500);
    tuple->farray(prefix+"_hitInFOI_TDC1", m_hitInFOITDC1.begin(), m_hitInFOITDC1.end(), prefix+"_n_InFOI", 500);
    tuple->farray(prefix+"_hitInFOI_TDC2", m_hitInFOITDC2.begin(), m_hitInFOITDC2.end(), prefix+"_n_InFOI", 500);
    if(prefix=="lab2"){
      tuple->farray(prefix+"_allMuonHits_X",  m_allMuonHitsX.begin(), m_allMuonHitsX.end(),  prefix+"_n_AllHits", 10000);
      tuple->farray(prefix+"_allMuonHits_dX", m_allMuonHitsDX.begin(), m_allMuonHitsDX.end(), prefix+"_n_AllHits", 10000);
      tuple->farray(prefix+"_allMuonHits_Y",  m_allMuonHitsY.begin(), m_allMuonHitsY.end(),  prefix+"_n_AllHits", 10000);
      tuple->farray(prefix+"_allMuonHits_dY", m_allMuonHitsDY.begin(), m_allMuonHitsDY.end(), prefix+"_n_AllHits", 10000);
      tuple->farray(prefix+"_allMuonHits_Z",  m_allMuonHitsZ.begin(), m_allMuonHitsZ.end(),  prefix+"_n_AllHits", 10000);
      tuple->farray(prefix+"_allMuonHits_dZ", m_allMuonHitsDZ.begin(), m_allMuonHitsDZ.end(), prefix+"_n_AllHits", 10000);
      tuple->farray(prefix+"_allMuonHits_ID", m_allMuonHitsID.begin(), m_allMuonHitsID.end(), prefix+"_n_AllHits", 10000);
      tuple->farray(prefix+"_allMuonHits_uncrossed", m_allMuonHitsUncrossed.begin(), m_allMuonHitsUncrossed.end(), prefix+"_n_AllHits", 10000);
      tuple->farray(prefix+"_allMuonHits_TDC1", m_allMuonHitsTDC1.begin(), m_allMuonHitsTDC1.end(), prefix+"_n_AllHits", 10000);
      tuple->farray(prefix+"_allMuonHits_TDC2", m_allMuonHitsTDC2.begin(), m_allMuonHitsTDC2.end(), prefix+"_n_AllHits", 10000);
    }

    for(int station = 0; station < m_NStation ; ++station )
    {
      std::stringstream ss; ss<< station+1 ; const std::string& mychar2 = ss.str();
      test &= tuple->column(prefix+"Stn"+mychar2+"X", m_smalldist_X[station]);
      test &= tuple->column(prefix+"Stn"+mychar2+"Y", m_smalldist_Y[station]);
      test &= tuple->column(prefix+"Stn"+mychar2+"Z", m_smalldist_Z[station]);
    }

  }


  if(m_estimate){
    // estimate the momentum of the muon-track assuming it cames from the primary vertex
    // first --> do a linear fit with the closest hits in FOI, starting from M1 to M5,
    //           if successful call the tool to estimate the momentum
    //
    m_5pZM1   = 0.;
    m_5qOverP = 0.;
    m_5sigmaQOverP2 = 0.;
    m_5pXPvtx = 0.;
    m_5pYPvtx = 0.;
    m_5pZPvtx = 0.;

    int FromM1 = 1;
    bool fcode = linFit(FromM1);
    if ( !fcode ) {
      debug() << "linFit: linear fit closest hits in FOI failed " << endmsg;
    } else {
      bool pcode = calculatePt(FromM1);
      if ( !pcode ) debug() << "calculatePt: calculation momentum of muon track failed From M1" << endmsg;
    }
    test &= tuple->column(prefix+"_5pZM1", m_5pZM1);
    test &= tuple->column(prefix+"_5qOverP", m_5qOverP);
    test &= tuple->column(prefix+"_5sigmaQOverP2", m_5sigmaQOverP2);
    test &= tuple->column(prefix+"_5pXvtx", m_5pXPvtx);
    test &= tuple->column(prefix+"_5pYvtx", m_5pYPvtx);
    test &= tuple->column(prefix+"_5pZvtx", m_5pZPvtx);

    // estimate the momentum of the muon-track assuming it cames from the primary vertex
    // first --> do a linear fit with the closest hits in FOI, starting from M2 to M5,
    //           if successful call the tool to estimate the momentum
    //
    m_4pZM1   = 0.;
    m_4qOverP = 0.;
    m_4sigmaQOverP2 = 0.;
    m_4pXPvtx = 0.;
    m_4pYPvtx = 0.;
    m_4pZPvtx = 0.;

    int FromM1_2 = 0;
    bool fcode_2 = linFit(FromM1_2);
    if ( !fcode_2 ) {
      debug() << "linFit: linear fit closest hits in FOI failed " << endmsg;
    } else {
      bool pcode_2 = calculatePt(FromM1_2);
      if ( !pcode_2 ) debug() << "calculatePt: calculation momentum of muon track failed From M2" << endmsg;
    }
    test &= tuple->column(prefix+"_4pZM1", m_4pZM1);
    test &= tuple->column(prefix+"_4qOverP", m_4qOverP);
    test &= tuple->column(prefix+"_4sigmaQOverP2", m_4sigmaQOverP2);
    test &= tuple->column(prefix+"_4pXvtx", m_4pXPvtx);
    test &= tuple->column(prefix+"_4pYvtx", m_4pYPvtx);
    test &= tuple->column(prefix+"_4pZvtx", m_4pZPvtx);
  }

  if (nHits>0)
  {
    dist_ave = dist_ave/nHits;
  }

  if ( msgLevel(MSG::VERBOSE) )
    verbose() <<" So the dist_ave for track "<< track->key()<<"  ("<< track->charge()
              << ")   will be: " << dist_ave<< endmsg;

  test &= tuple->column( prefix+"_DistAve", dist_ave ) ;
  return test ? StatusCode::SUCCESS : StatusCode::FAILURE;

} // fillVars


/// track fitting with linear Chi^2 from M1/M2 to M5
bool TupleToolMuonIDCalib::linFit(int &FromM1)
{

  m_sx = m_sy = 0.;
  m_bx = m_by = 0.;
  m_errbx = m_errsx = m_covbsx = 0.;
  m_errby = m_errsy = m_covbsy = 0.;
  m_chi2x = m_chi2y = 0.;


  int StartI = 0;              // fit from M1 to M5
  if (FromM1==0) StartI = 1;   // fit from M2 to M5

  int nPunti = 0;
  for ( int i = StartI; i < m_NStation; ++i ) {
    if (m_smalldist_X[i]==0.0) continue;
    nPunti++;
  }

  double dof = nPunti - 2.;
  if (dof== 0.) return false;

  double xc11,xc12,xc22,xv1,xv2,xxx;
  xc11 = xc12 = xc22 = xv1 = xv2 = xxx = 0.;
  double yc11,yc12,yc22,yv1,yv2,yyy;
  yc11 = yc12 = yc22 = yv1 = yv2 = yyy = 0.;
  double xdet,ydet;
  xdet = ydet = 0.;
  double xerr,yerr;
  xerr = yerr = 0.;

  for ( int i = StartI; i < m_NStation; ++i ) {

    if (m_smalldist_dX[i]== 0.0) continue;
    double x,dx,y,dy,z; //dz;

    x = m_smalldist_X[i];
    y = m_smalldist_Y[i];
    z = m_smalldist_Z[i];
    dx = m_smalldist_dX[i];
    dy = m_smalldist_dY[i];
    //dz = m_smalldist_dZ[i];

    xerr = 2.* dx;
    yerr = 2.* dy;

    // then fit them with a min chi^2 in the 2 projections xz and yz
    //1) XZ projection:
    xc11 += z*z/xerr/xerr;
    xc12 +=   z/xerr/xerr;
    xc22 += 1.0/xerr/xerr;
    xv1  += z*x/xerr/xerr;
    xv2  +=   x/xerr/xerr;
    xxx  += x*x/xerr/xerr;

    //2) YZ projection:
    yc11 += z*z/yerr/yerr;
    yc12 +=   z/yerr/yerr;
    yc22 += 1.0/yerr/yerr;
    yv1  += z*y/yerr/yerr;
    yv2  +=   y/yerr/yerr;
    yyy  += y*y/yerr/yerr;

  }

  if( (xdet = xc11*xc22 - xc12*xc12) == 0 ) return false;
  if( (ydet = yc11*yc22 - yc12*yc12) == 0 ) return false;
  m_sx = (xv1*xc22-xv2*xc12)/xdet;
  m_sy = (yv1*yc22-yv2*yc12)/ydet;
  m_bx = (xv2*xc11-xv1*xc12)/xdet;
  m_by = (yv2*yc11-yv1*yc12)/ydet;

  m_errbx = sqrt(xc11/xdet);
  m_errsx = sqrt(xc22/xdet);
  m_covbsx = -xc12/xdet;

  m_errby = sqrt(yc11/ydet);
  m_errsy = sqrt(yc22/ydet);
  m_covbsy = -yc12/ydet;

  m_chi2x = (xxx + m_sx*m_sx*xc11 + m_bx*m_bx*xc22 - 2.*m_sx*xv1 -2.*m_bx*xv2 + 2*m_sx*m_bx*xc12)/dof;
  m_chi2y = (yyy + m_sy*m_sy*yc11 + m_by*m_by*yc22 - 2.*m_sy*yv1 -2.*m_by*yv2 + 2*m_sy*m_by*yc12)/dof;

  return true;
}

//=============================================================================
// Determination of the field polarity
//=============================================================================
StatusCode TupleToolMuonIDCalib::FindFieldPolarity()
{
  // compute fiels polarity
  // we will use this value for all tracks

  double Zfirst = m_stationZ[0];   // M1

  Gaudi::XYZPoint  begin( 0., 0., 0. );
  Gaudi::XYZPoint  end( 0., 0., Zfirst );
  Gaudi::XYZVector bdl;
  double z;

  StatusCode sc = m_bIntegrator -> calculateBdlAndCenter( begin, end, 0., 0., z, bdl );
  if (sc.isFailure()){
    return Error("Failed to find field centre !", StatusCode::FAILURE);
  }

  if ( bdl.x() > 0.0 ) {
    m_FieldPolarity =  1;
  }
  else {
    m_FieldPolarity = -1;
  }

  debug() << "Integrated B field is "<< bdl.x() << " Tm" <<
    "  centered at Z="<< z/Gaudi::Units::m << " m"<<endmsg;

  return StatusCode::SUCCESS;
}

//====================================================================================
/// estimate the momentum of the muon-track assuming it cames from the primary vertex
//====================================================================================
bool TupleToolMuonIDCalib::calculatePt(int &FromM1)
{

  double pZM1   = 0.;
  double qOverP = 0.;
  double sigmaQOverP2 = 0.;
  double pXPvtx = 0.;
  double pYPvtx = 0.;
  double pZPvtx = 0.;

  double Zfirst = m_stationZ[0];           //M1
  if ( FromM1==0 ) Zfirst = m_stationZ[1]; //M2

  Gaudi::XYZPoint trackPos(m_bx + m_sx*Zfirst,
                           m_by + m_sy*Zfirst, Zfirst);
  LHCb::State state(LHCb::StateVector(trackPos,
                                      Gaudi::XYZVector(m_sx, m_sy, 1.0 ), 1./10000.));

 //
  Gaudi::XYZPoint  begin( 0., 0., 0. );
  Gaudi::XYZPoint  end(state.x() , state.y(), state.z() );
  Gaudi::XYZVector bdl;
  double zCenter;

  StatusCode sc = m_bIntegrator -> calculateBdlAndCenter(begin, end, state.tx(),
                                                         state.ty(), zCenter, bdl );
  if (!sc){
    error() <<" Failed to integrate field !!"<<endmsg;
    return false;
  }

 // copied from the TrackPtKick tool by M. Needham
  double q = 0.;
  double p = 1e6 * Gaudi::Units::MeV;

  double tX;
  double xCenter;
  double zeta_trk;
  double tx_vtx;
  double zeta_vtx;
  double curv;

  if ( fabs( bdl.x() ) > TrackParameters::hiTolerance ) {
    //can estimate momentum and charge

    //Rotate to the  0-0-z axis and do the ptkick
    tX       = state.tx();
    xCenter  = state.x() + tX * ( zCenter - state.z() );

    zeta_trk = -tX / sqrt( 1.0 + tX*tX );
    tx_vtx   = xCenter / zCenter;
    zeta_vtx = -tx_vtx/ sqrt( 1.0 + tx_vtx*tx_vtx );

    // curvature
    curv = ( zeta_trk - zeta_vtx );

    // charge
    int sign = 1;
    if( curv < TrackParameters::hiTolerance ) {
      sign *= -1;
    }
    if ( bdl.x() < TrackParameters::hiTolerance ) {
      sign *= -1;
    }
    q = -1. * m_FieldPolarity*sign;

    // momentum
    p = Gaudi::Units::eplus * Gaudi::Units::c_light *fabs(bdl.x())
      * sqrt((1.0 +tX*tX+gsl_pow_2(state.ty()))
             /(1.0 +gsl_pow_2(tX)))/fabs(curv);

    //   Addition Correction factor for the angle of the track!
    if ( m_ParabolicCorrection.size() == 2u ) {
      //p*= (a + b*tx*tx )
      p+= m_Constant;
      p*= ( m_ParabolicCorrection[0] + (m_ParabolicCorrection[1] * tX * tX ));
    }

  }
  else {
    // can't estimate momentum or charge
    error() << "B integral is 0!" << endmsg;
    return false;
  }

  pZM1 = p;
  qOverP = q / p;
  const double err2 = gsl_pow_2(m_resParams[0]) + gsl_pow_2(m_resParams[1]/p) ;
  sigmaQOverP2 = err2*gsl_pow_2(1.0/p);

   // set MuonTrack momentum variables (momentum at primary vertex)
  if ( (tx_vtx*tx_vtx + state.ty()*state.ty()) > 1.) {
    // can't estimate momentum vector components
    error() << "can't estimate px, py, pz " << endmsg;
    return false;
  }
  double pz_vtx =  p * sqrt(1- tx_vtx*tx_vtx - state.ty()*state.ty() );
  Gaudi::XYZVector momentum_vtx( tx_vtx * pz_vtx,
                                 state.ty()* pz_vtx,
                                 pz_vtx);
  pXPvtx = momentum_vtx.X();
  pYPvtx = momentum_vtx.Y();
  pZPvtx = momentum_vtx.Z();

  if (FromM1==1)
  {
    m_5pZM1 = pZM1;
    m_5qOverP = qOverP;
    m_5sigmaQOverP2 = sigmaQOverP2;
    m_5pXPvtx = pXPvtx;
    m_5pYPvtx = pYPvtx;
    m_5pZPvtx = pZPvtx;
  }
  else if (FromM1==0)
  {
    m_4pZM1 = pZM1;
    m_4qOverP = qOverP;
    m_4sigmaQOverP2 = sigmaQOverP2;
    m_4pXPvtx = pXPvtx;
    m_4pYPvtx = pYPvtx;
    m_4pZPvtx = pZPvtx;
  }

  return true;
}

// extrapolation through the stations to get expected positions with
// errors due to multiple scattering
//=======================================================
bool  TupleToolMuonIDCalib::estrapola(){
//=======================================================

  LHCb::State ExtraState =*(m_stateP);

  for (int station=0; station<m_NStation; station++) {

    StatusCode sc = m_extrapolator->propagate( ExtraState, m_stationZ[station], LHCb::Tr::PID::Muon() );
    if( sc.isFailure() ) {
      debug() << " ==> Extrapolating to z = " << m_stationZ[station] << " failed "<<endmsg;
      return false;
    }

    m_TextraX[station] = ExtraState.x();
    m_TextraY[station] = ExtraState.y();
    m_ms2X[station] = ExtraState.errX2();
    m_ms2Y[station] = ExtraState.errY2();

    int region=0;
    while (region<3) {
      if ( fabs( m_TextraX[station] ) < (m_regionOuterX[station] / pow(2.,3-region)) &&
           fabs( m_TextraY[station] ) < (m_regionOuterY[station] / pow(2.,3-region)) ) break;
      region++;
    }
    m_TextraReg[station] = region;
  }
  return true;
}
