# An introduction to LHCb Software

{% objectives "Learning Objectives" %}

* Learn the key concepts needed to work with the LHCb software
* Learn how to launch the LHCb software with `lb-run`

{% endobjectives %} 

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

At LHCb, we base our software on the [Gaudi](https://gaudi.web.cern.ch/gaudi/) framework, which was specifically designed with the above questions in mind.
It's worth getting an idea of some of the most important Gaudi concepts at this point.
After this, we will jump right into running the software and getting useful things done.

**Event Loop:**
Because the individual bunch crossings (events) are almost completely independent of each other, it makes sense to process them one by one, without holding them all in memory at once.
Gaudi provides a global EventLoop, which allows you to process events one by one.

**Transient Event Store:**
A single event contains lots of different data objects (Particles, Vertices, Tracks, Hits).
In Gaudi, these are organized in the Transient Event Store (TES).
You can think of it as a per-event file system with locations like `/Event/Rec/Track/Best` or `/Event/Phys/MyParticles`.
When running over the event stream, Gaudi allows you to get and put from/to these locations.
The contents of the TES are emptied at the end of the processing of each event.

**Algorithms:**
An *Algorithm* is a C++ class that can be inserted into the EventLoop.
These allow you to perform a certain function for each event (like filtering according to trigger decision, reconstructing particles).

**Tools:**
Often, algorithms will want to make use of some common function (vertex fitting, calculating distances, associating a primary vertex).
These are implemented as *Tools*, which are shared between Algorithms.

**Options:**
To make all of this configurable, Gaudi allows you to set properties of *Algorithms* and *Tools* from a Python script, called an *option* file.
In an option file, you can specify which Algorithms are run in which order, and set their properties (strings, integers, doubles, and lists and dicts of these things can be set).
You can then start the Gaudi EventLoop using this option file, and it will set up and run the corresponding C++ objects with specified settings.

You can find comprehensive documentation in the [Gaudi Doxygen](https://gaudi.web.cern.ch/gaudi/doxygen/v30r3/index.html) or the [Gaudi Manual](https://gaudi.web.cern.ch/gaudi/resources/GUG.pdf).

Usually, you will work with one of the LHCb software projects that are based on Gaudi.
One of the most important ones is *DaVinci*, which provides lots of *Algorithms* and *Tools* for physics analysis.

You can run DaVinci using the following command [on lxplus](../first-analysis-steps/prerequisites.md#Pre-workshop checklist):
```bash
lb-run DaVinci/v45r8 gaudirun.py
```

This will run the `gaudirun.py` command using version v45r8 of DaVinci. (`lb-run` sets the specified environment for `gaudirun.py` to run in.)
`gaudirun.py` is a script that sets up the EventLoop.
You should get the following output:

```
====================================================================================================================================
                                                   Welcome to DaVinci version v45r8
                                          running on lxplus707.cern.ch on Thu Sep 19 14:17:08 2019
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
lb-run DaVinci/v45r8 gaudirun.py options.py
```

An `option.py` is just a regular Python script that specifies how to set things up in the software.
Many of the following lessons will teach you how to do something with DaVinci by showing you how to write or extend an `options.py`.
You can use the above command to test it.
You can also specify several option files like this:
```bash
lb-run DaVinci/v45r8 gaudirun.py options1.py options2.py
```
They will then both be used to set up DaVinci.

Do you want to get an overview of which versions of DaVinci exist? Use
```bash
lb-run --list DaVinci
```

{% callout "Which version of DaVinci should I use?" %}


All available versions of DaVinci are given on the [DaVinci releases][releases]
page. Which one should you use? There are a couple of guidelines to follow:

1. When starting a new analysis, use the latest version suitable for the data
   you would like to analyse.
2. When continuing an analysis, use the same version of DaVinci consistently
   throughout. This includes cases where you want to repeat an existing
   analysis but with modified settings, e.g. re-running a Stripping line.

There are a set of DaVinci versions for Upgrade studies (versions v50r1 and
above) and a set for everything else. Generally, you will want the latest
version in the latter set, such as when making ntuples from Run 1 or Run 2
data.

These lessons use [DaVinci v45r8][v45r8], which was the latest Run 1/2 version at the
time the text was last revised.

[releases]: http://lhcbdoc.web.cern.ch/lhcbdoc/davinci/releases/
[v45r8]: http://lhcbdoc.web.cern.ch/lhcbdoc/davinci/releases/v45r8/

**Note:** Older versions of DaVinci may not be available on the default platform ```x86_64-centos7-gcc8-opt```.
To get around this we can pick the best suitable platform by using ```lb-run -c best DaVinci/vXXrYpZ ...```.
More details about the platform string are available in [HSF-TN-2018-01](https://hepsoftwarefoundation.org/notes/HSF-TN-2018-01.pdf).


{% endcallout %}

Do you want to start a shell that already contains the LHCb environment, so you don't have to use `lb-run`?
Execute
```bash
lb-run DaVinci/v45r8 $SHELL
```
Note that sometimes this environment can result in failing scripts due to struggles with your shell's rc file (e.g., `~/.bashrc`). Using
```bash
lb-run DaVinci/v45r8 bash --norc
```
avoids this, but means you won't be able to use any aliases, etc, included in the ignored rc file.

A simple `gaudirun.py` should work as well now.
Typing `exit` or using `Ctrl-d` will close the shell and leave the LHCb environment behind.

{% callout "Using SetupProject instead of lb-run" %}

When reading through other tutorials, you will come across `SetupProject`.
This is an older way of setting up a shell that is configured to run LHCb software.
`lb-run` is the new way of doing things and has some nice benefits over `SetupProject`.
For most purposes, `SetupProject DaVinci v45r8` is equivalent to
```bash
lb-run DaVinci/v45r8 $SHELL
```
but you should really avoid doing things this way as this method is no longer supported for the latest project releases. (The environment for DaVinci v45r8, for example, cannot be started this way.)

{% endcallout %} 
