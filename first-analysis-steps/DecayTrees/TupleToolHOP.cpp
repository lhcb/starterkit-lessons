// $Id: TupleToolHOP.cpp,v 0.1 2016-05-20 20:01:40 jpalac Exp $
// Include files

// local
#include "TupleToolHOP.h"

//#include <SmartRefVector.h>
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleProperty.h"
#include "Kernel/IDVAlgorithm.h"
#include <Kernel/GetIDVAlgorithm.h>

#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"

#include "Event/Particle.h"

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : HOPTupleTool
//
// 2016-05-20 : Carla Marin Benito, Vinicius Franco Lima
//
//
//This Tool Implements the variables described in LHCb Note: LHCb-INT-2015-037
//see https://cds.cern.ch/record/2102345
//
//The HOP variables are conceived to exploit the kinematic characteristics of B
//decays into final states involving electrons.
//
//The general idea is to use the fact that the decay particles should have total
//pt with respect to the B flight direction of 0. Using this fact we can re-weight
//the electron momentum using the non-electron parts of the decay.
//
//It is important to note the way this tool works: It applies the momentum
//correction to the electrons in the decay and calculates the HOP mass from there,
//not correcting intermediate state momenta, but final state particles.
//We do, however take into account the intermediate ressonance momentum measurement
//in order to calculate the HOP correction itself.
//
//This tool is comprised of 3 methods:
//
// TupleTooHOP::ClassifyParticles: gets the top particle and two particle containers as
//input and loops over the decay chain classifying basic children in electron and non-electron
//type. When all the children of a composite particle belong to the same type, the composite
//particle receives the same type and it is itself – instead of each children – stored in the
//appropiate container. This is done in order not to loose presicion, since when the children
//are afterwards combined to obtain the mother’s corrected mass no fit but a simple addition
//of momenta is used.
//
// TupleToolHOP::HOPProjectMomentum: takes two particles as input, the first one is assumed
//to be the top of the decay. The direction of flight of this particle is obtained taking the best
//interaction vertex associated to it and its decay vertex. Then the transverse momentum of
//the second particle with respect to this direction is computed and returned.
//
// TupleToolHOP::fill: gets the top particle, the particles for which the variables should be
//computed, a string with the head name that will be used for the names of the variables that
//will be written to the ntuple and the ntuple itself as input. It first checks that the particle
//for which the variables should be computed is the same as the top of the chain. Since our
//algorithm is recursive on all the decay chain we only need to run it once from the top of it.
//Then the daughters are classified using the TupleToolHOP::ClassifyParticles function
//and their transverse momentum with respect to the direction of flight of the top particle
//obtained with the TupleToolHOP::HOPProjectMomentum. The transverse momentum for
//all electron and non-electron particles are added up separately and the HOP ratio is
//computed. This value is stored in the ntuple in the variable head HOP. Afterwards, it is
//used to correct the 4-momentum of each particle in the electron container. Finally, all the
//4-momenta of the electron particles are sumed together and added to the 4-momentum
//of the non-electron particles. The mother’s corrected mass is obtained from this total
//corrected 4-momentum and stored in the ntuple with the name head HOP MASS. The
//electron corrected mass is also obtained from the corrected 4-momentum of all the electron
//particles added together and is saved as head HOP ELECTRON MASS.
//

// Declaration of the Tool Factory
DECLARE_COMPONENT( TupleToolHOP )


//=============================================================================

StatusCode TupleToolHOP::initialize()
{
  const StatusCode sc = TupleToolBase::initialize();
  if ( sc.isFailure() ) return sc;

  m_dva = Gaudi::Utils::getIDVAlgorithm ( contextSvc(), this ) ;
  if ( !m_dva )
      return Error("Couldn't get parent DVAlgorithm");

  m_ppSvc = svc<LHCb::IParticlePropertySvc>( "LHCb::ParticlePropertySvc", true );
  if ( !m_ppSvc )
      return Error("Couldn't get ParticlePropertySvc");
  m_electronMassSquared = std::pow( m_ppSvc->find("e+")->mass()/Gaudi::Units::MeV, 2);

  if ( msgLevel(MSG::VERBOSE) )
    verbose() << "TupleToolHOP::initialize " << "Done" << endmsg ;

  return sc;
}

//=============================================================================
StatusCode TupleToolHOP::fill( const Particle* mother
		, const Particle* P
		, const std::string& head
		, Tuples::Tuple& tuple )
{
	StatusCode sc = StatusCode::SUCCESS;
	if ( msgLevel(MSG::VERBOSE) )
		verbose() << "TupleToolHOP::fill " << endmsg ;

	if (mother == P)
	{
		if ( msgLevel(MSG::VERBOSE) )
			verbose() << "TupleToolHOP::fill " << ": " << "particle is mother, let's run " << endmsg ;

		bool test = true;
		const std::string prefix = fullName(head);

		SmartRefVector<LHCb::Particle> electronContainer;
		SmartRefVector<LHCb::Particle> nonElectronContainer;
		if ( msgLevel(MSG::VERBOSE) )
			verbose() << "TupleToolHOP::fill " << ": " << "start ClassifyParticles " << endmsg ;

		ClassifyParticles(*P, electronContainer, nonElectronContainer);

		if ( msgLevel(MSG::VERBOSE) )
		{
			verbose() << "TupleToolHOP::fill " << ": " << "electrons found: " << electronContainer << endmsg ;
			verbose() << "TupleToolHOP::fill " << ": " << "non-electrons found: " << nonElectronContainer << endmsg ;
		}

		//Get pt wrt mother direction of flight
		float total_pt_e =0, total_pt_non_e=0;
		Gaudi::LorentzVector total_4p_e = Gaudi::LorentzVector(0,0,0,0);
		Gaudi::LorentzVector total_4p_non_e = Gaudi::LorentzVector(0,0,0,0);

		if ( msgLevel(MSG::VERBOSE) )
			verbose() << "TupleToolHOP::fill " << ": " << "loop on electrons " << endmsg ;
		for (auto& e: electronContainer)
		{
			if ( msgLevel(MSG::VERBOSE) )
			{
				verbose() << "TupleToolHOP::fill " << ": " << "electron is " << e << endmsg ;
				verbose() << "TupleToolHOP::fill " << ": " << "start HOPProjectMomentum " << endmsg ;
			}
      total_4p_e += e->momentum();
			total_pt_e = HOPProjectMomentum(P, &total_4p_e);
		}

		if ( msgLevel(MSG::VERBOSE) )
			verbose() << "TupleToolHOP::fill " << ": " << "loop on non-electrons " << endmsg ;

		for (auto& noE: nonElectronContainer)
		{
			if ( msgLevel(MSG::VERBOSE) )
			{
				verbose() << "TupleToolHOP::fill " << ": " << "non-electron is " << noE << endmsg ;
				verbose() << "TupleToolHOP::fill " << ": " << "start HOPProjectMomentum " << endmsg ;
			}
			total_4p_non_e += noE->momentum();
			total_pt_non_e = HOPProjectMomentum(P, &total_4p_non_e);
		}

		if ( msgLevel(MSG::VERBOSE) )
		{
			verbose() << "TupleToolHOP::fill " << ": " << "Total ptElectron:  "<< total_pt_e << endmsg ;
			verbose() << "TupleToolHOP::fill " << ": " << "Total pt non-Electron:  "<< total_pt_non_e << endmsg ;
			verbose() << "TupleToolHOP::fill " << ": " << "Total 4p non-Electron:  "<< total_4p_non_e << endmsg ;
		}

		//Compute HOP correction
		double hop = 0;
		if (total_pt_e != 0)
			hop = total_pt_non_e/total_pt_e;
		if ( msgLevel(MSG::VERBOSE) )
			verbose() << "TupleToolHOP::fill " << ": " << "hop: " << hop << endmsg ;

		//Fill tuple with HOP correction
		test &= tuple->column( prefix + "_HOP", hop);
		if ( msgLevel(MSG::VERBOSE) )
			verbose() << "TupleToolHOP::fill " << ": " << "hop written to ntuple" << endmsg ;

    // Correct elec momenta with HOP value
		Gaudi::LorentzVector sumCorrected=Gaudi::LorentzVector(0,0,0,0);

		if ( msgLevel(MSG::VERBOSE) )
			verbose() << "TupleToolHOP::fill " << ": " << "Electron  mass squared in MeV^2 : " << m_electronMassSquared << endmsg ;

		for (auto& e2: electronContainer)
		{
      if(e2->isBasicParticle()) // directly correct elec momenta
      {
        Gaudi::LorentzVector correctedMomentum = Gaudi::LorentzVector((e2->momentum().Px())*hop,
                                                                      (e2->momentum().Py())*hop,
                                                                      (e2->momentum().Pz())*hop,
                                                                      sqrt((std::pow(e2->momentum().Px(),2) +
                                                                            std::pow(e2->momentum().Py(),2) +
                                                                            std::pow(e2->momentum().Pz(),2) ) *std::pow(hop,2) +
                                                                           std::pow( m_ppSvc->find(e2->particleID())->mass() /
                                                                                     Gaudi::Units::MeV, 2)));
        if ( msgLevel(MSG::VERBOSE) )
        {
          verbose() << "TupleToolHOP::fill " << ": " << "Electron PX: " << e2->momentum().Px() << ", Corrected PX: "<< e2->momentum().Px()*hop   << endmsg ;
          verbose() << "TupleToolHOP::fill " << ": " << "Electron PY: " << e2->momentum().Py() << ", Corrected PY: "<< e2->momentum().Py()*hop   << endmsg ;
          verbose() << "TupleToolHOP::fill " << ": " << "Electron PZ: " << e2->momentum().Pz() << ", Corrected PZ: "<< e2->momentum().Pz()*hop   << endmsg ;
          verbose() << "TupleToolHOP::fill " << ": " << "Electron E: " << e2->momentum().E() << ", Corrected E: "<< sqrt((std::pow(e2->momentum().Px(),2)+ std::pow(e2->momentum().Py(),2)+ std::pow(e2->momentum().Pz(),2))*std::pow(hop,2) + std::pow( m_ppSvc->find(e2->particleID())->mass()/Gaudi::Units::MeV ,2))   << endmsg ;
        }
        sumCorrected += correctedMomentum;
        if ( msgLevel(MSG::VERBOSE) )
          verbose() << "TupleToolHOP::fill " << ": " << "Electron Corrected mass: " << correctedMomentum.M() << endmsg ;
      }

      else // correct momenta of each children separately and add them up. Is this what we want??
      {
        const auto& dau = e2->daughters();
        for(const auto& idaughter3 :dau)
        {
          Gaudi::LorentzVector correctedMomentum = Gaudi::LorentzVector((idaughter3->momentum().Px())*hop,
                                                                        (idaughter3->momentum().Py())*hop,
                                                                        (idaughter3->momentum().Pz())*hop,
                                                                        sqrt((std::pow(idaughter3->momentum().Px(),2)+
                                                                              std::pow(idaughter3->momentum().Py(),2)+
                                                                              std::pow(idaughter3->momentum().Pz(),2))*std::pow(hop,2) +
                                                                             std::pow( m_ppSvc->find(idaughter3->particleID())->mass() /
                                                                                       Gaudi::Units::MeV ,2)));
          sumCorrected += correctedMomentum;
        }
      }
		}

		if ( msgLevel(MSG::VERBOSE) )
    {
			verbose() << "TupleToolHOP::fill " << ": " << "Total Electron Corrected mass: " << (sumCorrected).M() << endmsg ;
			verbose() << "TupleToolHOP::fill " << ": " << "Non Electron mass: " << (total_4p_non_e).M() << endmsg ;
			verbose() << "TupleToolHOP::fill " << ": " << "hop mass: " << (total_4p_non_e+sumCorrected).M() << endmsg ;
			verbose() << "TupleToolHOP::fill " << ": " << "hop mass calculated: " << sqrt(pow((total_4p_non_e+sumCorrected).E(),2)
                                                                                    - pow((total_4p_non_e+sumCorrected).P(),2))
                << endmsg ;
    }

    // Fill HOP_MASS and HOP_ELECTRON_MASS
		test &= tuple->column( prefix + "_HOP_MASS", (total_4p_non_e+sumCorrected).M());
		test &= tuple->column( prefix + "_HOP_ELECTRON_MASS", (sumCorrected).M());
		if ( msgLevel(MSG::VERBOSE) )
			verbose() << "TupleToolHOP::fill " << ": " << "hop mass written to ntuple" << endmsg ;

    return StatusCode(test);
	}

	return sc ;
}

// =====================================================
// Classify particles in electron type and others
// =====================================================
int TupleToolHOP::ClassifyParticles(
   const LHCb::Particle& top,
   SmartRefVector<LHCb::Particle>& electronContainer,
   SmartRefVector<LHCb::Particle>& nonElectronContainer) const
{
  if ( msgLevel(MSG::VERBOSE) )
    verbose() << "TupleToolHOP::ClassifyParticles " << endmsg ;

  // Base case: basic particle
  if (top.isBasicParticle())
  {
    if ( msgLevel(MSG::VERBOSE) )
      verbose() << "TupleToolHOP::ClassifyParticles: " << top.particleID().pid() << " is basic" << endmsg ;

    int p_isElectron = (top.particleID().abspid() == 11) ? 1 : 0 ;
    int p_hasSameChildren = 2; // has no children since it is basic

    if ( msgLevel(MSG::VERBOSE) )
      verbose() << "TupleToolHOP::ClassifyParticles: " << top.particleID().pid() << " is electron: " << p_isElectron << endmsg ;

    return encodeTuple(p_isElectron, p_hasSameChildren);
  }
  // Complex Case ( Intermediate particles in the decay, need to figure out how to correctly classify them)
  else
  {
    if ( msgLevel(MSG::VERBOSE) )
    {
      verbose() << "TupleToolHOP::ClassifyParticles: " << top.particleID().pid() << " is NOT basic" << endmsg ;
      verbose() << "TupleToolHOP::ClassifyParticles: " << "start loop on daughters" << endmsg ;
    }

    const auto& dau = top.daughters();

    std::vector<int> t_list;
    int dau_type = 3; // means only first children beeing checked
    bool are_equal = true;
    for(const auto& idaughter :dau)
    {
      int dt = ClassifyParticles(*idaughter,
                                                      electronContainer,
                                                      nonElectronContainer);
      t_list.push_back(dt);
      // check if all daughters are of same type
      if (dau_type == 3) // first children, get only its type
        dau_type = isElectron(dt);
      else // other childrens, compare to previous
      {
        if (dau_type != isElectron(dt))
          are_equal = false;
      }
      if ( msgLevel(MSG::VERBOSE) )
      {
        verbose() << "TupleToolHOP::ClassifyParticles: " << idaughter->particleID().pid() << " electron: " << isElectron(dt) << endmsg;
        verbose() << "TupleToolHOP::ClassifyParticles: " << idaughter->particleID().pid() << " same child: " << hasSameChildren(dt) << endmsg;
      }
    }
    if ( msgLevel(MSG::VERBOSE) )
    {
        verbose() << "TupleToolHOP::ClassifyParticles: " << top.particleID().pid() << " daughters: " << dau << endmsg;
        //verbose() << "TupleToolHOP::ClassifyParticles: " << top.particleID().pid() << " t_list: " << t_list << endmsg;
        verbose() << "TupleToolHOP::ClassifyParticles: " << top.particleID().pid() << " equal dau: " << are_equal << endmsg;
    }

    int t = ClassifyParticles_Merge(dau, t_list,
                                    are_equal,
                                    electronContainer,
                                    nonElectronContainer);

    if ( msgLevel(MSG::VERBOSE) )
    {
      verbose() << "TupleToolHOP::ClassifyParticles: " << top.particleID().pid() << " electron: " << isElectron(t) << endmsg;
      verbose() << "TupleToolHOP::ClassifyParticles: " << top.particleID().pid() << " same child: " << hasSameChildren(t) << endmsg;
    }

    return t;
  }
}

// =====================================================
// Merge function for ClassifyParticles
// =====================================================
int TupleToolHOP::ClassifyParticles_Merge(
    const SmartRefVector<LHCb::Particle>& dau,
    std::vector<int>& t_list,
    bool are_equal,
    SmartRefVector<LHCb::Particle>& electronContainer,
    SmartRefVector<LHCb::Particle>& nonElectronContainer) const{

  if ( msgLevel(MSG::VERBOSE) )
    verbose() << "TupleToolHOP::ClassifyParticlesMerge " << endmsg ;

  int t_mother_isElectron = 0;
  int t_mother_hasSameChildren = 0;
  if (are_equal)
  {
    t_mother_hasSameChildren = 1;
    t_mother_isElectron = isElectron(t_list[0]); // inherits children type
    if ( msgLevel(MSG::VERBOSE) )
    {
      verbose() << "TupleToolHOP::ClassifyParticlesMerge: " << "equal dau" << endmsg ;
      verbose() << "TupleToolHOP::ClassifyParticlesMerge: " << "don't save children" << endmsg ;
    }
  }

  else
  {
    t_mother_hasSameChildren = 0;
    t_mother_isElectron = 2; // mother has "no type"

    if ( msgLevel(MSG::VERBOSE) )
    {
      verbose() << "TupleToolHOP::ClassifyParticlesMerge: " << "diff dau" << endmsg ;
      verbose() << "TupleToolHOP::ClassifyParticlesMerge: " << "save children if not yet done" << endmsg ;
    }

    //SmartRefVector<ClassifyParticles_Tuple>::const_iterator it = t_list.begin();
    int i = 0;
    for(const auto& idaughter: dau)
    {
      int it = t_list[i];
      if ( msgLevel(MSG::VERBOSE) )
        verbose() << "TupleToolHOP::ClassifyParticlesMerge: " << idaughter->particleID().pid() << " same child: " << hasSameChildren(it) << endmsg ;

      if (hasSameChildren(it)==1 || hasSameChildren(it)==2)
      {
        if ( msgLevel(MSG::VERBOSE) )
        {
          verbose() << "TupleToolHOP::ClassifyParticlesMerge: " << idaughter->particleID().pid() << " has same or no children" << endmsg;
          verbose() << "TupleToolHOP::ClassifyParticlesMerge: " << idaughter->particleID().pid() << " will be saved" << endmsg;
          verbose() << "TupleToolHOP::ClassifyParticlesMerge: " << idaughter->particleID().pid() << " is e: " << isElectron(it) << endmsg;
        }

        if (isElectron(it)==1)
        {
          electronContainer.push_back(idaughter);
          if ( msgLevel(MSG::VERBOSE) )
            verbose() << "TupleToolHOP::ClassifyParticlesMerge: " << idaughter->particleID().pid() << " saved as electron" << endmsg;
        }

        else
        {
          nonElectronContainer.push_back(idaughter);
          if ( msgLevel(MSG::VERBOSE) )
            verbose() << "TupleToolHOP::ClassifyParticlesMerge: " << idaughter->particleID().pid() << " saved as no electron" << endmsg;
        }
      }
      i++;
    }
  }

  if ( msgLevel(MSG::VERBOSE) )
  {
    verbose() << "TupleToolHOP::ClassifyParticlesMerge: " << "mother is e: " << t_mother_isElectron << endmsg;
    verbose() << "TupleToolHOP::ClassifyParticlesMerge: " << "mother has equal childs: " << t_mother_hasSameChildren << endmsg;
  }

  return encodeTuple(t_mother_isElectron, t_mother_hasSameChildren);
}

// =====================================================
// PT recalculation
// =====================================================
float TupleToolHOP::HOPProjectMomentum(const LHCb::Particle* top,
                                       const Gaudi::LorentzVector* part_four_mom) const{
       if ( msgLevel(MSG::VERBOSE) )
         verbose() << "TupleToolHOP::HOPProjectMomentum" << ": " << endmsg ;

      const LHCb::VertexBase* PV;
      const LHCb::VertexBase* SV;
      ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>> bFlightVector, bFlightVectorUnit;
      PV = m_dva->bestVertex (top);
      SV = top->endVertex();
      bFlightVector = SV->position() - PV->position();

      bFlightVectorUnit = (SV->position() - PV->position()).Unit();
      double pInBFlightDir = (part_four_mom->Px()*bFlightVectorUnit.X()
                              + part_four_mom->Py()*bFlightVectorUnit.Y()
                              + part_four_mom->Pz()*bFlightVectorUnit.Z()) ;
      double cosAngle = pInBFlightDir/part_four_mom->P();
      double ptInBFlightDir = part_four_mom->P()*sqrt(1-std::pow(cosAngle,2));

       if ( msgLevel(MSG::VERBOSE) )
       {
         verbose() << "TupleToolHOP::HOPProjectMomentum" << ": " << " PV: "<<  PV->position()   << endmsg ;
         verbose() << "TupleToolHOP::HOPProjectMomentum" << ": " << " SV: "<<  SV->position()   << endmsg ;
         verbose() << "TupleToolHOP::HOPProjectMomentum" << ": " << " b Flight vector: "<<  bFlightVector   << endmsg ;
         verbose() << "TupleToolHOP::HOPProjectMomentum" << ": " << " particle 4-momentum: "<<  part_four_mom   << endmsg ;
         verbose() << "TupleToolHOP::HOPProjectMomentum" << ": " << " Momentum Magnitude in bFlight direction: "<<  pInBFlightDir << endmsg ;
         verbose() << "TupleToolHOP::HOPProjectMomentum" << ": " << " Cosine of angle: "<<  cosAngle   << endmsg ;
         verbose() << "TupleToolHOP::HOPProjectMomentum" << ": " << " Pt wrt B flight direction: "<<  ptInBFlightDir   << endmsg ;
       }
      return ptInBFlightDir;
}
