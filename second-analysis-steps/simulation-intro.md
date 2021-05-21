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

## Choosing your Gauss Version

Each release of Gauss v49 represents a different release of Sim09. You may want to run an older version of Gauss to verify something in a pre-existing MC dataset or for any number of other reasons. Here is a table matching each Gauss v49 release to each Sim09 version:

.. list-table:: Sim09 Version - Gauss Correspondance
   :widths: 25 25
   :header-rows: 1
   * - Sim09 Version
     - Original Gauss Releases
   * - Sim09c
     - v49r8
   * - Sim09d
     - v49r10
   * - Sim09e
     - v49r11
   * - Sim09f
     - v49r12
   * - Sim09g
     - v49r13
   * - Sim09h
     - v49r14
   * - Sim09i
     - v49r17
   * - Sim09j
     - v49r18
   * - Sim09k
     - v49r20

<!--
| Sim09 Version | Original Gauss Releases |
| -- | -- |
| Sim09b | v49r5 |
| Sim09c | v49r8 |
| Sim09d | v49r10 |
| Sim09e | v49r11 |
| Sim09f | v49r12 |
| Sim09g | v49r13 |
| Sim09h | v49r14 |
| Sim09i | v49r17 |
| Sim09j | v49r18 |
| Sim09k | v49r20 |
-->

Any Gauss release not listed here corresponds to the lower of the Sim09 versions it is between (e.g. Gauss v49r9 is Sim09c).


Once in a few years, major Sim versions are released, which involve newer versions of GEANT4, Pythia8 and other software, as well as major fixes to the detector description. Prior to production release, an extensive testing is performed by the Simulation group. The next major release is Sim10 (Gauss versions above v52) which is currently under validation. It will be used for both Run 1-2 and upgrade simulations in the future. The current Sim09 also supports production for upgrade. Sim versions suitable for upgrade simulations are marked with "Up" label, e.g. Sim10-Up08.


Whilst these act as guidelines, to find the version of Gauss used for a specific production you can run this command:

`lb-dirac dirac-bookkeeping-production-information <prodID>`.

Here `<prodID>` represents your production number, this is a 5 or 6 digit number. If you don't know this, it can be found by using this command:

`lb-dirac dirac-bookkeeping-decays-path <Dec Number>`.

Here the production ID will be the last number given in each line. Note that you may need to use ProdID-1 for flagged samples or ProdID-2 for filtered samples since the generation, filtering and stripping are done in different steps.
