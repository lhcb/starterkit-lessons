# Interactively exploring a DST

{% objectives "Learning Objectives" %}

* Open a DST in an interactive python session
* Print all nodes in a DST
* Explore the contents of the TES
* Inspect a track
* Inspect a stripping location

{% endobjectives %} 

Data is stored in files called DSTs, which are processed
by DaVinci to make nTuples. However you can also explore
them interactively from a Python session.

This is particularly useful if you want to quickly find
something out, or the more complex processing in DaVinci
is not working as expected.

The file we [downloaded from the grid](files-from-grid)
contains simulated data, with stripping and trigger decisions
and so on. In this lesson we assume the file you downloaded is called `00070793_00000001_7.AllStreams.dst`.

To take a look at the contents of the TES, we need to write a small
Python file:

```python
import sys

import GaudiPython as GP
from GaudiConf import IOHelper
from Configurables import DaVinci

dv = DaVinci()
dv.DataType = '2016'
dv.Simulation = True

# Retrieve file path to open as the last command line argument
inputFiles = [sys.argv[-1]]
IOHelper('ROOT').inputFiles(inputFiles)

appMgr = GP.AppMgr()
evt = appMgr.evtsvc()

appMgr.run(1)
```

We first configure the application using the `DaVinci` configurable, set up
the input data, and then create the application manager and tell it to start
running.

Place this into a file called `explore.py` and run the following
command in a new terminal:

```bash
$ lb-run --ext=ipython DaVinci/v45r1 ipython -i explore.py 00070793_00000001_7.AllStreams.dst
```

This will configure the application, initialise it, and put you in a Python
prompt. We are now ready to explore the TES, which is accessible via the
`evt` variable. We can start by printing out some of the TES locations which
exist for this event.

```python
>>> evt.dump()
```

You could look at the properties of some tracks in the first event by typing
inside the Python session:

```python
>>> tracks = evt['/Event/Rec/Track/Best']
>>> print(tracks[0])
```

The next question is, how do you know what TES locations that could exist? As
we saw `evt.dump()` prints a few of them, but not all, including
`/Event/Rec/Track/Best`.

We can use the directory-like structure of the TES to walk through the
hierachy from the root node.

```python
>>> root = TES['/Event']
>>> evt.leaves(root)
<ROOT.vector<IRegistry*> object at 0x11d1deb0
>>> root_children = evt.leaves()
>>> root_children.size()
23L
>>> root_children[0]
<ROOT.DataSvcHelpers::RegistryEntry object at 0x126576b0>
>>> root_children[0].identifier()
'/Event/Gen'
```

So from the `root` node we can find the locations of the children! We can put
this into a little recursive function that walks through all children of the
root, all the children of each child, and so on.

```python
def nodes(evt, root='/Event'):
    """List all nodes in `evt` starting from `node`."""
    node_list = [root]
    for leaf in evt.leaves(evt[root]):
        node_list += nodes(evt, leaf.identifier())
    return node_list
```

The easiest way to use this is to add it to your `explore.py` script
and re-run as before. Then, in your IPython session, enter `nodes(evt)`.

This will list a large number of TES locations which are present in the input
file. But it still doesn't show `/Event/Rec/Track/Best`! Why not?

This is because locations can be created _on demand_, only created when we
first try to access them. Such locations are typically 'packed' in the input
file, for example: `/Event/pRec/Track/Best`. The objects inside these packed
locations get 'unpacked' to some other location, which happens auto-magically
when you try to access the _unpacked_ location. Often you can just try
removing the small `p` from the location (`/Event/Rec/Track/Best`) to trigger
the unpacking. Other times you have to make sure the application is correctly
configured to enable unpacking.


You can also inspect the particles and vertices built by your stripping line.
However not every event will contain a candidate for your line, so the second
tool we need is something that will advance us through events until the
stripping decision was positive:

```python
def advance(decision):
    """Advance until stripping decision is true, returns
    number of events by which we advanced"""
    n = 0
    while evt['/Event']:
        reports = evt['/Event/Strip/Phys/DecReports']
        if reports.hasDecisionName('Stripping{}Decision'.format(decision)):
            break

        appMgr.run(1)
        n += 1
    return n
```

Add this to your script and restart `ipython` as before.

{% callout "Detecting file ends" %}

To avoid an infinite loop, we check that we still have events to process by
seeing if `evt['/Event']` exists. This root location is present in every
event in the input file; if it doesn't exist, we've run out of events to
process.

{% endcallout %} 

Using the name of our stripping line we can now advance through the
DST until we reach an event which contains a candidate:

```python
>>> line = 'D2hhPromptDst2D2KKLine'
>>> advance(line)
```

The candidates built for you can now be found at `/Event/AllStreams/Phys/D2hhPromptDst2D2KKLine/Particles`:

```python
>>> cands = evt['/Event/AllStreams/Phys/{}/Particles'.format(line)]
>>> print(cands.size())
```

This tells you how many candidates there are in this event and you can access the first
one with:

```python
>>> print(cands[0])
```

Which will print out some information about the [Particle](https://lhcb-doxygen.web.cern.ch/lhcb-doxygen/davinci/latest/d0/d13/class_l_h_cb_1_1_particle.html). In our case a `$ D^{* +} $` ([particle ID number](http://pdg.lbl.gov/2019/reviews/rpp2018-rev-monte-carlo-numbering.pdf) 413). You can access its daughters with
`cands[0].daughtersVector()[0]` and `cands[0].daughtersVector()[1]`,
which will be a `$ D^{0} $` and a `$ \pi^{+} $`.

There is a useful tool for printing out decay trees, which you can
pass the top level particle to and it will print out the daughters etc:

```python
>>> print_decay = appMgr.toolsvc().create(
...   'PrintDecayTreeTool', interface='IPrintDecayTreeTool'
... )
...
>>> print_decay.printTree(cands[0])
```

With our candidates in hand, it would be nice to be able to retrieve and
compute the variables we need for an analysis. On to [LoKi
functors](loki-functors)!

{% callout "Fast DST browsing" %}

While here we have discussed for pedagogical reasons all the configuration
options needed in order to browse a `DST` file, in your daily life as a
physicist it is often useful to use the `bender` application that belongs to
the `Bender` project.

For example, to explore the `DST` we could have simply done:

```
lb-run Bender/latest bender -d 2016 00070793_00000001_7.AllStreams.dst
```

where `-d 2016` is the `DataType` flag.
This leaves us in a prompt in which we can proceed as discussed in this
lesson, with the advantage that some functions are already provided
for us, such as `seekStripDecision` (which replaces our `advance`) or
`ls` and `get`, which allow to list and get TES locations.
Other examples of useful functions are listed in the `bender` starting
banner.

`Bender` also provides a useful command `dst-dump`, which is a quick way of
figuring out what objects are present on a `DST` and where. Try out:
```
lb-run Bender/latest dst-dump -d 2016 -f -n 100 00070793_00000001_7.AllStreams.dst
```
The `-f` option tells `Bender` to try and "unpack" the locations such as
`/Event/AllStreams/pPhys/Particles` that we mentioned above, while `-n 100`
tells it to only process the first 100 events on the `DST`.
Give this a try if you're ever stuck figuring out where your candidates are hiding!

{% endcallout %} 


{% callout "Configuration debugging" %}

You can run an interactive exploration session using whatever application
configuration you like, which can come in handy if your `DecayTreeTuple`
isn't behaving how you expect.

As long as all your configuration comes before the `GP.AppMgr()` line, that
configuration will be included in the exploration session.

{% endcallout %} 
