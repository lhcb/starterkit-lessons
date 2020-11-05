from Configurables import DaVinci
from GaudiConf import IOHelper

# Load algorithms
from Configurables import CombineParticles
from Configurables import DaVinci
from Configurables import DecayTreeTuple
from DecayTreeTuple.Configuration import *

# Load input particles
from StandardParticles import StdAllNoPIDsPions as Pions
from StandardParticles import StdAllLooseKaons as Kaons
# Load Selection objects
from PhysConf.Selections import Selection
from PhysConf.Selections import SelectionSequence

# Build the D0 from the pions and kaons
d0_decay_products = {
    'K-': '(PT > 750*MeV) & (P > 4000*MeV) & (MIPCHI2DV(PRIMARY) > 4)',
    'K+': '(PT > 750*MeV) & (P > 4000*MeV) & (MIPCHI2DV(PRIMARY) > 4)'
}
d0_comb = "(AMAXDOCA('') < 0.2*mm) & (ADAMASS('D0') < 100*MeV)"
# We can split long selections across multiple lines
d0_mother = (
    '(VFASPF(VCHI2/VDOF)< 9)'
    '& (BPVDIRA > 0.9997)'
    "& (ADMASS('D0') < 70*MeV)"
)

d0 = CombineParticles('Combine_D0',
                      DecayDescriptor='[D0 -> K- K+]cc',
                      DaughtersCuts=d0_decay_products,
                      CombinationCut=d0_comb,
                      MotherCut=d0_mother)

d0_sel = Selection(
    'Sel_D0',
    Algorithm=d0,
    RequiredSelections=[Kaons]
)

dstar_decay_products = {'pi+': '(TRCHI2DOF < 3) & (PT > 100*MeV)'}
dstar_comb = "(ADAMASS('D*(2010)+') < 400*MeV)"
dstar_mother = (
    "(abs(M-MAXTREE('D0'==ABSID,M)-145.42) < 10*MeV)"
    '& (VFASPF(VCHI2/VDOF)< 9)'
)

dstar = CombineParticles(
    'Combine_Dstar',
    DecayDescriptor='[D*(2010)+ -> D0 pi+]cc',
    DaughtersCuts=dstar_decay_products,
    CombinationCut=dstar_comb,
    MotherCut=dstar_mother
)

dstar_sel = Selection(
    'Sel_Dstar',
    Algorithm=dstar,
    RequiredSelections=[d0_sel, Pions]
)

dstar_seq = SelectionSequence('Dstar_Seq', TopSelection=dstar_sel)

# Create an ntuple
dtt = DecayTreeTuple('TupleDstToD0pi_D0ToKK')
dtt.Inputs = dstar_seq.outputLocations()
dtt.Decay = '[D*(2010)+ -> ^D0 ^pi+]CC'

DaVinci().UserAlgorithms += [dstar_seq.sequence(), dtt]

# DaVinci configuration
DaVinci().InputType = 'DST'
DaVinci().TupleFile = 'DVntuple.root'
DaVinci().PrintFreq = 1000
DaVinci().DataType = '2016'
DaVinci().Simulation = True
# Only ask for luminosity information when not using simulated data
DaVinci().Lumi = not DaVinci().Simulation
DaVinci().EvtMax = 1000

IOHelper().inputFiles([('root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/2016/ALLSTREAMS.DST/00062514/0000/00062514_00000008_7.AllStreams.dst')],
                      clear=True)
