# Replace a mass hypothesis

{% objectives "Learning Objectives" %}
* Create a new selection starting from the stripping line output
* Change the particle hypothesis made by the stripping line
{% endobjectives %} 

There are many situations where you want to repurpose a stripping line to look for a new decay that is similar in topology but distinct from what was put into the stripping line. The easiest thing to do is to change some of the hypothesis on particle IDs made in the stripping. This lesson will show you how to do that.

{% callout "Reinterpreting stripping selections" %}
Note that with this method you can never find more candidates than the 
stripping has already found.  However, you can reinterpret parts of the decay 
to look for new decay modes.
{% endcallout %}

As an example we will switch the decays of the D0 from (K- K+) to (K- pi+). 

There is an algorithm that allows us to replace parts of the decay descriptor called `SubstitutePID`:

```python
# configure an algorithm to substitute the K+ (resp. the K-)
# in the D0 (resp the D~0) decay by a pion 
from Configurables import SubstitutePID
subs = SubstitutePID(
    'MakeD02Kpi',
    Code = "DECTREE('[D*(2010)+ -> (D0 -> K- K+) pi+]CC')",
    # note that SubstitutePID can't handle automatic CC
    Substitutions = {
    'Charm -> (D0 -> K- ^K+) Meson': 'pi+',
    'Charm -> (D~0 -> K+ ^K-) Meson': 'pi-'}
)
```

The algorithm is configured with a name `MakeD02Kpi`. In the `Code` argument we need to specify the initial selection. This is done by using LoKi functors. Since we know we will be using an already prepared selection, we can simply use the `DECTREE` functor to search for candidates fulfilling this decay structure. See the [lesson on LoKi](../first-analysis-steps/loki-functors.md) for more info on what you can do here. 

Now we are ready to specify which hypotheses to change. `Substitutions` is a dictionary where the keys are decay descriptors and the values are the names of the replacement particles. The particle that should be replaced is marked with a `^`. So in the example above
```python
'Charm -> (D0 -> K- ^K+) Meson': 'pi+'
```
means: Look for a decay of a Charm-particle into D0 plus any meson, where the D0 decays to (K- K+) and replace the K+ with a pi+.

Note that `SubstitutePID` does not automatically handle complex conjugation via the `CC` operator. Therefore you have to specify all substitutions explicitely. 

Next we have to handle the input and output of this algorithm. This is accomplished using [particle selections](https://twiki.cern.ch/twiki/bin/view/LHCb/ParticleSelection). The input to our substitution algorithm will be the candidates produced by the stripping. In order to make them look like a selection we can use the `DataOnDemand` service:
```python
from PhysSelPython.Wrappers import Selection
from PhysSelPython.Wrappers import SelectionSequence
from PhysSelPython.Wrappers import DataOnDemand

# Stream and stripping line we want to use
stream = 'AllStreams'
line = 'D2hhPromptDst2D2KKLine'
tesLoc = '/Event/{0}/Phys/{1}/Particles'.format(stream, line)

# get the selection(s) created by the stripping
strippingSels = [DataOnDemand(Location=tesLoc)]
```

The output of the algorithm has to be packaged into a new selection:

```python
# create a selection using the substitution algorithm
selSub = Selection(
    'Dst2D0pi_D02Kpi_Sel',
    Algorithm=Subs,
    RequiredSelections=strippingSels
)
```

Note how the input stripping selection is daisy-chained to the output selection through the `RequiredSelections` (it has to be a list of selections) argument. The new selection is added into a `SelectionSequence` for further use by DaVinci:

```python
selSeq = SelectionSequence('SelSeq', TopSelection=selSub)
```

We are now ready to produce an ntuple on our newly created selection. As usual we configure a `DecayTreeTuple`, which now is looking for the candidates, which have the redefined D0 decay:
```python
# Create an ntuple to capture D*+ decays from the new selection
dtt = DecayTreeTuple('TupleDstToD0pi_D0ToKpi')
dtt.Inputs = [selSeq.outputLocation()]
# note the redefined decay of the D0
dtt.Decay = '[D*(2010)+ -> ^(D0 -> ^K- ^pi+) ^pi+]CC'
```

The input to the `DecayTreeTuple` is taken as the `outputLocations` of the `SelectionSequence` we just created. 

{% callout "Why use DaVinci selections?" %}
Selections and SelectionSequences are an elegant way to organize the required 
algorithms that perform the job of selecting data.
The [particle selection toolkit](https://twiki.cern.ch/twiki/bin/view/LHCb/ParticleSelection) uses python tricks to make the management of even complicated sequences of selections rather straight forward. In particular the toolkit ensures that all necessary algorithms are run in the correct order to produce the desired selections. It is therefore mainly a tool to manage dependencies. For technical details, have a look at the [TWiki page](https://twiki.cern.ch/twiki/bin/view/LHCb/ParticleSelection).
{% endcallout %}

Finally we add the `SelectionSequence` and the `DecayTreeTuple` to the DaVinci application. Since we are adding more than one algorithm we need a `GaudiSequencer` that takes care of calling everything in the right order:
```python
# add our new selection and the tuple into the sequencer
seq = GaudiSequencer('MyTupleSeq')
seq.Members += [selSeq.sequence()]
seq.Members += [dtt]
DaVinci().appendToMainSequence([seq])
```

The solution to this exercise `ntuple_switchHypo.py`, is [available 
here](./code/18-switch-mass-hypo/ntuple_switchHypo.py).

{% callout "Think about it" %}
Why can't we use this method to look for D* decaying to D0+photon ? 
{% endcallout %}
