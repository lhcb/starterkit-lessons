# Building your own decay
## A Historical Approach

{% objectives "Learning Objectives" %}
* Build a decay chain using the most basic tools
* Understand the limitations and problems of these tools
{% endobjectives %} 

{% callout "Lesson caveat" %}
In this lesson we will explain how to work with the most basic building 
blocks of the Selection Framework.
This is not the most optimal way to use it, but it is included here because their use is very generalized, for example in the Stripping, and understanding them is very useful for understanding most of our current code.
At the end of this lesson its shortcomings will be highlighted and a better way to approach the problem will be presented in the [following lesson](building-decays-part2.html).
{% endcallout %}

Now we'll learn to apply the concepts of the Selection Framework by running through a full example:
using the DST files from the [Downloading a file from the Grid](../first-analysis-steps/files-from-grid.md) lesson, we will build our own $$D^\ast\rightarrow D^0(\rightarrow K^{-} K^{+}) \pi$$ decay chain from scratch.
Get your [LoKi skills](../first-analysis-steps/loki-functors.md) ready and let's start.

{% callout "Getting started" %}
There's no need to download the files from the Grid for this lesson.
We can simply open them using the `root` protocol thanks to the fact that they are replicated at CERN:

```python
from GaudiConf import IOHelper
IOHelper().inputFiles([('root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/2016/ALLSTREAMS.DST/00062514/0000/00062514_00000008_7.AllStreams.dst')],
                      clear=True)
```

The starting code for this exercise can be found [here](code/building-decays/00.start.py).
{% endcallout %}

Our input pions and kaons can be imported from the `StandardParticles` package:

```python
from StandardParticles import StdAllNoPIDsPions as Pions
from StandardParticles import StdAllLooseKaons as Kaons
```

This is an ideal way to get pre-made particles with the standard LHCb configuration.

{% callout "Where do `StandardParticles` come from?" %}
One important type of `Selection` is the `AutomaticData`, which builds objects from their TES location using a centrally predefined algorithm.
The `StandardParticles`/`CommonParticles` packages (one imports the other), which you can find [here](https://gitlab.cern.ch/lhcb/Phys/tree/master/Phys/CommonParticles), allow to access premade particles with reasonable reconstruction/selections for us to use with `AutomaticData`.

For example, in our specific case, we use the `AutomaticData` class with the `Phys/StdAllNoPIDsPions/Particles` and `Phys/StdAllLooseKaons/Particles` locations to access the output of the `StdAllNoPIDsPions` and `StdAllLooseKaons` algorithms, respectively (see [here](https://gitlab.cern.ch/lhcb/Phys/blob/master/Phys/CommonParticles/python/CommonParticles/StdAllNoPIDsPions.py) and [here](https://gitlab.cern.ch/lhcb/Phys/blob/master/Phys/CommonParticles/python/CommonParticles/StdAllLooseKaons.py)).
Therefore, the following code would be equivalent to what we have used:
```python
from PhysConf.Selections import AutomaticData
Pions = AutomaticData('Phys/StdAllNoPIDsPions/Particles')
Kaons = AutomaticData('Phys/StdAllLooseKaons/Particles')
```
{% endcallout %}

Once we have the input kaons, we can combine them to build a $$D^0$$ by means of the `CombineParticles` algorithm.
This algorithm performs the combinatorics for us according to a given decay descriptor and puts the resulting particle in the TES, allowing also to apply some cuts on them:

 - `DaughtersCuts` is a dictionary that maps each child particle type to a LoKi 
   particle functor that determines if that particular particle satisfies our 
   selection criteria. Optionally, one can specify also a `Preambulo` property 
   that allows us to make imports, preprocess functors, etc (more on this in 
   the [LoKi functors](../first-analysis-steps/loki-functors.md) lesson). 
   For example:

    ```python
    d0_daughters = {
      'K-': '(PT > 750*MeV) & (P > 4000*MeV) & (MIPCHI2DV(PRIMARY) > 4)',
      'K+': '(PT > 750*MeV) & (P > 4000*MeV) & (MIPCHI2DV(PRIMARY) > 4)'
    }
    ```

 - `CombinationCut` is a particle array LoKi functor (note the `A` prefix, see more [here](https://twiki.cern.ch/twiki/bin/view/LHCb/LoKiHybridFilters#Particle_Array_Functors)) that is given the array of particles in a single combination (the *children*) as input (in our case two kaons). This cut is applied before the vertex fit so it is typically used to save CPU time by performing some sanity cuts such as `AMAXDOCA` or `ADAMASS` before the CPU-consuming fit:
 
    ```python
    d0_comb = "(AMAXDOCA('') < 0.2*mm) & (ADAMASS('D0') < 100*MeV)"
    ```
 
 - `MotherCut` is a selection LoKi particle functor that acts on the particle produced by the vertex fit (the *parent*) from the input particles, which allows to apply cuts on those variables that require a vertex, for example:

    ```python
    # We can split long selections across multiple lines
    d0_mother = (
      '(VFASPF(VCHI2/VDOF)< 9)'
      '& (BPVDIRA > 0.9997)'
      "& (ADMASS('D0') < 70*MeV)"
    )
    ```
With all the selections ready, we can build a combiner as

```python
from Configurables import CombineParticles
d0 = CombineParticles(
    'Combine_D0',
    DecayDescriptor='[D0 -> K- K+]cc',
    DaughtersCuts=d0_daughters,
    CombinationCut=d0_comb,
    MotherCut=d0_mother
)
```

{% challenge "A small question" %}
- Do you understand this selection?
- Do you know what each of these LoKi functors does?
{% endchallenge %}

Now we have to build a `Selection` out of it so we can later on put all pieces together:

```python
from PhysConf.Selections import Selection
d0_sel = Selection(
    'Sel_D0',
    Algorithm=d0,
    RequiredSelections=[Kaons]
)
```

We can already see that this two-step process (building the `CombineParticles` and the `Selection`) is a bit cumbersome.
This can be simplified using a `SimpleSelection` object, which will be discussed in the next lesson.

For the time being, let's finish building our candidates.
Now we can use another `CombineParticles` to build the $$D^\ast$$ with pions and the $$D^0$$'s as inputs, and applying a filtering only on the soft pion:

```python
dstar_daughters = {'pi+': '(TRCHI2DOF < 3) & (PT > 100*MeV)'}
dstar_comb = "(ADAMASS('D*(2010)+') < 400*MeV)"
dstar_mother = (
    "(abs(M-MAXTREE('D0'==ABSID,M)-145.42) < 10*MeV)"
    '& (VFASPF(VCHI2/VDOF)< 9)'
)

dstar = CombineParticles(
    'Combine_Dstar',
    DecayDescriptor='[D*(2010)+ -> D0 pi+]cc',
    DaughtersCuts=dstar_daughters,
    CombinationCut=dstar_comb,
    MotherCut=dstar_mother
)

dstar_sel = Selection(
    'Sel_Dstar',
    Algorithm=dstar,
    RequiredSelections=[d0_sel, Pions]
)
```

{% callout "Building shared selections" %}
In some cases we may want to build several decays in the same script with 
some common particles/selection;
for example, in our case we could have been building D0->K pi in the same script, and then we would have wanted to select the soft pion in the same way when building the Dstar.
In this situation, we can make use of the `FilterDesktop` algorithm, which takes a TES location and filters the particles inside according to a given LoKi functor in the `Code` property, which then can be given as input to a `Selection`:

```python
from Configurables import FilterDesktop
soft_pion = FilterDesktop('Filter_SoftPi',
                          Code='(TRCHI2DOF < 3) & (PT > 100*MeV)')
soft_pion_sel = Selection('Sel_SoftPi',
                          Algorithm=soft_pion,
                          RequiredSelections=[Pions])
dstar = CombineParticles(
    'CombineDstar',
    DecayDescriptor='[D*(2010)+ -> D0 pi+]cc',
    CombinationCut="(ADAMASS('D*(2010)+') < 400*MeV)",
    MotherCut=(
        "(abs(M-MAXTREE('D0'==ABSID,M)-145.42) < 10*MeV)"
        '& (VFASPF(VCHI2/VDOF)< 9)'
    )
)
dstar_sel = Selection(
    'Sel_Dstar',
    Algorithm=dstar,
    RequiredSelections=[d0_sel, soft_pion_sel]
)
```

This allows us to save time by performing the filtering of the soft pions only once, and to keep all the common cuts in a single place, avoiding duplication of code.
{% endcallout %}


We can now build build a `SelectionSequence` to add to the `DaVinci` execution sequence

```python
from PhysConf.Selections import SelectionSequence
dstar_seq = SelectionSequence('Dstar_Seq', TopSelection=dstar_sel)
from Configurables import DaVinci
DaVinci().UserAlgorithms += [dstar_seq.sequence()]
```

{% challenge "Work to do" %}
- Finish the script by adapting the basic `DaVinci` configuration from its 
corresponding 
[lesson](../first-analysis-steps/minimal-dv-job.md) and 
check the output ntuple (run over 10000 events to make sure your tuple has 
enough entries). The solution can be found 
[here](code/building-decays/01.historical.py)).
- Add a `PrintSelection` algorithm in your selections and run again. It gets a 
  `Selection` as input and it can be used the same way, except it will print 
  the decay tree everytime making use of the `PrintDecayTree` algorithm which 
  was discussed in the [Exploring a 
  DST](../first-analysis-steps/interactive-dst.md) lesson.
- Compare your selection with what is done in the actual Stripping, which can be found [here](https://gitlab.cern.ch/lhcb/Stripping/blob/master/Phys/StrippingArchive/python/StrippingArchive/Stripping28/StrippingCharm/StrippingD2hh.py). You can appreciate the power of the Selection Framework in the modularity of that Stripping.
{% endchallenge %}

By looking at the final script, there is one striking thing:
there is a lot of repetition (`CombineParticles`-`Selection` sequence) which leads to complicated naming schemes, due to the fact that we want our `Selection` or `CombineParticle` objects to have a unique name.
To help with these name clashes and to allow a much more streamlined `Selection` building, the `PhysConf.Selections` module offers a large set of more optimized classes, which we'll discuss in the [next lesson](building-decays-part2.html).
