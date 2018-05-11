# List of TupleTool Variables

{% objectives "Learning Objectives" %}
* What are each TupleTool's variables?
{% endobjectives %} 

{% callout "List of DecayTreeTuples" %}
DecayTreeTuples:

 -  DecayTreeTuple 
 -  DecayTreeTupleANNPID 
 -  DecayTreeTupleBase 
 -  DecayTreeTupleDalitz 
 -  DecayTreeTupleHerschel 
 -  DecayTreeTupleJets 
 -  DecayTreeTupleMC 
 -  DecayTreeTupleMuonCalib 
 -  DecayTreeTupleReco 
 -  DecayTreeTupleTracking 
 -  DecayTreeTupleTrigger 

{% endcallout %} 

```python

```

- This the tuple tool for MCTupleToolAngles 
[.cpp](MCTupleToolAngles.html) 
[.h](MCTupleToolAngles2.html) 
 
{% callout "DecayTreeTuple" %}
DecayTreeTuple:
 - TupleToolAllTracks.cpp
 - TupleToolAllTracks.h
 - TupleToolAngles.cpp
 - TupleToolAngles.h
 - TupleToolBeamSpot.cpp
 - TupleToolBeamSpot.h
 - TupleToolBremInfo.cpp
 - TupleToolBremInfo.h
 - TupleToolCaloDigits.cpp
 - TupleToolCaloDigits.h
 - TupleToolCaloHypo.cpp
 - TupleToolCaloHypo.h
 - TupleToolCorrectedMass.cpp
 - TupleToolCorrectedMass.h
 - TupleToolDecayTreeFitter.cpp
 - TupleToolDecayTreeFitter.h
 - TupleToolDira.cpp
 - TupleToolDira.h
 - TupleToolGeometry.cpp
 - TupleToolGeometry.h
 - TupleToolHOP.cpp
 - TupleToolHOP.h
 - TupleToolIsolationTwoBody.cpp
 - TupleToolIsolationTwoBody.h
 - TupleToolKinematic.cpp
 - TupleToolKinematic.h
 - TupleToolMassHypo.cpp
 - TupleToolMassHypo.h
 - TupleToolNeutrinoReco.cpp
 - TupleToolNeutrinoReco.h
 - TupleToolP2VV.cpp
 - TupleToolP2VV.h
 - TupleToolParticleReFit.cpp
 - TupleToolParticleReFit.h
 - TupleToolParticleStats.cpp
 - TupleToolParticleStats.h
 - TupleToolPhotonInfo.cpp
 - TupleToolPhotonInfo.h
 - TupleToolPi0Info.cpp
 - TupleToolPi0Info.h
 - TupleToolPid.cpp
 - TupleToolPid.h
 - TupleToolPropertime.cpp
 - TupleToolPropertime.h
 - TupleToolSelResults.cpp
 - TupleToolSelResults.h
 - TupleToolSubMass.cpp
 - TupleToolSubMass.h
 - TupleToolSwimmingInfo.cpp
 - TupleToolSwimmingInfo.h
 - TupleToolTagging.cpp
 - TupleToolTagging.h
 - TupleToolTwoParticleMatching.cpp
 - TupleToolTwoParticleMatching.h
 - TupleToolVeto.cpp
 - TupleToolVeto.h
 - TupleToolVtxIsoln.cpp
 - TupleToolVtxIsoln.h
{% endcallout %} 

{% callout "DecayTreeTupleANNPID" %}
DecayTreeTupleANNPID 
 - TupleToolANNPID.cpp
 - TupleToolANNPID.h
 - TupleToolANNPIDTraining.cpp
 - TupleToolANNPIDTraining.h
{% endcallout %} 

{% callout "DecayTreeTupleANNPID" %}

- DecayTreeTupleBase 
{% endcallout %} 

{% callout "DecayTreeTupleANNPID" %}
- DecayTreeTupleDalitz 
{% endcallout %} 

{% callout "DecayTreeTupleANNPID" %}
- MCTupleToolDalitz.cpp
- MCTupleToolDalitz.h
- TupleToolDalitz.cpp
- TupleToolDalitz.h	 
{% endcallout %} 

{% callout "DecayTreeTupleANNPID" %}
- DecayTreeTupleHerschel 
{% endcallout %} 

{% callout "DecayTreeTupleANNPID" %}
- DecayTreeTupleJets 
{% endcallout %} 

{% callout "DecayTreeTupleANNPID" %}
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

{% callout "DecayTreeTupleANNPID" %}
- DecayTreeTupleMC 
{% endcallout %} 

{% callout "DecayTreeTupleANNPID" %}
- DecayTreeTupleMuonCalib 
{% endcallout %} 

{% callout "DecayTreeTupleANNPID" %}
- DecayTreeTupleReco 
{% endcallout %} 

{% callout "DecayTreeTupleANNPID" %}
- DecayTreeTupleTracking 
{% endcallout %} 

{% callout "DecayTreeTupleANNPID" %}
- DecayTreeTupleTrigger 
{% endcallout %} 



TupleToolKinematic.cpp

Numbered list coming up:

  1. We are number 1
  
  
  
  
  
  
  
  
  DecayTreeTuple for the more general tools.
DecayTreeTupleANNPID for the NeuralNet-based PID tools.
DecayTreeTupleDalitz for Dalitz analysis.
DecayTreeTupleJets for obtaining information on jets.
DecayTreeTupleMC gives us access to MC-level information.
DecayTreeTupleMuonCalib for muon calibration tools.
DecayTreeTupleReco for reconstruction-level information, such as TupleToolTrackInfo.
DecayTreeTupleTracking for more detailed tools regarding tracking.
DecayTreeTupleTrigger for accessing to the trigger information of the candidates.

