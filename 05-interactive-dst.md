---
layout: page
title: First Steps in LHCb
subtitle: Interactively exploring a DST
minutes: 10
---
> ## Learning Objectives {.objectives}
>
> * Open a DST in an interactive python session
> * Print all nodes in a DST
> * Explore the contents of the TES
> * Inspect a track
> * Inspect a stripping location 

Data is stored in files called DSTs, which are processed
by DaVinci to make nTuples. However you can also explore
them interactively from a python session.

This is particularly useful if you want to quickly find
something out, or the more complex processing in DaVinci
is not working as expected.

The file we [downloaded from the grid](05-files-from-grid.html)
contains simulated data, with stripping and trigger decisions
and so on. Here we assumed the file you downloaded is called `00035742_00000002_1.allstreams.dst`.
To take a look at the contents of the TES we need a simple
options file:

~~~ {.python}
import sys

import GaudiPython as GP
from GaudiConf import IOHelper
from Configurables import LHCbApp, ApplicationMgr, DataOnDemandSvc
from Configurables import DecodeRawEvent
from Configurables import CondDB
from Configurables import DaVinci


appConf = ApplicationMgr()

dv = DaVinci()
dv.DataType = "2012"

dre = DecodeRawEvent()
dre.DataOnDemand = True

lhcbApp = LHCbApp()
lhcbApp.Simulation = True
CondDB().Upgrade = False

# Pass file to open as first command line argument
inputFiles = [sys.argv[-1]]
IOHelper('ROOT').inputFiles(inputFiles)

appMgr = GP.AppMgr()
evt = appMgr.evtsvc()

appMgr.run(1)

evt.dump()
~~~

Place this into a file called `first.py` and run the following
commands in a new terminal:

```bash
$ SetupProject DaVinci v36r6
$ ipython -i first.py 00035742_00000002_1.allstreams.dst
```

This will open the DST and print out some of the TES locations
which exist for this event. We are now ready to explore the TES,
which is accessible via the `evt` variable. For example you could
look at the properties of some tracks for the first event by typing
inside the python session:

```python
tracks = evt['/Event/Rec/Track/Best']
print tracks[0]
```

The next question is, how do you know what TES locations that could
exist? As we saw `evt.dump()` prints a few of them, but not all. In
addition there are some special ones that only exist if you try to
access them. The following snippet allows you to discover most TES
locations that are interesting:

```python
def nodes(evt, node=None):
    """List all nodes in `evt`"""
    nodenames = []

    if node is None:
        root = evt.retrieveObject('')
        node = root.registry()

    if node.object():
        nodenames.append(node.identifier())
        for l in evt.leaves(node):
            # skip a location that takes forever to load
            # XXX How to detect these automatically??
            if "Swum" in l.identifier():
                continue

            temp = evt[l.identifier()]
            nodenames += nodes(evt, l)

    else:
        nodenames.append(node.identifier())

    return nodenames
```

The easiest way to use it is to add it to your `first.py` script
and re-run it with `ipython -i first.py 00035742_00000002_1.allstreams.dst`.
This will list a large number of TES locations, but even so there
are some which you have to know about. Another oddity is that some
locations are "packed", for example: `/Event/AllStreams/pPhys/Particles`.
You can not access these directly at this location. Instead you
have to know what location the contents will get unpacked to when
you want to use it. Often you can just try removing the small `p`
from the location (`/Event/AllStreams/Phys/Particles`).

You can also inspect the particles and vertices built by your stripping
line. However not every event will contain a candidate for your line,
so the first tool we need is something that will advance us until
the stripping decision was positive:

```python
def advance(decision):
    """Advance until stripping decision is true, returns
    number of events by which we advanced"""
    n = 0
    while True:
        appMgr.run(1)

        if not evt["/Event/DAQ/RawEvent"]:
            print "Reached end of input files"
            break

        n += 1
        dec=evt['/Event/Strip/Phys/DecReports']
        if dec.hasDecisionName("Stripping%sDecision"%decision):
            break

    return n
```

Add this to your script and restart `ipython` as before. Using
the name of our stripping line we can now advance through the DST
until we reach an event which contains a candidate:

```python
line = 'D2hhCompleteEventPromptDst2D2RSLine'
advance(line)
```

The candidates built for you can now be found at `/Event/AllStreams/Phys/D2hhCompleteEventPromptDst2D2RSLine/Particles`:

```python
cands = evt['/Event/AllStreams/Phys/D2hhCompleteEventPromptDst2D2RSLine/Particles']
print cands.size()
```

This tells you how many candidates there are in this event and you can access the first
one with:

```python
print cands[0]
```

Which will print out some information about the [Particle](http://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/davinci/releases/v36r6/doxygen/d0/d13/class_l_h_cb_1_1_particle.html#details). In our case a D*. You can access its daughters with
`cands[0].daughtersVector()[0]` and `cands[0].daughtersVector()[1]`,
which will be a D0 and a pion.

There is a useful tool for printing out decay trees, which you can
pass the top level particle to and it will print out the daughters etc:

```
print_decay = appMgr.toolsvc().create('PrintDecayTreeTool', interface="IPrintDecayTreeTool")
print_decay.printTree(cands[0])
```
