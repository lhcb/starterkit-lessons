from Gaudi.Configuration import importOptions, FileCatalog
from Configurables import DaVinci, DecayTreeTuple
from DecayTreeTuple.Configuration import *

# Stream and stripping line we want to use
stream = 'AllStreams'
line = 'D2hhPromptDst2D2KKLine'

# Create an ntuple to capture D*+ decays from the StrippingLine line
dtt = DecayTreeTuple('TupleDstToD0pi_D0ToKK')
dtt.Inputs = ['/Event/{0}/Phys/{1}/Particles'.format(stream, line)]
dtt.Decay = '[D*(2010)+ -> (D0 -> K- K+) pi+]CC'

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

# Use input data from the bookkeeping query with XML catalog
importOptions(
    "MC_2016_27163002_"
    "Beam6500GeV2016MagDownNu1.625nsPythia8_Sim09b_"
    "Trig0x6138160F_Reco16_Turbo03_"
    "Stripping28NoPrescalingFlagged_ALLSTREAMS.DST.py"
    )
FileCatalog().Catalogs = [
    "xmlcatalog_file:MC_2016_27163002_"
    "Beam6500GeV2016MagDownNu1.625nsPythia8_Sim09b_"
    "Trig0x6138160F_Reco16_Turbo03_"
    "Stripping28NoPrescalingFlagged_ALLSTREAMS.DST.xml"
    ]
