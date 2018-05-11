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
- DecayTreeTupleMC - gives us access to MC-level information.
- DecayTreeTupleMuonCalib - for muon calibration tools.
- DecayTreeTupleReco - for reconstruction-level information, such as TupleToolTrackInfo.
- DecayTreeTupleTracking - for more detailed tools regarding tracking.
- DecayTreeTupleTrigger - for accessing to the trigger information of the candidates. [Class Reference here.](http://lhcb-doxygen.web.cern.ch/lhcb-doxygen/analysis/latest/d3/dd1/class_tuple_tool_trigger.html)
{% endcallout %}


Below you will find links to the source code and header files for each of the TupleTools.

 
{% callout "DecayTreeTuple - for the more general tools" %}
DecayTreeTuple:
 - [TupleToolAllTracks.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolAllTracks.cpp)
 - [TupleToolAllTracks.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolAllTracks.h)
 - [TupleToolAngles.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolAngles.cpp)
 - [TupleToolAngles.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolAngles.h)
 - [TupleToolBeamSpot.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolBeamSpot.cpp)
 - [TupleToolBeamSpot.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolBeamSpot.h)
 - [TupleToolBremInfo.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolBremInfo.cpp)
 - [TupleToolBremInfo.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolBremInfo.h)
 - [TupleToolCaloDigits.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolCaloDigits.cpp)
 - [TupleToolCaloDigits.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolCaloDigits.h)
 - [TupleToolCaloHypo.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolCaloHypo.cpp)
 - [TupleToolCaloHypo.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolCaloHypo.h)
 - [TupleToolCorrectedMass.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolCorrectedMass.cpp)
 - [TupleToolCorrectedMass.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolCorrectedMass.h)
 - [TupleToolDecayTreeFitter.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolDecayTreeFitter.cpp)
 - [TupleToolDecayTreeFitter.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolDecayTreeFitter.h)
 - [TupleToolDira.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolDira.cpp)
 - [TupleToolDira.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolDira.h)
 - [TupleToolGeometry.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolGeometry.cpp)
 - [TupleToolGeometry.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolGeometry.h)
 - [TupleToolHOP.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolHOP.cpp)
 - [TupleToolHOP.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolHOP.h)
 - [TupleToolIsolationTwoBody.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolIsolationTwoBody.cpp)
 - [TupleToolIsolationTwoBody.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolIsolationTwoBody.h)
 - [TupleToolKinematic.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolKinematic.cpp)
 - [TupleToolKinematic.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolKinematic.h)
 - [TupleToolMassHypo.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolMassHypo.cpp)
 - [TupleToolMassHypo.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolMassHypo.h)
 - [TupleToolNeutrinoReco.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolNeutrinoReco.cpp)
 - [TupleToolNeutrinoReco.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolNeutrinoReco.h)
 - [TupleToolP2VV.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolP2VV.cpp)
 - [TupleToolP2VV.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolP2VV.h)
 - [TupleToolParticleReFit.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolParticleReFit.cpp)
 - [TupleToolParticleReFit.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolParticleReFit.h)
 - [TupleToolParticleStats.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolParticleStats.cpp)
 - [TupleToolParticleStats.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolParticleStats.h)
 - [TupleToolPhotonInfo.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolPhotonInfo.cpp)
 - [TupleToolPhotonInfo.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolPhotonInfo.h)
 - [TupleToolPi0Info.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolPi0Info.cpp)
 - [TupleToolPi0Info.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolPi0Info.h)
 - [TupleToolPid.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolPid.cpp)
 - [TupleToolPid.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolPid.h)
 - [TupleToolPropertime.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolPropertime.cpp)
 - [TupleToolPropertime.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolPropertime.h)
 - [TupleToolSelResults.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolSelResults.cpp)
 - [TupleToolSelResults.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolSelResults.h)
 - [TupleToolSubMass.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolSubMass.cpp)
 - [TupleToolSubMass.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolSubMass.h)
 - [TupleToolSwimmingInfo.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolSwimmingInfo.cpp)
 - [TupleToolSwimmingInfo.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolSwimmingInfo.h)
 - [TupleToolTagging.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolTagging.cpp)
 - [TupleToolTagging.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolTagging.h)
 - [TupleToolTwoParticleMatching.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolTwoParticleMatching.cpp)
 - [TupleToolTwoParticleMatching.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolTwoParticleMatching.h)
 - [TupleToolVeto.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolVeto.cpp)
 - [TupleToolVeto.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolVeto.h)
 - [TupleToolVtxIsoln.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolVtxIsoln.cpp)
 - [TupleToolVtxIsoln.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTuple/src/TupleToolVtxIsoln.h)
{% endcallout %} 

{% callout "DecayTreeTupleANNPID - for the NeuralNet-based PID tools" %}
DecayTreeTupleANNPID 
 - TupleToolANNPID.cpp
 - TupleToolANNPID.h
 - TupleToolANNPIDTraining.cpp
 - TupleToolANNPIDTraining.h
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
- DecayTreeTupleHerschel 
{% endcallout %} 

{% callout "DecayTreeTupleJets - for obtaining information on jets" %}
DecayTreeTupleJets 
- TupleToolJetRelations.cpp
- TupleToolJetRelations.h	
- TupleToolJetTag.cpp
- TupleToolJetTag.h	
- TupleToolJets.cpp
- TupleToolJets.h	
- TupleToolJetsBase.cpp	
- TupleToolJetsBase.h	
- TupleToolJetsForB.cpp			
- TupleToolJetsForB.h			
- TupleToolWZJets.cpp
- TupleToolWZJets.h
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
 - [TupleToolTriggerBase.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolTriggerBase.cpp)
 - [TupleToolTriggerBase.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolTriggerBase.h)
 - [TupleToolTrigger.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolTrigger.cpp)
 - [TupleToolTrigger.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolTrigger.h)
 - [TupleToolTriggerRecoStats.cpp](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolTriggerRecoStats.cpp)
 - [TupleToolTriggerRecoStats.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleTrigger/src/TupleToolTriggerRecoStats.h)
{% endcallout %} 



The class references and descriptions of each of the individual tuple tools can be found [here](http://lhcb-doxygen.web.cern.ch/lhcb-doxygen/analysis/latest/annotated.html), (Scroll down to T!).
