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

This is how your option file could look like now:

~~~ {.python}
from GaudiConf import IOHelper
from Configurables import DaVinci, DecayTreeTuple
from DecayTreeTuple.Configuration import *
from StrippingConf.Configuration import StrippingConf, StrippingStream
from StrippingSettings.Utils import strippingConfiguration
from StrippingArchive.Utils import buildStreams
from StrippingArchive import strippingArchive

strip = 'stripping21'
streams = buildStreams(stripping=strippingConfiguration(strip), archive=strippingArchive(strip))

custom_stream = StrippingStream("CustomStream")
custom_line = 'StrippingD2hhCompleteEventPromptDst2D2RSLine'

for stream in streams:
    for line in stream.lines:
        if line.name() == custom_line:
            custom_stream.appendLines([line])

stream = 'CustomStream'
line = 'D2hhCompleteEventPromptDst2D2RSLine'

dtt = DecayTreeTuple('TupleDstToD0pi_D0ToKpi')
dtt.Inputs = [
    '/Event/{0}/Phys/{1}/Particles'.format(stream, line)
]
dtt.Decay = '[D*(2010)+ -> ^(D0 -> ^K- ^pi+) ^pi+]CC'
dtt.addBranches({
    'Dst': '[D*(2010)+ -> (D0 -> K- pi+) pi+]CC',
    'Dst_pi': '[D*(2010)+ -> (D0 -> K- pi+) ^pi+]CC',
    'D0': '[D*(2010)+ -> ^(D0 -> K- pi+) pi+]CC',
    'D0_K': '[D*(2010)+ -> (D0 -> ^K- pi+) pi+]CC',
    'Dp_pi': '[D*(2010)+ -> (D0 -> K- ^pi+) pi+]CC'
})
dtt.ToolList = [
    'TupleToolGeometry',
    'TupleToolKinematic',
    'TupleToolPid',
    'TupleToolPrimaries',
    'TupleToolTrackInfo',
    'TupleToolEventInfo'
]
# Add the delta mass variable, dm = m(Kpipi) - m(Kpi)
dst_tt = dtt.Dst.addTupleTool('LoKi__Hybrid__TupleTool/DstTupleTool')
dst_tt.Variables = {
    'delta_M': 'M - M1'
}

DaVinci().UserAlgorithms += [dtt]
DaVinci().InputType = 'DST'
DaVinci().TupleFile = 'DVntuple.root'
DaVinci().PrintFreq = 1000
DaVinci().DataType = '2011'
DaVinci().Simulation = False
# Only ask for luminosity information when using collision data
DaVinci().Lumi = not DaVinci().Simulation
DaVinci().EvtMax = -1

IOHelper().inputFiles([
    './00041838_00047508_1.charmcompleteevent.dst'
], clear=True)
~~~


