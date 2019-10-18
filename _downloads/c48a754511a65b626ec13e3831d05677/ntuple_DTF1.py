from GaudiConf import IOHelper
from Configurables import (
    DaVinci,
    DecayTreeTuple,
)
from DecayTreeTuple.Configuration import *

# Stream and stripping line we want to use
stream = 'AllStreams'
line = 'D2hhPromptDst2D2KKLine'

# Create an ntuple to capture D*+ decays from the StrippingLine line
dtt = DecayTreeTuple('TupleDstToD0pi_D0ToKK')
dtt.Inputs = ['/Event/{0}/Phys/{1}/Particles'.format(stream, line)]
dtt.Decay = '[D*(2010)+ -> ^(D0 -> ^K- ^K+) ^pi+]CC'

# add a kinematic fitter
dtt.addBranches({
    'Dstar': '[D*(2010)+ -> (D0 -> K- K+) pi+]CC',
})
dtt.Dstar.addTupleTool('TupleToolDecayTreeFitter/ConsD')
dtt.Dstar.ConsD.constrainToOriginVertex = True
dtt.Dstar.ConsD.Verbose = True
dtt.Dstar.ConsD.daughtersToConstrain = ['D0']

# add another fitter, this time we will change a mass hypothesis
dtt.Dstar.addTupleTool('TupleToolDecayTreeFitter/ConsDKpi')
dtt.Dstar.ConsDKpi.constrainToOriginVertex = True
dtt.Dstar.ConsDKpi.Verbose = True
dtt.Dstar.ConsDKpi.daughtersToConstrain = ['D0']

# make the hypothesis that actually we had the decay D0->pi+pi-
# note that you have to explicitely give both charges
# CC does not work here!
dtt.Dstar.ConsDKpi.Substitutions = {
    'Charm -> (D0 -> ^K- K+) Meson': 'pi-',
    'Charm -> (D~0 -> ^K+ K-) Meson': 'pi+',
    'Charm -> (D0 -> K- ^K+) Meson': 'pi+',
    'Charm -> (D~0 -> K+ ^K-) Meson': 'pi-'
}


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
