## Modern Selection Framework

{% objectives "Learning Objectives" %}

* Build a decay chain with the most optimized tools
* Learn the advantages of these specialized tools

{% endobjectives %} 

As discussed previously, the Selection Framework can become a bit cumbersome in terms of the naming and construction of the `Selection`-`CombineParticles` repetitions.
For this reason, the Selection Framework offers a more streamlined set of `Selection`s to deal with these issues.

This two-step process for building the `Selection` (creating an algorithm and building a selection with it) can be simplified by using a helper function in the `PhysConf.Selections` module, called `SimpleSelection`.
It gets a selection name, the algorithm type we want to run, the inputs and any other parameters that need to be passed to the algorithm (as keyword arguments), and returns a `Selection` object build in the same two-step way.
With that in mind, we can rewrite the previous two pieces of code as

```python
import GaudiConfUtils.ConfigurableGenerators as ConfigurableGenerators
from PhysConf.Selections import SimpleSelection
d0_sel = SimpleSelection(
    'Sel_D0',
    ConfigurableGenerators.CombineParticles,
    [Kaons],
    DecayDescriptor='[D0 -> K- K+]cc',
    DaughtersCuts=d0_decay_products,
    CombinationCut=d0_comb,
    MotherCut=d0_vertex
)
```

Note how we needed to use the `CombineParticles` from `GaudiConfUtils.ConfigurableGenerators` instead of the `PhysConf.Selections` one to make this work.
This is because the LHCb algorithms are configured as singletons and it is mandatory to give them a name, which we don't want to in `SimpleSelection` (we want to skip steps!).

{% callout "Why `ConfigurableGenerators`?" %}

If we had tried to simply use `CombineParticles` inside our 
`SimpleSelection`, we would have seen it fail with the following error

```
NameError: Could not instantiate Selection because input Configurable CombineParticles has default name. This is too unsafe to be allowed.
```

The reason for this is that all LHCb algorithms need an explicit and unique name.
The solution for our problem, in which we actually don't care about the `CombineParticles` name, is the `GaudiConfUtils.ConfigurableGenerators` package: it contains wrappers around algorithms such as `CombineParticles` or `FilterDesktop` allowing them to be instantiated without an explicit name argument.

{% endcallout %}

In this case, we could also wonder about the need for the `CombineParticles` generator.
While `SimpleSelection` will allow us to do anything we would do with `Selection`, there exist a few specialized versions of it that allow us to address its most common usages:

  - `CombineSelection` is used for the `Selection` – `CombineParticles` combination.
  The previous example would be written then:

    ```python
    from PhysConf.Selections import CombineSelection
    d0_sel = CombineSelection(
        'Sel_D0',
        [Kaons],
        DecayDescriptor='[D0 -> K- K+]cc',
        DaughtersCuts=d0_decay_products,
        CombinationCut=d0_comb,
        MotherCut=d0_vertex
    )
    ```

  - `Combine3BodySelection` and `Combine4BodySelection` are the selection version of the `DaVinci::N{3,4}BodyDecays` DaVinci algorithms (you can see an example of their use as generators [here](https://gitlab.cern.ch/lhcb/Stripping/blob/master/Phys/StrippingArchive/python/StrippingArchive/Stripping28/StrippingRD/StrippingBeauty2XGamma.py#L869)), that allow to perform 3- and 4-body combinations with an improved CPU efficiency thanks to the existence of a set of cuts on the intermediate particle combinations (`Combination12Cut`, `Combination123Cut`).
  These are very useful in timing-critical environments, such as the Stripping.

  - `TupleSelection` allows to build a `Selection` with `DecayTreeTuple` as an algorithm.

  - `FilterSelection` is used to produce a `Selection` – `FilterDesktop` combination.
  It's used in a similar way as `CombineSelection`.


{% challenge "Work to do" %}

- Rewrite the previous script to select our signal in terms of 
`SimpleSelections` and the other algorithms we just learned.
- Introduce `FilterSelection` by performing the soft pion selection outside the `CombineParticles` as discussed in the *Building shared selections* callout in the [previous lesson](building-decays-part1).
The solution can be found [here](code/building-decays/02.optimized.py).

{% endchallenge %}

To finalize, it is also very useful to know about the existence of certain selection algorithms specialized in filtering according to very commonly used criteria.
These can be used as inputs for `SimpleSelection` to make sure the latter only run on those events that pass the filter.
The most interesting are (see the [code](https://gitlab.cern.ch/lhcb/Phys/blob/master/PhysSel/PhysSelPython/python/PhysSelPython/Wrappers.py) for the full list, along with examples on how to use them):

  - `TriggerSelection`, including `L0`/`Hlt1`/`Hlt2` specific versions. These are used to filter on certain trigger decisions (NB: their job is simply to filter, so they cannot be used as particle input for another selection).
  The same interface can be used for filtering on Stripping decisions by using the `StrippingSelection` class.
  With it, the example from the Starterkit [minimal DaVinci job](../first-analysis-steps/minimal-dv-job.md), in which the output of a Stripping line was passed to `DecayTreeTuple`, could be written in a more CPU-efficient way:

    ```python
    line = 'D2hhPromptDst2D2KKLine'
    strip_sel = StrippingSelection("Strip_sel",
                                   "HLT_PASS('StrippingD2hhPromptDst2D2KKLineDecision')")
    strip_input = AutomaticData('Phys/{0}/Particles'.format(line))
    tuple_sel = TupleSelection('Tuple_sel',
                               [strip_sel, strip_input],
                               Decay='[D*(2010)+ -> (D0 -> K- K+) pi+]CC')
    ```

  This avoids running `DecayTreeTuple` on empty events, since the `strip_sel` stops processing before.
  This will only be helpful for rare Stripping lines, since the overhead of running `DecayTreeTuple` on empty events is small, but this has been proven useful in more complex workflows.
  Additionally, it takes care of `RootInTes` for you.

{% challenge "A small test" %}

Try running the [minimal DaVinci job](../first-analysis-steps/minimal-dv-job.md) with and without the `StrippingSelection`/`DecayTreeTuple` selections, and compare their performance

In this particular case, there is a simple way to achieve a CPU-efficient code with `DecayTreeTuple`, thanks to the use of `DaVinci` pre-event filters;
 ```python
 from PhysConf.Filters import LoKi_Filters
 filter_ = LoKi_Filters(STRIP_Code="HLT_PASS('StrippingD2hhPromptDst2D2KKLineDecision')")

 DaVinci().EventPreFilters = filter_.filters("FILTERS")
```

{% endchallenge %}

  - Related to the previous ones, `TisTosSelection` are used to filter according to TIS/TOS.
  A whole range of them is available: `L0TOSSelection`, `L0TISSelection`, `Hlt1TOSSelection`, `Hlt1TISSelection`, `Hlt2TOSSelection` and `Hlt2TISSelection`.

  - `ValidBPVSelection`, which is used to check that a valid associated primary vertex is present.

While it's hard to find simple examples in which the utility of these tools is clearly highlighted, it's important to note that they constitute a modular framework that allows to build very complex workflows from very simple pieces.
In these situations, the Selection Framework is the ideal tool to keep the code bug-free and CPU-efficient.
