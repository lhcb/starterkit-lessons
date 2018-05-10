// Include files
// local
#include "TupleToolAllTracks.h"
// from Gaudi
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
#include "GaudiKernel/Transform3DTypes.h"
// from Phys
#include "Kernel/GetIDVAlgorithm.h"
#include "Kernel/IDVAlgorithm.h"
#include "Kernel/IDistanceCalculator.h"
#include "Kernel/IVertexFit.h"
#include "Kernel/IPVReFitter.h"
#include "Kernel/IParticleDescendants.h"
// from LHCb
#include "Event/Particle.h"
//from LoKi
#include "LoKi/ParticleProperties.h"
//from boost
#include "boost/algorithm/string/replace.hpp"

DECLARE_COMPONENT( TupleToolAllTracks )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TupleToolAllTracks::TupleToolAllTracks( const std::string& type,
                                        const std::string& name,
                                        const IInterface* parent )
  : TupleToolBase ( type, name , parent )
  , m_dva(0)
  , m_dist(0)
  , m_pVertexFit(0)
  , m_particleDescendants(0)
{
  declareInterface<IParticleTupleTool>(this);

  declareProperty( "InputParticles" ,m_InputParticles = {"Phys/StdAllNoPIDsPions/Particles"}, "List of input Particles");
  declareProperty( "Target"         ,m_TargetParticle = "Charm"                             , "Particle to add Particles to");
  declareProperty( "VertexMode"     ,m_VModedummy     = 0                                   , "Decay vertex to which the particle is added");
  declareProperty( "ANNPID"         ,m_ANNPIDCut      = -1.f                                , "Cut on ProbNN of added input particle type");                                     //NB: if -1, no cut is placed
  declareProperty( "Theta"          ,m_theta          = -1.f                                , "Cut on theta angle of the added input particle");                                 //NB: if -1, no cut is placed
  declareProperty( "DeltaPhi"       ,m_deltaphi       = -1.f                                , "Cut on the difference between phi angles of target and added particle");          //NB: if -1, no cut is placed
  declareProperty( "GhostProb"      ,m_GhostProb      =  1.f                                , "Cut on the added input particle ghost probability");                              //NB: if -1, no cut is placed
  declareProperty( "DLLPID"         ,m_DLLPIDCut      = -9999.f                             , "Cut on DLL of added input particle type");                                        //NB: PIDPi == -PIDK!
  declareProperty( "NewVertexChi2"  ,m_VertexChi2Cut  = -1.f                                , "Cut on Vertex chi2/ndof with added particle");                                    //NB: if -1, no cut is placed
  declareProperty( "ImprovedVertex" ,m_improvedVertex = -1.f                                , "Cut on Vertex chi2/ndof w added particle - Vertex chi2/ndof w/o added particle"); //NB: if -1, no cut is placed
  declareProperty( "IPchi2"         ,m_ipchi2Cut      = -1.f                                , "Cut on IPchi2 of added particle w.r.t. new vertex");                              //NB: if -1, no cut is placed
  declareProperty( "PVIPchi2"       ,m_PVipchi2Cut    = -1.f                                , "Cut on IPchi2 of added particle w.r.t. closest primary vertex");                  //NB: if -1, no cut is placed
  declareProperty( "MHi"            ,m_HiMassCut      = -1.f*Gaudi::Units::MeV              , "Cut on invariant mass of Target+input particles");                                //NB: if -1, no cut is placed
  declareProperty( "AddMax"         ,m_mult           = 200                                 , "Maximal number of added particles");
  declareProperty( "WritePXPYPZ"    ,m_addpxpypz      = false                               , "Write PX, PY, PZ of added particle alone and target+particle combination");
  declareProperty( "CorrectedMass"  ,m_addcorrm       = false                               , "Write corrected mass and it's error of mother+added particle");
}

//=============================================================================

StatusCode TupleToolAllTracks::initialize() {
  if( ! TupleToolBase::initialize() ) return StatusCode::FAILURE;

  for(auto location = m_InputParticles.begin(); location != m_InputParticles.end() ; location++ ){
    info() << "Adding particles from " << *location << " to a " << m_TargetParticle << endmsg;
    if (!exist<Particle::Range>(*location)){
      if (msgLevel(MSG::WARNING)) warning() << "No particles at " << *location << "! Skipping this TES location!" << endmsg;
      m_InputParticles.erase(location);
    }
  }
  if(m_InputParticles.empty())return StatusCode::FAILURE;

  //declareProperty can't deal with enums(?)
  m_VertexMode = static_cast<VMode>(m_VModedummy);

  if(m_addcorrm && m_VertexMode != VMode::ToMother && msgLevel(MSG::WARNING))
    warning() << "Writing the corrected mass is only implemented for VertexMode = 0.\n"
                 "No corrected mass output will be written to the tuple!" << endmsg;

  if(m_improvedVertex != -1 && m_VertexMode == VMode::NewVertex && msgLevel(MSG::WARNING))
    warning() << "Requested an improved vertex cut, but running in mode where a new vertex is made.\n"
                 "This cut will have no effect!" << endmsg;


  //get tools
  m_dva                 = Gaudi::Utils::getIDVAlgorithm ( contextSvc() ) ;
  m_dist                = tool<IDistanceCalculator>("LoKi::DistanceCalculator",this);
  m_pvReFitter          = tool<IPVReFitter>("LoKi::PVReFitter:PUBLIC", this );
  m_pVertexFit          = m_dva->vertexFitter();
  m_particleDescendants = tool<IParticleDescendants> ( "ParticleDescendants");
  if( !m_dva )                 return Error("Couldn't get parent DVAlgorithm",                 StatusCode::FAILURE);
  if( !m_dist )                return Error("Unable to retrieve the IDistanceCalculator tool", StatusCode::FAILURE);
  if( !m_pvReFitter )          return Error("Unable to retrieve the LoKi::PVReFitter",         StatusCode::FAILURE);
  if( !m_pVertexFit )          return Error("Unable to retrieve the IVertexFit tool",          StatusCode::FAILURE);
  if( !m_particleDescendants ) return Error("Unable to retrieve the ParticleDescendants tool", StatusCode::FAILURE);

  return StatusCode::SUCCESS;
}

//=============================================================================

StatusCode TupleToolAllTracks::fill( const Particle* mother
                                     , const Particle* Mother
                                     , const std::string& head
                                     , Tuples::Tuple& tuple )
{
  const std::string prefix="Added";
  Assert( Mother && mother && m_dist
          , "This should not happen, you are inside TupleToolAllTracks.cpp :(" );

  // set test true. It will be the StatusCode that is returned
  bool test = true;
  const auto pion_mass   = 139.57 *Gaudi::Units::MeV;
  const auto kaon_mass   = 493.68 *Gaudi::Units::MeV;
  const auto proton_mass = 938.272*Gaudi::Units::MeV;

  //declare variables that will be written to the tuple
  std::vector<float> h_oldip, h_oldipchi2, h_newip, h_newipchi2, h_minipchi2;
  std::vector<float> h_opening, h_fdchi2_new, h_fdchi2_old, h_fd_new, h_fd_old;
  std::vector<float> h_vertexchi2_new, h_vertexchi2_old;
  std::vector<float> h_trackchi2, h_ghost;
  std::vector<int>   h_tracktype;
  std::vector<float> h_PIDk, h_PIDe, h_PIDmu, h_PIDp, h_ProbNNp, h_ProbNNk, h_ProbNNe, h_ProbNNpi, h_ProbNNmu;
  std::vector<float> h_PT, h_ETA, h_PHI, h_THETA, h_DELPHI, h_PX, h_PY, h_PZ;
  std::vector<float> h_PID, h_ProbNNPID;
  std::vector<float> T_plus_h_M, M_h_CorrM, M_h_Delta_CorrM, T_plus_h_M_pion, M_h_CorrM_pion, M_h_Delta_CorrM_pion;
  std::vector<float> T_plus_h_M_proton, M_h_CorrM_proton, M_h_Delta_CorrM_proton, T_plus_h_M_kaon, M_h_CorrM_kaon, M_h_Delta_CorrM_kaon;
  std::vector<float> T_plus_h_PT, T_plus_h_ETA, T_plus_h_PHI, T_plus_h_PX, T_plus_h_PY, T_plus_h_PZ;
  std::vector<bool>  VRFS;
  std::vector<float> newvertex_x,newvertex_y,newvertex_z,newPVx,newPVy,newPVz;
  std::vector<Gaudi::SymMatrix3x3> vertexcov_new, pvcov_new;

  Particle *Target = nullptr;

  //check if mother is target
  auto mother_is_target = !m_TargetParticle.compare("Mother") //==0 means strings are equal
                          || Mother->particleID().abspid() == (LoKi::Particles::pidFromName(m_TargetParticle)).abspid();
  if(mother_is_target){
    Target = const_cast<Particle*>(Mother);    
  }
  else{
    for( auto tmpDaughter : Mother->daughtersVector()){
      //tmp daughter is charm meson or baryon
      bool default_charm = ( (400 < tmpDaughter->particleID().abspid() && tmpDaughter->particleID().abspid() < 500)
                             | (4000 < tmpDaughter->particleID().abspid() && tmpDaughter->particleID().abspid() < 5000) );
      //declare particles in the decay chain as target
      if(!m_TargetParticle.compare("Charm")){
        if(default_charm)
          Target = const_cast<Particle*>(tmpDaughter);
        else continue;
      }
      else if(tmpDaughter->particleID().abspid() == (LoKi::Particles::pidFromName(m_TargetParticle)).abspid())
        Target = const_cast<Particle*>(tmpDaughter);
      else continue;
    }
  }

  //If no target was found, or the mother is a basic particle, go to the next event
  if(Target == nullptr) return StatusCode(test);
  if(Mother->isBasicParticle()) return StatusCode(test);

  //get the vertex from the decay described in the DecayDescriptor (old vertex)
  //here we have 3 different scenarios:
  // - the new particle should be added to the target's mother
  //   (e.g. Lb -> (Sc->Lc {pi}) mu nu, where Sc is strongly decaying to the target and a pion we want to add)
  // - the new particle should be added directly to the target decay vertex
  //   (e.g. {psi(2S)?} -> (J/psi -> mu mu) {pi} {pi}, where the two pions are directly added to the dimuon vertex, because of the short J/psi lifetime)
  // - the new particle and the target form a new vertex
  //   (e.g. {B?} -> D0 {pi}, where D0 is target, pi is added particle. Since the D0 decay vertex will be upstream of the B vertex due to it's lifetime, we need to make a new vertex)
  //   contrary to the first 2 scenarios, it makes no sense to compare vertices in this case, since the vertex didn't exist before.
  Vertex v;
  Particle::ConstVector parts2Vertex;
  if(m_VertexMode == VMode::ToMother)
    parts2Vertex = Mother->daughtersVector();
  else if(m_VertexMode == VMode::ToTarget)
    parts2Vertex = Target->daughtersVector();
  else
    parts2Vertex.push_back(Target);
  if(m_VertexMode != VMode::NewVertex)
    m_pVertexFit->fit(v,parts2Vertex);

  // for all input particle locations
  for(auto location : m_InputParticles){
    // get particles parts
    auto parts = get<Particle::Range>(location);
    if (msgLevel(MSG::DEBUG)) debug() << "Getting particles from " << location << " with " << (parts).size() << " particles" << endmsg;
    for(auto *part : parts){
      if (!part){ info() << "particle to be added not found" << endmsg;continue;}

      auto PID = part->particleID().pid();
      //is basic or not one of the other basic particles that don't have a prototrack... (is there a query if a particle has a protoparticle?)
      bool proto_exits = part->isBasicParticle() && std::abs(PID) != 22 && std::abs(PID) != 111 && std::abs(PID) != 221;

      //cut on track types includes Velo, VeloR, Long, Upstream and Downstream tracks
      //skip tracks if they are already in the decay chain
      if(proto_exits && ((part->proto()->track()->type() < Track::Types::Velo || part->proto()->track()->type() > Track::Types::Downstream)
                         || isTrackInDecay(part,Mother)))continue;

      auto pidK = -999.f, pide = -999.f, pidmu = -999.f, pidP = -999.f;
      auto probNNe = -1.f, probNNk = -1.f, probNNp = -1.f, probNNpi = -1.f, probNNmu = -1.f, ghprob = -1.f;
      auto ProbNNPID = 0, sign_long = 0, tracktype = 0;

      if(proto_exits){
        tracktype = part->proto()->track()->type();
        pidK  = part->proto()->info(ProtoParticle::CombDLLk,-999);
        pide  = part->proto()->info(ProtoParticle::CombDLLe,-999);
        pidmu = part->proto()->info(ProtoParticle::CombDLLmu,-999);
        pidP  = part->proto()->info(ProtoParticle::CombDLLp,-999);
        std::vector<float> probNNs;
        probNNpi = part->proto()->info(ProtoParticle::ProbNNpi,-1);probNNs.push_back(probNNpi);
        probNNmu = part->proto()->info(ProtoParticle::ProbNNmu,-1);probNNs.push_back(probNNmu);
        probNNk  = part->proto()->info(ProtoParticle::ProbNNk,-1);probNNs.push_back(probNNk);
        probNNp  = part->proto()->info(ProtoParticle::ProbNNp,-1);probNNs.push_back(probNNp);
        probNNe  = part->proto()->info(ProtoParticle::ProbNNe,-1);probNNs.push_back(probNNe);
        ghprob   = part->proto()->track()->ghostProbability();
        //get PID according most probable ProbNN value of the track
        auto itelPID = std::distance(probNNs.begin(),std::max_element(probNNs.begin(), probNNs.end()));
        part->particleID().pid() < 0 ? sign_long = -1 : sign_long = 1;
        auto PIDfromProbNN = [=] () {
          if      (itelPID == 0) return  sign_long*211;
          else if (itelPID == 1) return -sign_long*13;
          else if (itelPID == 2) return  sign_long*321;
          else if (itelPID == 3) return  sign_long*2212;
          else if (itelPID == 4) return -sign_long*11;
          else return -1;
        };
        ProbNNPID = PIDfromProbNN();
      }
      //Ghost Probability cut
      if(ghprob > m_GhostProb) continue;

      //get PID variables for the input particle type and make the desired cut
      auto PIDVariables_underPIDHypothesis = [=] () -> std::pair<float,float> {
        if     (std::abs(PID)==211) return {probNNpi,-pidK};
        else if(std::abs(PID)==321) return {probNNk,pidK};
        else if(std::abs(PID)==2212)return {probNNp,pidP};
        else if(std::abs(PID)==13)  return {probNNmu,pidmu};
        else if(std::abs(PID)==11)  return {probNNe,pide};
        else return {1.f,100.f};
      };
      if(PIDVariables_underPIDHypothesis().first < m_ANNPIDCut || PIDVariables_underPIDHypothesis().second < m_DLLPIDCut) continue;

      //calculate the Target+particle mass and make the desired cut
      const auto TH = Gaudi::LorentzVector(Target->momentum()+part->momentum());
      auto Th_mass = TH.M();
      if(m_HiMassCut != -1*Gaudi::Units::MeV && Th_mass > m_HiMassCut) continue;
      //Theta angle
      float theta_part = part->momentum().theta();
      if( theta_part < m_theta) continue;

      float Delta_phi = Target->momentum().phi() - part->momentum().phi();
      if(std::fabs(Delta_phi) < m_deltaphi)continue;

      //PV min ipchi2
      auto PVinfo = getminipchi(part);
      if(m_PVipchi2Cut != -1 && PVinfo.first < m_PVipchi2Cut) continue;

      //Fit a vertex with the added particle
      bool VertexRefit = true;
      Vertex vtxWithExtraTrack;
      Particle *tmp_part = new Particle(*part);      
      parts2Vertex.push_back(tmp_part);
      VertexRefit &= m_pVertexFit->fit(vtxWithExtraTrack,parts2Vertex);
      parts2Vertex.pop_back();
      delete tmp_part;

      double oldip = 0, oldipchi2 = 0, newip, newipchi2, oldfd = 0, oldfdchi2 = 0, newfd, newfdchi2;
      //Vertex chi2's
      auto newvertchi2 = vtxWithExtraTrack.chi2PerDoF();
      if(m_VertexChi2Cut != -1 && (newvertchi2 > m_VertexChi2Cut || newvertchi2 < 0)) continue;      
      auto oldvertchi2 = m_VertexMode != VMode::NewVertex ? v.chi2PerDoF() : 0.f;
      if(m_improvedVertex != -1 && m_VertexMode != VMode::NewVertex && (newvertchi2 > oldvertchi2 + m_improvedVertex || newvertchi2 < 0)) continue;
      //Vertex IP's
      m_dist->distance( part,&vtxWithExtraTrack,newip,newipchi2 );
      if(newipchi2 < 0 || (m_ipchi2Cut != -1 && newipchi2 > m_ipchi2Cut)) continue;
      if(m_VertexMode != VMode::NewVertex){
        m_dist->distance( part,&v,oldip,oldipchi2 );
        if(oldipchi2 < 0) continue;
      }
      
      h_minipchi2.push_back(PVinfo.first);

      //FD's
      m_dist->distance ( PVinfo.second.get(), &vtxWithExtraTrack, newfd, newfdchi2 );
      if(m_VertexMode != VMode::NewVertex)
        m_dist->distance ( PVinfo.second.get(), &v, oldfd, oldfdchi2 );
      
      //opening angle between added particle and mother
      h_opening.push_back(Mother->momentum().Dot(part->momentum())/sqrt(Mother->momentum().mag2()*part->momentum().mag2()));

      //Masses and corrected masses
      std::vector<float> tmp_vec;
      ROOT::Math::PxPyPzM4D<double> Tg,h;
      Tg.SetPxPyPzE(Target->momentum().px(),Target->momentum().py(),Target->momentum().pz(),Target->momentum().E());
      h.SetPxPyPzE(part->momentum().px(),part->momentum().py(),part->momentum().pz(),part->momentum().E());

      T_plus_h_M.push_back(Th_mass);
      if(m_addcorrm && m_VertexMode == VMode::ToMother){
        tmp_vec = getcorrm(PVinfo.second.get(),&vtxWithExtraTrack,(Mother->momentum() + part->momentum()));
        M_h_CorrM.push_back(tmp_vec.at(0));
        M_h_Delta_CorrM.push_back(tmp_vec.at(1));
      }

      h.SetM(pion_mass);
      T_plus_h_M_pion.push_back((static_cast<Gaudi::LorentzVector>(Tg)+static_cast<Gaudi::LorentzVector>(h)).M());
      if(m_addcorrm && m_VertexMode == VMode::ToMother){
        tmp_vec = getcorrm(PVinfo.second.get(),&vtxWithExtraTrack,(Mother->momentum() + static_cast<Gaudi::LorentzVector>(h)));
        M_h_CorrM_pion.push_back(tmp_vec.at(0));
        M_h_Delta_CorrM_pion.push_back(tmp_vec.at(1));
      }

      h.SetM(kaon_mass);
      T_plus_h_M_kaon.push_back((static_cast<Gaudi::LorentzVector>(Tg)+static_cast<Gaudi::LorentzVector>(h)).M());
      if(m_addcorrm && m_VertexMode == VMode::ToMother){
        tmp_vec = getcorrm(PVinfo.second.get(),&vtxWithExtraTrack,(Mother->momentum() + static_cast<Gaudi::LorentzVector>(h)));
        M_h_CorrM_kaon.push_back(tmp_vec.at(0));
        M_h_Delta_CorrM_kaon.push_back(tmp_vec.at(1));
      }

      h.SetM(proton_mass);
      T_plus_h_M_proton.push_back((static_cast<Gaudi::LorentzVector>(Tg)+static_cast<Gaudi::LorentzVector>(h)).M());
      if(m_addcorrm && m_VertexMode == VMode::ToMother){
        tmp_vec = getcorrm(PVinfo.second.get(),&vtxWithExtraTrack,(Mother->momentum() + static_cast<Gaudi::LorentzVector>(h)));
        M_h_CorrM_proton.push_back(tmp_vec.at(0));
        M_h_Delta_CorrM_proton.push_back(tmp_vec.at(1));
      }

      T_plus_h_PT     .push_back(TH.pt());
      T_plus_h_ETA    .push_back(TH.eta());
      T_plus_h_PHI    .push_back(TH.phi());
      if(m_addpxpypz){
        T_plus_h_PX   .push_back(TH.px());
        T_plus_h_PY   .push_back(TH.py());
        T_plus_h_PZ   .push_back(TH.pz());
        h_PX          .push_back(part->momentum().px());
        h_PY          .push_back(part->momentum().py());
        h_PZ          .push_back(part->momentum().pz());
      }
      h_PT            .push_back(part->momentum().pt());
      h_ETA           .push_back(part->momentum().eta());
      h_PHI           .push_back(part->momentum().phi());
      h_THETA         .push_back(theta_part);
      h_DELPHI        .push_back(Delta_phi);
      if(m_VertexMode != VMode::NewVertex){
        h_oldip         .push_back((float)oldip);
        h_oldipchi2     .push_back((float)oldipchi2);
        h_fdchi2_old    .push_back((float)oldfdchi2);
        h_fd_old        .push_back((float)oldfd);
        h_vertexchi2_old.push_back(oldvertchi2);
      }
      h_newip         .push_back((float)newip);
      h_newipchi2     .push_back((float)newipchi2);      
      h_fdchi2_new    .push_back((float)newfd);      
      h_fd_new        .push_back((float)newfdchi2);
      h_vertexchi2_new.push_back(newvertchi2);      
      if(proto_exits){
        h_PIDk        .push_back(pidK);
        h_PIDe        .push_back(pide);
        h_PIDp        .push_back(pidP);
        h_PIDmu       .push_back(pidmu);
        h_ProbNNpi    .push_back(probNNpi);
        h_ProbNNp     .push_back(probNNp);
        h_ProbNNe     .push_back(probNNe);
        h_ProbNNk     .push_back(probNNk);
        h_ProbNNmu    .push_back(probNNmu);
        h_ghost       .push_back(ghprob);
        h_trackchi2   .push_back(part->proto()->track()->chi2PerDoF());
        h_tracktype   .push_back(tracktype);
        h_ProbNNPID   .push_back(ProbNNPID);
      }
      h_PID           .push_back(PID);
      VRFS            .push_back(VertexRefit);
      newvertex_x     .push_back(vtxWithExtraTrack.position().X());
      newvertex_y     .push_back(vtxWithExtraTrack.position().Y());
      newvertex_z     .push_back(vtxWithExtraTrack.position().Z());
      newPVx          .push_back(PVinfo.second->position().X());
      newPVy          .push_back(PVinfo.second->position().Y());
      newPVz          .push_back(PVinfo.second->position().Z());
      vertexcov_new   .push_back(vtxWithExtraTrack.covMatrix());
      pvcov_new       .push_back(PVinfo.second->covMatrix());
    }
  }

  auto target_tup_name = m_TargetParticle;
  boost::algorithm::replace_all(target_tup_name,"*","St");
  boost::algorithm::replace_all(target_tup_name,"-","m");
  boost::algorithm::replace_all(target_tup_name,"+","p");
  boost::algorithm::replace_all(target_tup_name,"~","bar");
  boost::algorithm::replace_all(target_tup_name,"'","prime");
  boost::algorithm::replace_all(target_tup_name,"(","_");
  boost::algorithm::replace_all(target_tup_name,")","_");

  //If array is bigger than m_mult entries per mother, we are still safe with the StatusCode, because farray will only throw a warning and not a bad status code in this case
  test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_M"                ,T_plus_h_M.begin(), T_plus_h_M.end()                        ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_M_pion"           ,T_plus_h_M_pion.begin(), T_plus_h_M_pion.end()              ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_M_kaon"           ,T_plus_h_M_kaon.begin(), T_plus_h_M_kaon.end()              ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_M_proton"         ,T_plus_h_M_proton.begin(), T_plus_h_M_proton.end()          ,prefix+"_n_Particles",m_mult );
  if(m_addcorrm && m_VertexMode == VMode::ToMother){
    test &= tuple -> farray( fullName(head) + "_dauH_CorrM"                        ,M_h_CorrM.begin(), M_h_CorrM.end()                          ,prefix+"_n_Particles",m_mult );
    test &= tuple -> farray( fullName(head) + "_dauH_CorrM_pion"                   ,M_h_CorrM_pion.begin(), M_h_CorrM_pion.end()                ,prefix+"_n_Particles",m_mult );
    test &= tuple -> farray( fullName(head) + "_dauH_CorrM_kaon"                   ,M_h_CorrM_kaon.begin(), M_h_CorrM_kaon.end()                ,prefix+"_n_Particles",m_mult );
    test &= tuple -> farray( fullName(head) + "_dauH_CorrM_proton"                 ,M_h_CorrM_proton.begin(), M_h_CorrM_proton.end()            ,prefix+"_n_Particles",m_mult );
    test &= tuple -> farray( fullName(head) + "_dauH_CorrMErr"                     ,M_h_Delta_CorrM.begin(), M_h_Delta_CorrM.end()              ,prefix+"_n_Particles",m_mult );
    test &= tuple -> farray( fullName(head) + "_dauH_CorrMErr_pion"                ,M_h_Delta_CorrM_pion.begin(), M_h_Delta_CorrM_pion.end()    ,prefix+"_n_Particles",m_mult );
    test &= tuple -> farray( fullName(head) + "_dauH_CorrMErr_kaon"                ,M_h_Delta_CorrM_kaon.begin(), M_h_Delta_CorrM_kaon.end()    ,prefix+"_n_Particles",m_mult );
    test &= tuple -> farray( fullName(head) + "_dauH_CorrMErr_proton"              ,M_h_Delta_CorrM_proton.begin(), M_h_Delta_CorrM_proton.end(),prefix+"_n_Particles",m_mult );
  }
  test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_PT"               ,T_plus_h_PT.begin(), T_plus_h_PT.end()                      ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_ETA"              ,T_plus_h_ETA.begin(), T_plus_h_ETA.end()                    ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_PHI"              ,T_plus_h_PHI.begin(), T_plus_h_PHI.end()                    ,prefix+"_n_Particles",m_mult );
  if(m_addpxpypz){
    test &= tuple -> farray( prefix + "_" + target_tup_name + "H_PX"               ,T_plus_h_PX.begin(), T_plus_h_PX.end()                      ,prefix+"_n_Particles",m_mult );
    test &= tuple -> farray( prefix + "_" + target_tup_name + "H_PY"               ,T_plus_h_PY.begin(), T_plus_h_PY.end()                      ,prefix+"_n_Particles",m_mult );
    test &= tuple -> farray( prefix + "_" + target_tup_name + "H_PZ"               ,T_plus_h_PZ.begin(), T_plus_h_PZ.end()                      ,prefix+"_n_Particles",m_mult );
  }
  if(m_VertexMode != VMode::NewVertex)
    test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_FDCHI2_OLD"       ,h_fdchi2_old.begin(),  h_fdchi2_old.end()                   ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_FDCHI2_NEW"       ,h_fdchi2_new.begin(),  h_fdchi2_new.end()                   ,prefix+"_n_Particles",m_mult );
  if(m_VertexMode != VMode::NewVertex)
    test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_FD_OLD"           ,h_fd_old.begin(),  h_fd_old.end()                           ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_FD_NEW"           ,h_fd_new.begin(),  h_fd_new.end()                           ,prefix+"_n_Particles",m_mult );
  if(m_VertexMode != VMode::NewVertex)
    test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_VERTEXCHI2_OLD"   ,h_vertexchi2_old.begin(),  h_vertexchi2_old.end()           ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_VERTEXCHI2_NEW"   ,h_vertexchi2_new.begin(),  h_vertexchi2_new.end()           ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_VERTEX_X_NEW"     ,newvertex_x.begin(),  newvertex_x.end()                     ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_VERTEX_Y_NEW"     ,newvertex_y.begin(),  newvertex_y.end()                     ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_VERTEX_Z_NEW"     ,newvertex_z.begin(),  newvertex_z.end()                     ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_PV_X_NEW"         ,newPVx.begin(),  newPVx.end()                               ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_PV_Y_NEW"         ,newPVy.begin(),  newPVy.end()                               ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_PV_Z_NEW"         ,newPVz.begin(),  newPVz.end()                               ,prefix+"_n_Particles",m_mult );

  //The following line doesn't work with that type, so write the individual components to the tuple
  //test &= tuple ->   put   ( prefix + "_H_VERTEX_COV_NEW", const_cast<std::vector<Gaudi::SymMatrix3x3>*>(&h_vertexcov_new));
  std::vector<std::string> xyz = {"X","Y","Z"};
  for(unsigned int i = 0; i < 3; i++){
    for(unsigned int j = i; j < 3; j++){
      std::vector<float> tmp_cov_vec, tmp_pv_cov;
      for(unsigned int k = 0; k < vertexcov_new.size(); k++){
        tmp_cov_vec.push_back(vertexcov_new.at(k).At(i,j));
        tmp_pv_cov .push_back(pvcov_new    .at(k).At(i,j));
      }
      test &= tuple -> farray( prefix + "_" + target_tup_name + "H_VERTEX_COV_" + xyz.at(i) + xyz.at(j),tmp_cov_vec.begin(), tmp_cov_vec.end()  ,prefix+"_n_Particles",m_mult );
      test &= tuple -> farray( prefix + "_" + target_tup_name + "H_PV_COV_" +     xyz.at(i) + xyz.at(j),tmp_pv_cov.begin() , tmp_pv_cov.end()   ,prefix+"_n_Particles",m_mult );
    }
  }

  test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_VertexRefitStatus",VRFS.begin(),VRFS.end()                                     ,prefix+"_n_Particles",m_mult );
  if(m_VertexMode != VMode::NewVertex){
    test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_IPCHI2_OLD"       ,h_oldipchi2.begin(),  h_oldipchi2.end()                     ,prefix+"_n_Particles",m_mult );
    test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_IP_OLD"           ,h_oldip.begin(),  h_oldip.end()                             ,prefix+"_n_Particles",m_mult );
  }
  test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_IPCHI2_NEW"       ,h_newipchi2.begin(),  h_newipchi2.end()                     ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_IP_NEW"           ,h_newip.begin(),  h_newip.end()                             ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_" + target_tup_name + "H_MINIPCHI2"        ,h_minipchi2.begin(),  h_minipchi2.end()                     ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( fullName(head) + "_H_OPENING"                         ,h_opening.begin(),  h_opening.end()                         ,prefix+"_n_Particles",m_mult );
  if(m_addpxpypz){
    test &= tuple -> farray( prefix + "_H_PX"                                      ,h_PX.begin(),  h_PX.end()                                   ,prefix+"_n_Particles",m_mult );
    test &= tuple -> farray( prefix + "_H_PY"                                      ,h_PY.begin(),  h_PY.end()                                   ,prefix+"_n_Particles",m_mult );
    test &= tuple -> farray( prefix + "_H_PZ"                                      ,h_PZ.begin(),  h_PZ.end()                                   ,prefix+"_n_Particles",m_mult );
  }
  test &= tuple ->   farray( prefix + "_H_PT"                                      ,h_PT.begin(),  h_PT.end()                                   ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_H_ETA"                                     ,h_ETA.begin(),  h_ETA.end()                                 ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_H_PHI"                                     ,h_PHI.begin(),  h_PHI.end()                                 ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_H_THETA"                                   ,h_THETA.begin(),  h_THETA.end()                             ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_H_DELTAPHI"                                ,h_DELPHI.begin(),  h_DELPHI.end()                           ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_H_PID"                                     ,h_PID.begin(),  h_PID.end()                                 ,prefix+"_n_Particles",m_mult );
  test &= tuple ->   farray( prefix + "_H_PROBNNPID"                               ,h_ProbNNPID.begin(),  h_ProbNNPID.end()                     ,prefix+"_n_Tracks",m_mult );
  test &= tuple ->   farray( prefix + "_H_GHOST"                                   ,h_ghost.begin(),  h_ghost.end()                             ,prefix+"_n_Tracks",m_mult );
  test &= tuple ->   farray( prefix + "_H_TRACKCHI2"                               ,h_trackchi2.begin(),  h_trackchi2.end()                     ,prefix+"_n_Tracks",m_mult );
  test &= tuple ->   farray( prefix + "_H_TRACKTYPE"                               ,h_tracktype.begin(),  h_tracktype.end()                     ,prefix+"_n_Tracks",m_mult );
  test &= tuple ->   farray( prefix + "_H_PIDe"                                    ,h_PIDe.begin(),  h_PIDe.end()                               ,prefix+"_n_Tracks",m_mult );
  test &= tuple ->   farray( prefix + "_H_PIDK"                                    ,h_PIDk.begin(),  h_PIDk.end()                               ,prefix+"_n_Tracks",m_mult );
  test &= tuple ->   farray( prefix + "_H_PIDmu"                                   ,h_PIDmu.begin(),  h_PIDmu.end()                             ,prefix+"_n_Tracks",m_mult );
  test &= tuple ->   farray( prefix + "_H_PIDp"                                    ,h_PIDp.begin(),  h_PIDp.end()                               ,prefix+"_n_Tracks",m_mult );
  test &= tuple ->   farray( prefix + "_H_ProbNNpi"                                ,h_ProbNNpi.begin(),  h_ProbNNpi.end()                       ,prefix+"_n_Tracks",m_mult );
  test &= tuple ->   farray( prefix + "_H_ProbNNk"                                 ,h_ProbNNk.begin(),  h_ProbNNk.end()                         ,prefix+"_n_Tracks",m_mult );
  test &= tuple ->   farray( prefix + "_H_ProbNNp"                                 ,h_ProbNNp.begin(),  h_ProbNNp.end()                         ,prefix+"_n_Tracks",m_mult );
  test &= tuple ->   farray( prefix + "_H_ProbNNe"                                 ,h_ProbNNe.begin(),  h_ProbNNe.end()                         ,prefix+"_n_Tracks",m_mult );
  test &= tuple ->   farray( prefix + "_H_ProbNNmu"                                ,h_ProbNNmu.begin(),  h_ProbNNmu.end()                       ,prefix+"_n_Tracks",m_mult );

  return StatusCode(test);
}

//=============================================================================
// Check if the track is already in the decay
//=============================================================================
bool TupleToolAllTracks::isTrackInDecay(const Particle* particle, const Particle* mother){
  bool isInDecay = false;
  //loop over daughters
  auto stable_daughters = m_particleDescendants->finalStates(mother);
  if(stable_daughters.empty())return isInDecay;
  for(auto* it : stable_daughters){
    if(std::abs(it->particleID().pid()) != 22 && std::abs(it->particleID().pid()) != 111 && std::abs(it->particleID().pid()) != 221){
      if(it->proto()->track() == particle->proto()->track()){
        if ( msgLevel(MSG::DEBUG) ) debug() << "Track is in decay, skipping it" << endmsg;
        isInDecay = true;
        break;
      }
    }
  }
  return isInDecay;
}

//=============================================================================
// MINIPCHI2 for a Particle | Thanks to Roel for correcting this part of code!
//=============================================================================
std::pair<float, std::unique_ptr<RecVertex> > TupleToolAllTracks::getminipchi(const Particle* particle){
  float minchi2 = -1.f ;
  RecVertex *bestPV = nullptr;
  auto PV = m_dva->primaryVertices();
  if ( PV.empty() )return {minchi2,nullptr};
  for ( auto pv : PV ){
    double ip, chi2;
    m_dist->distance ( particle, pv, ip, chi2 );
    if( (chi2 < minchi2) || (minchi2 < 0.0) ){
      RecVertex* newPV = pv->clone();
      // remove particle refit vertex
      m_pvReFitter->remove( particle, newPV );
      m_dist->distance( particle, newPV, ip, chi2 );
      minchi2 = chi2;
      delete bestPV;
      bestPV = newPV;
    }
  }
  return {minchi2, std::unique_ptr<RecVertex>(bestPV)};
}

//=============================================================================
// Corrected mass for mother + additional particle
//=============================================================================
std::vector<float> TupleToolAllTracks::getcorrm(const RecVertex *PV, const Vertex *SV, const Gaudi::LorentzVector &new_mother){
  //Calculations taken from Lb -> p mu nu note: https://cds.cern.ch/record/1694339/files/LHCb-ANA-2014-048.pdf
  auto dir = SV->position() - PV->position();
  auto mom = new_mother.Vect();
  auto dmag2 = dir.Mag2();
  auto longit_norm = 0.f;
  if ( 0.f != dmag2 ) longit_norm = mom.Dot(dir)/dmag2;
  auto ptprime = mom - dir*longit_norm;
  float CorrM = (float)sqrt( pow(new_mother.M(),2) + ptprime.Mag2() ) + sqrt(ptprime.Mag2());

  //calculate derivatives of longit_norm
  auto dlon_dx = ( 2*dir.x()*mom.Dot(dir) - mom.x()*dmag2 )/(pow(dmag2,2));
  auto dlon_dy = ( 2*dir.y()*mom.Dot(dir) - mom.y()*dmag2 )/(pow(dmag2,2));
  auto dlon_dz = ( 2*dir.z()*mom.Dot(dir) - mom.z()*dmag2 )/(pow(dmag2,2));

  //derivatives of pt (from Michael Kolpin)
  std::vector<float> pt_derivatives;
  if(ptprime.Mag2() == 0.f)return {CorrM,-1.f};
  pt_derivatives.push_back( (longit_norm*ptprime.x() - dlon_dx*(ptprime.x() + ptprime.y() + ptprime.z()))/sqrt(ptprime.Mag2()) );
  pt_derivatives.push_back( (longit_norm*ptprime.y() - dlon_dy*(ptprime.x() + ptprime.y() + ptprime.z()))/sqrt(ptprime.Mag2()) );
  pt_derivatives.push_back( (longit_norm*ptprime.z() - dlon_dz*(ptprime.x() + ptprime.y() + ptprime.z()))/sqrt(ptprime.Mag2()) );

  //uncertainty of pt
  auto PV_cov = PV->covMatrix();
  auto SV_cov = SV->covMatrix();
  auto delta_ptprime2 = 0.f;
  for(auto i = 0; i < 3; i++){
    for(auto j = 0; j < 3; j++){
      delta_ptprime2 += pt_derivatives.at(i)*pt_derivatives.at(j)*( PV_cov.At(i,j) + SV_cov.At(i,j) );
      if(i != j) delta_ptprime2 *= 2;
    }
  }
  //uncertainty on m_corr
  float delta_Corr_M = (float)( sqrt(ptprime.Mag2())/sqrt(pow(new_mother.M(),2) + ptprime.Mag2()) + 1.0 ) * sqrt(delta_ptprime2);

  return {CorrM,delta_Corr_M};
}
