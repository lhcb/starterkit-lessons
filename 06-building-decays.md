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
  2. Combine a pion and a kaon to build a D0, filtering in the necessary physics quantities.
  3. Combine this D0 with a pion to build the D*, again filtering when necessary.

To do that, we need to know a little bit more about how the LHCb analysis framework works.
As discussed in the [Gaudi introduction](01-davinci.html), `Gaudi` is based on the event-by-event sequential (chained) execution of algorithms wrapped in a `GaudiSequencer`, which takes care of handling the execution order such that processing stops when an algorithm is *not passed*.
However, it doesn't handle is the data dependencies between these algorithms nor gives easy access to the used algorithms.
To solve this problem, the [Selection Framework](https://twiki.cern.ch/twiki/bin/view/LHCb/ParticleSelection) was created, and it is based on two types of objects: `Selection` and `SelectionSequence`.

> ## Selections {.callout}
> The `Selection` is the basic unit of the framework. It uses other `Selections` to process `LHCb::Particles` and writes them to a TES location easily findable through its `outputLocation` method. Additionally, it knows about other `Selections` that it requires to pass in order to obtain input particles through its `RequiredSelections` argument.
> A `Selection` requires *all* its `RequiredSelections` to pass.
>
> There are several types of selections, namely
>
> - `Selection`
> - `MergedSelection`, which is used to join the output of several `Selection` objects in a single TES location.
> - `AutomaticData` (also known as `DataOnDemand`), which builds objects from their TES location using a preconfigured map of (location, algorithm) pairs.

> ## Selection sequences {.callout}
> The `SelectionSequence` takes a `Selection` object, resolves its `Selection` requirements, and builds a flat, chained and ordered list of `Selections`. It then exports (via the `selection` method) a self-contained `GaudiSequencer` with all the algorithm configurables necessary to run the selection.
> It also makes the output locations of the data written by the head of the selection chain available via the `outputLocations` method.


To get our input particles we have two options:

 - Use the `DataOnDemand` class, a `Selection` which takes as input a TES location and gets the objects (building them if necessary) from there. This service has a list of 
 -

Selection framework

CommonParticles
CombineParticles
FilterDesktop
