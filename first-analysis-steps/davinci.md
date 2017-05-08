---
layout: page
title: First Steps in LHCb
subtitle: An introduction to LHCb Software
minutes: 10
---

> ## Learning Objectives {.objectives}
> * Learn the key concepts needed to work with the LHCb software
> * Learn how to launch the LHCb software with `lb-run`

Imagine you want to design and run a new particle detector.
Apart from organizing a collaboration, creating the design and specification, and several other tasks, you will also have to find solutions to many computational challenges.
It's worth thinking about these for a second:

 - How do we collect data as it is recorded by the detector?
 - How do we filter and process the recorded data efficiently?
 - How do we manage all the complex tasks required to work with collision data?
 - How do we organize all the data of a single bunch crossing in a flexible way?
 - How do we configure our software flexibly without having to recompile it?
 - Can you think of more?

How would you go about solving these?
The decisions you make will affect the performance of your experiment during datataking and analysis.

At LHCb, we base our software on the [Gaudi](https://proj-gaudi.web.cern.ch/proj-gaudi/) framework, which was specifically designed with the above questions in mind.
It's worth getting an idea of some of the most important Gaudi concepts at this point.
After this, we will jump right into running the software and getting useful things done.

**Event Loop**
Because the individual bunch crossings (events) are almost completely independent of each other, it makes sense to process them one by one, without holding them all in memory at once.
Gaudi provides a global EventLoop, which allows you to process events one by one.

**Transient Event Store**
A single event contains lots of different data objects (Particles, Vertices, Tracks, Hits, ...).
In Gaudi, these are organized in the Transient Event Store (TES).
You can think of it as a per-event file system with locations like `/Event/Rec/Track/Best` or `/Event/Phys/MyParticles`.
When running over the event stream, Gaudi allows you to get and put from/to these locations.
The contents of the TES are emptied at the end of the processing of each event.

**Algorithms**
An *Algorithm* is a C++ class that can be inserted into the EventLoop.
These allow you to perform a certain function for each event (like filtering according to trigger decision, reconstructing particles, ...).

**Tools**
Often, algorithms will want to make use of some common function (vertex fitting, calculating distances, associating a primary vertex, ...).
These are implemented as *Tools*, which are shared between Algorithms.

**Options**
To make all of this configurable, Gaudi allows you to set properties of *Algorithms* and *Tools* from a Python script, called an *option* file.
In an option file, you can specify which Algorithms are run in which order, and set their properties (strings, integers, doubles, and lists and dicts of these things can be set).
You can then start the Gaudi EventLoop using this option file, and it will set up and run the corresponding C++ objects with specified settings.

You can find comprehensive documentation in the [Gaudi Doxygen](https://proj-gaudi.web.cern.ch/proj-gaudi/releases/latest/doxygen/) or the [Gaudi Manual](http://lhcb-comp.web.cern.ch/lhcb-comp/Frameworks/Gaudi/Gaudi_v9/GUG/GUG.pdf).

Usually, you will work with one of the LHCb software projects that are based on Gaudi.
One of the most important ones is *DaVinci*, which provides lots of *Algorithms* and *Tools* for physics analysis.

You can run DaVinci using the following command:
```bash
lb-run DaVinci/v41r2 gaudirun.py
```

This will run the `gaudirun.py` command using version v41r2 of DaVinci.
`gaudirun.py` is a script that sets up the EventLoop.
You should get the following output:

```
# setting LC_ALL to "C"
ApplicationMgr    SUCCESS
====================================================================================================================================
                                                   Welcome to DaVinci version v41r2
                                          running on lxplus0107.cern.ch on Tue Oct 25 17:26:01 2016
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
lb-run DaVinci/v41r2 gaudirun.py options.py
```

An `option.py` is just a regular Python script that specifies how to set things up in the software.
Many of the following lessons will teach you how to do something with DaVinci by showing you how to write or extend an `options.py`.
You can use the above command to test it.
You can also specify several option files like this:
```bash
lb-run DaVinci/v41r2 gaudirun.py options1.py options2.py
```
They will then both be used to set up DaVinci.

Do you want to get an overview of which versions of DaVinci exist? Use
```bash
lb-run --list DaVinci
```
Do you want to start a shell that already contains the LHCb environment, so you don't have to use `lb-run`?
Execute
```bash
lb-run DaVinci/v41r2 $SHELL
```
A simple `gaudirun.py` should work as well now.
Typing `exit` will close the shell and leave the LHCb environment behind.

> ## Using SetupProject instead of lb-run {.callout}
> When reading through other tutorials, you will come across `SetupProject`.
> This is an older way of setting up a shell that is configured to run LHCb software.
> `lb-run` is the new way of doing things and has some nice benefits over `SetupProject`.
> For most purposes, `SetupProject DaVinci v41r2` is equivalent to
> ```bash
> lb-run DaVinci/v41r2 $SHELL
> ```
