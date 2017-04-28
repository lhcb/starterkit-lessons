---
layout: page
title: Second analysis steps
subtitle: Building your own decay. Modern Selection Framework
minutes: 15
---

> ## Learning Objectives {.objectives}
>
> * Build a decay chain with the most optimized tools
> * Learn the advantages of these specialized tools

Note how we needed to use the `CombineParticles` from `GaudiConfUtils.ConfigurableGenerators` instead of the `PhysConf.Selections` one to make this work.
This is because the LHCb algorithms are configured as singletons and it is mandatory to give them a name, which we don't want to in `SimpleSelection` (we want to skip steps!).

> ## The LHCb singletons {.callout}
> If we had tried to simply use `CombineParticles` inside our `SimpleSelection`, we would have seen it fail with the following error
> 
> ```output
> NameError: Could not instantiate Selection because input Configurable CombineParticles has default name. This is too unsafe to be allowed.
> ```
>
> The reason for this is that all LHCb algorithms need an explicit and unique name because they are *singletons*, and therefore an exception is thrown if we don't do that.
> A [singleton](http://en.wikipedia.org/wiki/Singleton_pattern) is a software design pattern that restricts the instantiation of a class to one object. In our case, only one algorithm with a given name can be instantiated.
> This allows to reuse and reload algorithms that have already been created in the configuration sequence, eg, we could have reloaded the `"Combine_D0"` `CombineParticles` by name and modified it (even in another file loaded in the same `gaudirun.py` call!):
>
> ```python
> d0_copy = CombineParticles('Combine_D0')
> print d0_copy.DecayDescriptor
>```
>
> This is very useful to build complicated configuration chains.
>
> The solution for the `SimpleSelection` problem, in which we actually don't care about the `CombineParticles` name, is the `GaudiConfUtils.ConfigurableGenerators` package: it contains wrappers around algorithms such as `CombineParticles` or `FilterDesktop` allowing them to be instantiated without an explicit name argument.

This two-step process for building the `Selection` (creating an algorithm and building a selection with it) can be simplified by using a helper function in the `PhysConf.Selections` module, called `SimpleSelection`.
It gets a selection name, the algorithm type we want to run, the inputs and any other parameters that need to be passed to the algorithm (as keyword arguments), and returns a `Selection` object build in the same two-step way.
With that in mind, we can rewrite the previous two pieces of code as

```python
import GaudiConfUtils.ConfigurableGenerators as ConfigurableGenerators
from PhysConf.Selections import SimpleSelection
d0_sel = SimpleSelection(
    'Sel_D0',
    ConfigurableGenerators.CombineParticles,
    [Pions, Kaons],
    DecayDescriptor='([D0 -> pi- K+]CC)',
    DaughtersCuts=d0_daughters,
    CombinationCut=d0_comb,
    MotherCut=d0_mother
)
```
