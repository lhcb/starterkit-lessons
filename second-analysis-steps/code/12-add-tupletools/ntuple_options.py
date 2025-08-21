from GaudiConf import IOHelper
from Configurables import DaVinci, DecayTreeTuple
from DecayTreeTuple.Configuration import *

# Stream and stripping line we want to use
stream = 'AllStreams'
line = 'D2hhCompleteEventPromptDst2D2RSLine'

# Create an ntuple to capture D*+ decays from the StrippingLine line
dtt = DecayTreeTuple('TupleDstToD0pi_D0ToKpi')
dtt.Inputs = ['/Event/{0}/Phys/{1}/Particles'.format(stream, line)]
dtt.Decay = '[D*(2010)+ -> (D0 -> K- pi+) pi+]CC'
track_tool = dtt.addTupleTool('TupleToolTrackInfo')
track_tool.Verbose = True
dtt.addTupleTool('TupleToolPrimaries')

dtt.addBranches({
    'Dstar': '[D*(2010)+ -> (D0 -> K- pi+) pi+]CC',
    'D0': '[D*(2010)+ -> ^(D0 -> K- pi+) pi+]CC',
    'Kminus': '[D*(2010)+ -> (D0 -> ^K- pi+) pi+]CC',
    'piplus': '[D*(2010)+ -> (D0 -> K- ^pi+) pi+]CC',
    'pisoft': '[D*(2010)+ -> (D0 -> K- pi+) ^pi+]CC'
})

dtt.D0.addTupleTool('TupleToolPropertime')

# Configure DaVinci
DaVinci().UserAlgorithms += [dtt]
DaVinci().InputType = 'DST'
DaVinci().TupleFile = 'DVntuple.root'
DaVinci().PrintFreq = 1000
DaVinci().DataType = '2012'
DaVinci().Simulation = True
# Only ask for luminosity information when not using simulated data
DaVinci().Lumi = not DaVinci().Simulation
DaVinci().EvtMax = -1

# Use the local input data
IOHelper().inputFiles([
    './00070793_00000001_7.AllStreams.dst'
], clear=True)
