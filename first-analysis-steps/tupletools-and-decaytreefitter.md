# TupleTools and DecayTreeFitter

## TupleTools and branches

{% objectives "Learning Objectives" %}

* Add extra TupleTools to the default DecayTreeTuple
* Configure the extra TupleTools
* Use branches
* Find useful TupleTools
* Learn how to use LoKi functors in a DecayTreeTuple

{% endobjectives %} 

Usually, the default information stored by `DecayTreeTuple` as shown in our [minimal DaVinci job](minimal-dv-job) is not enough for physics analysis.
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

The way the `DecayTreeTuple.Decay` is written in in our [minimal DaVinci job](minimal-dv-job),

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

Note that `TupleToolPropertime` requires there to be a PV in the event, otherwise it throws an error. You should add a PV checker before your tupling algorithm


```python
from Configurables import CheckPV
DaVinci().UserAlgorithms += [CheckPV(), dtt]
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
However, the best way to know what a tool does is check its documentation, either by opening its `.h` file or be searching for it in the latest [`doxygen`](https://lhcb-doxygen.web.cern.ch/lhcb-doxygen/davinci/latest/index.html).
Most tools are very well documented and will also inform you of their configuration options.
As an example, to get the information on the `TupleToolTrackInfo` we used before we could either check its [source code](https://gitlab.cern.ch/lhcb/Analysis/blob/master/Phys/DecayTreeTupleReco/src/TupleToolTrackInfo.h) or its [web documentation](https://lhcb-doxygen.web.cern.ch/lhcb-doxygen/davinci/latest/da/ddd/class_tuple_tool_track_info.html).
 In case we need more information or need to know *exactly* what the code does, the `fill` method is the one we need to look at.

 As a shortcut, the list of tupletools can also be found in doxygen at the top of the pages for the [`IParticleTupleTool`](https://lhcb-doxygen.web.cern.ch/lhcb-doxygen/davinci/latest/de/df8/struct_i_particle_tuple_tool.html) and the [`IEventTupleTool`](https://lhcb-doxygen.web.cern.ch/lhcb-doxygen/davinci/latest/d5/d88/struct_i_event_tuple_tool.html) interfaces (depending on whether they fill information about specific particles or the event in general).

{% endcallout %} 

The updated options can be found [here](./code/add-tupletools/ntuple_options.py).

{% challenge "Test your ntuple" %}

Run the options in the same way as in the [minimal DaVinci 
job](minimal-dv-job) lesson.
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

Picking up with the [LoKi functors lesson](loki-functors), let's store some specific bits of information discussed there in our ntuple.
To add LoKi-based leaves to the tree, we need to use the `LoKi::Hybrid::TupleTool`, which is configured with 3 arguments:

  1. Its *name*, specified in the `addTupleTool` call after a `/`.  This is 
     very useful (and recommended) if we want to have different 
     `LoKi::Hybrid::TupleTool` for each of our branches. For instance, we may 
     want to add different information for the D*, the D0 and the soft `$ \pi $`:

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
[doxygen](https://lhcb-doxygen.web.cern.ch/lhcb-doxygen/davinci/latest/d7/dae/namespace_lo_ki_1_1_cuts.html).
To check `SUMTREE` and `CHILD`, run the code above and check that the `Dstar_max_pt` and `Dstar_max_pt_preambulo` and the `Dstar_mass_D0` and `D0_mass` branches have exactly the same values.

{% endchallenge %} 

## How do I use DecayTreeFitter?

{% objectives "Learning Objectives" %}

* Add a kinematic fitter to a branch in the decay tree
* Apply a mass constraint
* Inspect the refitted decay tree

{% endobjectives %} 

Once you have made a hypothesis on the chain of decays that lead to your final state, you then can incorporate the additional knowledge that comes with this hypothesis to get a new best estimate for the particle parameters -- in particular their momenta. The additional knowledge is represented as constraints, which your decay tree has to fulfill.

For example, for the decay
```python
'[D*(2010)+ -> (D0 -> K- K+) pi+]CC'
```
you can make the assumption that the (K- K+) combine to form a D0 with a specific invariant mass. This results in a so called *mass constraint*. In addition the two kaons should originate from exactly the same point in space. If you know that your data only contains prompt D\* candidates, you can constrain them to do come from the primary vertex. Boundary conditions like those are called *vertex constraints* (the last of which is known as a *primary-vertex constraint*).

Applying kinematic constraints leads to new best estimates for the track parameters of the final-state particles. The process of calculating those is called a *kinematic (re)fit* and the `DecayTreeFitter` is the algorithm that performs this task for us. Access to this tool is provided by the TupleTool with the name `TupleToolDecayTreeFitter`.

{% callout "The physics and mathematics behind DecayTreeFitter" %}

For details of the method see the paper on [Decay chain fitting with a Kalman 
filter](https://arxiv.org/abs/physics/0503191).

{% endcallout %} 

So how do we use a `TupleToolDecayTreeFitter` in our DaVinci script? Let's create a branch to add the tool to. We'll just name it `'Dstar'`:
```python
dtt.addBranches({
    'Dstar': '[D*(2010)+ -> (D0 -> K- K+) pi+]CC',
})
```
To this branch we can now apply the `TupleToolDecayTreeFitter` with arbitrarily chosen name `consD`.
```python
dtt.Dstar.addTupleTool('TupleToolDecayTreeFitter/ConsD')
```
Now we can proceed with the configuration of the fitter. We are going to constrain the D0 to have originated from the primary vertex. We want all the output available, so we set the `verbose` option. Finally we want to apply the mass constraint to the D0:
```python
dtt.Dstar.ConsD.constrainToOriginVertex = True
dtt.Dstar.ConsD.Verbose = True
dtt.Dstar.ConsD.daughtersToConstrain = ['D0']
```
Note that you can constrain more than one intermediate state at once if that fits your decay.

When using the `TupleToolDecayTreeFitter` in a `DecayTreeTuple`, all the variables created by the other TupleTools are not affected by the change, but some new variables are created, one set per `DecayTreeFitter` instance. Depending on whether the `Verbose` option is specified, the new variables are created for the head particle only or for the head particle and its decay products too.

If the decay products are not stable particles and decay further, the decay products of the decay products have no new variables associated to them by default.
Since in many cases this information might be useful, there is an option to store the information from those tracks
```python
dtt.Dstar.ConsD.UpdateDaughters = True
```

{% callout "DecayTreeFitter and LoKi functors" %}

Alternatively, many of the operations described above can be done by using the 
`DecayTreeFitter` via LoKi functors using `DTF_FUN` functor in `LoKi::Hybrid::TupleTool`.
The advantage of this method is that it allows to customize the output variables as shown below.
```python
LoKi_DTFFun = dtt.Dstar.addTupleTool("LoKi::Hybrid::TupleTool/LoKi_DTFFun")
LoKi_DTFFun.Variables = {
    "DTFFun_Dstart_P"   : "DTF_FUN(P, True, 'D0')",
    "DTFFun_Dstar_PT"   : "DTF_FUN(PT, True, 'D0')",
    "DTFFun_Dstar_M"    : "DTF_FUN(M, True, 'D0')",
    "DTFFun_DTF_CHI2"   : "DTF_CHI2(True, 'D0')",
    "DTFFun_DTF_NDOF"   : "DTF_NDOF(True, 'D0')",
    "DTFFun_D0_M"       : "DTF_FUN(CHILD(M,  '[D*(2010)+ ->^D0 pi+]CC'), True, 'D0')",
    "DTFFun_D0_PE"      : "DTF_FUN(CHILD(E,  '[D*(2010)+ ->^D0 pi+]CC'), True, 'D0')",
    "DTFFun_D0_PX"      : "DTF_FUN(CHILD(PX, '[D*(2010)+ ->^D0 pi+]CC'), True, 'D0')",
    "DTFFun_D0_PY"      : "DTF_FUN(CHILD(PY, '[D*(2010)+ ->^D0 pi+]CC'), True, 'D0')",
    "DTFFun_D0_PZ"      : "DTF_FUN(CHILD(PZ, '[D*(2010)+ ->^D0 pi+]CC'), True, 'D0')"
}
```
The first argument of `DTF_FUN` is the LoKi functor that defines the output variable. The second (boolean) argument defines if *primary-vertex constraint* is required or not. 
The third argument is optional and specifies a particle or list of particles to be mass-constrained. In the case of multiple mass constraints this argument should look like `strings(['particle1','particle2'])`. 
The quality of the fit can be accessed by `DTF_CHI2` functor. 

The `DecayTreeFitter` implementation described above has a disadvantage that it will re-run the fit for every variable requested by `DTF_FUN`. A more efficient and ***strongly recommended way*** to use LoKi-based `DecayTreeFitter` can be done using `LoKi__Hybrid__Dict2Tuple` tool as described in [DaVinci tutorial](https://twiki.cern.ch/twiki/bin/view/LHCb/DaVinciTutorial9b).

{% endcallout %} 

{% callout "Which constraints to apply" %}

It is important to be aware of the assumptions you make to build your ntuple. For 
example, after you require the vertex constraint you must be careful if using 
the `IPCHI2_OWNPV`, since the particle you are looking at is *forced* to point 
to the PV. Which constraints make most sense for you depends on the questions 
you want to ask in your analysis, so ask your supervisor/working group in case 
of doubt.

{% endcallout %} 

Once you have produced your ntuple you can have a look at the refitted variables.
```shell
root -l DVntuple.root
TupleDstToD0pi_D0ToKK->cd()
DecayTree->StartViewer()
```
Plotting the raw mass of the D* (without the fit) `Dstar_M`, you should see a broad signal around 2 GeV:

![Dstar raw](img/DstarRaw.png)

{% callout "Which mass variable to use" %}

In many ntuples you also find a mass variable called `_MM`. This (confusingly) 
refers to measured mass. However, it is usually better to use `_M`. `_MM` is 
the sum of the 4-momenta of the final state particles extrapolated back to the 
fitted vertex position, but not the result of the actual vertex fit. Remember that
a vertex fit acts like a vertex constraint, improving the opening-angle resolution.

{% endcallout %} 


Now let us look at the refitted mass of the `$ D^{*+} $`, with the `$ D^0 $` constrained to its nominal mass.
It is stored in the variable `Dstar_ConsD_M`.
If you plot this you will note that some values are unphysical.
So, let's restrict the range we look at to something that makes sense.

On the root prompt use the `arrow-up` key to get the last draw command and modify it to pipe the output into a histogram:
```shell
tv__tree->Draw("Dstar_ConsD_M>>h(200,2000,2030)","","");
```

![Dstar refitted](img/DstarRefit.png)

Note that this plot has 149 entries, although we only have 73 candidates in the raw mass spectrum. The reason for this is that we typically have several primary vertices per event. When you use the vertex constraint, the fitter is run for each of the possible vertex hypotheses available in the event. So all the `Dstar_ConsD-xxx` variables are in fact arrays, where the first value corresponds to the *best PV* hypothesis. We can plot only those by doing
```shell
tv__tree->Draw("Dstar_ConsD_M[0]>>h(200,2000,2030)","","");
```
and we get the final kinematically refitted Dstar mass:

![Dstar refitted best PV](img/DstarRefitBest.png)

Finally, let's check how the D0 mass constraint has played out.

```shell
tv__tree->Draw("Dstar_ConsD_D0_M[0]>>h(100,1800,1900)","","", 128, 0);
```

![constrained D0](img/D0Refit.png)

As expected, the D0 candidates are forced onto their PDG mass value.


{% challenge "Explore" %}

* Look at the `status` variable to check if the fits converged.
* Look at the chi2 distribution of the fit

{% endchallenge %} 

`DecayTreeFitter` can be told to change some of the hypotheses in the decay tree. This is very useful if you want to slightly change which decays you want to look at. As an example let's say we want to examine the decay of the D0 into K- pi+ instead of K- K+. For this we add a second fitter, giving it a new name `ConsDKpi`:
```python
dtt.Dstar.addTupleTool('TupleToolDecayTreeFitter/ConsDKpi')
dtt.Dstar.ConsDKpi.constrainToOriginVertex = True
dtt.Dstar.ConsDKpi.Verbose = True
dtt.Dstar.ConsDKpi.daughtersToConstrain = ['D0']
```
We now can tell the fitter to substitute one of the kaons in the D0 decay by a pion.
```python
dtt.Dstar.ConsDKpi.Substitutions = {
    'Charm -> (D0 -> K- ^K+) Meson': 'pi+',
    'Charm -> (D~0 -> K+ ^K-) Meson': 'pi-'
}
```
In the dictionary that is passed to the `Substitutions` property of the fitter, the keys are decay descriptors, where the respective particle to be substituted is marked with a `^`. The values are the respective new particle hypotheses. The substitution will only work if you start from a decay descriptor that actually matches your candidates. However, you are allowed to generalise parts of the decay. Here we replaced `D*(2010)` with the more general `Charm` and the bachelor `pi-` is just represented by a `Meson`.

Note that the substitution mechanism does not understand the `CC` symbol. Both charge states have to be specified explicitely.

Running the ntuple script again with these additions gives you fit results for the re-interpreted decay.

{% challenge "Challenge" %}

* Compare the outcome of the two fits with the different mass hypothesis
* Compare the fit quality between the correct and the the wrong hypothesis

{% endchallenge %} 


The solution to this exercise `ntuple_DTF1.py`, is [available
here](./code/decay-tree-fitter/ntuple_DTF1.py).
