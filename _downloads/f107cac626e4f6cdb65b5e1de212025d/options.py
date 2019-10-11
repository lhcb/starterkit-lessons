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
strip = 'stripping28r1'
streams = buildStreams(stripping=strippingConfiguration(strip),
                       archive=strippingArchive(strip))

line = 'D2hhPromptDst2D2KKLine'

custom_stream = StrippingStream('CustomStream')
custom_line = 'Stripping'+line 

for stream in streams:
    for sline in stream.lines:
        if sline.name() == custom_line:
            custom_stream.appendLines([sline])

# Create the actual Stripping configurable
filterBadEvents = ProcStatusCheck()

sc = StrippingConf(Streams=[custom_stream],
                   MaxCandidates=2000,
                   AcceptBadEvents=False,
                   BadEventSelection=filterBadEvents)


# Create an ntuple to capture D*+ decays from the StrippingLine line
dtt = DecayTreeTuple('TupleDstToD0pi_D0ToKK')
# The output is placed directly into Phys, so we only need to
# define the stripping line here
dtt.Inputs = ['/Event/Phys/{0}/Particles'.format(line)]
dtt.Decay = '[D*(2010)+ -> (D0 -> K- K+) pi+]CC'

# Configure DaVinci

# Important: The selection sequence needs to be inserted into
# the Gaudi sequence for the stripping to run
DaVinci().appendToMainSequence([event_node_killer, sc.sequence()])
DaVinci().UserAlgorithms += [dtt]
DaVinci().InputType = 'DST'
DaVinci().TupleFile = 'DVntuple.root'
DaVinci().PrintFreq = 1000
DaVinci().DataType = '2016'
DaVinci().Simulation = True
# Only ask for luminosity information when not using simulated data
DaVinci().Lumi = not DaVinci().Simulation
DaVinci().EvtMax = 5000
DaVinci().CondDBtag = 'sim-20161124-2-vc-md100'
DaVinci().DDDBtag = 'dddb-20150724'

# Use the local input data
IOHelper().inputFiles([
    './00062514_00000001_7.AllStreams.dst'
], clear=True)
