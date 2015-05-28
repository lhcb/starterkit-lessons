---
layout: page
title: First Steps in LHCb
subtitle: Introduction to the LHCb Software
minutes: 10
---

> ## Learning Objectives {.objectives}
> * Which basic problems is the LHCb software framework designed to solve?
> * Learn the key concepts needed to work with the LHCb software
>

A HEP experiment like LHCb needs to solve many computational challenges, some of which are

 - How do we collect data as it is recorded by the detector?
 - How do we filter and process the recorded data efficiently?
 - How do we manage all the complex tasks required to work with particle events?
 - How do we organize all the data of a single particle event in a flexible way?
 - How do we configure our software flexibly without having to recompile it?

At LHCb, we base our software on the [Gaudi](https://proj-gaudi.web.cern.ch/proj-gaudi/) framework, which was specifically designed with the above questions in mind.
In order to understand how the LHCb software works, you should know about the following basic Gaudi concepts:

 - Because the individual collisions are almost completely independent of each other, it makes sense to process them one by one (as a *stream*), without holding them all in memory at once.
   Gaudi provides a global *EventLoop*, which runs over individual events, and allows you to process them one by one
 - A single event contains lots of different data objects (*Particles*, *Vertices*, *Tracks*, *Hits*, ...).
   In Gaudi, these are organized in the *Transient Event Store* (TES).
   You can think of it as a per-event file system with locations like `/Event/Rec/Track/Best` or `/Event/Phys/MyParticles`.
   These usually contain containers of C++ objects.
   When running over the event stream, Gaudi allows you to `get` and `put` to/from these locations, like a key-value store.
 - An *Algorithm* is a C++ class with `initialize`, `execute` and `finalize` methods.
   These allow you to set up and perform a certain function on the event stream (Like filtering events, calculating/deleting data, ...).
 - Often, algorithms will want to make use of common functionality (vertex fitting, finding primary vertices, ...).
   These are implemented as *Tools*, which are shared between Algorithms.
 - To make all of this configurable, Gaudi allows you to set properties of *Algorithms* and *Tools* from a Python script, called an *option* file.
   In an option file, you can specify which Algorithms are run in which order, and set their properties (strings, integers, doubles, and lists and dicts of these things can be set).
   You can then start the Gaudi EventLoop using this option file, and it will set up and run the corresponding C++ objects with specified settings.

You can find comprehensive documentation in the [Gaudi Doxygen](https://proj-gaudi.web.cern.ch/proj-gaudi/releases/latest/doxygen/) or the [Gaudi Manual](http://lhcb-comp.web.cern.ch/lhcb-comp/Frameworks/Gaudi/Gaudi_v9/GUG/GUG.pdf).

Gaudi is just the foundation of the LHCb software that several other packages build on.
One of the most important packages is *DaVinci*, which provides code (lots of *Algorithms* and *Tools*) for analysing physics events.

You can run DaVinci using the following command:
```bash
lb-run DaVinci v36r6 gaudirun.py
```

This will run the `gaudirun.py` command using version v36r6 of DaVinci.
`gaudirun.py` is a script that sets up the EventLoop.
You should get the following output:

```
# setting LC_ALL to "C"
ApplicationMgr    SUCCESS 
====================================================================================================================================
                                                   Welcome to DaVinci version v36r6
                                          running on lxplus0077.cern.ch on Thu May 28 11:09:31 2015
====================================================================================================================================
ApplicationMgr       INFO Application Manager Configured successfully
HistogramPersis...WARNING Histograms saving not required.
ApplicationMgr       INFO Application Manager Initialized successfully
ApplicationMgr       INFO Application Manager Started successfully
EventSelector        INFO End of event input reached.
EventLoopMgr         INFO No more events in event selection 
ApplicationMgr       INFO Application Manager Stopped successfully
EventLoopMgr         INFO Histograms converted successfully according to request.
ToolSvc              INFO Removing all tools created by ToolSvc
ApplicationMgr       INFO Application Manager Finalized successfully
ApplicationMgr       INFO Application Manager Terminated successfully
```

During this run, DaVinci didn't do anything: We didn't specify any algorithms to run or any data to run over.
Usually, you will write an option file (e.g. `options.py`) and specify it as an argument to `gaudirun.py`:

```bash
lb-run DaVinci v36r6 gaudirun.py options.py
```

Many of the later lessons will introduce a certain functionality in DaVinci and show you how to use it in your `options.py`.
You can then use the above command to test it.

