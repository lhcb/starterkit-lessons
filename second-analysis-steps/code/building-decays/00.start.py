from Configurables import DaVinci
from GaudiConf import IOHelper

DaVinci().InputType = 'DST'
DaVinci().TupleFile = 'DVntuple.root'
DaVinci().PrintFreq = 1000
DaVinci().DataType = '2016'
DaVinci().Simulation = True
# Only ask for luminosity information when not using simulated data
DaVinci().Lumi = not DaVinci().Simulation
DaVinci().EvtMax = 1000

# Use the local input data
IOHelper().inputFiles([('./00062514_00000001_7.AllStreams.dst')],
                      clear=True)
