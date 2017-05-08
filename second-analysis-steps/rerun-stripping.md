# Run a different stripping line on simulated data

{% objectives "Learning Objectives" %}
* Modify the minimal DecayTreeTuple example to apply a different stripping 
version to an MC sample
{% endobjectives %} 

Ideally, our simulated samples should feature the same stripping cuts as the real data we want to work with.
We can be sure of this if the same stripping version has been used when processing the simulated and real data.

But often, our simulated sample will have a different version of the stripping applied to it.
For example, what if our data sample uses Stripping 21, while our MC sample uses Stripping 20?

In this case, we simply need to rerun our stripping line of choice from the correct stripping version with one caveat:
the decisions of the stripping that ran during the central MC production are placed in a default location in the TES (`/Event/Strip/Phys/DecReports`), so if we try to run our custom stripping it will complain because the location it wants to write the new decisions to already exists.
To solve this issue, we need to run an instance of `EventNodeKiller` to remove the decisions from the MC production so that our custom stripping can write there instead.
This is nice, because most tools expect to read the stripping decisions from the default location, so we won't have to reconfigure anything.

[This example](code/14-rerun-stripping/options.py) is an extended version of the [minimal DaVinci DecayTreeTuple job](../first-analysis-steps/minimal-dv-job.md) that additionally runs the corresponding stripping line from Stripping 21.

Take a look at the file and try to find out what has changed compared to the [minimal DaVinci example](code/09-minimal-dv/ntuple_options.py).

The key changes are

 - Removing the old stripping reports with a node killer

``` {.python}
from Configurables import EventNodeKiller
event_node_killer = EventNodeKiller('StripKiller')
event_node_killer.Nodes = ['/Event/AllStreams', '/Event/Strip']
```

 - Picking the right stripping line from Stripping 21 (which we prepare with `buildStreams`):
 - Building a custom stream that only contains the desired stripping line

``` {.python}
strip = 'stripping21'
streams = buildStreams(stripping=strippingConfiguration(strip),
                       archive=strippingArchive(strip))

custom_stream = StrippingStream('CustomStream')
custom_line = 'StrippingD2hhCompleteEventPromptDst2D2RSLine'

for stream in streams:
    for line in stream.lines:
        if line.name() == custom_line:
            custom_stream.appendLines([line])
```

 - Instantiating a `StrippingConf` for running the stripping

``` {.python}
sc = StrippingConf(Streams=[custom_stream],
                   MaxCandidates=2000,
                   AcceptBadEvents=False,
                   BadEventSelection=filterBadEvents)
```

 - Inserting the node killer and the stripping selection sequence into the Gaudi sequence

``` {.python}
DaVinci().appendToMainSequence([event_node_killer, sc.sequence()])
```

