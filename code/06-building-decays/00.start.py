from Configurables import DaVinci
from GaudiConf import IOHelper

DaVinci().InputType = 'DST'
DaVinci().TupleFile = 'DVntuple.root'
DaVinci().PrintFreq = 1000
DaVinci().DataType = '2012'
DaVinci().Simulation = True
# Only ask for luminosity information when not using simulated data
DaVinci().Lumi = not DaVinci().Simulation
DaVinci().EvtMax = 1000

# Use the local input data
IOHelper().inputFiles([('root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/2012/ALLSTREAMS.DST/00035742/0000/00035742_00000001_1.allstreams.dst')],
                      clear=True)
