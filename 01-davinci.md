---
layout: page
title: First Steps in LHCb
subtitle: General Introduction to DaVinci
minutes: 5
---

> ## Learning Objectives {.objectives}
> * Which problems specific to HEP/LHCb is Gaudi designed to solve?
> * What are Algorithms and Tools?
> * What is an option file?
> * What is the relationship between DaVinci and Gaudi?
> * What are `lb-run` and `lb-dev`?
> * How do I run Gaudi using my option file?
> * How can I browse the code/documentation of the LHCb software?
> * How can I check out an LHCb package?

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

TODO: This is all very abstract. Needs some diagrams and/or code examples.

**What is the relationship between DaVinci and Gaudi?**

**What are `lb-run` and `lb-dev`?**

Both are small programs that help you set up an environment for your LHCb software. If you just want to run software without making any modifications, `lb-run` is your tool of choice. In order to (for example) run `v36r6` of DaVinci, you just type

```bash
lb-run DaVinci v36r6 bash
```

and the script automatically starts a new bash session for you where all necessary environment variables are set for you. If you want to confirm that this indeed worked, just run `gaudirun.py`.

For an overview of all available versions of DaVinci type

```bash
lb-run --list DaVinci
```

If instead you want to make changes to the software, you will need to set up a development environment. `lb-dev` is your friend here:

```bash
lb-dev --name DaVinciDev DaVinci v36r6
```

<!-- This doesn't seem to work atm
Alternatively, if you want to use the most recent nightly builds:

```bash
lb-dev --name DaVinciDev --nightly lhcb-head DaVinci
```
-->

The output should look similar to this:

```
Successfully created the local project DaVinciTest in .

To start working:

  > cd ./DaVinciTest
  > getpack MyPackage vXrY

then

  > make
  > make test
  > make QMTestSummary

and optionally

  > make install

You can customize the configuration by editing the files 'CMakeLists.txt'
(see http://cern.ch/gaudi/CMake for details).
```

Follow those instructions and once you've compiled your software run

```bash
./run bash
```

inside the directory. This will (similar to `lb-run`) give you a new bash session with the right environment variables.



**How do I run Gaudi using my option file?**

On your terminal, after you have successfully initialised your environment type

```bash
gaudirun.py myoptionfile.py
```

**How can I check out an LHCb package?**

If you want to make local changes to a software package, you will have already used `lb-dev` to set up your development environment. In order to obtain the source code of the package you want to work on, use `getpack`. That is an LHCb-aware wrapper around SVN. For example if you want to write your custom stripping selections, write

```bash
lb-dev --name DaVinciDev # latest version number here
cd DaVinciDev
getpack Phys/StrippingSelection
# Make your local changes
make
./run bash
```

Under the hood, `getpack` will _checkout_ (≈ `git clone`) the corresponding SVN repository. If you have made changes that are supposed to be integrated into the official LHCb repositories, you can use `svn commit`. Be advised that you should always communicate with the package maintainers before committing changes.


**How can I browse the code/documentation of the LHCb software?**

The code is maintained in an SVN repository that can be accessed comfortably through two different web UIs:

 * [Trac](https://svnweb.cern.ch/trac/lhcb/)
 * [Svnweb](http://svnweb.cern.ch/world/wsvn/lhcb)

Documentation for the code is automatically generated by Doxygen. There is a page for each projects a list of which can be found here: [https://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/davinci/latest_doxygen/index.html](https://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/davinci/latest_doxygen/index.html).
