---
layout: page
title: First Steps in LHCb
subtitle: Running the stripping on simulated samples
minutes: 15
---
> ## Learning Objectives {.objectives}
>
> * Modify the minimal DecayTreeTuple example to apply a different stripping version

We want to use Stripping21 with our simulation sample, but only Stripping20 has been applied to it.
To get the correct stripping, we will need to run it as part of our DaVinci job.
This is easier than it might seem at first, because all necessary parts are already there and
just need to be put together.

Let's start by importing a few stripping-related tools at the top of our options file:

~~~ {.python}
from StrippingConf.Configuration import StrippingConf, StrippingStream
from StrippingSettings.Utils import strippingConfiguration
from StrippingArchive.Utils import buildStreams
from StrippingArchive import strippingArchive
~~~

This is how you get all the stripping streams for Stripping21:

~~~ {.python}
strip = 'stripping21'
streams = buildStreams(stripping=strippingConfiguration(strip), archive=strippingArchive(strip))
~~~

We will build our own custom stream with the desired stripping line.
By using our custom stream, we will only run a single stripping line and nothing else.

~~~ {.python}
custom_stream = StrippingStream("CustomStream")
custom_line = 'StrippingFullDSTDiMuonJpsi2MuMuDetachedLine'
~~~

Get the stripping line with the right name:

~~~ {.python
for stream in streams: 
    for line in stream.lines:
        if line.name() == custom_line:
            custom_stream.appendLines([line]) 
~~~

This is how the stripping is initialized

~~~ {.python}
from Configurables import ProcStatusCheck

sc = StrippingConf(Streams=[custom_stream],
                   MaxCandidates=2000,
                   AcceptBadEvents=False,
                   BadEventSelection=ProcStatusCheck())
~~~

Run the stripping as part of our DaVinci job.
Make sure to access `.sequence()` here.

~~~ {.python}
DaVinci().appendToMainSequence([sc.sequence()])
~~~

Make sure that your options file looks like [this example](examples/07-rerun-stripping/options.py).


