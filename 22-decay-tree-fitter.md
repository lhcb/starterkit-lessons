---
layout: page
title: HowTo DecayTreeFitter
subtitle: How do I use DecayTreeFitter? 
minutes: 10
---

> ## Learning Objectives {.objectives}
>
> * Add a kinematic fitter to a branch in the decay tree
> * Apply a mass constraint
> * Inspect the refitted decay tree

Once you have made a hypothesis on the chain of decays that have lead to your final state, you then can incorporate the additional knowledge that comes with this hypothesis to get a new best estimate for the particle parameters - in particular their momenta. The additional knowledge is represented as constaints, which your decay tree has to fulfill.

For example for the decay
```python
"[D*(2010)+ -> (D0 -> K- pi+) pi+]CC"
``` 
you make the assumption that the (K- pi+) combine to form a D0 with a specific invariant mass. This results in a so called *mass-constraint*. In adition the Kaon and the pion are supposed to originate from exactly the same point in space and the D* should come from the primary vertex. Boundary conditions like those are called *vertex-constraints*. 

Applying such kinematic constraints leads to new best estimates for the track parameters of the final state particles. The process of calculating those is called a *kinematic refit* and the `TupleToolDecayTreeFitter` is the algorithm that performs this task for us. 

> ## The physics and mathematics behind DecayTreeFitter {.callout}
> For details of the method see the paper on [Decay chain fitting with a Kalman filter](http://arxiv.org/abs/physics/0503191)

So how do we use a `TupleToolDecayTreeFitter` to our DaVinci script? Let's create a branch to add the tool to. Let's just name it `"Dstar"`:
```python
from Configurables import TupleToolDecayTreeFitter,TupleToolDecay
dtt.addTool(TupleToolDecay, name = 'Dstar')
dtt.Branches["Dstar"] = "[D*(2010)+ -> (D0 -> K- pi+) pi+]CC" 
```
To this branch we can now apply the `TupleToolDecayTreeFitter`. Recall that for adding tools we add them into the `ToolList` of the tuple or branch. But if we want to configure them we have in addtion to call `addTool`, which will make the configurable available in our python code like this:
```python
dtt.Dstar.ToolList =  ["TupleToolDecayTreeFitter/ConsD"]
dtt.Dstar.addTool(TupleToolDecayTreeFitter("ConsD"))
```
Now we can proceed with the configuration of the fitter. We are going to constrain the decay tree to the primary vertex of origin. We want all the output available, so we set the `verbose` option. Finally we want to apply the mass constraint on the D0:
```python
dtt.Dstar.ConsD.constrainToOriginVertex = True
dtt.Dstar.ConsD.Verbose = True
dtt.Dstar.ConsD.daughtersToConstrain = [ "D0" ]
```
Note that you can constrain more than one intermediate state at once if that fit's your decay. That's it. 

The solution to this exercise `ntuple_DTF1.py`, is [available 
here](./code/22-decay-tree-fitter/ntuple_DTF1.py).