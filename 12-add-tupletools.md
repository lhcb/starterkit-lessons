---
layout: page
title: First steps in LHCb
subtitle: TupleTools and branches
minutes: 15
---

> ## Learning Objectives {.objectives}
>
> * Add extra TupleTools to the default DecayTreeTuple
> * Configure the extra TupleTools
> * Use branches
> * Find useful TupleTools
> * Learn how to use LoKi functors in a DecayTreeTuple

Usually, the default information stored by `DecayTreeTuple` as shown in our [minimal DaVinci job](08-minimal-dv-job.html) is not enough for physics analysis.
Fortunately, most of the information we need can be added by adding C++ tools (known as `TupleTools`) to `dtt`;
there is an extensive library of these, some of which will be briefly discussed during the lesson.

> ## Default DecayTreeTuple tools {.callout}
> The default tools added in `DecayTreeTuple` are:
>
>  - `TupleToolKinematic`, which fills the kinematic information of the decay.
>  - `TupleToolPid`, which stores DLL and PID information of the particle.
>  - `TupleToolANNPID`, which stores the new NeuralNet-based PID information of the particle.
>  - `TupleToolGeometry`, which stores the geometrical variables (IP, vertex position, etc) of the particle.
>  - `TupleToolEventInfo`, which stores general information (event number, run number, GPS time, etc) of the event.

In order to add `TupleTools` to `dtt`, we have to use the `addTupleTool` method of `DecayTreeTuple` (only available when we have `from DecayTreeTuple.Configuration import *` in our script).
This method instantiates the tool, adds it to the list of tools to execute and returns it.
For example, if we want to fill the tracking information of our particles, we can add the `TupleToolTrackInfo` tool in the following way:

```python
track_tool = dtt.addTupleTool('TupleToolTrackInfo')
```

Some tools can be configured. For example, if we wanted further information from the tracks, such as the number of degrees of freedom of the track fit, we would have to turn on the verbose mode of the tool:

```python
track_tool.Verbose = True
```

If we don't need to configure the tool or we want to use the defaults, there's no need for storing the returned tool in a variable.
For example, if we wanted the information of the PV associated to our particle, we could just add the `TupleToolPrimaries` with no further configuration:

```python
dtt.addTupleTool('TupleToolPrimaries')
```

The way the `DecayTreeTuple.Decay` is written in in our [minimal DaVinci job](08-minimal-dv-job.html),

```python
dtt.Decay = '[D*(2010)+ -> (D0 -> K- pi+) pi+]CC'
```

means that the configured `TupleTools` will only run on the head of the decay chain, that is, the D*.
In order to select more particles for which we want the information stored, we need to mark them with a `^` symbol in the decay descriptor.
For example, if we want to also fill the information of the D0 and its children, we would modify the `dtt` to look like this:

```python
dtt.Decay = '[D*(2010)+ -> ^(D0 -> ^K- ^pi+) pi+]CC'
```

This will run all the configured `TupleTools` on the marked particles, with the caveat that some tools are only run on certain types of particles (eg, tracking tools on particles that have an associated track).
This configuration is not optimal, since there may be tools which we only want to run on the D's and some only on the children. Enter `Branches`, which allow us to specify which tools get applied to which particle in the decay (in addition to the `TupleTools` configured at the top level).

Branches let you define custom namespaces in your ntuple by means of a `dict`.
Its keys define the name of each branch (and, as a consequence, the prefix of the corresponding leaves in the ntuple), while the corresponding values are decay descriptors that specify which particles you want to include in the branch.

```python
dtt.addBranches({'Dstar' : '[D*(2010)+ -> (D0 -> K- pi+) pi+]CC',
                 'D0'    : '[D*(2010)+ -> ^(D0 -> K- pi+) pi+]CC',
                 'Kminus': '[D*(2010)+ -> (D0 -> ^K- pi+) pi+]CC',
                 'piplus': '[D*(2010)+ -> (D0 -> K- ^pi+) pi+]CC',
                 'pisoft': '[D*(2010)+ -> (D0 -> K- pi+) ^pi+]CC'})
```

Once the branches have been configured, they can be accessed as `dtt.PARTICLENAME`and `TupleTools` can be added as discussed before.
For example, if we want to store the proper time information of the D0, we would do

```python
dtt.D0.addTupleTool('TupleToolPropertime')
```

The usage of `Branches` is very important (and strongly encouraged) to keep the size of your ntuples small, since it prevents us from storing unneeded information (for example trigger information, which will be discussed at a later lesson).

> ## Where to find TupleTools {.callout}
> One of the most difficult things is to know which tool we need to add to our `DecayTreeTuple` in order to get the information we want.
> For this, it is necessary to know where to find `TupleTools` and their code.
>
> `TupleTools` are Gaudi tools that implement the [`IParticleTupleTool`](http://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/davinci/releases/latest/doxygen/d1/d77/class_i_particle_tuple_tool.html) interface (if they fill information about an `LHCb::Particle`) or the [`IEventTupleTool`](http://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/davinci/releases/latest/doxygen/d7/d5c/class_i_event_tuple_tool.html) interface (if they fill information about the event in general), so we can get the complete list of `TupleTools` of each type at the top of the doxygen page of the interface.
>
> Most tools are very well documented and will also inform you of their configuration options.
> In case we need more information or need to know *exactly* what the code does, the `fill` method is the one we need to look at.

The updated options can be found [here](./code/12-add-tupletools/ntuple_options.py).

> ## Test your ntuple {.challenge}
> Run the options in the same way as in the [minimal DaVinci job](08-minimal-dv-job.html) lesson.
> You will obtain a `DVntuple.root` file, which we can open and inspect with `ROOT`'s `TBrowser`:
>
> ```
> $ root DVntuple.root
> root [0]
> Attaching file DVntuple.root as _file0...
> root [1] TBrowser *b = new TBrowser()
> root [2]
> ```
>
> Try to locate the branches we have added, which are placed in the `TupleDstToD0pi_D0ToKpi/DecayTree`, and plot some distributions by double-clicking the leaves.

Picking up with the [LoKi functors lesson](06-loki-functors.html), let's store some specific bits of information discussed there in our ntuple.
To add LoKi-based leaves to the tree, we need to use the `LoKi::Hybrid::TupleTool`, which is configured with 3 arguments:

  - Its *name*, specified in the `addTupleTool` call after a `/`.  This is very useful (and recommended) if we want to have different `LoKi::Hybrid::TupleTool` for each of our branches. For instance, we may want to add different information for the D*, the D0 and the soft $\pi$:
    ```python
    dstar_hybrid = dtt.Dstar.addTupleTool('LoKi::Hybrid::TupleTool/LoKi_Dstar')
    d0_hybrid = dtt.D0.addTupleTool('LoKi::Hybrid::TupleTool/LoKi_D0')
    pisoft_hybrid = dtt.pisoft.addTupleTool('LoKi::Hybrid::TupleTool/LoKi_PiSoft')
    ```
  - The `Preambulo` property, which lets us perform preprocessing of the LoKi functors to simplify the code that is used to fill the leaves, for example creating combinations of LoKi functors or performing mathematical operations:
    ```python
    preamble = [
        'DZ = VFASPF(VZ) - BPV(VZ)',
        'TRACK_MAX_PT = MAXTREE(ISBASIC & HASTRACK, PT, -1)'
    ]
    dstar_hybrid.Preambulo = preamble
    d0_hybrid.Preambulo = preamble
    ```
  - The `Variables` property, consisting of a `dict` of (variable name, LoKi functor) pairs. In here, LoKi functors can be used, as well as any variable we may have defined in the `Preambulo`:
    ```python
    dstar_hybrid.Variables = {
        'mass': 'MM',
        'mass_D0': 'CHILD(MM, 1)',
        'pt': 'PT',
        'dz': 'DZ',
        'dira': 'BPVDIRA',
        'max_pt': 'MAXTREE(ISBASIC & HASTRACK, PT, -1)',
        'max_pt_preambulo': 'TRACK_MAX_PT',
        'sum_pt_pions': 'SUMTREE(211 == ABSID, PT)',
        'n_highpt_tracks': 'NINTREE(ISBASIC & HASTRACK & (PT > 1500*MeV))'
    }
    d0_hybrid.Variables = {
        'mass': 'MM',
        'pt': 'PT',
        'dira': 'BPVDIRA',
        'vtx_chi2': 'VFASPF(VCHI2)',
        'dz': 'DZ'
    }
    pisoft_hybrid.Variables = {
        'p': 'P',
        'pt': 'PT'
    }
    ```

In the code snippets specified above (available [here](code/12-add-tupletools/ntuple_options_loki.py)), you can see that the `NINTREE` functor counts the number of particles that pass the specified criteria. While this is not very useful for ntuple-building (we can always do it offline), it's a very powerful functor to use when building decay selections.

> ## Getting more practice {.challenge}
> In the `LoKi::Hybrid::TupleTool`we've used some  functors that have not been described previously. Find out what they do in the [doxygen](http://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/davinci/latest_doxygen/d7/dae/namespace_lo_ki_1_1_cuts.html).
> To check `SUMTREE` and `CHILD`, run the code above and check that the `Dstar_max_pt` and `Dstar_max_pt_preambulo` and the `Dstar_mass_D0` and `D0_mass` branches have exactly the same values.
