from Configurables import DaVinci, IoHelper

DaVinci().InputType = 'DST'
DaVinci().TupleFile = 'DVntuple.root'
DaVinci().PrintFreq = 1000
DaVinci().DataType = '2012'
DaVinci().Simulation = True
# Only ask for luminosity information when not using simulated data
DaVinci().Lumi = not DaVinci().Simulation
DaVinci().EvtMax = 1000

# Use the local input data
IOHelper().inputFiles([('root://eoslhcb.cern.ch//eos/lhcb/user/a/apearce/'
                        'Starterkit/Nov2015/'
                        '00035742_00000001_1.allstreams.dst')],
                      clear=True)
