{% objectives "Learning Objectives" %}
* Learn the basic structure of a decfile.
{% endobjectives %}

## Controlling the decay: DecFiles

The `DecFile` controls the decay itself (i.e. what `EvtGen` does) as well as provide any event-type specific configuration (e.g. generator cuts).
They exist in the `Gen/DecFile` package which we have already checked out and built in the beginning and are given as `.dec` files in `Gen/DecFiles/dkfiles`.
For example, the `.dec` file for event-type `27163003` is `Dst_D0pi,Kpi=DecProdCut.dec`:
```bash
# EventType: 27163003
#
# Descriptor: [D*(2010)+ -> (D0 -> K- pi+) pi+]cc
#
# NickName: Dst_D0pi,Kpi=DecProdCut
#
# Cuts: DaughtersInLHCb
#
# Documentation: D0 decays to right-sign mode (K- pi+) with a D* tag.
# EndDocumentation
#
# PhysicsWG: Charm
# Tested: Yes
# Responsible: Mat Charles
# Email: matthew.charles@<nospam>cern.ch
# Date: 20101210
#

Alias MyD0 D0
Alias MyantiD0 anti-D0
ChargeConj MyD0 MyantiD0

Decay D*+sig
  1.000 MyD0  pi+    VSS;
Enddecay
CDecay D*-sig

Decay MyD0
  1.0   K-  pi+    PHSP;
Enddecay
CDecay MyantiD0
#
End
```
The commented actually contains vital information and is parsed during the compilation to create the `<event-type>.py` file that is given to `Gauss` as one of its options!
The EventType is a series of flags which controls the generation. The rules for this are described in detail in [LHCb-2005-034](https://cds.cern.ch/record/855452/files/lhcb-2005-034.pdf) and also a [website](http://cloud.e5.physik.tu-dortmund.de/eventtype/) that allows you to build/parse event-types.
For example for the first digit: 1 = contains b quark, 2 = c quark, 3 = min bias ...
Similarly, the document specifies the conventions for the "NickName" - which also has to be the filename. Note that once MC has been produced from a given DecFile, it is not allowed to be changed, so you never need to worry about which version of DecFiles you are looking at when trying to understand existing samples.
The "Cuts" field specifies which one of a predetermined set of cut tools are used. Additional python code can be added to the resulting configuration file:

```bash
# InsertPythonCode:
# code ...
# EndInsertPythonCode
```

The bottom part of the decay file specifies the decay itself:
This DecFile defines a signal `D*+` which decays 100% to `D0` `pi+`, and the D0 in turn decays 100% into `K-` `pi+`. Important is the definition of "MyD0". If the decay was to "D0" rather than "MyD0", the D0 would decay via all of the decay modes implemented in DECAY.DEC.
The final part of each decay is the actual physics model used - in this case "VSS", the generic matrix element for vector to scaler-scaler transitions, and "PHSP", which is phase space only (matrix element = constant). Note that with PHSP the daughters are completely unpolarized - for anything other than (spin0) to (spin0 spin0) this will get the angular momentum wrong!
