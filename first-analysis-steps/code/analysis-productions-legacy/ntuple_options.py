from Configurables import DecayTreeTuple

## Specify the stream and stripping line
stream = "AllStreams"
line   = "D2hhPromptDst2D2KKLine"

## We create the DecayTreeTuple object, and indicate the Input 
## (i.e., the TES location where the desired candidates may be)
## as well as the decay descriptor
dtt = DecayTreeTuple("TupleDstToD0pi_D0ToKK")
dtt.Inputs = ["/Event/{0}/Phys/{1}/Particles".format(stream, line)]
dtt.Decay  = "[D*(2010)+ -> (D0 -> K- K+) pi+]CC"

from Configurables import DaVinci

DaVinci().UserAlgorithms += [dtt]

# In general, thanks to using the automatically_configure setting, you don't need what's below so it can be removed
"""
DaVinci().InputType       = "DST"
DaVinci().TupleFile       = "DVntuple.root"
DaVinci().PrintFreq       = 1000
DaVinci().DataType        = "2016"
DaVinci().Simulation      = True
DaVinci().Lumi            = not DaVinci().Simulation
DaVinci().EvtMax          = -1
DaVinci().CondDBtag       = "sim-20170721-2-vc-md100"
DaVinci().DDDBtag         = "dddb-20170721-3"
"""

# The Analysis Productions software automatically finds a valid remote file to test on so we can drop this bit
"""
from GaudiConf import IOHelper

IOHelper().inputFiles([
	"./00070793_00000001_7.AllStreams.dst"
], clear=True)
"""