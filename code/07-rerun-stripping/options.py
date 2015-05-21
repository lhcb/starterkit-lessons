
# This options file demonstrates how to run a stripping line
# from a specific stripping version on a local MC DST file
# It is based on the minimal DaVinci DecayTreeTuple example

from StrippingConf.Configuration import StrippingConf, StrippingStream
from StrippingSettings.Utils import strippingConfiguration
from StrippingArchive.Utils import buildStreams
from StrippingArchive import strippingArchive

# Build a new stream called 'CustomStream' that only
# contains the desired line
strip = 'stripping21'
streams = buildStreams(stripping=strippingConfiguration(strip),
                       archive=strippingArchive(strip))

custom_stream = StrippingStream("CustomStream")
custom_line = 'StrippingD2hhCompleteEventPromptDst2D2RSLine'

for stream in streams:
    for line in stream.lines:
        if line.name() == custom_line:
            custom_stream.appendLines([line])

line = 'D2hhCompleteEventPromptDst2D2RSLine'

# Create the actual Stripping configurable
from Configurables import ProcStatusCheck
filterBadEvents = ProcStatusCheck()

sc = StrippingConf(Streams=[custom_stream],
                   MaxCandidates=2000,
                   AcceptBadEvents=False,
                   HDRLocation = "DumpHDR",
                   BadEventSelection=filterBadEvents)

from GaudiConf import IOHelper
from Configurables import DaVinci, DecayTreeTuple
from DecayTreeTuple.Configuration import *

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
DaVinci().appendToMainSequence([sc.sequence()])
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
