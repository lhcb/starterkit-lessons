# List of TupleTool Variables

{% objectives "Learning Objectives" %}
* What are each TupleTool's variables?
{% endobjectives %} 

{% callout "List of DecayTreeTuples - General Categories" %}
DecayTreeTuples:

- DecayTreeTuple - for the more general tools.
- DecayTreeTupleANNPID - for the NeuralNet-based PID tools.
- DecayTreeTupleDalitz - for Dalitz analysis.
- DecayTreeTupleJets - for obtaining information on jets.
- DecayTreeTupleMC - gives us access to MC-level information.
- DecayTreeTupleMuonCalib - for muon calibration tools.
- DecayTreeTupleReco - for reconstruction-level information, such as TupleToolTrackInfo.
- DecayTreeTupleTracking - for more detailed tools regarding tracking.
- DecayTreeTupleTrigger - for accessing to the trigger information of the candidates.
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
- MCTupleToolDalitz.cpp
- MCTupleToolDalitz.h
- TupleToolDalitz.cpp
- TupleToolDalitz.h	 
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
 - MCTupleToolAngles.cpp
 - MCTupleToolAngles.h
 - MCTupleToolDecayType.cpp
 - MCTupleToolDecayType.h
 - MCTupleToolEventType.cpp
 - MCTupleToolEventType.h
 - MCTupleToolHierarchy.cpp
 - MCTupleToolHierarchy.h
 - MCTupleToolInteractions.cpp
 - MCTupleToolInteractions.h
 - MCTupleToolKinematic.cpp
 - MCTupleToolKinematic.h
 - MCTupleToolP2VV.cpp
 - MCTupleToolP2VV.h
 - MCTupleToolPID.cpp
 - MCTupleToolPID.h
 - MCTupleToolPrimaries.cpp
 - MCTupleToolPrimaries.h
 - MCTupleToolPrompt.cpp
 - MCTupleToolPrompt.h
 - MCTupleToolReconstructed.cpp
 - MCTupleToolReconstructed.h
 - MCTupleToolRedecay.cpp
 - MCTupleToolRedecay.h
 - TupleToolGeneration.cpp
 - TupleToolGeneration.h
 - TupleToolMCBackgroundInfo.cpp
 - TupleToolMCBackgroundInfo.h
 - TupleToolMCTruth.cpp
 - TupleToolMCTruth.h
{%endcallout %}

{% callout "DecayTreeTupleMuonCalib - for muon calibration tools" %}
DecayTreeTupleMuonCalib 
 - TupleToolMuonIDCalib.cpp
 - TupleToolMuonIDCalib.h
 - TupleToolMuonPid.cpp
 - TupleToolMuonPid.h
{% endcallout %} 

{% callout "DecayTreeTupleReco - for reconstruction-level information, such as TupleToolTrackInfo" %}
DecayTreeTupleReco 
 - TupleToolAllPhotons.cpp
 - TupleToolAllPhotons.h
 - TupleToolAllVeloTracks.cpp
 - TupleToolAllVeloTracks.h
 - TupleToolCPU.cpp
 - TupleToolCPU.h
 - TupleToolPrimaries.cpp
 - TupleToolPrimaries.h
 - TupleToolProtoPData.cpp
 - TupleToolProtoPData.h
 - TupleToolRecoStats.cpp
 - TupleToolRecoStats.h
 - TupleToolRICHPid.cpp
 - TupleToolRICHPid.h
 - TupleToolTrackInfo.cpp
 - [TupleToolTrackInfo.h](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleReco/src/TupleToolTrackInfo.h)
{% endcallout %} 

{% callout "DecayTreeTupleTracking - for more detailed tools regarding tracking" %}
DecayTreeTupleTracking 
 - TupleToolConeIsolation.cpp
 - TupleToolConeIsolation.h
 - TupleToolDEDX.cpp
 - TupleToolDEDX.h
 - TupleToolMCAssociatedClusters.cpp
 - TupleToolMCAssociatedClusters.h
 - TupleToolTrackIsolation.cpp
 - TupleToolTrackIsolation.h
 - TupleToolTrackKink.cpp
 - TupleToolTrackKink.h
 - TupleToolTrackPosition.cpp
 - TupleToolTrackPosition.h
 - TupleToolTrackTime.cpp
 - TupleToolTrackTime.h
 - TupleToolVELOClusters.cpp
 - TupleToolVELOClusters.h
 - TupleToolVeloTrackClusterInfo.cpp
 - TupleToolVeloTrackClusterInfo.h
 - TupleToolVeloTrackMatch.cpp
 - TupleToolVeloTrackMatch.h
{% endcallout %} 

{% callout "DecayTreeTupleTrigger - for access to the trigger information of the candidates" %}
DecayTreeTupleTrigger 
 - TupleToolEventInfo.cpp
 - TupleToolEventInfo.h
 - TupleToolFillingScheme.cpp
 - TupleToolFillingScheme.h
 - TupleToolL0Calo.cpp
 - TupleToolL0Calo.h
 - TupleToolL0Data.cpp
 - TupleToolL0Data.h
 - TupleToolStripping.cpp
 - TupleToolStripping.h
 - TupleToolTISTOS.cpp
 - TupleToolTISTOS.h
 - TupleToolTriggerBase.cpp
 - TupleToolTriggerBase.h
 - TupleToolTrigger.cpp
 - TupleToolTrigger.h
 - TupleToolTriggerRecoStats.cpp
 - TupleToolTriggerRecoStats.h
{% endcallout %} 


