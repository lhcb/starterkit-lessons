---
layout: page
title: First Steps in LHCb
subtitle: General Introduction to DaVinci
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

**What is the relationship between DaVinci and Gaudi?**

**How do I run Gaudi using my option file?**

On your terminal, after you have successfully initialised your environment type

```bash
gaudirun.py myoptionfile.py
```

