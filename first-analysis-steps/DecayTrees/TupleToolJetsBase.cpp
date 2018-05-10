#include "TupleToolJetsBase.h"
#include "LoKi/ParticleCuts.h"
#include "LoKi/AParticleCuts.h"

TupleToolJetsBase::TupleToolJetsBase( const std::string& type,
        const std::string& name,
        const IInterface* parent)
: TupleToolBase ( type, name , parent ),
    charge(LoKi::Cuts::ONE),
    positiveParticles(LoKi::Cuts::ONE ),
    negativeParticles(LoKi::Cuts::ONE ),
    neutralParticles(LoKi::Cuts::ONE ),
    maxPT(LoKi::Cuts::ONE ),
    m_M(LoKi::Cuts::ONE ),
    m_MM(LoKi::Cuts::ONE )
{}
#define SAVEPOINT(POINT,FUN)                    \
    (POINT?POINT->FUN:-1.0)
bool TupleToolJetsBase::WriteJetToTuple(const LHCb::Particle*jet,std::string prefix)
{
    //filter plus and minus signs out to the prefix
    std::map<std::string,std::string> replacements ;
    replacements["+"]="plus";
    replacements["-"]="minus";
    for (std::map<std::string,std::string>::iterator it = replacements.begin();it != replacements.end();++it)
    {
        size_t point = 0;
        while((point = prefix.find(it->first))!=std::string::npos)
            prefix = prefix.replace(point,it->second.size(),it->second);
    }

        if ( msgLevel(MSG::DEBUG) ) debug() << (jet?"Filling jet object ":"Filling dummy jet object ") << jet << " " << prefix << " in tuple at address "<<m_tuple<< endmsg;

    bool result = true;
    result &= (*m_tuple)->column( prefix+"_PX", (double)(  SAVEPOINT(jet,momentum().Px())));
    result &= (*m_tuple)->column( prefix+"_PY", (double)(  SAVEPOINT(jet,momentum().Py())));
    result &= (*m_tuple)->column( prefix+"_PZ", (double)(  SAVEPOINT(jet,momentum().Pz())));
    result &= (*m_tuple)->column( prefix+"_PE", (double)(  SAVEPOINT(jet,momentum().e())));
    result &= (*m_tuple)->column( prefix+"_PT", (double)(  SAVEPOINT(jet,momentum().Pt())));
    result &= (*m_tuple)->column( prefix+"_P", (double)(   SAVEPOINT(jet,momentum().P())));
    result &= (*m_tuple)->column( prefix+"_Eta", (double)( SAVEPOINT(jet,momentum().Eta())));
    result &= (*m_tuple)->column( prefix+"_Phi", (double)( SAVEPOINT(jet,momentum().Phi())));
    result &= (*m_tuple)->column( prefix+"_M", (double)( SAVEPOINT(jet,momentum().mass())));

    result &= (*m_tuple)->column( prefix+"_MLoKi", (double)(jet?m_M(jet):-99.0));
    result &= (*m_tuple)->column( prefix+"_MMLoKi", (double)( jet?m_MM(jet):-99.0));
    result &= (*m_tuple)->column( prefix+"_NTrk", (double)( jet->info(LHCb::JetIDInfo::Ntracks,-999.9)));
    result &= (*m_tuple)->column( prefix+"_N90", (double)( jet->info(LHCb::JetIDInfo::N90,-999.9)));
    result &= (*m_tuple)->column( prefix+"_MTF", (double)( jet->info(LHCb::JetIDInfo::MTF,-999.9)));
    result &= (*m_tuple)->column( prefix+"_NSatCells", (double)( jet->info(LHCb::JetIDInfo::NSatCalo,-999.9)));
    result &= (*m_tuple)->column( prefix+"_NHasPVInfo", (double)( jet->info(LHCb::JetIDInfo::NHasPV,-999.9)));
    result &= (*m_tuple)->column( prefix+"_JEC_Cor", (double)( jet->info(LHCb::JECInfo::JEC,-999.9)));
    result &= (*m_tuple)->column( prefix+"_JEC_PV", (double)( jet->info(LHCb::JECInfo::NPVsForJEC,-999.9)));
    result &= (*m_tuple)->column( prefix+"_JEC_Error", (double)( jet->info(LHCb::JECInfo::JECError,-999.9)));



    double weigth = 1.0;
    double myPT  = (SAVEPOINT(jet,momentum().Pt()));


    if(myPT < 20000)
        weigth = 1 - ((20000-myPT)/16000);

    //weigth for 5GeV jet:


    result &= (*m_tuple)->column( prefix+"_w", (double)( weigth));
    result &= (*m_tuple)->column( prefix+"_CPF", (double)( jet->info(LHCb::JetIDInfo::CPF,-999.9)));
    result &= (*m_tuple)->column( prefix+"_JetWidth", (double)( jet->info(LHCb::JetIDInfo::JetWidth,-999.9)));
    result &= (*m_tuple)->column( prefix+"_NSatECAL", (double)( jet->info(LHCb::JetIDInfo::NSatECAL,-999.9)));
    result &= (*m_tuple)->column( prefix+"_NSatHCAL", (double)( jet->info(LHCb::JetIDInfo::NSatHCAL,-999.9)));
    result &= (*m_tuple)->column( prefix+"_NIPChi2Inf4", (double)( jet->info(LHCb::JetIDInfo::NIPChi2Inf4,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfCharged", (double)( jet->info(LHCb::JetEnergyFractionInfo::Charged,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfChargedHadron", (double)( jet->info(LHCb::JetEnergyFractionInfo::ChargedHadron,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfMuon", (double)( jet->info(LHCb::JetEnergyFractionInfo::Muon,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfElectron", (double)( jet->info(LHCb::JetEnergyFractionInfo::Electron,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfNeutral", (double)( jet->info(LHCb::JetEnergyFractionInfo::Neutral,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfPhoton", (double)( jet->info(LHCb::JetEnergyFractionInfo::Photon,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfPi0", (double)( jet->info(LHCb::JetEnergyFractionInfo::Pi0,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfMPi0", (double)( jet->info(LHCb::JetEnergyFractionInfo::MergedPi0,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfRPi0", (double)( jet->info(LHCb::JetEnergyFractionInfo::ResolvedPi0,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfNHad", (double)( jet->info(LHCb::JetEnergyFractionInfo::NeutralHadron,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfNReco", (double)( jet->info(LHCb::JetEnergyFractionInfo::NeutralRecovery,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfCompo", (double)( jet->info(LHCb::JetEnergyFractionInfo::Composite,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfV0", (double)( jet->info(LHCb::JetEnergyFractionInfo::V0,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfD", (double)( jet->info(LHCb::JetEnergyFractionInfo::D,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfB", (double)( jet->info(LHCb::JetEnergyFractionInfo::B,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfBadParticle", (double)( jet->info(LHCb::JetEnergyFractionInfo::BadParticle,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfCharged0Momentum", (double)( jet->info(LHCb::JetEnergyFractionInfo::Charged0Momentum,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfChargedInfMomentum", (double)( jet->info(LHCb::JetEnergyFractionInfo::ChargedInfMomentum,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfBadPhotonMatchingT", (double)( jet->info(LHCb::JetEnergyFractionInfo::BadPhotonMatchingT,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfBadPhoton", (double)( jet->info(LHCb::JetEnergyFractionInfo::BadPhoton,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfIsolatedPhoton", (double)( jet->info(LHCb::JetEnergyFractionInfo::IsolatedPhoton,-999.9)));
    result &= (*m_tuple)->column( prefix+"_NLongTrk", (double)( jet->info(LHCb::JetEnergyFractionInfo::NLongTrk,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfLongTrk", (double)( jet->info(LHCb::JetEnergyFractionInfo::EfLongTrk,-999.9)));
    result &= (*m_tuple)->column( prefix+"_NDownTrk", (double)( jet->info(LHCb::JetEnergyFractionInfo::NDownTrk,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfDownTrk", (double)( jet->info(LHCb::JetEnergyFractionInfo::EfDownTrk,-999.9)));
    result &= (*m_tuple)->column( prefix+"_NUpTrk", (double)( jet->info(LHCb::JetEnergyFractionInfo::NUpTrk,-999.9)));
    result &= (*m_tuple)->column( prefix+"_EfUpTrk", (double)( jet->info(LHCb::JetEnergyFractionInfo::EfUpTrk,-999.9)));

    result &= (*m_tuple)->column( prefix+"_PU_NPVs", (double)( jet->info(LHCb::JetPileUpInfo::PUnPVs,-999.9)));
    result &= (*m_tuple)->column( prefix+"_PU_Cone06_SumPTLongTrk", (double)( jet->info(LHCb::JetPileUpInfo::PUSumPTLongTrkCone06,-999.9)));
    result &= (*m_tuple)->column( prefix+"_PU_Cone06_SumELongTrk", (double)( jet->info(LHCb::JetPileUpInfo::PUSumELongTrkCone06,-999.9)));
    result &= (*m_tuple)->column( prefix+"_PU_Cone06_NLongTrk", (double)( jet->info(LHCb::JetPileUpInfo::PUNLongTrkCone06,-999.9)));
    result &= (*m_tuple)->column( prefix+"_PU_Cone06_NUpVeloTrk", (double)( jet->info(LHCb::JetPileUpInfo::PUNUpVeloTrkCone06,-999.9)));
    result &= (*m_tuple)->column( prefix+"_PU_Cone10_SumPTLongTrk", (double)( jet->info(LHCb::JetPileUpInfo::PUSumPTLongTrkCone10,-999.9)));
    result &= (*m_tuple)->column( prefix+"_PU_Cone10_SumELongTrk", (double)( jet->info(LHCb::JetPileUpInfo::PUSumELongTrkCone10,-999.9)));
    result &= (*m_tuple)->column( prefix+"_PU_Cone10_NLongTrk", (double)( jet->info(LHCb::JetPileUpInfo::PUNLongTrkCone10,-999.9)));
    result &= (*m_tuple)->column( prefix+"_PU_Cone10_NUpVeloTrk", (double)( jet->info(LHCb::JetPileUpInfo::PUNUpVeloTrkCone10,-999.9)));
    result &= (*m_tuple)->column( prefix+"_PU_Cone15_SumPTLongTrk", (double)( jet->info(LHCb::JetPileUpInfo::PUSumPTLongTrkCone15,-999.9)));
    result &= (*m_tuple)->column( prefix+"_PU_Cone15_SumELongTrk", (double)( jet->info(LHCb::JetPileUpInfo::PUSumELongTrkCone15,-999.9)));
    result &= (*m_tuple)->column( prefix+"_PU_Cone15_NLongTrk", (double)( jet->info(LHCb::JetPileUpInfo::PUNLongTrkCone15,-999.9)));
    result &= (*m_tuple)->column( prefix+"_PU_Cone15_NUpVeloTrk", (double)( jet->info(LHCb::JetPileUpInfo::PUNUpVeloTrkCone15,-999.9)));












/*


















    result &= (*m_tuple)->column( prefix+"_ParticleMultiplicity", (double)(  SAVEPOINT(jet,daughters().size())));
  //  result &= (*m_tuple)->column( prefix+"_Charge", (double)( jet?charge(jet):-1.0));
    result &= (*m_tuple)->column( prefix+"_positiveParticleMultiplicity", (double)( jet?positiveParticles(jet):-1.0));
    result &= (*m_tuple)->column( prefix+"_negativeParticleMultiplicity", (double)( jet?negativeParticles(jet):-1.0));
    result &= (*m_tuple)->column( prefix+"_neutralParticleMultiplicity", (double)( jet?neutralParticles(jet):-1.0));
    result &= (*m_tuple)->column( prefix+"_chargedParticleMultiplicity", (double)( jet?positiveParticles(jet)+negativeParticles(jet):-1.0));
    result &= (*m_tuple)->column( prefix+"_maxPT", (double)( jet?maxPT(jet):-1.0));
   SmartRefVector< LHCb::Particle > SortedDaughters;
    //buffer to store intermediate result to speed things up a bit
    result &= (*m_tuple)->column( prefix+"_PT1", (double)(  jet?MaxSumNPart(jet,1, LoKi::Cuts::PT,&SortedDaughters):-1.0));
    result &= (*m_tuple)->column( prefix+"_PT2", (double)(  jet?MaxSumNPart(jet,2, LoKi::Cuts::PT,&SortedDaughters):-1.0));
    result &= (*m_tuple)->column( prefix+"_PT3", (double)(  jet?MaxSumNPart(jet,3, LoKi::Cuts::PT,&SortedDaughters):-1.0));
    result &= (*m_tuple)->column( prefix+"_PT4", (double)(  jet?MaxSumNPart(jet,4, LoKi::Cuts::PT,&SortedDaughters):-1.0));
    result &= (*m_tuple)->column( prefix+"_PT5", (double)(  jet?MaxSumNPart(jet,5, LoKi::Cuts::PT,&SortedDaughters):-1.0));
    result &= (*m_tuple)->column( prefix+"_PT10", (double)( jet?MaxSumNPart(jet,10,LoKi::Cuts::PT,&SortedDaughters):-1.0));
    result &= (*m_tuple)->column( prefix+"_PT15", (double)( jet?MaxSumNPart(jet,15,LoKi::Cuts::PT,&SortedDaughters):-1.0));
    result &= (*m_tuple)->column( prefix+"_PT25", (double)( jet?MaxSumNPart(jet,25,LoKi::Cuts::PT,&SortedDaughters):-1.0));

*/













    return result;
}

StatusCode TupleToolJetsBase::initialize()
{
    const StatusCode sc = TupleToolBase::initialize();
    if ( sc.isFailure() ) return sc;
    //get LoKi objects
    //charge = LoKi::Cuts::SUMTREE (LoKi::Cuts::Q, LoKi::Cuts::ALL, 0.0 ) ;
    positiveParticles = LoKi::Cuts::NINTREE (  LoKi::Cuts::Q > 0 );
    negativeParticles = LoKi::Cuts::NINTREE (  LoKi::Cuts::Q < 0 );
    neutralParticles  = LoKi::Cuts::NINTREE (  LoKi::Cuts::Q == 0 );
    maxPT = LoKi::Cuts::MAXTREE(LoKi::Cuts::PT,LoKi::Cuts::BASIC,-1);
    m_M = LoKi::Particles::Mass();
    m_MM = LoKi::Particles::MeasuredMass();
    return sc ;
}

double TupleToolJetsBase::MaxSumNPart(const LHCb::Particle* jet,
        unsigned int n,
        const LoKi::Types::Fun& fun,
        SmartRefVector< LHCb::Particle >* SortedDaughters)
{
    //  fun == LoKi::Cuts::ONE;
   // if (jet->daughters().size() <= n)
    //    return LoKi::Cuts::SUMTREE ( LoKi::Cuts::BASIC, fun)(jet);
    if (SortedDaughters&&SortedDaughters->size()) // Vector given and filled
        return LoKi::Cuts::ASUM (fun)(LHCb::Particle::ConstVector(SortedDaughters->begin(),SortedDaughters->begin()+n));
    SmartRefVector< LHCb::Particle > daughters; //empty vector to take address of in case SortedDaughters == NULL
    if (!SortedDaughters)
        SortedDaughters = &daughters;
    SortedDaughters->assign(jet->daughters().begin(),jet->daughters().end());
    sort(SortedDaughters->begin(),SortedDaughters->end(),Comperator<const LHCb::Particle*,const LHCb::Particle*>(fun));
    return LoKi::Cuts::ASUM (fun)(LHCb::Particle::ConstVector(SortedDaughters->begin(),SortedDaughters->begin()+n));
}
