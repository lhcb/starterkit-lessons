---
title: "The Gaudi framework"
teaching: 60
exercises: 60
questions:
- "Why is Gaudi used?"
objectives:
- "Learn about the Gaudi framework."
keypoints:
- "Work with Gaudi."
---

## Goals of the design

Gaudi is being improved with the following goals in mind:

* Improve scaling by making the interface simpler and more uniform. This allows simpler code, thereby allowing more complex code to be written.
* Improve memory usage, to be cache friendly and avoid pointers to pointers. Use C++11 move
* Thread-safe code, with no state or local threading

In order to reach those goals, the following choices should be made when designing Gaudi algorithms:

* Proper `const` usage: Since C++11, `const` and `mutable` are defined in terms of data races (see [this video](https://channel9.msdn.com/posts/C-and-Beyond-2012-Herb-Sutter-You-dont-know-blank-and-blank))
* No global state: impossible to multithread
* Tools: should be private or public
* Ban `beginEvent`/`endEvent`, since there might be multiple events
* No explicit `new`/`delete` usage
* Remove `get/put<MyData>("")`

## Gaudi::Functional

Rather than provide a series of specialised tools, the new `Gaudi::Functional` provides a general building block that is well defined and multithreading friendly. This standardizes the common patter of getting data our of the TES, working on it, and putting it back in (in a different location). This reduces the amount of code, makes it more uniform, and encourages 'best practice' procedures in coding in the event store. The 'annoying details', or scaffolding is left to the
framework.

The functionals are named by the data they work on (with examples):

* One input, no output: Consumer
    - `Rec/RecAlgs`: `EventTimeMonitor`, `ProcStatusAbortMoni`, `TimingTuple`
* No input, one or more output: `Producer`
* True/False only as output: `FilterPredicate`
    - `Phys/LoKiHlt`: `HDRFilter`, `L0Filter`, `ODINFilter`
    - `Phys/LoKiGen`: `MCFilter`
    - `Hlt/HltDAQ`: `HltRoutingBitsFilter`
    - `Rec/LumiAlgs`: `FilterFillingScheme`, `FilterOnLumiSummary`
* One or more input, one output: `Transformer`
* One or more input, more than one output: `MultiTransformer`
* Identical inputs, one output: `MergingTransformer`
    - `Calo/CaloPIDs`: `InCaloAcceptanceAlg`
    - `Tr/TrackUtils`: `TrackListMerger`
* One input, identical outputs: `SplittingTransformer`
    - `Hlt/HltDAQ`: `HltRawBankDecoderBase`
* Converting a scalar transformation to a vector one: `ScalarTransformer`
    - `Calo/CaloReco`: `CaloElectronAlg`, `CaloSinglePhotonAlg`

### Consumer

This is a class that takes in TES data. It looks like:

```cpp
class EventTimeMonitor : public Gaudi::Functional::Consumer<
                void(const LHCb::ODIN&),
                Gaudi::Functional::Traits::BaseClass_t<GaudiHistoAlg>
                > {
    
    EventTimeMonitor(const std::string& name, ISvcLocator* pSvcLocator)
        : Consumer(name, pSvcLocator, KeyValue{"Input"}, LHCb::ODINLocation::Default } ) 
    { ... }

    void EventTimeMonitor::operator()(const LHCb::ODIN& odin) const override
    { ... }
```

The class is created inheriting the templated class `Consumer`, with the signature of the `operator()` function as it's first argument, and the `Traits`, which allows the selection of a base class to use (default is `GaudiAlgorithm`.

The constructor should delegate the Consumer constructor, with one additional argument, the input that it will use. This is done by constructing a `KeyValue` with property `"input"` set (in this case, to the default location).

The work is then done in the `operator()` method.

## Producer

This is the inverse of the consumer. Produces `Out1` to `OutN` given nothing:

```cpp
template <typename... Out, typename Traits_>
class Producer<std::tuple<Out...>(), Traits_> {
      virtual std::tuple<Out...> operator()() const = 0;
}
```

The tuple that is produced can be replaced with a single type if only one output is produced. The pass is done by value, due to move improvements in modern C++. 

## FilterPredicate

## Transformers


## AnyDataHandle



# Conversion

Try to convert everything to a `Gaudi::Functional` algorithm. Split into smaller basic algorithms and build a `SuperAlgorithm` from the smaller ones if needed.

At least try to migrate to data handles + tool handles.

Make sure the event loop code is `const`; do not cache event dependent data. Interface code, especially for tools, should be `const`.

Try to migrate away from `beginEvent`, `endEvent` incidents.

Please add as many tests as possible!


