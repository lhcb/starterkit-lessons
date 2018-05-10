#include "TupleToolTrackInfo.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Event/Particle.h"
//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolTrackInfo
//
// 2008-03-17 : Stephane Poss
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
// actually acts as a using namespace TupleTool
DECLARE_COMPONENT( TupleToolTrackInfo )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolTrackInfo::TupleToolTrackInfo( const std::string& type,
                                          const std::string& name,
                                          const IInterface* parent )
    : TupleToolBase ( type, name , parent )
{
  declareInterface<IParticleTupleTool>(this);
  declareProperty( "StoreFirstMeasurement", m_storeFirstMeasurement = false, "Store first measurement" );
}

//=============================================================================

StatusCode TupleToolTrackInfo::fill( const LHCb::Particle*
                                     , const LHCb::Particle* P
                                     , const std::string& head
                                     , Tuples::Tuple& tuple )
{
  const std::string prefix = fullName(head);

  bool test = true;
  if ( !P ) return StatusCode::FAILURE;

  //first just return if the particle isn't supposed to have a track
  if ( !P->isBasicParticle() ) return StatusCode::SUCCESS;

  const LHCb::ProtoParticle* protop = P->proto();
  if (!protop) return StatusCode::SUCCESS;

  const LHCb::Track* track = protop->track();
  if (!track) return StatusCode::SUCCESS;

  if (msgLevel(MSG::DEBUG))
    debug() << prefix << " " << track->type()
            << " "+prefix+"_TRACK_CHI2 " << track->chi2() << endmsg ;
  if (msgLevel(MSG::VERBOSE)) verbose() << *track << endmsg ;
  test &= tuple->column( prefix+"_TRACK_Type",  track->type() );
  test &= tuple->column( prefix+"_TRACK_Key",   track->key() );

  if(isVerbose()) test &= tuple->column( prefix+"_TRACK_CHI2",  track->chi2() );
  int nDoF = track->nDoF();
  if(isVerbose()) test &= tuple->column( prefix+"_TRACK_NDOF",  nDoF );
  if (nDoF)
  {
    test &= tuple->column( prefix+"_TRACK_CHI2NDOF", track->chi2()/nDoF );
    test &= tuple->column( prefix+"_TRACK_PCHI2", track->probChi2() );
    if(isVerbose())
    {
      if ( track->info(LHCb::Track::AdditionalInfo::FitVeloNDoF,0) > 0 )
      {
        test &= tuple->column( prefix+"_TRACK_VeloCHI2NDOF",
                               track->info(LHCb::Track::AdditionalInfo::FitVeloChi2, -1.)/
                               track->info(LHCb::Track::AdditionalInfo::FitVeloNDoF, 0) );
      }
      else test &= tuple->column( prefix+"_TRACK_VeloCHI2NDOF",-1.);
      if ( track->info(LHCb::Track::AdditionalInfo::FitTNDoF,0) > 0 )
      {
        test &= tuple->column( prefix+"_TRACK_TCHI2NDOF",
                               track->info(LHCb::Track::AdditionalInfo::FitTChi2, -1.)/
                               track->info(LHCb::Track::AdditionalInfo::FitTNDoF, 0) );
      }
      else test &= tuple->column( prefix+"_TRACK_TCHI2NDOF",-1.);
    }

  }
  else
  {
    if (msgLevel(MSG::VERBOSE)) verbose() << "No NDOF" << endmsg ;
    test &= tuple->column( prefix+"_TRACK_PCHI2",-1.);
    test &= tuple->column( prefix+"_TRACK_CHI2NDOF", -1 );
    if(isVerbose()) test &= tuple->column( prefix+"_TRACK_VeloCHI2NDOF",-1.);
    if(isVerbose()) test &= tuple->column( prefix+"_TRACK_TCHI2NDOF",-1.);
  }
  if ( isVerbose() )
  {
    // -- Run I+II
    int nVeloR      = 0;
    int nVeloPhi    = 0;
    int nVelo       = 0;
    int nVeloPileUp = 0;
    int nTT         = 0;
    int nIT         = 0;
    int nOT         = 0;
    // -- Run III
    int nVP         = 0;
    int nUT         = 0;
    int nFT         = 0;

    //-- hopefully unique double constructed from multiplying all hit IDs
    double veloUTID = 1.;
    double ttUTID   = 1.;
    double itUTID   = 1.;
    double otUTID   = 1.;
    double vpUTID   = 1.;
    double utUTID   = 1.;
    double ftUTID   = 1.;

    for ( LHCb::LHCbID id : track->lhcbIDs() ){
      if ( id.isVelo() ){
        veloUTID *= (double(id.veloID().channelID())/1000000.);
        ++nVelo;
        if( id.isVeloR() )      ++nVeloR;
        if( id.isVeloPhi() )    ++nVeloPhi;
        if( id.isVeloPileUp() ) ++nVeloPileUp;
      }else if( id.isTT() ){
        ttUTID *= (double(id.stID().channelID())/10000000.);
        ++nTT;
      }else if( id.isIT() ){
        itUTID *= (double(id.stID().channelID())/20000000.);
        ++nIT;
      }else if( id.isOT() ){
        otUTID *=  (double(id.otID().channelID())/50000000.);
        ++nOT;
        // -- numbers to divide by for upgrade are 'untuned'
      }else if( id.isVP() ){
        vpUTID *=  (double(id.vpID().channelID())/1000000.);
        ++nVP;
      }else if( id.isUT() ){
        utUTID *=  (double(id.stID().channelID())/10000000.);
        ++nUT;
      }else if( id.isFT() ){
        ftUTID *=  (double(id.ftID().channelID())/100000000.);
        ++nFT;
      }
    }
    // -- unique ids
    test &= tuple->column( prefix+"_TRACK_VELO_UTID", veloUTID );
    test &= tuple->column( prefix+"_TRACK_TT_UTID",   ttUTID   );
    test &= tuple->column( prefix+"_TRACK_IT_UTID",   itUTID   );
    test &= tuple->column( prefix+"_TRACK_OT_UTID",   otUTID   );
    test &= tuple->column( prefix+"_TRACK_VP_UTID",   vpUTID   );
    test &= tuple->column( prefix+"_TRACK_UT_UTID",   utUTID   );
    test &= tuple->column( prefix+"_TRACK_FT_UTID",   ftUTID   );

    // -- nHits
    test &= tuple->column( prefix+"_TRACK_nVeloHits",       nVelo       );
    test &= tuple->column( prefix+"_TRACK_nVeloRHits",      nVeloR      );
    test &= tuple->column( prefix+"_TRACK_nVeloPhiHits",    nVeloPhi    );
    test &= tuple->column( prefix+"_TRACK_nVeloPileUpHits", nVeloPileUp );
    test &= tuple->column( prefix+"_TRACK_nTTHits",         nTT         );
    test &= tuple->column( prefix+"_TRACK_nITHits",         nIT         );
    test &= tuple->column( prefix+"_TRACK_nOTHits",         nOT         );
    test &= tuple->column( prefix+"_TRACK_nVPHits",         nVP         );
    test &= tuple->column( prefix+"_TRACK_nUTHits",         nUT         );
    test &= tuple->column( prefix+"_TRACK_nFTHits",         nFT         );

    // -- history of the track (= which algorithm the track was made with)
    test &= tuple->column( prefix+"_TRACK_History", track->history() );

  }
  
  const LHCb::State* uState = track->stateAt( LHCb::State::FirstMeasurement );
  if ( m_storeFirstMeasurement || isVerbose()) {
    test &= tuple->column( prefix+"_TRACK_FirstMeasurementX"     , uState ? uState->x()      : -999 );
    test &= tuple->column( prefix+"_TRACK_FirstMeasurementY"     , uState ? uState->y()      : -999 );
    test &= tuple->column( prefix+"_TRACK_FirstMeasurementZ"     , uState ? uState->z()      : -999 );
    test &= tuple->column( prefix+"_TRACK_FirstMeasurementQOverP", uState ? uState->qOverP() : -999 );
    test &= tuple->column( prefix+"_TRACK_FirstMeasurementTx"    , uState ? uState->tx()     : -999 );
    test &= tuple->column( prefix+"_TRACK_FirstMeasurementTy"    , uState ? uState->ty()     : -999 );
  }


  test &= tuple->column( prefix+"_TRACK_MatchCHI2",
                         track->info(LHCb::Track::AdditionalInfo::FitMatchChi2,-1) );
  test &= tuple->column( prefix+"_TRACK_GhostProb",
                         track->ghostProbability() );
  test &= tuple->column( prefix+"_TRACK_CloneDist",
                         track->info(LHCb::Track::AdditionalInfo::CloneDist,-1) );
  test &= tuple->column( prefix+"_TRACK_Likelihood",
                         track->likelihood() );

  return StatusCode(test);
}
