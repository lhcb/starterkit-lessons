---
layout: page
title: First Steps in LHCb
subtitle: Building your own decay
minutes: 15
---

> ## Learning Objectives {.objectives}
>
> * Learn the concepts behind the LHCb selection framework
> * Build a decay chain 

In order to perform most physics analyses we need to build a *decay chain* with reconstructed (or MC) particles that represents the physics process we want to study.
In LHCb, this decay chain can be built through `LHCb::Particle` (`LHCb::MCParticle`) objects that represent individual particles and contain links to their children, also represented by the same type of object.

We'll learn all the concepts involved by running through a full example:
using the DST file we downloaded in the [Downloading a file from the Grid](05-files-from-grid.html) lesson, we will build our own $D^*\rightarrow D^0(\rightarrow K \pi) \pi$ decay chain from scratch.
Get your [LoKi skills](https://lhcb.github.io/first-analysis-steps/06-loki-functors.html) ready and let's start.

The typical approach is to build the decay from the bottom up. Therefore, we need to

  1. Get input pions and kaons and filter them according to our physics needs.
  2. Combine a pion and a kaon to build a D0, and apply selection cuts to it.
  3. Combine this D0 with a pion to build the D*, again filtering when necessary.

To do that, we need to know a little bit more about how the LHCb analysis framework works.
As discussed in the [Gaudi introduction](01-davinci.html), `Gaudi` is based on the event-by-event sequential (chained) execution of algorithms wrapped in a `GaudiSequencer`, which takes care of handling the execution order such that processing stops when an algorithm is *not passed*.
However, it does not handle the data dependencies between these algorithms nor does it give easy access to them.
To solve this problem, the [Selection Framework](https://twiki.cern.ch/twiki/bin/view/LHCb/ParticleSelection) was created, and it is based on two types of objects: `Selection` and `SelectionSequence`.

> ## Selections {.callout}
> The `Selection` is the basic unit of the framework. It uses other `Selections` to process `LHCb::Particles` and writes them to a TES location easily findable through its `outputLocation` method. Additionally, it knows about other `Selections` that it requires to pass in order to obtain input particles through its `RequiredSelections` argument.
> A `Selection` requires *all* of its `RequiredSelections` to pass.
>
> There are several types of selections, namely
>
> - `Selection`, the most basic class.
> - `MergedSelection`, which is used to join the output of several `Selection` objects into a single TES location.
> - `DataOnDemand` (also known as `AutomaticData`), which builds objects from their TES location using a preconfigured map of (location, algorithm) pairs.

> ## Selection sequences {.callout}
> The `SelectionSequence` takes a `Selection` object, resolves its `Selection` requirements, and builds a flat, chained and ordered list of `Selections`. It then exports (via the `selection` method) a self-contained `GaudiSequencer` with all the algorithm configurables necessary to run the selection.
> It also makes the output locations of the data written by the selection chain available via the `outputLocations` method.

To get our input particles we use the `DataOnDemand` service:

```python
from PhysSelPython.Wrappers import DataOnDemand
Pions = DataOnDemand('Phys/StdAllNoPIDsPions/Particles')
Kaons = DataOnDemand('Phys/StdAllLooseKaons/Particles')
```

> ## Finding the correct inputs for `CombineParticles` algorithms {.callout}
> As discussed previously, the `DataOnDemand` or `AutomaticData` selection builds objects from their TES location.
> In Gaudi, the TES location of the output of an algorithm is generally determined as `Phys/ALGO_NAME/OBJECT_TYPE`, where `OBJECT_TYPE` refers to `Particles`, `Vertices`, etc.
>
> The `CommonParticles` package, which you can find [here](https://svnweb.cern.ch/trac/lhcb/browser/Stripping/trunk/Phys/CommonParticles/python/CommonParticles), allows to access premade particles with reasonable selections for us to use with `DataOnDemand`.
> For example, in our specific case, we use the `DataOnDemand` class with the `Phys/StdAllNoPIDsPions/Particles` and `Phys/StdAllLooseKaons/Particles` locations to access the output of the `StdAllNoPIDsPions` and `StdAllLooseKaons` algorithms, respectively:
> 

Once we have the input pions and kaons, we can combine them to build a D0 by means of the `CombineParticles` algorithm.
This algorithm performs the combinatorics for us according to a given decay descriptor and puts the resulting particle in the TES, allowing also to apply some cuts on them:

 - `DaughtersCuts` is a dictionary that maps each child particle type to a LoKi particle functor that determines if that particular particle satisfies our selection criteria. Optionally, one can specify also a `Preambulo` property that allows us to make imports, preprocess functors, etc (more on this in the [LoKi functors](https://lhcb.github.io/first-analysis-steps/06-loki-functors.html) lesson). For example:

```python
d0_daughters = {'pi-': '(PT > 750*MeV) & (P > 4000*MeV) & (MIPCHI2DV(PRIMARY) > 4)',
                'K+': '(PT > 750*MeV) & (P > 4000*MeV) & (MIPCHI2DV(PRIMARY) > 4)'}
```

 - `CombinationCut` is a particle array LoKi functor (note the `A` prefix, see more [here](https://twiki.cern.ch/twiki/bin/view/LHCb/LoKiHybridFilters#Particle_Array_Functors)) that is given the array of particles in a single combination (the *children*) as input (in our case a kaon and a pion). This cut is applied before the vertex fit so it is typically used to save CPU time by performing some sanity cuts such as `AMAXDOCA` or `ADAMASS` before the CPU-consuming fit:
 
```python
d0_comb = "(AMAXDOCA('') < 0.2*mm) & (ADAMASS('D0') < 100*MeV)"
```
 
 - `MotherCut` is a selection LoKi particle functor that acts on the particle produced by the vertex fit (the *parent*) from the input particles, which allows to apply cuts on those variables that require a vertex, for example:

```python
d0_mother = "(VFASPF(VCHI2/VDOF)< 9) & (BPVDIRA > 0.9997) & (ADMASS('D0') < 70*MeV)"
```

Then, we can build a combiner as

```python
from Configurables import CombineParticles
d0 = CombineParticles("Combine_D0",
                      DecayDescriptor='([D0 -> pi- K+]CC)',
                      DaughtersCuts=d0_daughters,
                      CombinationCut=d0_comb,
                      MotherCut=d0_mother)
```

Now we have to build a `Selection` out of it so we can later on put all pieces together:

```python
from PhysSelPython.Wrappers import Selection
d0_sel = Selection("Sel_D0",
                   Algorithm=d0,
                   RequiredSelections=[Pions, Kaons])
```

Now we can use another `CombineParticles` to build the D* with pions and the D0's as inputs, and applying a filtering only on the soft pion:

```python
dstar_daughters = {'pi+': '(TRCHI2DOF < 3) & (PT > 100*MeV)'}
dstar_comb = "(ADAMASS('D*(2010)+') < 400*MeV)"
dstar_mother = "(abs(M-MAXTREE('D0'==ABSID,M)-145.42) < 10*MeV) & (VFASPF(VCHI2/VDOF)< 9)"
dstar = CombineParticles('CombineDstar',
                         DecayDescriptor='[D*(2010)+ -> D0 pi+]cc',
                         DaughtersCuts=dstar_daughters,
                         CombinationCut=dstar_comb,
                         MotherCut=dstar_mother)
dstar_sel = Selection('Sel_Dstar',
                      Algorithm=dstar,
                      RequiredSelections=[d0_sel, Pions])
```

> ## Performing shared selections {.callout}
> In some cases we may want to build several decays in the same script with some common particles/selection;
> for example, in our case we could have been building D0->KK in the same script, and then we would have wanted to select the soft pion in the same way when building the D*.
> In this situation, we can make use of the `FilterDesktop` algorithm, which takes a TES location and filters the particles inside according to a given LoKi functor in the `Code` property, which then can be given as input to a `Selection`:
> 
> ```python
> from Configurables import FilterDesktop
> soft_pion = FilterDesktop("Filter_SoftPi",
>                           Code='(TRCHI2DOF < 3) & (PT > 100*MeV)')
> soft_pion_sel = Selection("Sel_SoftPi",
>                           Algorithm=soft_pion,
>                           RequiredSelections=[Pions])
> dstar = CombineParticles("CombineDstar",
>                          DecayDescriptor='[D*(2010)+ -> D0 pi+]cc',
>                          CombinationCut="(ADAMASS('D*(2010)+') < 400*MeV)",
>                          MotherCut="(abs(M-MAXTREE('D0'==ABSID,M)-145.42) < 10*MeV) & (VFASPF(VCHI2/VDOF)< 9)")
> dstar_sel = Selection("Sel_Dstar",
>                       Algorithm=dstar,
>                       RequiredSelections=[d0_sel, soft_pion_sel])
> ```
>
> This allows us to save time by performing the filtering of the soft pions only once, and to keep all the common cuts in a single place, avoiding duplication of code.


We can now build build a `SelectionSequence` to add to the `DaVinci` execution sequence

```python
from PhysSelPython.Wrappers import SelectionSequence
dstar_seq = SelectionSequence('Dstar_Seq', TopSelection=dstar_sel)
from Configurables import DaVinci
DaVinci().UserAlgorithms += [dstar_seq.sequence()]
```

> ## Work to do {.challenge}
>  - Finish the script (the base of which can be found [here](code/06-building-decays/build_decays.py)) by adapting the basic `DaVinci` configuration from its corresponding [lesson](09-minimal-dv-job.html) and check the output ntuple.
>  - Do you know what the used LoKi functors (`AMAXDOCA`, `ADAMASS`, `MIPCHI2DV`, etc) do? 

Congratulations! You've built the base of a stripping line!
