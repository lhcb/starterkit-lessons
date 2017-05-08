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

The file we [downloaded from the grid](files-from-grid.html)
contains simulated data, with stripping and trigger decisions
and so on. Here we assumed the file you downloaded is called `00035742_00000002_1.allstreams.dst`.
To take a look at the contents of the TES, we need to write a small
Python file:

```python
import sys

import GaudiPython as GP
from GaudiConf import IOHelper
from Configurables import DaVinci

dv = DaVinci()
dv.DataType = '2012'

# Pass file to open as first command line argument
inputFiles = [sys.argv[-1]]
IOHelper('ROOT').inputFiles(inputFiles)

appMgr = GP.AppMgr()
evt = appMgr.evtsvc()

appMgr.run(1)
evt.dump()
```

Place this into a file called `first.py` and run the following
command in a new terminal:

```bash
$ lb-run DaVinci/v41r2 ipython -i first.py 00035742_00000002_1.allstreams.dst
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
            if 'Swum' in l.identifier():
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

        if not evt['/Event/Rec/Header']:
            print 'Reached end of input files'
            break

        n += 1
        dec=evt['/Event/Strip/Phys/DecReports']
        if dec.hasDecisionName('Stripping{0}Decision'.format(decision)):
            break

    return n
```

Add this to your script and restart `ipython` as before.

> ## Detecting file ends {.callout}
>
> It is not easy to detect that the input file has ended. Especially
> if you want to get it right for data and simulation. Checking that
> `/Event/Rec/Header` exists is a safe bet in simulation and data if
> your file has been processed by `Brunel` (the event reconstruction
> software). It might not work in other cases.

Using the name of our stripping line we can now advance through the
DST until we reach an event which contains a candidate:

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

Which will print out some information about the [Particle](http://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/davinci/releases/v41r2/doxygen/d0/d13/class_l_h_cb_1_1_particle.html#details). In our case a D*. You can access its daughters with
`cands[0].daughtersVector()[0]` and `cands[0].daughtersVector()[1]`,
which will be a D0 and a pion.

There is a useful tool for printing out decay trees, which you can
pass the top level particle to and it will print out the daughters etc:

```python
print_decay = appMgr.toolsvc().create(
  'PrintDecayTreeTool', interface='IPrintDecayTreeTool'
)
print_decay.printTree(cands[0])
```

With our candidates in hand, it would be nice to be able to retrieve and
compute the variables we need for an analysis. On to [LoKi
functors](loki-functors.html)!

> ## Fast DST browsing {.callout}
>
> While here we have discussed for pedagogical reasons all the configuration
> options needed in order to browse a `DST` file, in your daily life as a
> physicist it is often useful to use the `bender` application that belongs to
> the `Bender` project.
>
> For example, to explore the `DST` we could have simply done:
>
> ```
> lb-run Bender/latest bender 00035742_00000002_1.allstreams.dst
> ```
>
> This leaves us in a prompt in which we can proceed as discussed in this
> lesson, with the advantage that some functions are already provided
> for us, such as `seekStripDecision` (which replaces our `advance`) or
> `ls` and `get`, which allow to list and get TES locations.
> Other examples of useful functions are listed in the `bender` starting
> banner.
>
> `Bender` also provides a useful command `dst-dump`, which is a quick way of
> figuring out what objects are present on a `DST` and where. Try out:
> ```
> lb-run Bender/latest dst-dump -f -n 100 00035742_00000002_1.allstreams.dst
> ```
> The `-f` option tells `Bender` to try and "unpack" the locations such as
> `/Event/AllStreams/pPhys/Particles` that we mentioned above, while `-n 100`
> tells it to only process the first 100 events on the `DST`.
> Give this a try if you're ever stuck figuring out where your candidates are hiding!
