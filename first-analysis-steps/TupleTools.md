# List of TupleTool Variables

{% objectives "Learning Objectives" %}
* What are each TupleTool's variables?
{% endobjectives %} 

{% callout "List of DecayTreeTuples - General Categories" %}
DecayTreeTuples:

- DecayTreeTuple - for the more general tools.
- DecayTreeTupleANNPID - for the NeuralNet-based PID tools. [Class Reference here.](http://lhcb-doxygen.web.cern.ch/lhcb-doxygen/analysis/latest/dc/dd6/class_tuple_tool_a_n_n_p_i_d.html)
- DecayTreeTupleDalitz - for Dalitz analysis. [Class Reference here.](http://lhcb-doxygen.web.cern.ch/lhcb-doxygen/analysis/latest/df/d6a/class_tuple_tool_dalitz.html)
- DecayTreeTupleJets - for obtaining information on jets. [Class Reference here.](http://lhcb-doxygen.web.cern.ch/lhcb-doxygen/analysis/latest/d9/d48/class_tuple_tool_jets.html)
- DecayTreeTupleMC - gives us access to MC-level information. [Class Reference here.](http://lhcb-doxygen.web.cern.ch/lhcb-doxygen/analysis/latest/da/dd6/class_tuple_tool_m_c_truth.html)
- DecayTreeTupleMuonCalib - for muon calibration tools. [Class Reference here.](http://lhcb-doxygen.web.cern.ch/lhcb-doxygen/analysis/latest/d4/de9/class_tuple_tool_muon_i_d_calib.html)
- DecayTreeTupleReco - for reconstruction-level information, such as TupleToolTrackInfo. [Class Reference here.](http://lhcb-doxygen.web.cern.ch/lhcb-doxygen/analysis/latest/da/ddd/class_tuple_tool_track_info.html)
- DecayTreeTupleTracking - for more detailed tools regarding tracking. [Class Reference here.](http://lhcb-doxygen.web.cern.ch/lhcb-doxygen/analysis/latest/d4/d31/class_tuple_tool_cone_isolation.html)
- DecayTreeTupleTrigger - for accessing to the trigger information of the candidates. [Class Reference here.](http://lhcb-doxygen.web.cern.ch/lhcb-doxygen/analysis/latest/d3/dd1/class_tuple_tool_trigger.html)
{% endcallout %}


Below you will find links to the source code and header files for each of the TupleTools.

 
{% callout "DecayTreeTuple - for the more general tools" %}
DecayTreeTuple:
 - [TupleToolAllTracks.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolAllTracks.cpp)
 - [TupleToolAllTracks.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolAllTracks.h)
 - [TupleToolAngles.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolAngles.cpp)
 - [TupleToolAngles.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolAngles.h)

1. _CosTheta, 
2. _Theta, 

 - [TupleToolBeamSpot.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolBeamSpot.cpp)
 - [TupleToolBeamSpot.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolBeamSpot.h)

1. _BeamX, 
2. _BeamY, 

 - [TupleToolBremInfo.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolBremInfo.cpp)
 - [TupleToolBremInfo.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolBremInfo.h)

1. _DiEBremMultiplicity, 
2. _DiEBremP, 
3. _BremOrigin, 
4. _BremP, 
5. _HasBremAdded, 


 - [TupleToolCaloDigits.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolCaloDigits.cpp)
 - [TupleToolCaloDigits.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolCaloDigits.h)
 - [TupleToolCaloHypo.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolCaloHypo.cpp)
 - [TupleToolCaloHypo.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolCaloHypo.h)

_hasCaloHypo, 
_hasBremCaloHypo, 
_CaloHypo_E, 
_CaloHypo_X, 
_CaloHypo_Y, 
_CaloHypo_Z, 
_CaloHypo_DeltaTheta, 
_CaloHypoDaugh0_E, 
_CaloHypoDaugh0_X, 
_CaloHypoDaugh0_Y, 
_CaloHypoDaugh0_Z, 
_CaloHypoDaugh1_E,
_CaloHypoDaugh1_X, 
_CaloHypoDaugh1_Y, 
_CaloHypoDaugh1_Z, 
_CaloHypoPileUpEnergy, 
_CaloHypo, 
_BremCaloHypo, 
_CaloHypoDaugh0,
_CaloHypoDaugh1, 

 - [TupleToolCorrectedMass.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolCorrectedMass.cpp)
 - [TupleToolCorrectedMass.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolCorrectedMass.h)

_MCORR, 
_MCORRERR, 
_MCORRVERTEXERR, 

 - [TupleToolDecayTreeFitter.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolDecayTreeFitter.cpp)
 - [TupleToolDecayTreeFitter.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolDecayTreeFitter.h)
 - [TupleToolDira.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolDira.cpp)
 - [TupleToolDira.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolDira.h)

_DiraAngleError, 
_DiraCosError, 
_DiraAngle, 
_DiraCos, 

 - [TupleToolGeometry.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolGeometry.cpp)
 - [TupleToolGeometry.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolGeometry.h)

_MINIP, 
_MINIPCHI2, 
_MINIPNEXTBEST, 
_MINIPCHI2NEXTBEST, 
_XERR, 
_YERR, 
_ZERR, 
_CHI2, 
_NDOF, 



 - [TupleToolHOP.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolHOP.cpp)
 - [TupleToolHOP.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolHOP.h)

_HOP, 
_HOP_ELECTRON_MASS, 

 - [TupleToolIsolationTwoBody.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolIsolationTwoBody.cpp)
 - [TupleToolIsolationTwoBody.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolIsolationTwoBody.h)

_test, 
_MuonChi2DOF, 
_MuonChi2DOF, 
_P1_massiso, 
_P2_massiso, 
_P1_Q, 
_P2_Q, 
_P1_chi2iso, 
_P2_chi2iso, 
_P1_iso5, 
_P2_iso5, 

 - [TupleToolKinematic.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolKinematic.cpp)
 - [TupleToolKinematic.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolKinematic.h)


_P, 
_PT, 
_REFP, 
_PreFitMass, 
_MM, 
_MMERR, 
_M, 
_AtVtx_P, 


 - [TupleToolMassHypo.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolMassHypo.cpp)
 - [TupleToolMassHypo.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolMassHypo.h)

_M_with,

 - [TupleToolNeutrinoReco.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolNeutrinoReco.cpp)
 - [TupleToolNeutrinoReco.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolNeutrinoReco.h)

_Nu_Parl, 
_Nu_Perp, 
_Nu_a, 
_Nu_X, 
_Nu_Y, 
_Nu_pb, 
_Nu_pd, 
_Nu_Hi, 
_Nu_Lo, 

 - [TupleToolP2VV.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolP2VV.cpp)
 - [TupleToolP2VV.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolP2VV.h)

_ThetaL, 
_ThetaK, 
_Phi, 
_ThetaTr, 
_PhiTr, 
_ThetaVtr, 

 - [TupleToolParticleReFit.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolParticleReFit.cpp)
 - [TupleToolParticleReFit.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolParticleReFit.h)

_status, 
_ENDVERTEX_NDOF, 
_ENDVERTEX_CHI2, 
_MM, 
_MMERR, 
_P, 
_PT, 
_P, 
_TAU, 
_TAUERR, 
_TAUCHI2, 

 - [TupleToolParticleStats.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolParticleStats.cpp)
 - [TupleToolParticleStats.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolParticleStats.h)
 - [TupleToolPhotonInfo.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolPhotonInfo.cpp)
 - [TupleToolPhotonInfo.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolPhotonInfo.h)

_Converted, 
_Prs, 
_Matching, 
_PID, 
_CL ,
_ShowerShape, 

 - [TupleToolPi0Info.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolPi0Info.cpp)
 - [TupleToolPi0Info.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolPi0Info.h)

_Type, 
_CL, 
_gamma0_PT, 
_gamma1_PT, 
_MCPi0_id, 
_MCPi0_key, 
_MCPi0Mother_id, 
_MCPi0Mother_key, 
_MCPi0GrandMother_id, 
_MCPi0GrandMother_key, 
_MCPi0GrandGrandMother_id, 
_MCPi0GrandGrandMother_key, 





 - [TupleToolPid.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolPid.cpp)
 - [TupleToolPid.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolPid.h)


_ID, 
_PIDe, 
_PIDmu, 
_PIDK, 
_PIDp, 
_ProbNNe, 
_ProbNNk, 
_ProbNNp, 
_ProbNNpi, 
_ProbNNmu, 
_ProbNNghost, 
_hasMuon, 
_isMuon, 
_hasRich, 
_UsedRichAerogel, 
_UsedRich1Gas, 
_UsedRich2Gas, 
_RichAboveElThres, 
_RichAboveMuThres, 
_RichAbovePiThres, 
_RichAboveKaThres, 
_RichAbovePrThres, 
_hasCalo, 
_RichDLLe, 
_RichDLLmu, 
_RichDLLpi, 
_RichDLLk, 
_RichDLLp, 
_RichDLLbt, 
_InAccMuon, 
_isMuonLoose, 
_isMuonTight, 
_MuonMuLL, 
_MuonBkgLL, 
_MuonNShared, 
_MuonChi2Corr, 
_InAccEcal, 
_CaloEcalE, 
_EcalPIDe, 
_EcalPIDmu, 
_InAccHcal, 
_CaloHcalE, 
_HcalPIDe, 
_HcalPIDmu, 
_InAccPrs, 
_PrsPIDe, 
_CaloPrsE, 
_InAccSpd, 
_CaloSpdE, 
_InAccBrem, 
_BremPIDe, 
_VeloCharge, 


 - [TupleToolPropertime.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolPropertime.cpp)
 - [TupleToolPropertime.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolPropertime.h)
 
 _TAU, 
_TAUERR, 
_TAUCHI2, 

 - [TupleToolSelResults.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolSelResults.cpp)
 - [TupleToolSelResults.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolSelResults.h)
 
 _M
_VtxChi2_+flag , 
_VtxnDoF+flag, 
_VtxM+flag , 
_IP+flag, 
_IPChi2_+flag, 

 - [TupleToolSubMass.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolSubMass.cpp)
 - [TupleToolSubMass.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolSubMass.h)
 - [TupleToolSwimmingInfo.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolSwimmingInfo.cpp)
 - [TupleToolSwimmingInfo.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolSwimmingInfo.h)
 - [TupleToolTagging.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolTagging.cpp)
 - [TupleToolTagging.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolTagging.h)
 
 _TAGDECISION, 
_TAGOMEGA, 
_TAGDECISION_OS, 
_TAGOMEGA_OS, 
_TAGGER, 
_DEC, 
_PROB, 

 - [TupleToolTwoParticleMatching.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolTwoParticleMatching.cpp)
 - [TupleToolTwoParticleMatching.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolTwoParticleMatching.h)
 
 _NMatchingCand_+fullName(head)+m_headSuffix, 
_Overlap_loaded_+fullName(head)+m_headSuffix, 
_Overlap_original_+fullName(head)+m_headSuffix,  

 - [TupleToolVeto.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolVeto.cpp)
 - [TupleToolVeto.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolVeto.h)
 
_Veto+flag, 
_VetoOther+flag, 

 - [TupleToolVtxIsoln.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolVtxIsoln.cpp)
 - [TupleToolVtxIsoln.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolVtxIsoln.h)
 
_VtxIsoln.m_varNames[index], 
_VtxIsoln.name, 

{% endcallout %} 

{% callout "DecayTreeTupleANNPID - for the NeuralNet-based PID tools" %}
DecayTreeTupleANNPID 
 - [TupleToolANNPID.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleANNPID/src/TupleToolANNPID.cpp)
 - [TupleToolANNPID.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleANNPID/src/TupleToolANNPID.h)

_prefix+_ProbNNe, 
_prefix+_ProbNNmu, 
_prefix+_ProbNNpi, 
_prefix+_ProbNNk, 
_prefix+_ProbNNp, 
_prefix+_ProbNNd, 
_prefix+_ProbNNghost,
 
 - [TupleToolANNPIDTraining.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleANNPID/src/TupleToolANNPIDTraining.cpp)
 - [TupleToolANNPIDTraining.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleANNPID/src/TupleToolANNPIDTraining.h)
{% endcallout %} 

{% callout "DecayTreeTupleBase" %}
DecayTreeTupleBase 
{% endcallout %} 

{% callout "DecayTreeTupleDalitz - for Dalitz analysis" %}
DecayTreeTupleDalitz 
 - [TupleToolDalitz.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleDalitz/src/TupleToolDalitz.cpp)
 - [MCTupleToolDalitz.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleDalitz/src/MCTupleToolDalitz.h)
 - [TupleToolDalitz.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleDalitz/src/TupleToolDalitz.cpp)
 - [TupleToolDalitz.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleDalitz/src/TupleToolDalitz.h)
 - [TupleToolDalitz.icpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleDalitz/src/TupleToolDalitz.icpp)
{% endcallout %} 

{% callout "DecayTreeTupleHerschel" %}
DecayTreeTupleHerschel 
 - [TupleToolHerschel.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleHerschel/src/TupleToolHerschel.cpp)
 - [TupleToolHerschel.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleHerschel/src/TupleToolHerschel.h)

{% endcallout %} 

{% callout "DecayTreeTupleJets - for obtaining information on jets" %}
DecayTreeTupleJets 
 - [TupleToolJetRelations.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleJets/src/TupleToolJetRelations.cpp)
 - [TupleToolJetRelations.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleJets/src/TupleToolJetRelations.h)
 - [TupleToolJetsBase.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleJets/src/TupleToolJetsBase.cpp)
 - [TupleToolJetsBase.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleJets/src/TupleToolJetsBase.h)
 - [TupleToolJets.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleJets/src/TupleToolJets.cpp)
 - [TupleToolJetsForB.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleJets/src/TupleToolJetsForB.cpp)
 - [TupleToolJetsForB.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleJets/src/TupleToolJetsForB.h)
 - [TupleToolJets.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleJets/src/TupleToolJets.h)
 - [TupleToolJetTag.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleJets/src/TupleToolJetTag.cpp)
 - [TupleToolJetTag.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleJets/src/TupleToolJetTag.h)
 - [TupleToolWZJets.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleJets/src/TupleToolWZJets.cpp)
 - [TupleToolWZJets.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleJets/src/TupleToolWZJets.h)
{% endcallout %} 

{% callout "DecayTreeTupleMC - gives us access to MC-level information" %}
DecayTreeTupleMC 
 - [MCTupleToolAngles.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolAngles.cpp)
 - [MCTupleToolAngles.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolAngles.h)
 - [MCTupleToolDecayType.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolDecayType.cpp)
 - [MCTupleToolDecayType.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolDecayType.h)
 - [MCTupleToolEventType.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolEventType.cpp)
 - [MCTupleToolEventType.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolEventType.h)
 - [MCTupleToolHierarchy.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolHierarchy.cpp)
 - [MCTupleToolHierarchy.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolHierarchy.h)
 - [MCTupleToolInteractions.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolInteractions.cpp)
 - [MCTupleToolInteractions.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolInteractions.h)
 - [MCTupleToolKinematic.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolKinematic.cpp)
 - [MCTupleToolKinematic.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolKinematic.h)
 - [MCTupleToolP2VV.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolP2VV.cpp)
 - [MCTupleToolP2VV.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolP2VV.h)
 - [MCTupleToolPID.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolPID.cpp)
 - [MCTupleToolPID.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolPID.h)
 - [MCTupleToolPrimaries.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolPrimaries.cpp)
 - [MCTupleToolPrimaries.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolPrimaries.h)
 - [MCTupleToolPrompt.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolPrompt.cpp)
 - [MCTupleToolPrompt.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolPrompt.h)
 - [MCTupleToolReconstructed.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolReconstructed.cpp)
 - [MCTupleToolReconstructed.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolReconstructed.h)
 - [MCTupleToolRedecay.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolRedecay.cpp)
 - [MCTupleToolRedecay.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/MCTupleToolRedecay.h)
 - [TupleToolGeneration.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/TupleToolGeneration.cpp)
 - [TupleToolGeneration.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/TupleToolGeneration.h)
 - [TupleToolMCBackgroundInfo.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/TupleToolMCBackgroundInfo.cpp)
 - [TupleToolMCBackgroundInfo.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/TupleToolMCBackgroundInfo.h)
 - [TupleToolMCTruth.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/TupleToolMCTruth.cpp)
 - [TupleToolMCTruth.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMC/src/TupleToolMCTruth.h)

{%endcallout %}

{% callout "DecayTreeTupleMuonCalib - for muon calibration tools" %}
DecayTreeTupleMuonCalib 
 - [TupleToolMuonIDCalib.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMuonCalib/src/TupleToolMuonIDCalib.cpp)
 - [TupleToolMuonIDCalib.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMuonCalib/src/TupleToolMuonIDCalib.h)
 - [TupleToolMuonPid.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMuonCalib/src/TupleToolMuonPid.cpp)
 - [TupleToolMuonPid.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleMuonCalib/src/TupleToolMuonPid.h)
{% endcallout %} 

{% callout "DecayTreeTupleReco - for reconstruction-level information, such as TupleToolTrackInfo" %}
DecayTreeTupleReco 
 - [TupleToolAllPhotons.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleReco/src/TupleToolAllPhotons.cpp)
 - [TupleToolAllPhotons.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleReco/src/TupleToolAllPhotons.h)
 - [TupleToolAllVeloTracks.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleReco/src/TupleToolAllVeloTracks.cpp)
 - [TupleToolAllVeloTracks.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleReco/src/TupleToolAllVeloTracks.h)
 - [TupleToolCPU.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleReco/src/TupleToolCPU.cpp)
 - [TupleToolCPU.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleReco/src/TupleToolCPU.h)
 - [TupleToolPrimaries.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleReco/src/TupleToolPrimaries.cpp)
 - [TupleToolPrimaries.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleReco/src/TupleToolPrimaries.h)
 - [TupleToolProtoPData.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleReco/src/TupleToolProtoPData.cpp)
 - [TupleToolProtoPData.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleReco/src/TupleToolProtoPData.h)
 - [TupleToolRecoStats.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleReco/src/TupleToolRecoStats.cpp)
 - [TupleToolRecoStats.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleReco/src/TupleToolRecoStats.h)
 - [TupleToolRICHPid.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleReco/src/TupleToolRICHPid.cpp)
 - [TupleToolRICHPid.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleReco/src/TupleToolRICHPid.h)
 - [TupleToolTrackInfo.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleReco/src/TupleToolTrackInfo.cpp)
 - [TupleToolTrackInfo.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleReco/src/TupleToolTrackInfo.h)
{% endcallout %} 

{% callout "DecayTreeTupleTracking - for more detailed tools regarding tracking" %}
DecayTreeTupleTracking 
 - [TupleToolConeIsolation.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTracking/src/TupleToolConeIsolation.cpp)
 - [TupleToolConeIsolation.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTracking/src/TupleToolConeIsolation.h)
 - [TupleToolDEDX.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTracking/src/TupleToolDEDX.cpp)
 - [TupleToolDEDX.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTracking/src/TupleToolDEDX.h)
 - [TupleToolMCAssociatedClusters.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTracking/src/TupleToolMCAssociatedClusters.cpp)
 - [TupleToolMCAssociatedClusters.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTracking/src/TupleToolMCAssociatedClusters.h)
 - [TupleToolTrackIsolation.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTracking/src/TupleToolTrackIsolation.cpp)
 - [TupleToolTrackIsolation.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTracking/src/TupleToolTrackIsolation.h)
 - [TupleToolTrackKink.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTracking/src/TupleToolTrackKink.cpp)
 - [TupleToolTrackKink.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTracking/src/TupleToolTrackKink.h)
 - [TupleToolTrackPosition.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTracking/src/TupleToolTrackPosition.cpp)
 - [TupleToolTrackPosition.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTracking/src/TupleToolTrackPosition.h)
 - [TupleToolTrackTime.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTracking/src/TupleToolTrackTime.cpp)
 - [TupleToolTrackTime.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTracking/src/TupleToolTrackTime.h)
 - [TupleToolVELOClusters.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTracking/src/TupleToolVELOClusters.cpp)
 - [TupleToolVELOClusters.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTracking/src/TupleToolVELOClusters.h)
 - [TupleToolVeloTrackClusterInfo.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTracking/src/TupleToolVeloTrackClusterInfo.cpp)
 - [TupleToolVeloTrackClusterInfo.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTracking/src/TupleToolVeloTrackClusterInfo.h)
 - [TupleToolVeloTrackMatch.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTracking/src/TupleToolVeloTrackMatch.cpp)
 - [TupleToolVeloTrackMatch.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTracking/src/TupleToolVeloTrackMatch.h)
{% endcallout %} 

{% callout "DecayTreeTupleTrigger - for access to the trigger information of the candidates" %}
DecayTreeTupleTrigger 
 - [TupleToolEventInfo.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolEventInfo.cpp)
 - [TupleToolEventInfo.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolEventInfo.h)
 - [TupleToolFillingScheme.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolFillingScheme.cpp)
 - [TupleToolFillingScheme.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolFillingScheme.h)
 - [TupleToolL0Calo.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolL0Calo.cpp)
 - [TupleToolL0Calo.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolL0Calo.h)
 - [TupleToolL0Data.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolL0Data.cpp)
 - [TupleToolL0Data.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolL0Data.h)
 - [TupleToolStripping.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolStripping.cpp)
 - [TupleToolStripping.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolStripping.h)
 - [TupleToolTISTOS.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolTISTOS.cpp)
 - [TupleToolTISTOS.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolTISTOS.h)

_Dec, 
_TIS, 
_TOS, 
_TUS, 
_TPS, 

 - [TupleToolTriggerBase.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolTriggerBase.cpp)
 - [TupleToolTriggerBase.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolTriggerBase.h)
 - [TupleToolTrigger.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolTrigger.cpp)
 - [TupleToolTrigger.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolTrigger.h)
 - [TupleToolTriggerRecoStats.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolTriggerRecoStats.cpp)
 - [TupleToolTriggerRecoStats.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolTriggerRecoStats.h)
{% endcallout %} 



The class references and descriptions of each of the individual tuple tools can be found [here](http://lhcb-doxygen.web.cern.ch/lhcb-doxygen/analysis/latest/annotated.html), (scroll down to T).
