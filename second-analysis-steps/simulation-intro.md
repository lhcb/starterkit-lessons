{% objectives "Learning Objectives" %}
* Understand how a signal decay sample is produced in the LHCb framework
{% endobjectives %}

## What is Gauss?

Gauss is the LHCb simulation framework which manages the creation of simulated events by interfacing to multiple external applications. Most commonly, an event is created via the following procedure:

1. The `ProductionTool` (Pythia, GenXicc, ...) generates an event with the required signal particle. Either by generating minimum bias events until a matching particle is found or by ensuring one is produced in every event. The resulting event is comprised of either stable particles or unstable particles which are known to either EvtGen or Geant4 and can be decayed.
2. The signal particle is decayed using the `DecayTool` (EvtGen) to the desired final state, all remaining unstable particles are decayed independently.
3. The signal and its decay products might be required to pass generator level cuts implemented as a `CutTool`.
4. Particles are transported through the detector simulation.

{% callout "Things to remember" %}
1. The detector simulation is **__by far__** the most time consuming step (minutes, compared to seconds for the rest). So make sure your generator cuts remove events you cannot possibly reconstruct or select later on. Additional options are available to increase the speed, please talk to your MC liaisons!
2. The generator cuts are only applied to the signal that was forced to decay to the specific final state. _Any_ other true signal candidate is not required to pass. Furthermore, if the signal candidate flies in negative `z` direction, the event is mirrored to optimise the use of available CPU resources.
3. The number of generated events refers to the number entering step 4 above, so those passing the generator level cuts. __Not__ the number of events produced by the `ProductionTool` in the first step.
{% endcallout %}
