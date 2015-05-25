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

### Adding more information in our tuple

Usually, the default information stored by `DecayTreeTuple` as shown in our [minimal DaVinci
job](09-minimal-dv-job.html) is not enough for physics analysis. 
Fortunately, most of the information we need can be added by adding C++ tools (known as `TupleTools`) to `dtt`;
there is an extensive library of these, which will be briefly discussed later.

> ## Default DecayTreeTuple tools {.callout}
> The default tools added in `DecayTreeTuple` are:
>
>  - `TupleToolKinematic`, which fills the kinematic information of the decay.
>  - `TupleToolPid`, which stores DLL and PID information of the particle.
>  - `TupleToolANNPID`, which stores the new NeuralNet-based PID information of the particle.
>  - `TupleToolGeometry`, which stores the geometrical variables (IP, vertex position, etc) of the particle.
>  - `TupleToolEventInfo`, which stores general information (event number, run number, GPS time, etc) of the event.

In order to add a tool to `dtt`, we have to use the `addTupleTool` method of `DecayTreeTuple` (only available when we have `from DecayTreeTuple.Configuration import *` in our script), which adds the tool and returns it.
For example, if we want to fill the tracking information of our particles, we can add the `TupleToolTrackInfo` tool in the following way:

```python
track_tool = dtt.addTupleTool('TupleToolTrackInfo')
```

Some tools accept some configuration parameters. For example, if we wanted further information from the tracks, such as the number of degrees of freedom of the track fit, we would have to turn on the verbose mode of the tool

```python
track_tool.Verbose = True
```

If we don't need to configure the tool or we want to use the defaults, there's no need for storing the returned tool in a variable.
For example, if we wanted the information of the PV associated to our particle, we could just add the `TupleToolPrimaries` with no further configuration:

```python
dtt.addTupleTool('TupleToolPrimaries')
```

### Selectively filling the tuple (or how to make your tuples smaller)

The way the `DecayTreeTuple.Decay` is configured, ie, 

```python
dtt.Decay = '[D*(2010)+ -> (D0 -> K- pi+) pi+]CC'
```

will only run the configured `TupleTools` on the head of the decay, that is, the D*.
In order to select the particles for which we want the information stored, we need to mark them with a `^` symbol on the decay descriptor.
For example, if we want to fill the information of the D0 and its children, we would modify the `dtt` configuration to look like this:

```python
dtt.Decay = '[D*(2010)+ -> ^(D0 -> ^K- ^pi+) pi+]CC'
```

This will run all the configured `TupleTools` on the marked particles.
Clearly, this configuration is not optimal, since there may be tools which we only want to run on the D's and some only on the children. Enter `Branches`, which allow us to specify which tools get applied to which particle in the decay (in addition to the `TupleTools` configured at the top level).

Branches are configured by means of a `dict` that relates the variable name and the decay descriptor needed to select it. This also allows to give more descriptive names to the particles:

```python
dtt.addBranches({'Dstar' : '^[D*(2010)+ -> (D0 -> K- pi+) pi+]CC',
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
> TODO

