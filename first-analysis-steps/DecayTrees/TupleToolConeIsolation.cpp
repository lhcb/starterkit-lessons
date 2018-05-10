// Include files
#include "Event/Particle.h"
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
#include "GaudiKernel/PhysicalConstants.h"
#include <gsl/gsl_sf_legendre.h>
// local
#include "TupleToolConeIsolation.h"
//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolConeIsolation
//
// 2015-02-19 : Simone Bifani, Michel De Cian
// 2016-03-10 : Adlene Hicheur
//-----------------------------------------------------------------------------
#define MAXNPIZEROS 1000
// Declaration of the Algorithm Factory
DECLARE_COMPONENT( TupleToolConeIsolation )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolConeIsolation::TupleToolConeIsolation( const std::string &type,
						const std::string &name,
						const IInterface *parent )
: TupleToolBase ( type, name, parent )
{

  declareInterface<IParticleTupleTool>( this );

  declareProperty( "MinConeSize", m_minConeSize = 0.4,
                   "Set the minimum cone size (default = 0.4)" );
  declareProperty( "MaxConeSize", m_maxConeSize = 0.6,
                   "Set the maximum cone size (default = 0.6)" );
  declareProperty( "SizeStep", m_sizeStep = 0.1,
                   "Set the cone size step between two iterations (default = 0.1)" );
  declareProperty( "MinhPi0Mass", m_MinhPi0Mass = 0*Gaudi::Units::MeV, "Set the lower seed-Pi0 invariant mass cut (default = 0 MeV)" );
  declareProperty( "MaxhPi0Mass", m_MaxhPi0Mass = 5000*Gaudi::Units::MeV, "Set the upper seed-Pi0 invariant mass cut (default = 5000 MeV)" );
  declareProperty( "FillCharged", m_fillCharged = true,
                   "Flag to fill the charged cone (default = true)" );
  declareProperty( "ExtraParticlesLocation", m_extraParticlesLocation = "Phys/StdAllNoPIDsMuons/Particles",
                   "Set the type of particles that are considered in the charged cone (default = Phys/StdAllNoPIDsMuons/Particles)" );
  declareProperty( "TrackType", m_trackType = 3,
                   "Set the type of tracks that are considered in the cone (default = 3)" );

  declareProperty( "FillNeutral", m_fillNeutral = true,
                   "Flag to fill the neutral cone (default = true)" );
  declareProperty( "ExtraPhotonsLocation", m_extraPhotonsLocation = "Phys/StdLooseAllPhotons/Particles",
                   "Set the type of particles that are considered in the neutral cone (default = Phys/StdLooseAllPhotons/Particles)" );
  declareProperty( "FillPi0Info", m_fillPi0Info = false,
                   "Flag to fill the pi0 info (default = false)" );
  declareProperty( "FillMergedPi0Info", m_fillMergedPi0Info = false,
                   "Flag to fill the merged pi0 info (default = true)" );
  declareProperty( "PizerosLocation", m_PizerosLocation = "Phys/StdLoosePi02gg/Particles", "Set the type of pi0s that are considered (default = Phys/StdLoosePi02gg/Particles)" );
  declareProperty( "MergedPizerosLocation", m_MergedPizerosLocation = "Phys/StdLooseMergedPi0/Particles", "Set the type of merged pi0s that are considered (default = Phys/StdLooseMergedPi0/Particles)" );
  declareProperty( "FillAsymmetry", m_fillAsymmetry = false,
		   "Flag to fill the asymmetry variables (default = false)" );
  declareProperty( "FillDeltas", m_fillDeltas = false,
		   "Flag to fill the delta variables (default = false)" );
  declareProperty( "FillIsolation", m_fillIsolation = true,
                   "Flag to fill the isolation variables (default = true)" );
  declareProperty( "FillMaxPt", m_fillMaxPt = true,
                   "Flag to fill the momentum of the max-pT object in the cone (default = true)" );
  declareProperty( "MaxPtParticlesLocation", m_maxPtParticleLocation = "Phys/StdAllLooseMuons/Particles",
                   "Set the type of max-pT particles that are considered in the charged cone (default = Phys/StdAllLooseMuons/Particles)" );

  declareProperty( "FillComponents", m_fillComponents = false,
                   "Flag to fill the components of all relevant variables (default = false)" );

}
//=============================================================================
// Destructor
//=============================================================================
TupleToolConeIsolation::~TupleToolConeIsolation() {}
//=============================================================================
// Initialization
//=============================================================================
StatusCode TupleToolConeIsolation::initialize() {

  StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() )
    return sc;

  if ( !m_fillCharged && !m_fillNeutral ) {
    if ( msgLevel( MSG::FATAL ) )
      fatal() << "No cone to fill" << endmsg;
    return StatusCode::FAILURE;
  }

  if ( m_minConeSize > m_maxConeSize) {
    if ( msgLevel( MSG::FATAL ) )
      fatal() << "Max conesize smaller than min conesize" << endmsg;
    return StatusCode::FAILURE;
  }

  if ( msgLevel( MSG::DEBUG ) )
    debug() << "==> Initialize" << endmsg;

  if ( isVerbose() ) {
    m_fillCharged   = true;
    m_fillNeutral   = true;
    m_fillAsymmetry = true;
    m_fillDeltas    = true;
    m_fillIsolation = true;
    m_fillMaxPt     = true;
  }

  return StatusCode::SUCCESS;

}
//=============================================================================
// Fill the tuple
//=============================================================================
StatusCode TupleToolConeIsolation::fill( const LHCb::Particle *top,
					 const LHCb::Particle *seed,
					 const std::string &head,
					 Tuples::Tuple &tuple ) {

  const std::string prefix = fullName( head );

  if ( msgLevel( MSG::DEBUG ) )
    debug() << "==> Fill" << endmsg;

  // -- The vector m_decayParticles contains all the particles that belong to the given decay  according to the decay descriptor.

  // -- Clear the vector with the particles in the specific decay
  m_decayParticles.clear();

  // -- Add the mother (prefix of the decay chain) to the vector
  if ( msgLevel( MSG::DEBUG ) )
    debug() << "Filling particle with ID " << top->particleID().pid() << endmsg;
  m_decayParticles.push_back( top );

  // -- Save all particles that belong to the given decay in the vector m_decayParticles
  saveDecayParticles( top );

  // -- Check particle containers
  const LHCb::Particles *parts = get<LHCb::Particles>( m_extraParticlesLocation );
  if ( m_fillCharged )
    if ( parts->size() == 0 )
      if ( msgLevel( MSG::WARNING ) )
	warning() << "Could not retrieve extra-particles... Skipping" << endmsg;
  const LHCb::Particles *maxPts = get<LHCb::Particles>( m_maxPtParticleLocation );
  if ( m_fillMaxPt )
    if ( maxPts->size() == 0 )
      if ( msgLevel( MSG::WARNING ) )
	warning() << "Could not retrieve maxPt-particle... Skipping" << endmsg;
  const LHCb::Particles *photons = NULL;
if(exist<LHCb::Particles>( m_extraPhotonsLocation )){
  photons = get<LHCb::Particles>( m_extraPhotonsLocation );
  if ( m_fillNeutral )
    if ( photons->size() == 0 )
      if ( msgLevel( MSG::WARNING ) )
	warning() << "Could not retrieve extra-photons... Skipping" << endmsg;
}else{

warning()<< m_extraPhotonsLocation << " Not available"<<endmsg;

}

  const LHCb::Particles *pizeros = NULL;
if(exist<LHCb::Particles>( m_PizerosLocation )) {
 pizeros = get<LHCb::Particles>( m_PizerosLocation );
  if ( m_fillPi0Info )
    if ( pizeros->size() == 0 )
      if ( msgLevel( MSG::WARNING ) )
	warning() << "Could not retrieve pi0s... Skipping" << endmsg;
}else{

warning()<< m_PizerosLocation << " Not available"<<endmsg;

}

  const LHCb::Particles *mergedpizeros = NULL;
if(exist<LHCb::Particles>( m_MergedPizerosLocation )) {
 mergedpizeros = get<LHCb::Particles>( m_MergedPizerosLocation );
  if ( m_fillMergedPi0Info )
    if ( mergedpizeros->size() == 0 )
      if ( msgLevel( MSG::WARNING ) )
	warning() << "Could not retrieve merged pi0s... Skipping" << endmsg;
}else{

warning()<< m_MergedPizerosLocation << " Not available"<<endmsg;

}


  bool test = true;

  if ( seed ) {
    if ( msgLevel( MSG::VERBOSE ) )
      verbose() << "Start looping through different conesizes" << endmsg;

    // --  Loop over the different conesizes
    double coneSize = m_minConeSize;
    while ( coneSize <= m_maxConeSize ) {

      if ( msgLevel( MSG::VERBOSE ) )
        verbose() << "Filling variables with conesize " << coneSize << endmsg;

      // -- Convert the conesize to a string
      char coneNumber[4];
      sprintf( coneNumber, "%.2f", coneSize );
      std::string conesize( coneNumber );

      // -- Charged cone
      Gaudi::XYZVector ccP;
      if ( m_fillCharged ) {
	if ( parts->size() != 0 ) {
	  int ccMultiplicity = 0;
	  double ccScalarPt = 0.;
	  std::vector<double> ccMomentum;
	  std::vector<double> ccMaxPt;

	  StatusCode ccSc = ChargedCone( seed, parts, maxPts, coneSize, ccMultiplicity, ccScalarPt, ccMomentum, ccMaxPt );
	  if ( ccSc.isFailure() )
	    ccMultiplicity = -1;

	  ccP.SetX( ccMomentum[0] );
	  ccP.SetY( ccMomentum[1] );
	  ccP.SetZ( ccMomentum[2] );

	  double ccDeltaEta = seed->momentum().Eta() - ccP.Eta();
	  double ccDeltaPhi = fabs( seed->momentum().Phi() - ccP.Phi() );
	  if ( ccDeltaPhi > M_PI )
	    ccDeltaPhi = 2 * M_PI - ccDeltaPhi;

	  // -- Fill the tuple with the variables
	  test &= tuple->column( prefix + "_" + conesize + "_cc_mult", ccMultiplicity );
	  test &= tuple->column( prefix + "_" + conesize + "_cc_sPT",  ccScalarPt );
	  test &= tuple->column( prefix + "_" + conesize + "_cc_vPT",  sqrt( ccP.Perp2() ) );

	  if ( m_fillComponents ) {
	    test &= tuple->column( prefix + "_" + conesize + "_cc_PX", ccP.X() );
	    test &= tuple->column( prefix + "_" + conesize + "_cc_PY", ccP.Y() );
	    test &= tuple->column( prefix + "_" + conesize + "_cc_PZ", ccP.Z() );
	  }

	  if ( m_fillAsymmetry ) {
	    test &= tuple->column( prefix + "_" + conesize + "_cc_asy_P",  ( seed->p()  - sqrt( ccP.Mag2() ) )  / (seed->p()  + sqrt( ccP.Mag2() ) ) );
	    test &= tuple->column( prefix + "_" + conesize + "_cc_asy_PT", ( seed->pt() - sqrt( ccP.Perp2() ) ) / (seed->pt() + sqrt( ccP.Perp2() ) ) );
	    if ( m_fillComponents ) {
	      test &= tuple->column( prefix + "_" + conesize + "_cc_asy_PX", ( seed->momentum().Px() - ccP.X() )  / (seed->momentum().Px() + ccP.X()) );
	      test &= tuple->column( prefix + "_" + conesize + "_cc_asy_PY", ( seed->momentum().Py() - ccP.Y() )  / (seed->momentum().Py() + ccP.Y()) );
	      test &= tuple->column( prefix + "_" + conesize + "_cc_asy_PZ", ( seed->momentum().Pz() - ccP.Z() )  / (seed->momentum().Pz() + ccP.Z()) );
	    }
	  }

	  if ( m_fillDeltas ) {
	    test &= tuple->column( prefix + "_" + conesize + "_cc_deltaEta", ccDeltaEta );
	    test &= tuple->column( prefix + "_" + conesize + "_cc_deltaPhi", ccDeltaPhi );
	  }

	  if ( m_fillIsolation )
	    test &= tuple->column( prefix + "_" + conesize + "_cc_IT", seed->pt() / sqrt( pow(seed->momentum().Px() + ccP.X(), 2) + pow(seed->momentum().Py() + ccP.Y(), 2) ) );

	  if ( m_fillMaxPt ) {
	    test &= tuple->column( prefix + "_" + conesize + "_cc_maxPt_Q",  (int) ccMaxPt[4] );
	    test &= tuple->column( prefix + "_" + conesize + "_cc_maxPt_PT", sqrt( pow(ccMaxPt[0], 2) + pow(ccMaxPt[1], 2) ) );
	    if ( m_fillComponents ) {
	      test &= tuple->column( prefix + "_" + conesize + "_cc_maxPt_PX", ccMaxPt[0] );
	      test &= tuple->column( prefix + "_" + conesize + "_cc_maxPt_PY", ccMaxPt[1] );
	      test &= tuple->column( prefix + "_" + conesize + "_cc_maxPt_PZ", ccMaxPt[2] );
	      test &= tuple->column( prefix + "_" + conesize + "_cc_maxPt_PE", ccMaxPt[3] );
	    }
	  }
	}
	else {
	  test &= tuple->column( prefix + "_" + conesize + "_cc_mult", 0  );
	  test &= tuple->column( prefix + "_" + conesize + "_cc_sPT",  0. );
	  test &= tuple->column( prefix + "_" + conesize + "_cc_vPT",  0. );

	  if ( m_fillComponents ) {
	    test &= tuple->column( prefix + "_" + conesize + "_cc_PX", 0. );
	    test &= tuple->column( prefix + "_" + conesize + "_cc_PY", 0. );
	    test &= tuple->column( prefix + "_" + conesize + "_cc_PZ", 0. );
	  }

	  if ( m_fillAsymmetry ) {
	    test &= tuple->column( prefix + "_" + conesize + "_cc_asy_P",  1. );
	    test &= tuple->column( prefix + "_" + conesize + "_cc_asy_PT", 1. );
	    if ( m_fillComponents ) {
	      test &= tuple->column( prefix + "_" + conesize + "_cc_asy_PX", 1. );
	      test &= tuple->column( prefix + "_" + conesize + "_cc_asy_PY", 1. );
	      test &= tuple->column( prefix + "_" + conesize + "_cc_asy_PZ", 1. );
	    }
	  }

	  if ( m_fillDeltas ) {
	    test &= tuple->column( prefix + "_" + conesize + "_cc_deltaEta", 0. );
	    test &= tuple->column( prefix + "_" + conesize + "_cc_deltaPhi", 0. );
	  }

	  if ( m_fillIsolation )
	    test &= tuple->column( prefix + "_" + conesize + "_cc_IT", 1. );

	  if ( m_fillMaxPt ) {
	    test &= tuple->column( prefix + "_" + conesize + "_cc_maxPt_Q",  0  );
	    test &= tuple->column( prefix + "_" + conesize + "_cc_maxPt_PT", 0. );
	    if ( m_fillComponents ) {
	      test &= tuple->column( prefix + "_" + conesize + "_cc_maxPt_PX", 0. );
	      test &= tuple->column( prefix + "_" + conesize + "_cc_maxPt_PY", 0. );
	      test &= tuple->column( prefix + "_" + conesize + "_cc_maxPt_PZ", 0. );
	      test &= tuple->column( prefix + "_" + conesize + "_cc_maxPt_PE", 0. );
	    }
	  }
	}
      }

      // -- Neutral cone
      Gaudi::XYZVector ncP;
      if ( m_fillNeutral ) {
	if ( photons->size() != 0 ) {
	  int ncMultiplicity = 0;
	  double ncScalarPt = 0.;
	  std::vector<double> ncMomentum;
	  std::vector<double> ncMaxPt;
          double LegH1, LegH2, LegH3, LegH4;
	  double avConfLevel, maxConfLevel;
	  StatusCode ncSc = NeutralCone( seed, photons, coneSize, ncMultiplicity, ncScalarPt, ncMomentum, ncMaxPt , avConfLevel, maxConfLevel, LegH1, LegH2, LegH3, LegH4);
	  if ( ncSc.isFailure() )
	    ncMultiplicity = -1;

	  ncP.SetX( ncMomentum[0] );
	  ncP.SetY( ncMomentum[1] );
	  ncP.SetZ( ncMomentum[2] );

	  double ncDeltaEta = seed->momentum().Eta() - ncP.Eta();
	  double ncDeltaPhi = fabs( seed->momentum().Phi() - ncP.Phi() );
	  if ( ncDeltaPhi > M_PI )
	    ncDeltaPhi = 2 * M_PI - ncDeltaPhi;

	  // -- Fill the tuple with the variables
	  test &= tuple->column( prefix + "_" + conesize + "_nc_mult", ncMultiplicity);
	  test &= tuple->column( prefix + "_" + conesize + "_nc_sPT",  ncScalarPt );
	  test &= tuple->column( prefix + "_" + conesize + "_nc_vPT",  sqrt( ncP.Perp2() ) );

	  if ( m_fillComponents ) {
	    test &= tuple->column( prefix + "_" + conesize + "_nc_PX", ncP.X() );
	    test &= tuple->column( prefix + "_" + conesize + "_nc_PY", ncP.Y() );
	    test &= tuple->column( prefix + "_" + conesize + "_nc_PZ", ncP.Z() );
	    test &= tuple->column( prefix + "_" + conesize + "_nc_avCL", avConfLevel);
	    test &= tuple->column( prefix + "_" + conesize + "_nc_maxCL", maxConfLevel);
	  }

	  if ( m_fillAsymmetry ) {
	    test &= tuple->column( prefix + "_" + conesize + "_nc_asy_P",  ( seed->p()  - sqrt( ncP.Mag2() ) )  / (seed->p()  + sqrt( ncP.Mag2() ) ) );
	    test &= tuple->column( prefix + "_" + conesize + "_nc_asy_PT", ( seed->pt() - sqrt( ncP.Perp2() ) ) / (seed->pt() + sqrt( ncP.Perp2() ) ) );
	    if ( m_fillComponents ) {
	      test &= tuple->column( prefix + "_" + conesize + "_nc_asy_PX", ( seed->momentum().Px() - ncP.X() )  / (seed->momentum().Px() + ncP.X()) );
	      test &= tuple->column( prefix + "_" + conesize + "_nc_asy_PY", ( seed->momentum().Py() - ncP.Y() )  / (seed->momentum().Py() + ncP.Y()) );
	      test &= tuple->column( prefix + "_" + conesize + "_nc_asy_PZ", ( seed->momentum().Pz() - ncP.Z() )  / (seed->momentum().Pz() + ncP.Z()) );
	    }
	  }

	  if ( m_fillDeltas ) {
	    test &= tuple->column( prefix + "_" + conesize + "_nc_deltaEta", ncDeltaEta );
	    test &= tuple->column( prefix + "_" + conesize + "_nc_deltaPhi", ncDeltaPhi );
	  }

	  if ( m_fillIsolation )
	    test &= tuple->column( prefix + "_" + conesize + "_nc_IT", seed->pt() / sqrt( pow(seed->momentum().Px() + ncP.X(), 2) + pow(seed->momentum().Py() + ncP.Y(), 2) ) );

	  if ( m_fillMaxPt ) {
	    test &= tuple->column( prefix + "_" + conesize + "_nc_maxPt_PT", sqrt( pow(ncMaxPt[0], 2) + pow(ncMaxPt[1], 2) ) );
	    if ( m_fillComponents ) {
	      test &= tuple->column( prefix + "_" + conesize + "_nc_maxPt_PX", ncMaxPt[0] );
	      test &= tuple->column( prefix + "_" + conesize + "_nc_maxPt_PY", ncMaxPt[1] );
	      test &= tuple->column( prefix + "_" + conesize + "_nc_maxPt_PZ", ncMaxPt[2] );
              test &= tuple->column(prefix + "_" + conesize + "_nc_H1",LegH1);
              test &= tuple->column(prefix + "_" + conesize + "_nc_H2",LegH2);
              test &= tuple->column(prefix + "_" + conesize + "_nc_H3",LegH3);
              test &= tuple->column(prefix + "_" + conesize + "_nc_H4",LegH4);
	      //      test &= tuple->column(prefix + "_" + conesize + "_nc_Mass",TheMass);
	    }
	  }
	}
	else {
	  test &= tuple->column( prefix + "_" + conesize + "_nc_mult", 0  );
	  test &= tuple->column( prefix + "_" + conesize + "_nc_sPT",  0. );
	  test &= tuple->column( prefix + "_" + conesize + "_nc_vPT",  0. );

	  if ( m_fillComponents ) {
	    test &= tuple->column( prefix + "_" + conesize + "_nc_PX", 0. );
	    test &= tuple->column( prefix + "_" + conesize + "_nc_PY", 0. );
	    test &= tuple->column( prefix + "_" + conesize + "_nc_PZ", 0. );
	    test &= tuple->column( prefix + "_" + conesize + "_nc_avCL", 0.);
	    test &= tuple->column( prefix + "_" + conesize + "_nc_maxCL", 0.);
	  }

	  if ( m_fillAsymmetry ) {
	    test &= tuple->column( prefix + "_" + conesize + "_nc_asy_P",  1. );
	    test &= tuple->column( prefix + "_" + conesize + "_nc_asy_PT", 1. );
	    if ( m_fillComponents ) {
	      test &= tuple->column( prefix + "_" + conesize + "_nc_asy_PX", 1. );
	      test &= tuple->column( prefix + "_" + conesize + "_nc_asy_PY", 1. );
	      test &= tuple->column( prefix + "_" + conesize + "_nc_asy_PZ", 1. );
	    }
	  }

	  if ( m_fillDeltas ) {
	    test &= tuple->column( prefix + "_" + conesize + "_nc_deltaEta", 0. );
	    test &= tuple->column( prefix + "_" + conesize + "_nc_deltaPhi", 0. );
	  }

	  if ( m_fillIsolation )
	    test &= tuple->column( prefix + "_" + conesize + "_nc_IT", 1. );

	  if ( m_fillMaxPt ) {
	    test &= tuple->column( prefix + "_" + conesize + "_nc_maxPt_PT", 0. );
	    if ( m_fillComponents ) {
	      test &= tuple->column( prefix + "_" + conesize + "_nc_maxPt_PX", 0. );
	      test &= tuple->column( prefix + "_" + conesize + "_nc_maxPt_PY", 0. );
	      test &= tuple->column( prefix + "_" + conesize + "_nc_maxPt_PZ", 0. );
              test &= tuple->column(prefix + "_" + conesize + "_nc_H1",-1.);
              test &= tuple->column(prefix + "_" + conesize + "_nc_H2",-1.);
              test &= tuple->column(prefix + "_" + conesize + "_nc_H3",-1.);
              test &= tuple->column(prefix + "_" + conesize + "_nc_H4",-1.);
	      //     test &= tuple->column(prefix + "_" + conesize + "_nc_Mass",-1.);
	    }
	  }
	}
      }

      if ( m_fillIsolation ) {
	if ( m_fillCharged && m_fillNeutral ) {
	  if ( ( parts->size() != 0 ) && ( photons->size() != 0 ) )
	    test &= tuple->column( prefix + "_" + conesize + "_IT", seed->momentum().Pt() / sqrt( pow(seed->momentum().Px() + ccP.X() + ncP.X(), 2) + pow(seed->momentum().Py() + ccP.Y() + ncP.Y(), 2) ) );
	  else
	    test &= tuple->column( prefix + "_" + conesize + "_IT", 1. );
	}
      }

      // -- Increase the counter with the stepsize
      coneSize += m_sizeStep;
    }

      if ( m_fillPi0Info ) {
	if ( pizeros->size() != 0 ) {
	  int Pi0Multiplicity = 0;
	  std::vector<double> MassWithPi0,Pi0MomX,Pi0MomY,Pi0MomZ,Pi0MomE,Pi0MomM,Pi0DeltaR,Pi0Ph1CL,Pi0Ph2CL;
	  StatusCode pizSc = pi0Comb( seed, pizeros, Pi0Multiplicity,MassWithPi0,Pi0MomX,Pi0MomY,Pi0MomZ,Pi0MomE,Pi0MomM,Pi0DeltaR,Pi0Ph1CL,Pi0Ph2CL);
	  // -- Fill the tuple with the variables
	  test &= tuple->column( prefix + "_pi0_mult", Pi0Multiplicity );
	  test &= tuple->farray( prefix+"_MasshPi0", MassWithPi0.begin(), MassWithPi0.end(), prefix+"_Pi0_ArrayLength", MAXNPIZEROS);
	  test &= tuple->farray( prefix+"_Pi0_PX", Pi0MomX.begin(), Pi0MomX.end(), prefix+"_Pi0_ArrayLength", MAXNPIZEROS);
	  test &= tuple->farray( prefix+"_Pi0_PY", Pi0MomY.begin(), Pi0MomY.end(), prefix+"_Pi0_ArrayLength", MAXNPIZEROS);
	  test &= tuple->farray( prefix+"_Pi0_PZ", Pi0MomZ.begin(), Pi0MomZ.end(), prefix+"_Pi0_ArrayLength", MAXNPIZEROS);
	  test &= tuple->farray( prefix+"_Pi0_E", Pi0MomE.begin(), Pi0MomE.end(), prefix+"_Pi0_ArrayLength", MAXNPIZEROS);
	  test &= tuple->farray( prefix+"_Pi0_M", Pi0MomM.begin(), Pi0MomM.end(), prefix+"_Pi0_ArrayLength", MAXNPIZEROS);
	  test &= tuple->farray( prefix+"_Pi0_DeltaR", Pi0DeltaR.begin(), Pi0DeltaR.end(), prefix+"_Pi0_ArrayLength", MAXNPIZEROS);
	  test &= tuple->farray( prefix+"_Pi0Ph1_CL", Pi0Ph1CL.begin(), Pi0Ph1CL.end(), prefix+"_Pi0_ArrayLength", MAXNPIZEROS);
	  test &= tuple->farray( prefix+"_Pi0Ph2_CL", Pi0Ph2CL.begin(), Pi0Ph2CL.end(), prefix+"_Pi0_ArrayLength", MAXNPIZEROS);
	} else {
	  std::vector<double> MassWithPi0 = {-1};
	  std::vector<double> Pi0MomX  = {-1e6};
	  std::vector<double> Pi0MomY  = {-1e6};
	  std::vector<double> Pi0MomZ  = {-1e6};
	  std::vector<double> Pi0MomE  = {-1e6};
	  std::vector<double> Pi0MomM  = {-1e6};
	  std::vector<double> Pi0DeltaR  = {-1e6};
	  std::vector<double> Pi0Ph1CL = {-1e6};
	  std::vector<double> Pi0Ph2CL = {-1e6};

	  test &= tuple->column( prefix + "_pi0_mult",0);
	  test &= tuple->farray( prefix+"_MasshPi0",MassWithPi0.begin(), MassWithPi0.end(), prefix+"_Pi0_ArrayLength",0);
	  test &= tuple->farray( prefix+"_Pi0_PX", Pi0MomX.begin(), Pi0MomX.end(), prefix+"_Pi0_ArrayLength", 0);
	  test &= tuple->farray( prefix+"_Pi0_PY", Pi0MomY.begin(), Pi0MomY.end(), prefix+"_Pi0_ArrayLength", 0);
	  test &= tuple->farray( prefix+"_Pi0_PZ", Pi0MomZ.begin(), Pi0MomZ.end(), prefix+"_Pi0_ArrayLength", 0);
	  test &= tuple->farray( prefix+"_Pi0_E", Pi0MomE.begin(), Pi0MomE.end(), prefix+"_Pi0_ArrayLength", 0);
	  test &= tuple->farray( prefix+"_Pi0_M", Pi0MomM.begin(), Pi0MomM.end(), prefix+"_Pi0_ArrayLength", 0);
	  test &= tuple->farray( prefix+"_Pi0_DeltaR", Pi0MomE.begin(), Pi0MomE.end(), prefix+"_Pi0_ArrayLength", 0);
	  test &= tuple->farray( prefix+"_Pi0Ph1_CL", Pi0Ph1CL.begin(), Pi0Ph1CL.end(), prefix+"_Pi0_ArrayLength", 0);
	  test &= tuple->farray( prefix+"_Pi0Ph2_CL", Pi0Ph2CL.begin(), Pi0Ph2CL.end(), prefix+"_Pi0_ArrayLength", 0);
	}
      }

      if ( m_fillMergedPi0Info ) {
	if ( mergedpizeros->size() != 0 ) {
	  //	  warning() << "Non-empty merged pi0 list" << endmsg;
	  int MergPi0Multiplicity = 0;

	  std::vector<double> MassWithMergPi0,Pi0MomX,Pi0MomY,Pi0MomZ,Pi0MomE,Pi0MomM,Pi0DeltaR,Pi0Ph1CL,Pi0Ph2CL;
	  StatusCode mpizSc = pi0Comb( seed, mergedpizeros, MergPi0Multiplicity,MassWithMergPi0,Pi0MomX,Pi0MomY,Pi0MomZ,Pi0MomE,Pi0MomM,Pi0DeltaR,Pi0Ph1CL,Pi0Ph2CL);
	  // -- Fill the tuple with the variables
	  test &= tuple->column( prefix + "_mergpi0_mult", MergPi0Multiplicity );
	  test &= tuple->farray( prefix+"_MasshMergPi0", MassWithMergPi0.begin(), MassWithMergPi0.end(), prefix+"_MergPi0_ArrayLength", MAXNPIZEROS);
	  test &= tuple->farray( prefix+"_MergPi0_PX", Pi0MomX.begin(), Pi0MomX.end(), prefix+"_MergPi0_ArrayLength", MAXNPIZEROS);
	  test &= tuple->farray( prefix+"_MergPi0_PY", Pi0MomY.begin(), Pi0MomY.end(), prefix+"_MergPi0_ArrayLength", MAXNPIZEROS);
	  test &= tuple->farray( prefix+"_MergPi0_PZ", Pi0MomZ.begin(), Pi0MomZ.end(), prefix+"_MergPi0_ArrayLength", MAXNPIZEROS);
	  test &= tuple->farray( prefix+"_MergPi0_E", Pi0MomE.begin(), Pi0MomE.end(), prefix+"_MergPi0_ArrayLength", MAXNPIZEROS);
	  test &= tuple->farray( prefix+"_MergPi0_M", Pi0MomM.begin(), Pi0MomM.end(), prefix+"_MergPi0_ArrayLength", MAXNPIZEROS);
	  test &= tuple->farray( prefix+"_MergPi0_DeltaR", Pi0DeltaR.begin(), Pi0DeltaR.end(), prefix+"_MergPi0_ArrayLength", MAXNPIZEROS);
	  test &= tuple->farray( prefix+"_MergPi0Ph1_CL", Pi0Ph1CL.begin(), Pi0Ph1CL.end(), prefix+"_MergPi0_ArrayLength", MAXNPIZEROS);
	  test &= tuple->farray( prefix+"_MergPi0Ph2_CL", Pi0Ph2CL.begin(), Pi0Ph2CL.end(), prefix+"_MergPi0_ArrayLength", MAXNPIZEROS);
	} else {
	  std::vector<double> MassWithMergPi0 = {-1};
	  std::vector<double> Pi0MomX  = {-1e6};
	  std::vector<double> Pi0MomY  = {-1e6};
	  std::vector<double> Pi0MomZ  = {-1e6};
	  std::vector<double> Pi0MomE  = {-1e6};
	  std::vector<double> Pi0MomM  = {-1e6};
	  std::vector<double> Pi0DeltaR  = {-1e6};
	  std::vector<double> Pi0Ph1CL = {-1e6};
	  std::vector<double> Pi0Ph2CL = {-1e6};

	  test &= tuple->column( prefix + "_mergpi0_mult",0);
	  test &= tuple->farray( prefix+"_MasshMergPi0",MassWithMergPi0.begin(), MassWithMergPi0.end(), prefix+"_MergPi0_ArrayLength",0);
	  test &= tuple->farray( prefix+"_MergPi0_PX", Pi0MomX.begin(), Pi0MomX.end(), prefix+"_MergPi0_ArrayLength", 0);
	  test &= tuple->farray( prefix+"_MergPi0_PY", Pi0MomY.begin(), Pi0MomY.end(), prefix+"_MergPi0_ArrayLength", 0);
	  test &= tuple->farray( prefix+"_MergPi0_PZ", Pi0MomZ.begin(), Pi0MomZ.end(), prefix+"_MergPi0_ArrayLength", 0);
	  test &= tuple->farray( prefix+"_MergPi0_E", Pi0MomE.begin(), Pi0MomE.end(), prefix+"_MergPi0_ArrayLength", 0);
	  test &= tuple->farray( prefix+"_MergPi0_M", Pi0MomM.begin(), Pi0MomM.end(), prefix+"_MergPi0_ArrayLength", 0);
	  test &= tuple->farray( prefix+"_MergPi0_DeltaR", Pi0MomE.begin(), Pi0MomE.end(), prefix+"_MergPi0_ArrayLength", 0);
	  test &= tuple->farray( prefix+"_MergPi0Ph1_CL", Pi0Ph1CL.begin(), Pi0Ph1CL.end(), prefix+"_MergPi0_ArrayLength", 0);
	  test &= tuple->farray( prefix+"_MergPi0Ph2_CL", Pi0Ph2CL.begin(), Pi0Ph2CL.end(), prefix+"_MergPi0_ArrayLength", 0);
	}
      }


  }
  else {
    if ( msgLevel( MSG::WARNING ) )
      warning() << "The seed particle is not valid. Skipping" << endmsg;

    return StatusCode::FAILURE;
  }

  if ( msgLevel( MSG::VERBOSE ) )
    verbose() << "Stop looping through different conesizes" << endmsg;

  return StatusCode( test );

}
//=============================================================================
// Save the particles in the decay chain (recursive function)
//=============================================================================
void TupleToolConeIsolation::saveDecayParticles( const LHCb::Particle *top ) {

  // -- Get the daughters of the top particle
  const SmartRefVector< LHCb::Particle > daughters = top->daughters();

  // -- Fill all the daugthers in m_decayParticles
  for ( SmartRefVector< LHCb::Particle >::const_iterator ip = daughters.begin(); ip != daughters.end(); ++ip ) {

    // -- If the particle is stable, save it in the vector, or...
    if ( (*ip)->isBasicParticle() ) {
      if ( msgLevel(MSG::DEBUG) )
        debug() << "Filling particle with ID " << (*ip)->particleID().pid() << endmsg;
      m_decayParticles.push_back( (*ip) );
    }
    else {
      // -- if it is not stable, call the function recursively
      m_decayParticles.push_back( (*ip) );
      if ( msgLevel(MSG::DEBUG) )
        debug() << "Filling particle with ID " << (*ip)->particleID().pid() << endmsg;
      saveDecayParticles( (*ip) );
    }
  }

  return;

}
//=============================================================================
// Loop over all the tracks in the cone which do not belong to the desired decay
//=============================================================================
StatusCode TupleToolConeIsolation::ChargedCone( const LHCb::Particle  *seed,
						const LHCb::Particles *parts,
						const LHCb::Particles *maxPts,
						const double rcut,
						int &multiplicity,
						double &scalarPt,
						std::vector<double> &momentum,
						std::vector<double> &maxPt ) {

  // -- Initialize values
  multiplicity = 0;
  scalarPt = 0;
  double conePx  = 0;
  double conePy  = 0;
  double conePz  = 0;
  double maxPtPx = 0;
  double maxPtPy = 0;
  double maxPtPz = 0;
  double maxPtPe = 0;
  double maxPtQ  = 0;

  // -- Get the momentum of the seed particle
  Gaudi::XYZVector seedMomentum = seed->momentum().Vect();

  for ( LHCb::Particles::const_iterator ip = parts->begin(); ip != parts->end(); ++ip ) {
    const LHCb::Particle *particle = (*ip);

    const LHCb::ProtoParticle *proto = particle->proto();
    if ( proto ) {

      const LHCb::Track *track = proto->track();
      if ( track ) {

        // -- Check if the track belongs to the decay itself
        bool isInDecay = isTrackInDecay( track );
        if ( isInDecay )
          continue;

        Gaudi::XYZVector trackMomentum = track->momentum();

        // -- Calculate the difference in Eta and Phi between the seed particle and a track
        double deltaPhi = fabs( seedMomentum.Phi() - trackMomentum.Phi() );
        if ( deltaPhi > M_PI )
          deltaPhi = 2 * M_PI - deltaPhi;
        double deltaEta = seedMomentum.Eta() - trackMomentum.Eta();
        double deltaR   = sqrt( pow(deltaPhi, 2) + pow(deltaEta, 2) );

	if ( ( rcut == 0 ) || ( deltaR < rcut ) ) {
	  if ( track->type() == m_trackType ) {

            multiplicity++;

            // -- Calculate scalar information
            scalarPt += track->pt();

            // -- Calculate vector information
            conePx += trackMomentum.X();
            conePy += trackMomentum.Y();
            conePz += trackMomentum.Z();

	    // -- Find max-pT particle
	    if ( maxPts->size() != 0 ) {
	      if ( track->pt() > sqrt( pow(maxPtPx, 2) + pow(maxPtPy, 2) ) ) {

		for ( LHCb::Particles::const_iterator iPt = maxPts->begin(); iPt != maxPts->end(); ++iPt ) {
		  const LHCb::Particle *particlePt = (*iPt);

		  const LHCb::ProtoParticle *protoPt = particlePt->proto();
		  if ( protoPt ) {

		    if ( protoPt == proto ) {

		      const LHCb::Track *trackPt = protoPt->track();
		      if ( trackPt ) {
			maxPtPx = trackPt->momentum().X();
			maxPtPy = trackPt->momentum().Y();
			maxPtPz = trackPt->momentum().Z();
			maxPtPe = particlePt->momentum().E();
			maxPtQ  = trackPt->charge();
		      }

		      break;
		    }
		  }
		}
	      }
	    }
	    else {
	      maxPtPx = 0.;
	      maxPtPy = 0.;
	      maxPtPz = 0.;
	      maxPtPe = 0.;
	      maxPtQ  = 0;
	    }
	  }
	}
      }
    }
  }

  momentum.push_back( conePx );
  momentum.push_back( conePy );
  momentum.push_back( conePz );

  maxPt.push_back( maxPtPx );
  maxPt.push_back( maxPtPy );
  maxPt.push_back( maxPtPz );
  maxPt.push_back( maxPtPe );
  maxPt.push_back( maxPtQ );

  return StatusCode::SUCCESS;

}
//=============================================================================
// Loop over all the photons in the cone
//=============================================================================
StatusCode TupleToolConeIsolation::NeutralCone( const LHCb::Particle  *seed,
						const LHCb::Particles *photons,
						const double rcut,
						int &multiplicity,
						double &scalarPt,
						std::vector<double> &momentum,
						std::vector<double> &maxPt,
						double& AvConfLevel,
						double& MaxConfLevel,
                                                double &LegendreRat1,
                                                double &LegendreRat2,
                                                double &LegendreRat3,
                                                double &LegendreRat4) {

  // -- Initialize values
  multiplicity = 0;
  scalarPt = 0;
  double conePx  = 0;
  double conePy  = 0;
  double conePz  = 0;
  double maxPtPx = 0;
  double maxPtPy = 0;
  double maxPtPz = 0;
  double numLeg1 = 0;
  double numLeg2 = 0;
  double numLeg3 = 0;
  double numLeg4 = 0;
  double coneE   = 0;
  double maxP    = 0;
  double maxCL = 0;
  double avCL = 0;

  // -- Get the momentum of the seed particle
  Gaudi::XYZVector seedMomentum = seed->momentum().Vect();


  for ( LHCb::Particles::const_iterator ip = photons->begin(); ip != photons->end(); ++ip ) {
    const LHCb::Particle *photon = (*ip);

    double photCL = photon->confLevel();

    // -- Get the momentum of the photon
    Gaudi::XYZVector photonMomentum = photon->momentum().Vect();
    // -- Calculate the difference in Eta and Phi between the seed particle and a photons
    double deltaPhi = fabs( seedMomentum.Phi() - photonMomentum.Phi() );
    if ( deltaPhi > M_PI )
      deltaPhi = 2 * M_PI - deltaPhi;
    double deltaEta = seedMomentum.Eta() - photonMomentum.Eta();
    double deltaR   = sqrt( pow(deltaPhi, 2) + pow(deltaEta, 2) );

    if ( ( rcut == 0 ) || ( deltaR < rcut ) ) {
      multiplicity++;

      // -- Calculate scalar information
      scalarPt += sqrt( photonMomentum.Perp2() );

      // -- Calculate vector information
      conePx += photonMomentum.X();
      conePy += photonMomentum.Y();
      conePz += photonMomentum.Z();

      //Calculate sum for Legendre ratio
      Double_t cosAng = (seedMomentum.unit()).Dot(photonMomentum.unit());
      if(cosAng > +1.0)
      {
        cosAng = +1.0;
        counter("Forced cos > 1 to 1")++;
      }
      if(cosAng < -1.0)
      {
        cosAng = -1.0;
        counter("Forced cos < -1 to -1")++;
      }

      coneE += std::sqrt(photonMomentum.Mag2());
      numLeg1 += std::sqrt(photonMomentum.Mag2())*gsl_sf_legendre_Pl(1,cosAng);
      numLeg2 += std::sqrt(photonMomentum.Mag2())*gsl_sf_legendre_Pl(2,cosAng);
      numLeg3 += std::sqrt(photonMomentum.Mag2())*gsl_sf_legendre_Pl(3,cosAng);
      numLeg4 += std::sqrt(photonMomentum.Mag2())*gsl_sf_legendre_Pl(4,cosAng);
      // Extra Photon
      if ( sqrt( photonMomentum.Perp2() ) > sqrt( pow(maxPtPx, 2) + pow(maxPtPy, 2) ) ) {
	maxPtPx = photonMomentum.X();
	maxPtPy = photonMomentum.Y();
	maxPtPz = photonMomentum.Z();
      }

      //average CL and search for max CL
      avCL += photCL;
      if(maxCL<photCL) maxCL = photCL;

    }
      if(std::sqrt(photonMomentum.Mag2())>maxP) {
        maxP = std::sqrt(photonMomentum.Mag2());
      }
  }

  momentum.push_back( conePx );
  momentum.push_back( conePy );
  momentum.push_back( conePz );

  maxPt.push_back( maxPtPx );
  maxPt.push_back( maxPtPy );
  maxPt.push_back( maxPtPz );

  AvConfLevel = avCL/multiplicity;
  MaxConfLevel = maxCL;

  LegendreRat1 = numLeg1/coneE;
  LegendreRat2 = numLeg2/coneE;
  LegendreRat3 = numLeg3/coneE;
  LegendreRat4 = numLeg4/coneE;
  return StatusCode::SUCCESS;

}
//=============================================================================
// Check if the track is already in the decay
//=============================================================================
bool TupleToolConeIsolation::isTrackInDecay( const LHCb::Track *track ) {

  bool isInDecay = false;

  for ( std::vector<const LHCb::Particle*>::iterator ip = m_decayParticles.begin(); ip != m_decayParticles.end(); ++ip ) {
    const LHCb::ProtoParticle *proto = (*ip)->proto();
    if ( proto ) {

      const LHCb::Track *myTrack = proto->track();
      if ( myTrack ) {

        if ( myTrack == track ) {
          if ( msgLevel( MSG::DEBUG ) )
            debug() << "Track is in decay, skipping it" << endmsg;
          isInDecay = true;
        }

      }
    }
  }

  return isInDecay;

}


//=============================================================================
// Loop over all the pi0 in the cone
//=============================================================================
StatusCode TupleToolConeIsolation::pi0Comb( const LHCb::Particle  *seed,
						const LHCb::Particles *pi0s,
                                                int &multiplicity,
					    std::vector<double> &InvMass,
					    std::vector<double> &Pi0PX,
					    std::vector<double> &Pi0PY,
					    std::vector<double> &Pi0PZ,
                                            std::vector<double> &Pi0E,
                                            std::vector<double> &Pi0M,
                                            std::vector<double> &Pi0DeltaR,
                                            std::vector<double> &Pi0Photon1CL,
                                            std::vector<double> &Pi0Photon2CL) {

  // -- Initialize values

  // -- Get the momentum of the seed particle
  Gaudi::XYZVector seedMomentum = seed->momentum().Vect();
  Gaudi::LorentzVector seed4Momentum = seed->momentum();

  for ( LHCb::Particles::const_iterator ip = pi0s->begin(); ip != pi0s->end(); ++ip ) {
    const LHCb::Particle *pion0 = (*ip);

    // -- Get the momentum of the pion0
    Gaudi::XYZVector pion0Momentum = pion0->momentum().Vect();
    Gaudi::LorentzVector pion04Momentum = pion0->momentum();
    double masshPi0 = (pion04Momentum+seed4Momentum).M();
    if(masshPi0 < m_MaxhPi0Mass && masshPi0 > m_MinhPi0Mass) {
      multiplicity++;
      InvMass.push_back(masshPi0);
      Pi0PX.push_back(pion0Momentum.X());
      Pi0PY.push_back(pion0Momentum.Y());
      Pi0PZ.push_back(pion0Momentum.Z());
      Pi0E.push_back(pion04Momentum.E());
      Pi0M.push_back(pion04Momentum.M());


      // -- Calculate the difference in Eta and Phi between the seed particle and the pi0
      double deltaPhi = fabs( seedMomentum.Phi() - pion0Momentum.Phi() );
      if ( deltaPhi > M_PI )
	deltaPhi = 2 * M_PI - deltaPhi;
      double deltaEta = seedMomentum.Eta() - pion0Momentum.Eta();
      double deltaR   = sqrt( pow(deltaPhi, 2) + pow(deltaEta, 2) );
      Pi0DeltaR.push_back(deltaR);

      // Get photon info:
      if ( !(pion0->isBasicParticle()) ) {

	const SmartRefVector< LHCb::Particle > photons = pion0->daughters();

	// -- Fill all the daugthers in m_decayParticles
	Int_t nph = 0;

	for ( SmartRefVector< LHCb::Particle >::const_iterator iph = photons.begin(); iph != photons.end(); ++iph ) {
	  double photonCL = (*iph)->confLevel();
	  if(nph==0) Pi0Photon1CL.push_back(photonCL);
	  if(nph==1) Pi0Photon2CL.push_back(photonCL);
	  nph++;
	}

      } else {
	Pi0Photon1CL.push_back(-999);
	Pi0Photon2CL.push_back(-999);
      }

    }

  }


  return StatusCode::SUCCESS;

}
