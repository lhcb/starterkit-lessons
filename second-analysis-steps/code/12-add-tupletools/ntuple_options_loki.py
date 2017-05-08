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

# Add LoKi HybridTools
dstar_hybrid = dtt.Dstar.addTupleTool('LoKi::Hybrid::TupleTool/LoKi_Dstar')
d0_hybrid = dtt.D0.addTupleTool('LoKi::Hybrid::TupleTool/LoKi_D0')
pisoft_hybrid = dtt.pisoft.addTupleTool('LoKi::Hybrid::TupleTool/LoKi_PiSoft')

preamble = ['DZ = VFASPF(VZ) - BPV(VZ)',
            'TRACK_MAX_PT = MAXTREE(ISBASIC & HASTRACK, PT, -1)']
dstar_hybrid.Preambulo = preamble
d0_hybrid.Preambulo = preamble

dstar_hybrid.Variables = {
    'mass': 'MM',
    'mass_D0': 'CHILD(MM, 1)',
    'pt': 'PT',
    'dz': 'DZ',
    'dira': 'BPVDIRA',
    'max_pt': 'MAXTREE(ISBASIC & HASTRACK, PT, -1)',
    'max_pt_preambulo': 'TRACK_MAX_PT',
    'n_highpt_tracks': 'NINTREE(ISBASIC & HASTRACK & (PT > 1500*MeV))'
}
d0_hybrid.Variables = {
    'mass': 'MM',
    'pt': 'PT',
    'dira': 'BPVDIRA',
    'vtx_chi2': 'VFASPF(VCHI2)',
    'dz': 'DZ'
}
pisoft_hybrid.Variables = {
    'p': 'P',
    'pt': 'PT'
}


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
    './00035742_00000002_1.allstreams.dst'
], clear=True)
