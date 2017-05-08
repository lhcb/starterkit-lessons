# Building your own decay. The Selection Framework

{% objectives "Learning Objectives" %}
* Learn the concepts behind the LHCb selection framework
* Learn the advantages of the LHCb selection framework
{% endobjectives %} 

In order to perform most physics analyses we need to build a *decay chain* with reconstructed particles that represents the physics process we want to study.
In LHCb, this decay chain can be built through `LHCb::Particle` and `LHCb::MCParticle` objects that represent individual particles and contain links to their children, also represented by the same type of object.

We'll learn all the concepts involved by running through our usual full example of the $$D^\ast\rightarrow D^0(\rightarrow K^{-} \pi^{+}) \pi$$ decay chain.

The LHCb approach to building decays is from the bottom up. Therefore, to build $$D^\ast\rightarrow D^0(\rightarrow K^{-} \pi^{+}) \pi$$ we need to

  1. Get input pions and kaons and filter them according to our physics needs.
  2. Combine a pion and a kaon to build a $$D^0$$, and apply selection cuts to it.
  3. Combine this $$D^0$$ with a pion to build the $$D^\ast$$, again filtering when necessary.

To do that, we need to know a little bit more about how the LHCb analysis framework works.

As discussed in the [Gaudi introduction](../first-analysis-steps/davinci.md), `Gaudi` is based on the event-by-event sequential (chained) execution of algorithms wrapped in a `GaudiSequencer`, which takes care of handling the execution order such that processing stops when an algorithm is *not passed*.
However, it does not handle the data dependencies between these algorithms nor does it give easy access to them.
To solve this problem, the [Selection Framework](https://twiki.cern.ch/twiki/bin/view/LHCb/ParticleSelection) was created, and it is based on two types of objects: `Selection` and `SelectionSequence`:

  - The `Selection` is the basic unit of the framework. It uses DaVinci algorithms to process `LHCb::Particles`and writes them to a TES location easily findable through its `outputLocation` method. Additionally, it knows about other `Selections` that it requires to pass in order to obtain input particles through its `RequiredSelections` argument. A `Selection` requires *all* of its `RequiredSelections` to pass.

  - The `SelectionSequence` takes a `Selection` object, resolves its `Selection` requirements, and builds a flat, chained and ordered list of `Selections`. It then exports (via the `selection` method) a self-contained `GaudiSequencer` with all the algorithm configurables necessary to run the selection.
 It also makes the output locations of the data written by the selection chain available via the `outputLocations` method.

Additionally, we need algorithms to perform the particle combination and filtering (or other data processing capabilities), since `Selection`s don't do any work by themselves, but just organize the dependencies properly.
Two of the most important data processing algorithms are `CombineParticles` and `FilterDesktop`, which will be discussed in the [next lesson](building-decays-part1.html).

The advantages of using this framework are several:

  - Building the algorithms with bare Configurables and chaining their `Input` and `Output` is a responsibility of the users.
  This means the user needs to put the algorithms in the correct order, not only chaining properly the inputs/outputs but also executing the algorithms in sequence. The use of the Selection Framework places all algorithms in *correct sequences in the optimal order*, allowing to achieve the maximal possible CPU efficiency.
  - Its *reusability*: one can use/re-use the same code for data analysis on "user"-level for MC processing for building of Stripping lines, since usually only the input particles need to be changed.
  - *Easiness of debugging*: one can visualize the selection chain, for example using the `PrintSelection` algorithm for debugging of the data flow.
  - Some (advanced) tasks are *virtually impossible* to do without it, such as accessing some features for MC µDST (MCTruth for inclusive lines, for example) or applying the momentum scaling in Turbo.

{% callout "The LHCb singletons and usability" %}
A big part of the reusability of the Selection objects is thanks to how the 
LHCb framework is designed:
all LHCb algorithms need an explicit and unique name because they are *singletons* (a [singleton](http://en.wikipedia.org/wiki/Singleton_pattern) is a software design pattern that restricts the instantiation of a class to one object).
As a consequence of this, only one algorithm with a given name can be instantiated.

This allows to reuse and reload algorithms that have already been created in a configuration sequence. For example:

 We could create a generic selection for building $$D^0$$ with a known name, put it in a `build_d0.py` file and use (and even modify) it in another file loaded in the same `gaudirun.py` call.
 We could write the data-only parts of our selection in one file and the MC-only separately (typically inputs are different), but setting the same names for the algorithms. Then, our `DecayTreeTuple` code could be common, as the selection would be loaded "by-name".

This is very useful to build complicated configuration chains, but it's *very easy* to have problems if our selection/algorithm names are not unique.
Therefore, it's very important to pay attention to the algorithm names (we will see how to minimize this problem in the [final Selection Framework lesson](building-decays-part2.html)).

{% endcallout %}
