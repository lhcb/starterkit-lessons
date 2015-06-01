"""
This options file demonstrates how to run a stripping line
from a specific stripping version on a local MC DST file
It is based on the minimal DaVinci DecayTreeTuple example
"""

from StrippingConf.Configuration import StrippingConf, StrippingStream
from StrippingSettings.Utils import strippingConfiguration
from StrippingArchive.Utils import buildStreams
from StrippingArchive import strippingArchive
from Configurables import (
    EventNodeKiller,
    ProcStatusCheck,
    DaVinci,
    DecayTreeTuple
)
from GaudiConf import IOHelper
from DecayTreeTuple.Configuration import *

# Node killer: remove the previous Stripping
event_node_killer = EventNodeKiller('StripKiller')
event_node_killer.Nodes = ['/Event/AllStreams', '/Event/Strip']

# Build a new stream called 'CustomStream' that only
# contains the desired line
strip = 'stripping21'
streams = buildStreams(stripping=strippingConfiguration(strip),
                       archive=strippingArchive(strip))

custom_stream = StrippingStream('CustomStream')
custom_line = 'StrippingD2hhCompleteEventPromptDst2D2RSLine'

for stream in streams:
    for line in stream.lines:
        if line.name() == custom_line:
            custom_stream.appendLines([line])

line = 'D2hhCompleteEventPromptDst2D2RSLine'

# Create the actual Stripping configurable
filterBadEvents = ProcStatusCheck()

sc = StrippingConf(Streams=[custom_stream],
                   MaxCandidates=2000,
                   AcceptBadEvents=False,
                   BadEventSelection=filterBadEvents)

# The output is placed directly into Phys, so we only need to
# define the stripping line here
line = 'D2hhCompleteEventPromptDst2D2RSLine'

# Create an ntuple to capture D*+ decays from the StrippingLine line
dtt = DecayTreeTuple('TupleDstToD0pi_D0ToKpi')
dtt.Inputs = ['/Event/Phys/{0}/Particles'.format(line)]
dtt.Decay = '[D*(2010)+ -> (D0 -> K- pi+) pi+]CC'

# Configure DaVinci

# Important: The selection sequence needs to be inserted into
# the Gaudi sequence for the stripping to run
DaVinci().appendToMainSequence([event_node_killer, sc.sequence()])
DaVinci().UserAlgorithms += [dtt]
DaVinci().InputType = 'DST'
DaVinci().TupleFile = 'DVntuple.root'
DaVinci().PrintFreq = 1000
DaVinci().DataType = '2012'
DaVinci().Simulation = True
# Only ask for luminosity information when not using simulated data
DaVinci().Lumi = not DaVinci().Simulation
DaVinci().EvtMax = 5000

# Use the local input data
IOHelper().inputFiles([
    './00035742_00000002_1.allstreams.dst'
], clear=True)
