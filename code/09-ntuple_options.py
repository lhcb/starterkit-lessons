from GaudiConf import IOHelper
from Configurables import DaVinci, DecayTreeTuple
from DecayTreeTuple.Configuration import *

# Stream and stripping line we want to use
stream = 'CharmCompleteEvent'
line = 'D2hhCompleteEventPromptDst2D2RSLine'

# Create an ntuple to capture D*+ decays from the StrippingLine line
dtt = DecayTreeTuple('TupleDstToD0pi_D0ToKpi')
dtt.Inputs = ['/Event/{0}/Phys/{1}/Particles'.format(stream, line)]
dtt.Decay = '[D*(2010)+ -> (D0 -> K- pi+) pi+]CC'

# Configure DaVinci
DaVinci().UserAlgorithms += [dtt]
DaVinci().InputType = 'DST'
DaVinci().TupleFile = 'DVntuple.root'
DaVinci().PrintFreq = 1000
DaVinci().DataType = '2011'
DaVinci().Simulation = False
# Only ask for luminosity information when not using simulated data
DaVinci().Lumi = not DaVinci().Simulation
DaVinci().EvtMax = -1

# Use the local input data
IOHelper().inputFiles([
  './00041838_00047508_1.charmcompleteevent.dst'
], clear=True)
