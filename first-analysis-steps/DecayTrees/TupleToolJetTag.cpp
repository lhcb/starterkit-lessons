//-----------------------------------------------------------------------------
//
// Implementation file for class TupleToolJetTag
//
// Runs one jet tagging algorithm and writes the value out to the tuple
//
//-----------------------------------------------------------------------------

//=============================================================================
// Setup
#include "Event/Particle.h"
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
#include "Kernel/IJets2Jets.h"

#include "TupleToolJetTag.h"

DECLARE_COMPONENT( TupleToolJetTag )

    //=============================================================================
    // Constructor
TupleToolJetTag::TupleToolJetTag(const std::string& type,
        const std::string& name,
        const IInterface* parent)
    : TupleToolJetsBase(type, name , parent)
      , m_TagTool(NULL)
{
    declareInterface<IParticleTupleTool>(this);

    declareProperty("tagToolName",
            m_tagToolName,
            "name of jet tagger");

    declareProperty("tagToolLabel",
            m_tagToolLabel,
            "label of jet tagger for tuple");

    declareProperty( "VerboseTag", m_verboseTag=false );


}


//=============================================================================
// Initalise
StatusCode TupleToolJetTag::initialize()
{
    const StatusCode sc = TupleToolBase::initialize();
    if ( sc.isFailure() ) return sc;

    m_TagTool = tool<IJetTagTool>(m_tagToolName,m_tagToolName, this);

    return sc;
}


//=============================================================================
// Execute
StatusCode TupleToolJetTag::fill(const LHCb::Particle*,
        const LHCb::Particle* jet,
        const std::string& head,
        Tuples::Tuple& tuple)
{
    bool result = true;
    if ( jet )
    {
        const std::string prefix = fullName(head);

        // Run jet tag tool
        std::map<std::string,double> property;
        m_TagTool->calculateJetProperty(jet, property);
        if(property.empty())
            if(msgLevel(MSG::ERROR))  debug() << "Write tag to ntuple --- not found "<< endmsg;
        if(m_tagToolName!="LoKi::BDTTag"){
            for( std::map<std::string, double>::iterator  it = property.begin();
                    it != property.end(); ++it )
            {

                    result &= (tuple)->column(prefix + "_" + m_tagToolLabel +"_" + (it->first) , (it->second));

            }
        }else{
            //spacial case for BDTTag.
            std::vector<double> fdrMin,ptSvrJet,nTrk,nTrkJet,drSvrJet;
            std::vector<double> absQSum,m,mCor,fdChi2,ipChi2Sum;
            std::vector<double>bdt0,bdt1,pass,tau,z,pt,backwards;
            for (int idx = 0;idx!=(int) property.size()+1;idx++){

                std::stringstream pre; pre << "Tag" << idx << "_";

                fdrMin.push_back(property[pre.str()+"fdrMin"]);
                ptSvrJet.push_back(property[pre.str()+"ptSvrJet"]);
                nTrk.push_back(property[pre.str()+"nTrk"]);
                nTrkJet.push_back(property[pre.str()+"nTrkJet"]);
                drSvrJet.push_back(property[pre.str()+"drSvrJet"]);
                absQSum.push_back(property[pre.str()+"absQSum"]);
                m.push_back(property[pre.str()+"m"]);
                mCor.push_back(property[pre.str()+"mCor"]);
                fdChi2.push_back(property[pre.str()+"fdChi2"]);
                ipChi2Sum.push_back(property[pre.str()+"ipChi2Sum"]);
                bdt0.push_back(property[pre.str()+"bdt0"]);
                bdt1.push_back(property[pre.str()+"bdt1"]);
                pass.push_back(property[pre.str()+"pass"]);
                tau.push_back(property[pre.str()+"tau"]);
                z.push_back(property[pre.str()+"z"]);
                pt.push_back(property[pre.str()+"pt"]);
                backwards.push_back(property[pre.str()+"backwards"]);
                int        idx2 = idx +1;
                std::stringstream pre2; pre2 << "Tag" << idx2 << "_";
                if(property.find(pre2.str()+"fdrMin") == property.end())
                    break;
            }

        result &= (tuple)->column(prefix + "_" + m_tagToolLabel +"_Tag" , property["Tag"]);
            result &= (tuple)->farray(prefix + "_" + m_tagToolLabel +"_fdrMin" , fdrMin.begin(), fdrMin.end(), prefix + "_" + m_tagToolLabel +"_NbTag", 20);
            result &= (tuple)->farray(prefix + "_" + m_tagToolLabel +"_ptSvrJet" , ptSvrJet.begin(), ptSvrJet.end(), prefix + "_" + m_tagToolLabel +"_NbTag", 20);
            result &= (tuple)->farray(prefix + "_" + m_tagToolLabel +"_nTrk" , nTrk.begin(), nTrk.end(), prefix + "_" + m_tagToolLabel +"_NbTag", 20);
            result &= (tuple)->farray(prefix + "_" + m_tagToolLabel +"_nTrkJet" , nTrkJet.begin(), nTrkJet.end(), prefix + "_" + m_tagToolLabel +"_NbTag", 20);
            result &= (tuple)->farray(prefix + "_" + m_tagToolLabel +"_drSvrJet" , drSvrJet.begin(), drSvrJet.end(), prefix + "_" + m_tagToolLabel +"_NbTag", 20);
            result &= (tuple)->farray(prefix + "_" + m_tagToolLabel +"_absQSum", absQSum.begin(), absQSum.end(), prefix + "_" + m_tagToolLabel +"_NbTag", 20);
            result &= (tuple)->farray(prefix + "_" + m_tagToolLabel +"_m", m.begin(), m.end(), prefix + "_" + m_tagToolLabel +"_NbTag", 20);
            result &= (tuple)->farray(prefix + "_" + m_tagToolLabel +"_mCor", mCor.begin(), mCor.end(), prefix + "_" + m_tagToolLabel +"_NbTag", 20);
            result &= (tuple)->farray(prefix + "_" + m_tagToolLabel +"_fdChi2", fdChi2.begin(), fdChi2.end(), prefix + "_" + m_tagToolLabel +"_NbTag", 20);
            result &= (tuple)->farray(prefix + "_" + m_tagToolLabel +"_ipChi2Sum", ipChi2Sum.begin(), ipChi2Sum.end(), prefix + "_" + m_tagToolLabel +"_NbTag", 20);
            result &= (tuple)->farray(prefix + "_" + m_tagToolLabel +"_bdt0", bdt0.begin(), bdt0.end(), prefix + "_" + m_tagToolLabel +"_NbTag", 20);
            result &= (tuple)->farray(prefix + "_" + m_tagToolLabel +"_bdt1", bdt1.begin(), bdt1.end(), prefix + "_" + m_tagToolLabel +"_NbTag", 20);
            result &= (tuple)->farray(prefix + "_" + m_tagToolLabel +"_pass", pass.begin(), pass.end(), prefix + "_" + m_tagToolLabel +"_NbTag", 20);
            result &= (tuple)->farray(prefix + "_" + m_tagToolLabel +"_tau", tau.begin(), tau.end(), prefix + "_" + m_tagToolLabel +"_NbTag", 20);
            result &= (tuple)->farray(prefix + "_" + m_tagToolLabel +"_z", z.begin(), z.end(), prefix + "_" + m_tagToolLabel +"_NbTag", 20);
            result &= (tuple)->farray(prefix + "_" + m_tagToolLabel +"_pt", pt.begin(), pt.end(), prefix + "_" + m_tagToolLabel +"_NbTag", 20);
            result &= (tuple)->farray(prefix + "_" + m_tagToolLabel +"_backwards", backwards.begin(), backwards.end(), prefix + "_" + m_tagToolLabel +"_NbTag", 20);

        }
    }

    return StatusCode(result);
}
