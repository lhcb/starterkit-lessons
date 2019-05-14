{% objectives "Learning Objectives" %}
* Learn how to customise the generated decay
* Learn how to modify the used decay channels
* Learn how to modify/remove generator level cuts
{% endobjectives %} 
# Modifying the decay

## Adding a decay channel

In order to make our decay model more realistic we can add in known resonances. E.g. instead of solely relying on phase-space, we can add in the prominent $$\Phi\to K^{+}K^{-}$$ resonance to hopefully. In the decfile, we can another line to the decay channels of the $$D^0$$:
```bash
Decay MyD0                                                                                                                                                                                                                                                 
  0.5 K+ K- mu+ mu- PHSP;                                                                                                                                                                                                                                
  0.5 phi mu+ mu- PHSP;                                                                                                                                                                                                                                
Enddecay                                                                                                                                                                                                                                                   
CDecay MyantiD0                                                                                                                                                                                                                                            
```
This triggers EvtGen to produce the $$\phi$$ resonance in 50% of the cases and $$\phi$$ is subsequently decayed. However, we have not told EvtGen that the $$\phi$$ should be decayed only to a $$K^+K^-$$, hence it will randomly choose from all possible decays it knows about. Instead of modifying the common `phi`, we apply the same trick as we did for the $$D^0$$ decay:
```bash
Alias MyPhi phi
ChargeConj MyPhi MyPhi

Decay MyD0
  0.25 K+ K- mu+ mu- PHSP;  
  0.75 MyPhi mu+ mu- PHSP;  
Enddecay
CDecay MyantiD0

Decay MyPhi
  1.000 K+  K-    VSS;
Enddecay
```
After changing the decfile, you have to rerun `make`. Try out the modified decfile with Gauss, you should see a large spike in $$m(K^+K^-)$$.

## Generator level cuts

Detector simulation is computationally expensive, and event generation is comparatively fast. Cuts at generator level save a huge amount of CPU and disk space (which means you can have more actually useful events) almost for free. At generator level you can only cut on pre-resolution quantities, so normally you want the generator cuts to be 100% efficient for selected events (within epsilon). The default example is to immediately remove events where the daughters are far outside the LHCb acceptance.
This is implemented in "DaugthersInLHCb", aka "DecProdCut" in the NickName. This requires that each "stable charged particle" is in a loose region around the LHCb acceptance (10-400 mrad in Theta).
Cut tools need to be implemented in C++ and reside in the package `Gen/GenCuts`.

## Removing the generator cuts

The absolute efficiencies for generator cuts can be obtained from the respective [website](http://lhcbdoc.web.cern.ch/lhcbdoc/STATISTICS/SIM09STAT/index.shtml) or the produced `GeneratorLog.xml` file, which contains:
```xml
<efficiency name = "generator level cut">
    <after> 5 </after>
    <before> 27 </before>
    <value> 0.18519 </value>
    <error> 0.074757 </error>
</efficiency>
```
However, if you need the efficiencies as functions of some other observables this is not sufficient. Instead one might want to create a generator-only sample without any cuts applied. Two possibilities exist:

1. Modify the DecFile and recompile the package
2. Overwrite the python configuration originally configured by `<event-type>.py`

The second option is usually easier and in the example used so far only requires one additional line of configuration
```python
Generation().SignalPlain.CutTool = ""
```
which must be included after `27163003.py` is sourced (e.g. in `Gauss-Job.py`). You can convince yourself that this alters the observed
distributions and leads to a generator level cut efficiency of 100%. A large sample can be found on EOS: `root://eosuser.cern.ch//eos/user/l/lhcbsk/sim-lesson/GaussNoGenCut-27163003-10000ev.xgen`
Have a look at the pseudorapidity distribution of the head particle. This illustrates another default behavior of the generation of signal decays in Gauss: The generated events' z-axis if inverted if the selected signal particle's momentum along that axis is negative.


## Modifying the cut tool
If you need to modify the cut tool, you generally can pick between multiple options with increasing complexity and time until in production:

1. Configure an existing cut-tool in `Gen/GenCuts` if possible.
2. Use `LoKi` functors for `GenParticle` (starting with a `G`) in a `LoKi::GenCutTool`.
3. Last resort for really special things: write your own C++ implementation of the `IGenCutTool` interface.

`LoKi::GenCutTool` are a good solution when you need to impose additional requirements beyond those provided by `DaughtersInLHCb`, for example a minimum for the transverse mometum of a `D0`.
For local tests, this can be easily implemented by overwriting the default cut tool set by `27175000.py`:
```python
from Configurables import LoKi__GenCutTool
from Gauss.Configuration import *
Generation().SignalPlain.CutTool = "LoKi::GenCutTool/TightCut"

from Configurables import LoKi__GenCutTool
gen=Generation()
gen.SignalPlain.addTool ( LoKi__GenCutTool , 'TightCut' )

tightCut = Generation().SignalPlain.TightCut
tightCut.Decay     = '^[D*(2010)+ ==> ^( D0 ==> ^K+ ^K- ^mu+ ^mu- ) pi+]CC'
tightCut.Preambulo += [
   'from GaudiKernel.SystemOfUnits import GeV',
   'inAcc         =  in_range ( 0.005 , GTHETA , 0.400 )',
   'goodD0        =  ( GPT > 2.0 * GeV )',
]
tightCut.Cuts = {
   '[D0]cc': 'goodD0',
   '[K+]cc': 'inAcc',
   '[mu+]cc': 'inAcc'
}
```
You can again check that this works and a larger sample of 10,000 events can be found `root://eosuser.cern.ch//eos/user/l/lhcbsk/sim-lesson/GaussTightCut-27163003-10000ev.xgen`
.
You might also notice a slight slow-down in the rate at which events are produced: by default, 
an event failing the generator cut (which is applied after Pythia and EvtGen are done) triggers a reset of
the entire generation phase of the simulation. Therefore, very tight generator level cuts in combination
with a signal particle that only rarely occurs in minimum bias events can results in the generation phase
taking manifold longer than the simulation of the detector response (and you might want to rethink your
strategy for event generation).
For some ideas on which kind of cuts to apply, you can have a look here: https://twiki.cern.ch/twiki/bin/view/LHCb/GeneratorLevelTightCuts

{% callout "Modifying cut tools for production" %}
As the cut tools are to be configured in the DecFiles, they form an integral part of the event-type itself.
Hence, any modification that changes the produced events usually requires the release of a new DecFile including a new event-type or a new simulation sub-version so events end up in a different bookkeeping location.
{% endcallout %} 
