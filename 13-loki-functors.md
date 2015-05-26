---
layout: page
title: First steps in LHCb
subtitle: LoKi functors. What are they and how can we use them
minutes: 15
---

> ## Learning Objectives {.objectives}
>
> * Understand what LoKi functors are
> * Learn how to use them in a DecayTreeTuple
> * Be able to find functors that do what we want

LoKi functors are designed to flexibly compute and compare properties of the current decay, from simple quantities such as the tranverse momentum of particle to complicated ones like helicity angles.
Internally, functors are implemented as C++ classes that take an object of type `TYPE1` and return another of `TYPE2`.
They can be used both in C++ and in python code, and can be combined with each other using logical operations.

According to `TYPE2` there are 3 types of functors:
 
 - *Functions*, which return `double`.
 - *Predicates*, which return a `bool`.
 - *Streamers*, which return a `std::vector` of some other type `TYPE3`.

When filling tuples, the most used functors are functions, while predicates are typically used for selections.

According to `TYPE1`, there are many types of functors, the most important of which are (you can find a full list in the [LoKi FAQ](https://twiki.cern.ch/twiki/bin/view/LHCb/FAQ/LoKiFAQ#How_to_code_own_LoKi_functor)):

 - *Particle functors*, which take `LHCb::Particle*` as input. 
 - *Vertex functors*, which take `LHCb::VertexBase*` as input.
 - *MC particle functors*, which take `LHCb::MCParticle*` as input. 
 - *MC vertex functors*, which take `LHCb::MCVertex*` as input.
 - *Array particle functors*, which take a `LoKi::Range_` (an array of particles) as input.
 - *Track functors*, which take `LHCb::Track` as input.

To understand what we can do with LoKi functors, we will start with exploring a [DST interactively](05-interactive-dst.html).
Open the DST and get the first candidate in the `D2hhCompleteEventPromptDst2D2RS` line:

```python
cands = evt['/Event/AllStreams/Phys/D2hhCompleteEventPromptDst2D2RSLine/Particles']
cand = cands[0]
```

We can now try to get very simple properties of the B candidate, such as its transverse momentum or measured mass.

```python
from LoKiPhys.decorators import PT, MM
print PT(cand)
print MM(cand)
```

If we want to get the properties of the B vertex, for example its chi2, we need to pass the correct object to the LoKi functors

```python
from LoKiPhys.decorators import VCHI2
print VCHI2(cand.endVertex())
```

This is inconvenient when running from python options, since in that case we don't have any way of calling the `endVertex` method. 
In that case, we can use the `VFASPF` adaptor functor, which allows to use vertex functors as if they were particle functors (note how the functor is built by combining two functors)

```python
from LoKiPhys.decorators import VFASPF
VCHI2(cand.endVertex()) == VFASPF(VCHI2)(cand)
```

The calculation of some of the properties, such as the IP or DIRA, require the knowledge of the primary vertex associated to the candidate.
For this, there is a special set of functors, starting with the `BPV` (for Best PV) prefix, which get the PV for us using the `IPhysDesktop::relatedVertex` method and calculate the value of the property.
This can only be done in the context of a DaVinci application (Stripping, for example), which is where `IPhysDesktop` is defined.
In `GaudPython`, we can get the PVs ourselves.

```python
pv_finder_tool = appMgr.toolsvc().create('GenericParticle2PVRelator<_p2PVWithIPChi2, OfflineDistanceCalculatorName>/P2PVWithIPChi2', interface='IRelatedPVFinder')
pvs = evt['/Event/AllStreams/Rec/Vertex/Primary']
best_pv = pv_finder_tool.relatedPV(cand, pvs)
from LoKiPhys.decorators import DIRA
print DIRA(best_pv)(cand)
```

TODO: Until now we've looked at the properties of the B, but what if we want to access its daughters?

