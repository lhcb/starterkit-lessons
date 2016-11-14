---
title: "Intro to the Gaudi framework"
teaching: 20
exercises: 30
questions:
- "What does Gaudi do?"
objectives:
- "Learn the basic concepts of Gaudi."
keypoints:
- "Learn to use Gaudi."
---

# What is the Gaudi framework?

The Gaudi framework runs over a list of events, providing ways to process them and store data in a new format. It creates and manages **Data Objects**, which can hold a variety of data. A **Transiant Event Store (TES)** stores data objects in a way to make them accessible to the rest of the framework, and parts of it can be made persistent in a ROOT format file. The data in the TES is created and accessed by **Algorithms**, which produce data objects and process data objects. Gaudi also
provides **Services**, which provide access to other parts of the framework, such as histograms. **Tools** are lightweight routines that are also available. The Application Manager manages these components.

## Algorithms

This is the most important component of the framework for an user to know. Algorithms are called once per physics event, and (traditionally) implement three methods beyond constructor/destructor: `initialize`, `execute`, and `finalize` (see the details on the upgrade). Also, `beginRun` and `endRun` are available, though be careful not to misuse state.

### Properties

Algorithms are a **Configurable**, which means they can be accessed in Python and **Properties** can be manipulated there. In the classic API, a property is declared in the *constuctor*, using:

```cpp
declareProperty("PropertyName", f_value, "Description of property");
```

Here, `f_value` is a reference to a variable for an int, string, etc. It is almost always a member variable for the class so that you can access it in the other methods.

To use a property, you can simply access it on the configurable in Python:

```python
my_algorithm.PropertyName = 42
```

## Data Objects

To place an item on the TES, it must inherit from `DataObject` (in the classic system) and must have a unique `CLID` identifier. This can be done manually, or can be automated with the [GaudiObjDesc](http://lhcb-comp.web.cern.ch/LHCb-comp/Frameworks/DataDictionary/default.htm) (GOD) system in the LHCb project.

> ## GaudiObjDesc example
>
> To use GOD, add a directory `xml` to your package, and place a GOD xml file in it, with a structure similar to:
>
> <pre><code>xml
&lt;?xml version="1.0" encoding="UTF-8"?&gt;
&lt;!DOCTYPE gdd SYSTEM "gdd.dtd"&gt;
&lt;gdd&gt;
    &lt;package name="ThreeVecPackage"&gt;
        &lt;class
          name="ThreeVecEx"
          author="tbd"
          desc="Some description"
        &gt;
            &lt;base name="DataObject"/&gt;
            &lt;attribute name="x" type="double" desc="Value x"/&gt;
            &lt;attribute name="y" type="double" desc="Value y"/&gt;
            &lt;attribute name="z" type="double" desc="Value z"/&gt;
        &lt;/class&gt;
    &lt;/package&gt;
&lt;/gdd&gt;
</code></pre>
> 
> GOD will automatically create accessors for you, such as `GetX` and `SetX`, and will specialize as needed if the base class is `DataObject`. The output header file will be accessable as `"Event/ThreeVecEx"`, and the class will be in the `LHCb` namespace. 
>
> The project you are in must be one that includes at least LHCb (such as Lbcom, DaVinci, etc) and you need the following lines in your CMakeLists:
>
> ```cmake
include(GaudiObjDesc)
god_build_headers(xml/*.xml)
god_build_dictionary(xml/*.xml)
```
{: .discussion}

## Transient Event Store (TES)

The TES is a place where you can store items on a per-event basis. It should be viewed as non-mutable; meaning that once you place an item in it, it should never change. Persistency is optional and will be covered later. The path to an event should always start with `"/Event"`, though Gaudi is smart enough to assume that a path that does not start with a slash is a relative path, and will get `"/Event/"` prepended to it.
In the classic framework, you used `get` and `put` functions to access the TES. For this you must use `GaudiAlgorithm` instead of `Algorithm`, which is a specialization to add access to the TES. You will need to add `GaudiAlgLib` to the linked library list for the GaudiAlgorithm.

To place an item in the event store, create a pointer to a new object, and then put it in the event store in an `execute` method:

```cpp
auto data = new DataObject();
put(data, "/Event/SomeData");
```

The event store will take ownership of the object, so do not delete it.

To retrieve it, also in an `execute` method:

```cpp
auto data = get<DataObject>("/Event/SomeData");
```

> ## Project: Data communication
>
> Set up two algorithms, one that produces data and one that consumes it. You can copy the above GOD file to give yourself an object to pass.
> 
> You can get a blank template to start this project with:
> 
> ~~~
$ svn export https://github.com/henryiii/DevelopKit/trunk/code/GaudiTemplate MyGaudiProject
~~~
> {: .input}
>
> This will give you a directory called MyGaudiProject with the basics ready. Edit the CMakeLists and directory name inside the project, then you will just need to work on the files in `src` and `options`.
>
> To facilitate building, a `build_and_run.sh` script is included in the project. This will automatically run the commands needed to build the project and run the options file.
>
> > ## Solution
> >  
> > Code solving this is in `/code/GaudiClassicAPI/GaudiDataTrans`
> >
> {: .solution}
{: .challenge}



> ## Futher reading
> 
> There are further examples in the Gaudi repository: [Gaudi/GaudiExamples/src](https://gitlab.cern.ch/gaudi/Gaudi/tree/master/GaudiExamples/src).
> 
> * [Gaudi workshop 2016](https://indico.cern.ch/event/556551/)
> 
{: .callout}
