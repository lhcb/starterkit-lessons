# TupleTools and branches

{% objectives "Learning Objectives" %}
* Add extra TupleTools to the default DecayTreeTuple
* Configure the extra TupleTools
* Use branches
* Find useful TupleTools
* Learn how to use LoKi functors in a DecayTreeTuple
{% endobjectives %} 

Usually, the default information stored by `DecayTreeTuple` as shown in our [minimal DaVinci job](minimal-dv-job.html) is not enough for physics analysis.
Fortunately, most of the information we need can be added by adding C++ tools (known as `TupleTools`) to `dtt`;
there is an extensive library of these, some of which will be briefly discussed during the lesson.

{% callout "Default DecayTreeTuple tools" %}
The default tools added in `DecayTreeTuple` are:

 - `TupleToolKinematic`, which fills the kinematic information of the decay;
 - `TupleToolPid`, which stores DLL and PID information of the particle;
 - `TupleToolANNPID`, which stores the new NeuralNet-based PID information of the particle;
 - `TupleToolGeometry`, which stores the geometrical variables (IP, vertex position, etc) of the particle;
 - `TupleToolEventInfo`, which stores general information (event number, run number, GPS time, etc) of the event.
{% endcallout %} 

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

The way the `DecayTreeTuple.Decay` is written in in our [minimal DaVinci job](minimal-dv-job.html),

```python
dtt.Decay = '[D*(2010)+ -> (D0 -> K- K+) pi+]CC'
```

means that the configured `TupleTools` will only run on the head of the decay chain, that is, the `D*(2010)+`.
In order to select the particles for which we want the information stored, we need to mark them with a `^` symbol in the decay descriptor.
For example, if we want to fill the information of the `D0` and its children, as well as the soft `pi+`, we would modify the above line to look like this:

```python
dtt.Decay = '[D*(2010)+ -> ^(D0 -> ^K- ^K+) ^pi+]CC'
```

This will run all the configured `TupleTools` on the marked particles, with the caveat that some tools are only run on certain types of particles (eg, tracking tools on particles that have an associated track).
This configuration is not optimal, since there may be tools which we only want to run on the D's and some only on the children. Enter `Branches`, which allow us to specify which tools get applied to which particle in the decay (in addition to the `TupleTools` configured at the top level).

*Branches* let you define custom namespaces in your ntuple by means of a `dict`.
Its keys define the name of each branch (and, as a consequence, the prefix of the corresponding leaves in the ntuple), while the corresponding values are decay descriptors that specify which particles you want to include in the branch.

```python
dtt.addBranches({'Dstar' : '[D*(2010)+ -> (D0 -> K- K+) pi+]CC',
                 'D0'    : '[D*(2010)+ -> ^(D0 -> K- K+) pi+]CC',
                 'Kminus': '[D*(2010)+ -> (D0 -> ^K- K+) pi+]CC',
                 'Kplus' : '[D*(2010)+ -> (D0 -> K- ^K+) pi+]CC',
                 'pisoft': '[D*(2010)+ -> (D0 -> K- K+) ^pi+]CC'})
```

Note that in order to use branches, we have to make sure that all particles we want to use are marked in the main decay descriptor (`dtt.Decay`).
DaVinci will ignore branches for particles that have not been marked in `dtt.Decay`!

Once the branches have been configured, they can be accessed as `dtt.PARTICLENAME`and `TupleTools` can be added as discussed before.
For example, if we want to store the proper time information of the D0, we would do

```python
dtt.D0.addTupleTool('TupleToolPropertime')
```

{% callout "Do I really have to type my decay descriptor that many times?" %}
No! You can use the `dtt.setDescriptorTemplate()` method to set up your 
decay descriptor and branches in just one line!
Note this may not be available in some older versions of `DaVinci', 
but [this snippet](https://gitlab.cern.ch/snippets/147) will add it to an older version.
With that out of the way, you can simply use
```python
dtt.setDescriptorTemplate('${Dstar}[D*(2010)+ -> ${D0}(D0 -> ${Kminus}K- ${Kplus}K+) ${pisoft}pi+]CC')
```
This will set up both `dtt.Decay` and `Branches` for you.
{% endcallout %} 

The usage of `Branches` is very important (and strongly encouraged) to keep the size of your ntuples small, since it prevents us from storing unneeded information (for example trigger information, which will be discussed at a later lesson).

{% callout "Where to find TupleTools" %}
One of the most difficult things is to know which tool we need to add to our 
`DecayTreeTuple` in order to get the information we want.
For this, it is necessary to know where to find `TupleTools` and their code.
`TupleTools` are spread in 9 packages under `Analysis/Phys` (see the master branch in `git` [here](https://gitlab.cern.ch/lhcb/Analysis/tree/master/Phys)), all starting with the prefix `DecayTreeTuple`, according to the type of information they fill in our ntuple:

- `DecayTreeTuple` for the more general tools;
- `DecayTreeTupleANNPID` for the NeuralNet-based PID tools;
- `DecayTreeTupleDalitz` for Dalitz analysis;
- `DecayTreeTupleJets` for obtaining information on jets;
- `DecayTreeTupleMC` gives us access to MC-level information;
- `DecayTreeTupleMuonCalib` for muon calibration tools;
- `DecayTreeTupleReco` for reconstruction-level information, such as `TupleToolTrackInfo`;
- `DecayTreeTupleTracking` for more detailed tools regarding tracking;
- `DecayTreeTupleTrigger` for accessing to the trigger information of the candidates.

The `TupleTools` are placed in the `src` folder within each package and it's usually easy to get what they do just by looking at their name.
However, the best way to know what a tool does is check its documentation, either by opening its `.h` file or be searching for it in the latest [`doxygen`](http://lhcb-doxygen.web.cern.ch/lhcb-doxygen/davinci/latest/index.html).
Most tools are very well documented and will also inform you of their configuration options.
As an example, to get the information on the `TupleToolTrackInfo` we used before we could either check its [source code](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleReco/src/TupleToolTrackInfo.h) or its [web documentation](http://lhcb-doxygen.web.cern.ch/lhcb-doxygen/davinci/latest/da/ddd/class_tuple_tool_track_info.html).
 In case we need more information or need to know *exactly* what the code does, the `fill` method is the one we need to look at.

 As a shortcut, the list of tupletools can also be found in doxygen at the top of the pages for the [`IParticleTupleTool`](http://lhcb-doxygen.web.cern.ch/lhcb-doxygen/davinci/latest/de/df8/struct_i_particle_tuple_tool.html) and the [`IEventTupleTool`](http://lhcb-doxygen.web.cern.ch/lhcb-doxygen/davinci/latest/d5/d88/struct_i_event_tuple_tool.html) interfaces (depending on whether they fill information about specific particles or the event in general).
{% endcallout %} 

The updated options can be found [here](./code/add-tupletools/ntuple_options.py).

{% challenge "Test your ntuple" %}
Run the options in the same way as in the [minimal DaVinci 
job](minimal-dv-job.html) lesson.
You will obtain a `DVntuple.root` file, which we can open and inspect with `ROOT`'s `TBrowser`:

```
$ root DVntuple.root
root [0]
Attaching file DVntuple.root as _file0...
root [1] TBrowser b
root [2]
```

Try to locate the branches we have added, which are placed in the `TupleDstToD0pi_D0ToKpi/DecayTree`, and plot some distributions by double-clicking the leaves.
{% endchallenge %} 

Picking up with the [LoKi functors lesson](loki-functors.html), let's store some specific bits of information discussed there in our ntuple.
To add LoKi-based leaves to the tree, we need to use the `LoKi::Hybrid::TupleTool`, which is configured with 3 arguments:

  1. Its *name*, specified in the `addTupleTool` call after a `/`.  This is 
     very useful (and recommended) if we want to have different 
     `LoKi::Hybrid::TupleTool` for each of our branches. For instance, we may 
     want to add different information for the D*, the D0 and the soft $$\pi$$:
    ```python
    dstar_hybrid = dtt.Dstar.addTupleTool('LoKi::Hybrid::TupleTool/LoKi_Dstar')
    d0_hybrid = dtt.D0.addTupleTool('LoKi::Hybrid::TupleTool/LoKi_D0')
    pisoft_hybrid = dtt.pisoft.addTupleTool('LoKi::Hybrid::TupleTool/LoKi_PiSoft')
    ```
  2. The `Preambulo` property, which lets us perform preprocessing of the LoKi 
     functors to simplify the code that is used to fill the leaves, for example 
     creating combinations of LoKi functors or performing mathematical 
     operations:
    ```python
    preamble = [
        'DZ = VFASPF(VZ) - BPV(VZ)',
        'TRACK_MAX_PT = MAXTREE(ISBASIC & HASTRACK, PT, -1)'
    ]
    dstar_hybrid.Preambulo = preamble
    d0_hybrid.Preambulo = preamble
    ```
  3. The `Variables` property, consisting of a `dict` of (variable name, LoKi 
     functor) pairs. In here, LoKi functors can be used, as well as any 
     variable we may have defined in the `Preambulo`:
    ```python
    dstar_hybrid.Variables = {
        'mass': 'M',
        'mass_D0': 'CHILD(M, 1)',
        'pt': 'PT',
        'dz': 'DZ',
        'dira': 'BPVDIRA',
        'max_pt': 'MAXTREE(ISBASIC & HASTRACK, PT, -1)',
        'max_pt_preambulo': 'TRACK_MAX_PT',
        'sum_pt_pions': 'SUMTREE(211 == ABSID, PT)',
        'n_highpt_tracks': 'NINTREE(ISBASIC & HASTRACK & (PT > 1500*MeV))'
    }
    d0_hybrid.Variables = {
        'mass': 'M',
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

In the code snippets specified above (available [here](code/add-tupletools/ntuple_options_loki.py)), you can see that the `NINTREE` functor counts the number of particles that pass the specified criteria. While this is not very useful for ntuple-building (we can always do it offline), it's a very powerful functor to use when building decay selections.

{% challenge "Getting more practice" %}
In the `LoKi::Hybrid::TupleTool`we've used some  functors that have not been 
described previously. Find out what they do in the 
[doxygen](http://lhcb-doxygen.web.cern.ch/lhcb-doxygen/davinci/latest/d7/dae/namespace_lo_ki_1_1_cuts.html).
To check `SUMTREE` and `CHILD`, run the code above and check that the `Dstar_max_pt` and `Dstar_max_pt_preambulo` and the `Dstar_mass_D0` and `D0_mass` branches have exactly the same values.
{% endchallenge %} 
