from GaudiConf import IOHelper
from Configurables import DaVinci, DecayTreeTuple
from DecayTreeTuple.Configuration import *

# Stream and stripping line we want to use
stream = 'AllStreams'
line = 'D2hhPromptDst2D2KKLine'

# Create an ntuple to capture D*+ decays from the StrippingLine line
dtt = DecayTreeTuple('TupleDstToD0pi_D0ToKK')
dtt.Inputs = ['/Event/{0}/Phys/{1}/Particles'.format(stream, line)]
# Note that we mark all particles, otherwise the branches won't work
dtt.Decay = '[D*(2010)+ -> ^(D0 -> ^K- ^K+) ^pi+]CC'
track_tool = dtt.addTupleTool('TupleToolTrackInfo')
track_tool.Verbose = True
dtt.addTupleTool('TupleToolPrimaries')

dtt.addBranches({
    'Dstar': '[D*(2010)+ -> (D0 -> K- K+) pi+]CC',
    'D0': '[D*(2010)+ -> ^(D0 -> K- K+) pi+]CC',
    'Kminus': '[D*(2010)+ -> (D0 -> ^K- K+) pi+]CC',
    'Kplus': '[D*(2010)+ -> (D0 -> K- ^K+) pi+]CC',
    'pisoft': '[D*(2010)+ -> (D0 -> K- K+) ^pi+]CC'
})

dtt.D0.addTupleTool('TupleToolPropertime')

# Configure DaVinci
DaVinci().UserAlgorithms += [dtt]
DaVinci().InputType = 'DST'
DaVinci().TupleFile = 'DVntuple.root'
DaVinci().PrintFreq = 1000
DaVinci().DataType = '2016'
DaVinci().Simulation = True
# Only ask for luminosity information when not using simulated data
DaVinci().Lumi = not DaVinci().Simulation
DaVinci().EvtMax = -1
DaVinci().CondDBtag = 'sim-20161124-2-vc-md100'
DaVinci().DDDBtag = 'dddb-20150724'


# Use the local input data
IOHelper().inputFiles([
    './00062514_00000001_7.AllStreams.dst'
], clear=True)
