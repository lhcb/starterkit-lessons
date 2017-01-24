---
title: "Upgrading an Algorithm"
teaching: 40
exercises: 10
questions:
- "What is the procedure for convering an algorithm?"
objectives:
- "Learn about upgrading algorithms to the new Gaudi framework."
keypoints:
- "Work with Algorithms."
---

## Steps to convert

The following are the five steps to upgrade an algorithm.

### Identify TES interactions

First, find and identify the input data and output data. You are looking for `get`, `put`, or `getIfExists`. For example:

> ## Example: Locating TES interactions
> 
> ~~~
> egrep 'get|put' Pr/PrAlgorithms/src/PrMatchNN.cpp
> ~~~
> {: .input}
> 
> #### Output:
>
> ~~~
> declareProperty("VeloInput", m_veloLocation=LHC...
> declareProperty("SeedInput", m_seedLocation=LHC...
> declareProperty("MatchOutput", m_matchLocation=...
> put(matchs, m_matchLocation);
> LHCb::Tracks* velos = getIfExists<LHCb::Tracks>(m_veloLocation);
> LHCb::Tracks* seeds = getIfExists<LHCb::Tracks>(m_seedLocation);
> ~~~
> {: .output}
> 
> In this case, the `Velo Tracks` + `Seed Tracks` are producing `Output Tracks`.
{: .discussion}

### Deduce functional algorithm to use

   Out/in        |      0     |          1-n             |       vector         |
----------------:|:----------:|:------------------------:|:---------------------|
 **0**           |            | `Consumer`               |                      |
 **1**           | `Producer` | `Transformer`            | `MergingTransformer` |
 **n**           | `Producer` | `MultiTransformer`       |                      |
 **vector**      |            | `SplittingTransform`     |                      |
 **boolean**     |            | `FilterPredicate`        |                      |
 **boolean+1-n** |            | `MultiTransformerFilter` |                      |


> ## Possible inputs
>
> * **0** : no input, pure producer
> * **1-n** : one or several independent inputs. This number and the type of each input must be known at compile time
> * **vector** : any number of inputs (not know at compile time), all of the same type, aka a vector of inputs of the same type
{: .callout}

> ## Possible outputs
>
> * **0** : no output, pure consumer
> * **1-n** : one or several independent outputs. This number and the type of each output must be known at compile time
> * **vector** : any number of outputs (not know at compile time), all of the same type, aka a vector of outputs of the same type
> * **boolean** : an additional boolean output, saying whether we should carry on or stop the processing, aka a filter
{: .callout}


### Modify Algorithm declaration

* Change inheritance from Algorithm to your functional algorithm base class. 
* Template the functional algorithm with the “signature” of the algorithm, that is its data flow.

> ## Example: Declaration
> 
> In the case of `Velo Tracks` + `Seed Tracks` producing `Output Tracks`:
>
> ~~~
> Gaudi::Functional::Transformer
>       <LHCb::Tracks(const LHCb::Tracks&, const LHCb::Tracks&)>
> ~~~
> {: .code}
> 
> In the case of `Tracks` producing `Vector of Tracks`:
>
> ~~~
> Gaudi::Functional::SplittingTransformer
>       <std::vector<LHCb::Tracks> (const LHCb::Tracks&)>
> ~~~
> {: .code}
{: .discussion}

* All inputs are now using `const` references.
* All declarations of members storing locations of input/output can be dropped (automatic retrieval from TES).
* Equivalent properties are automatically defined by the functional framework.

> ## Example: Member decleration
> 
> #### Member decleration:
>
> ~~~
> std::string m_veloLocation;
> ~~~
> {: .code}
> 
> #### Used in constructor as:
>
> ~~~
> declareProperty("VeloInput", m_veloLocation=...
> ~~~
> {: .code}
{: .discussion}


> ## Example: PrMatchNN
> 
> #### Old:
>
> ~~~
> #include "GaudiAlg/GaudiAlgorithm.h"
> class PrMatchNN : public GaudiAlgorithm {
>    ...
>    std::string m_veloLocation;
>    std::string m_seedLocation;
>    std::string m_matchLocation;
> ~~~
> {: .code}
> 
> #### New:
>
> ~~~
> #include "GaudiAlg/Transformer"
> class PrMatchNN :
>     public Gaudi::Functional::Transformer
>     <LHCb::Tracks(const LHCb::Tracks&, const LHCb::Tracks&)> {
>     ...
> ~~~
> {: .code}
{: .discussion}


### Modify constructor/destructor

#### Changes

* Initialize functional algorithm rather than `Algorithm`
* New arguments are needed
    * Input location / list of input locations in TES
    * Output / list of output locations in TES
* Locations are given using `KeyValue` objects, such as `KeyValue{"VeloInput", LHCb::TrackLocation::Velo}`
* This creates the adequate property in the back
    * So old `declareProperty` lines can be dropped
* For each location, a default might be given

> ## Example: PrMatchNN
> 
> #### Old:
>
> ~~~
> PrMatchNN::PrMatchNN(const std::string& name, ISvcLocator* pSvcLocator) :
>   GaudiAlgorithm(name, pSvcLocator), ... {
>     declareProperty("VeloInput", m_veloLocation=...
>     declareProperty("SeedInput", m_seedLocation=...
>     declareProperty("MatchOutput", m_matchLocation=...
> ~~~
> {: .code}
>
> #### New:
> 
> ~~~
> PrMatchNN::PrMatchNN(const std::string & name, ISvcLocator* pSvcLocator) :
>  Transformer(name, pSvcLocator,
>    {KeyValue{"VeloInput", LHCb::TrackLocation::Velo},
>     KeyValue{"SeedInput", LHCb::TrackLocation::Seed},
>     KeyValue{"MatchOutput", LHCb::TrackLocation::Match}}),
>  ... {
> ~~~
> {: .code}
{: .discussion}

### Convert execute into operator(...)

The declaration of `operator()` will have a signature matching the data flow, and will be made using `const` references. The definition is of the form:

~~~
Output operator() (const Input&, ...) const override;
~~~
{: .code}

The input is a `const` reference, the method itself is both `const` and will `override` another method. The output is returned by value. It is a good idea to add `std::move(...)` around the return value to make sure a copy is not made; compilers, especially newer ones, may do this for you.

> ## Compared to Execute
> 
> * Drop all interactions with TES (`get`/`put`)
> * Replace memory allocation of the output by simple creation on the stack
> * Thus change accordingly `->` into `.`, drop some `*`
> * Return output rather than a `StatusCode`
> * Throw an exception when you were returning a bad `StatusCode`
>
{. callout}

> ## Example: PrMatchNN
>
> To simplify the following example, the status code checks have been removed.
>
> #### Execute method:
>
> ~~~
> LHCb::Tracks* matchs = new LHCb::Tracks;
> put(matchs, m_matchLocation);
> LHCb::Tracks* velos = getIfExists<LHCb::Tracks>(m_veloLocation);
> LHCb::Tracks* seeds = getIfExists<LHCb::Tracks>(m_seedLocation);
> StatusCode sc = m_matchTool->match(*velos, *seeds, *matchs);
> return sc;
> ~~~
> {: .code}
>
> #### Operator ():
>
> ~~~
> LHCb::Tracks matchs;
> StatusCode sc = m_matchTool->match(velos, seeds, matchs);
> return matchs;
> ~~~
> {: .code}
>
> To convert, first the TES interactions were dropped (now part of the header), memory allocation was removed (created on the stack), dereferencing pointers was removed, and the object was returned instead of the `StatusCode`.
{: .discussion}

## Improving

These changes were made to give faster, more readable code, easy transition to parallel Gaudi, and more checks at compile time. To truly get improvements to the code, a few further steps should be taken to update the algorithms while it is already being modified and reviewed.

### Property

`Gaudi::Property` provides a simple, clean way of declaring properties, and almost obsoletes the old `declareProperty`. It is not completely backward compatible, but fixes are trivial and usually result in better code. The property setup will exist in one place (header) instead of two (header and constructor).

> ## Example: Property conversion
> 
> #### Old header
> 
> ~~~
> bool m_skipFailedFitAtInput;
> double m_maxChi2DoF;
> ~~~
> {: .code}
> 
> #### Old constructor
> 
> ~~~
> declareProperty("SkipFailedFitAtInput", m_skipFailedFitAtInput=true);
> declareProperty("MaxChi2DoF", m_maxChi2DoF=9999, "Max chi2 per track");
> ~~~
> {: .code}
> 
> #### New header
> 
> ~~~
> Gaudi::Property<bool> m_skipFailedFitAtInput
>     {this, "SkipFailedFitAtInput", true };
> Gaudi::Property<double> m_maxChi2DoF
>     {this, "MaxChi2DoF", 9999, "Max chi2 per track"};
> ~~~
> {: .code}
{: .discussion} 

In most cases, this change is transperent and you can use the property the same way. In cases where it does not work, such as when using `->` for a pointer property, you can use `.value()->` instead. Also, printing a property to an iostream is customized, giving name and value. If the old print is needed, use `.value()` to output just the value.

### Modern C++

This is a great time to update to modern C++ syntax. Although all C++11 and much of C++14 is readily available, even in the base-line GCC 4.9 compiler, the following changes are especially useful:

* Member initialization: By moving member initialization to the declaration, you can simplify the constructor and discover missing initializations.
* Use `nullptr` instead of `0` or `NULL`: Type safety is always a good thing, and can catch pointer bugs.
* Use `using` constructor if possible: if everything can be moved out of the constructor, you can use a `using` directive instead to further clean up code.
* Use `auto` and range-based loops: Often it is nicer to your reader to be explicit, but in many cases, the type does not matter or is something complex, like an iterator, that really does not need to be known. Those can be moved to using `auto` to clean up code. Also, closely related, moving to range-based for loops can make code much, much easier to read.
* Lambdas: Using the built in lambdas instead of `boost::lambda` and its quirks. Also can be used to move some tasks to the STL that used to be loops.


### Thread safety

Moving to the new framework *should* take care of thread safety by using `const`, but there are a few ways around it that, unfortionatly, are common in our code base. You can remove `const` with `const_cast`, and a member variable can be changed in a `const` method if it is declared `mutable`. As a general rule, you should only see `mutable` in C++11 when the variable itself is thread-safe (for example, a mutex). It should *not* be used to simply make adapting bad code easier. To remove
them, you can usually simply remove the `const_cast` or `mutable`, then trace down the compiler errors to see what is happening.

Often these are introduced because code is not using the TES for data transmission; the new `AnyDataHandle` makes that trivial to do now, so the best solution is to put the data in the TES using an `AnyDataHandle`.

You'll want to remove `Incident`s, as well. You'll see something like this:

~~~
void handle(const Incident&) override final;
~~~
{: .code}

And, in the constructor:

~~~
incSvc()->addListener(this, IncidentType::EndEvent);
~~~
{: .code}

In mose cases, this is only needed to reset a cache at the start of the event. The cache needs to be removed to be thread safe anyway, so that also solves the `Incident` issue. In the other cases, the TES or function arguments should replace the communication that the `Incident` was providing.

> ## Further reading
> 
> * [5th hackthon talk](https://indico.cern.ch/event/595667/) (this lesson was based on the presentation here by S. Ponce.)
{: .callout}
