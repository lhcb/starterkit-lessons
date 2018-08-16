# Run a different stripping line on simulated data

{% objectives "Learning Objectives" %}
* Modify the minimal DecayTreeTuple example to apply a different stripping 
version to an MC sample
{% endobjectives %} 

Ideally, our simulated samples should feature the same stripping cuts as the real data we want to work with.
We can be sure of this if the same stripping version has been used when processing the simulated and real data.

But often, our simulated sample will have a different version of the stripping applied to it.
For example, what if our data sample uses Stripping 28r1, while our MC sample uses Stripping 28?

In this case, we simply need to rerun our stripping line of choice from the correct stripping version with one caveat:
the decisions of the stripping that ran during the central MC production are placed in a default location in the TES (`/Event/Strip/Phys/DecReports`), so if we try to run our custom stripping it will complain because the location it wants to write the new decisions to already exists.
To solve this issue, we need to run an instance of `EventNodeKiller` to remove the decisions from the MC production so that our custom stripping can write there instead.
This is nice, because most tools expect to read the stripping decisions from the default location, so we won't have to reconfigure anything.

[This example](code/14-rerun-stripping/options.py) is an extended version of the [minimal DaVinci DecayTreeTuple job](../first-analysis-steps/minimal-dv-job.md) that additionally runs the corresponding stripping line from Stripping 28r1.

Take a look at the file and try to find out what has changed compared to the [minimal DaVinci example](code/09-minimal-dv/ntuple_options.py).

The key changes are

 - Removing the old stripping reports with a node killer

```python
from Configurables import EventNodeKiller
event_node_killer = EventNodeKiller('StripKiller')
event_node_killer.Nodes = ['/Event/AllStreams', '/Event/Strip']
```

 - Picking the right stripping line from Stripping 28r1 (which we prepare with `buildStreams`):
 - Building a custom stream that only contains the desired stripping line

```python
strip = 'stripping28r1'
streams = buildStreams(stripping=strippingConfiguration(strip),
                       archive=strippingArchive(strip))

custom_stream = StrippingStream('CustomStream')
custom_line = 'Stripping'+line

for stream in streams:
    for sline in stream.lines:
        if sline.name() == custom_line:
            custom_stream.appendLines([sline])
```

 - Instantiating a `StrippingConf` for running the stripping

```python
sc = StrippingConf(Streams=[custom_stream],
                   MaxCandidates=2000,
                   AcceptBadEvents=False,
                   BadEventSelection=filterBadEvents)
```

 - Updating the path for `dtt.Inputs`: the output is now placed directly in `Phys`
 ```python
 dtt.Inputs = ['/Event/Phys/{0}/Particles'.format(line)]
 ```

 - Inserting the node killer and the stripping selection sequence into the Gaudi sequence

```python
DaVinci().appendToMainSequence([event_node_killer, sc.sequence()])
```

It is strongly recommended to run restripping with the same version of DaVinci as the one used for the corresponding stripping production: this ensures all the settings are configured the same exact way as for your data. For the Stripping 28r1, DaVinci v41r4p4 was used. Notice that this version is not available for the default platform `x86_64-slc6-gcc62-opt`.
To work around this you can pick up the best suitable platform using `lb-run -c best DaVinci/v41r4p4 ...` or by [setting the `application.platform` attribute in ganga](https://lhcb.github.io/starterkit-lessons/first-analysis-steps/davinci-grid.html#picking-up-a-right-platform).
See the [ProcessingPasses twiki page](https://twiki.cern.ch/twiki/bin/view/Main/ProcessingPasses) to find which version of DaVinci was used for each stripping campaign.
