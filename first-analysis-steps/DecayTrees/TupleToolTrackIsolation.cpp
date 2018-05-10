#include "Event/Particle.h"

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
#include "GaudiKernel/PhysicalConstants.h"

// local
#include "TupleToolTrackIsolation.h"

//-----------------------------------------------------------------------------
// Implementation file for class : TupleToolTrackIsolation
//
// 2009-05-06 : Michel De Cian
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( TupleToolTrackIsolation )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  TupleToolTrackIsolation::TupleToolTrackIsolation( const std::string& type,
                                                    const std::string& name,
                                                    const IInterface* parent )
    : TupleToolBase ( type, name , parent )
{
  declareInterface<IParticleTupleTool>(this);
  declareProperty( "MinConeAngle", m_minConeAngle = 0.5,
                   "Set the minimal deltaR of the cone (default = 0.5), in radians");
  declareProperty( "MaxConeAngle", m_maxConeAngle = 1.0,
                   "Set the maximum deltaR of the cone (default = 1.0), in radians");
  declareProperty( "StepSize", m_stepSize = 0.1,
                   "Set the step of deltaR between two iterations (default = 0.1), in radians");
  declareProperty( "TrackType", m_trackType = 3,
                   "Set the type of tracks which are considered inside the cone (default = 3)");
  declareProperty( "FillAsymmetry", m_fillAsymmetry = false,
                   "Flag to fill the asymmetry variables (default = false)");
  declareProperty( "FillDeltaAngles", m_fillDeltaAngles = false,
                   "Flag to fill the delta angle variables (default = false)");
}

//=============================================================================
// Destructor
//=============================================================================
TupleToolTrackIsolation::~TupleToolTrackIsolation() {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode TupleToolTrackIsolation::initialize()
{
  StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  if ( m_minConeAngle > m_maxConeAngle )
  {
    return Error( "Max conesize smaller than min conesize." );
  }

  if ( isVerbose() ) { m_fillAsymmetry = m_fillDeltaAngles = true; }

  return sc;
}

//=============================================================================
// Fill the tuple
//=============================================================================
StatusCode TupleToolTrackIsolation::fill( const LHCb::Particle *top,
                                          const LHCb::Particle *part,
                                          const std::string    &head,
                                          Tuples::Tuple        & tuple )
{
  bool test = true;

  if ( part )
  {

    const std::string prefix = fullName(head);
    if ( msgLevel(MSG::DEBUG) ) debug() << "==> Fill" << endmsg;

    // -- The vector m_decayParticles contains all the particles that belong to the given decay
    // -- according to the decay descriptor.

    // -- Clear the vector with the particles in the specific decay
    m_decayParticles.clear();

    // -- Add the mother (prefix of the decay chain) to the vector
    if ( msgLevel(MSG::DEBUG) ) debug() << "Filling particle with ID " << top->particleID().pid() << endmsg;
    m_decayParticles.push_back( top );

    // -- Save all particles that belong to the given decay in the vector m_decayParticles
    saveDecayParticles( top );

    // -- Get all tracks in the event
    LHCb::Tracks* tracks = get<LHCb::Tracks>(LHCb::TrackLocation::Default);
    if ( tracks->empty() )
    {
      return Warning( "Could not retrieve tracks. Skipping" );
    }

    if ( msgLevel(MSG::VERBOSE) ) verbose() << "Start looping through different conesizes" << endmsg;

    // --  Loop over the different conesizes
    double coneSizeCounter = m_minConeAngle;
    while ( coneSizeCounter <= m_maxConeAngle )
    {

      if ( msgLevel(MSG::VERBOSE) ) verbose() << "Filling variables with conesize " << coneSizeCounter << endmsg;

      // -- Retrieve momentum information of tracks in the cone
      std::pair<std::vector<double>, int> myPair = ConeP(part, tracks, coneSizeCounter);
      const std::vector<double> & myVector = myPair.first;

      const double conePx = myVector[0];
      const double conePy = myVector[1];
      const double conePz = myVector[2];

      const double conePx2 = conePx*conePx;
      const double conePy2 = conePy*conePy;

      const double coneP = std::sqrt( conePx2 + conePy2 + conePz*conePz );
      const double conePt = std::sqrt( conePx2 + conePy2 );

      // -- Create a vector with the summed momentum of all tracks in the cone
      Gaudi::XYZVector momentumInCone;
      momentumInCone.SetX(conePx);
      momentumInCone.SetY(conePy);
      momentumInCone.SetZ(conePz);

      // -- Calculate the difference in Eta and Phi between the summed momentum of all tracks in the cone and the
      // -- track of the particle in question
      const double deltaEta = part->momentum().Eta() - momentumInCone.Eta();
      double deltaPhi = fabs(part->momentum().Phi() - momentumInCone.Phi());
      if(deltaPhi > M_PI) deltaPhi  = 2*M_PI-deltaPhi;

      // -- Convert the conesize to a string
      char coneNumber[4];
      sprintf(coneNumber, "%.2f", coneSizeCounter);
      std::string conesize(coneNumber);

      // -- Fill the tuple with the variables
      test &= tuple->column( prefix+"_cpx_"+conesize, conePx);
      test &= tuple->column( prefix+"_cpy_"+conesize, conePy);
      test &= tuple->column( prefix+"_cpz_"+conesize, conePz);
      test &= tuple->column( prefix+"_cpt_"+conesize, conePt );
      test &= tuple->column( prefix+"_cp_"+conesize,  coneP );
      test &= tuple->column( prefix+"_cmult_"+conesize, myPair.second);

      // -- Fill the difference in Eta and Phi between the summed momentum of all tracks in the cone and the
      // -- track of the particle in question if requested
      if(m_fillDeltaAngles)
      {
        test &= tuple->column( prefix+"_deltaEta_"+conesize, deltaEta );
        test &= tuple->column( prefix+"_deltaPhi_"+conesize, deltaPhi );
      }

      // -- Fill the asymmetry information if requested
      if(m_fillAsymmetry)
      {
        test &= tuple->column( prefix+"_pxasy_"+conesize,
                               (part->momentum().Px() - conePx)/(part->momentum().Px() + conePx) );
        test &= tuple->column( prefix+"_pyasy_"+conesize,
                               (part->momentum().Py() - conePy)/(part->momentum().Py() + conePy) );
        test &= tuple->column( prefix+"_pzasy_"+conesize,
                               (part->momentum().Pz() - conePz)/(part->momentum().Pz() + conePz) );
        test &= tuple->column( prefix+"_pasy_"+conesize,
                               (part->p() - coneP)/(part->p() + coneP) );
        test &= tuple->column( prefix+"_ptasy_"+conesize,
                               (part->pt() - conePt)/(part->pt() + conePt) );
      }

      // -- Increase the counter with the stepsize
      coneSizeCounter += m_stepSize;

    }

    if ( msgLevel(MSG::VERBOSE) ) verbose() << "Stop looping through different conesizes" << endmsg;

  }
  else
  {
    return Warning( "The particle in question is not valid" );
  }

  return StatusCode(test);
}

//=============================================================================
// Save the particles in the decay chain (recursive function)
//=============================================================================
void TupleToolTrackIsolation::saveDecayParticles( const LHCb::Particle *top)
{

  // -- Get the daughters of the top particle
  const SmartRefVector< LHCb::Particle > & daughters = top->daughters();

  // -- Fill all the daugthers in m_decayParticles
  for( SmartRefVector< LHCb::Particle >::const_iterator idau = daughters.begin();
       idau != daughters.end(); ++idau )
  {

    // -- If the particle is stable, save it in the vector, or...
    if( (*idau)->isBasicParticle() )
    {
      if ( msgLevel(MSG::DEBUG) ) debug() << "Filling particle with ID " << (*idau)->particleID().pid() << endmsg;
      m_decayParticles.push_back( (*idau) );
    }
    else
    {
      // -- if it is not stable, call the function recursively
      m_decayParticles.push_back( (*idau) );
      if ( msgLevel(MSG::DEBUG) ) debug() << "Filling particle with ID " << (*idau)->particleID().pid() << endmsg;
      saveDecayParticles( (*idau) );
    }

  }

}

//=============================================================================
// Loop over all the tracks in the cone which do not belong to the desired decay
//=============================================================================
std::pair< std::vector<double>, int>
TupleToolTrackIsolation::ConeP(const LHCb::Particle *part,
                               const LHCb::Tracks* tracks,
                               const double rcut)
{

  // -- Get the (4-) momentum of the particle in question
  const Gaudi::LorentzVector& partMomentum = part->momentum();
  double sumPx = 0.0;
  double sumPy = 0.0;
  double sumPz = 0.0;
  int counter  = 0;

  for( LHCb::Tracks::const_iterator it = tracks->begin(); it != tracks->end(); ++it )
  {
    LHCb::Track* track = (*it);

    // -- Check if the track belongs to the decay itself
    bool isInDecay = isTrackInDecay(track);
    if(isInDecay) continue;

    // -- Get the (3-) momentum of the track
    const Gaudi::XYZVector& trackMomentum = track->momentum();
    //double tracketa = track->pseudoRapidity();
    const double trackpx = trackMomentum.X();
    const double trackpy = trackMomentum.Y();
    const double trackpz = trackMomentum.Z();

    // -- Calculate the difference in Eta and Phi between the particle in question and a track
    double deltaPhi = fabs( partMomentum.Phi() - trackMomentum.Phi() );
    if(deltaPhi > M_PI) deltaPhi  = 2*M_PI-deltaPhi;

    const double deltaEta = partMomentum.Eta() - trackMomentum.Eta();

    const double deltaR = std::sqrt(deltaPhi * deltaPhi + deltaEta * deltaEta);

    // -- Add the tracks to the summation if deltaR is smaller than the cut value of deltaR
    if ( deltaR < rcut && track->type() == m_trackType )
    {
      sumPx += trackpx;
      sumPy += trackpy;
      sumPz += trackpz;
      ++counter;
    }


  }

  // -- Fill a vector and return it
  std::vector<double> myVector;
  myVector.push_back(sumPx);
  myVector.push_back(sumPy);
  myVector.push_back(sumPz);

  std::pair<std::vector<double>, int> myPair;
  myPair.first = myVector;
  myPair.second = counter;

  return myPair;

}

//=============================================================================
// Check if the track is already in the decay
//=============================================================================
bool TupleToolTrackIsolation::isTrackInDecay(const LHCb::Track* track){

  bool isInDecay = false;

  for(  std::vector<const LHCb::Particle*>::iterator it = m_decayParticles.begin() ; it != m_decayParticles.end() ; ++it ){

    const LHCb::ProtoParticle* proto = (*it)->proto();
    if(proto){
      const LHCb::Track* myTrack = proto->track();

      if(myTrack){

        if(myTrack == track){
          if ( msgLevel(MSG::DEBUG) ) debug() << "Track is in decay, skipping it" << endmsg;
          isInDecay = true;
        }
      }

    }

  }

  return isInDecay;
}
