"""Build D* -> D0 (->K pi) pi decays from scratch."""

from PhysSelPython.Wrappers import (
    Selection,
    SelectionSequence,
    DataOnDemand,
    SimpleSelection
)
import GaudiConfUtils.ConfigurableGenerators as ConfigurableGenerators
from Configurables import CombineParticles, DaVinci

Pions = DataOnDemand('Phys/StdAllNoPIDsPions/Particles')
Kaons = DataOnDemand('Phys/StdAllLooseKaons/Particles')

d0_decay_products = {
    'pi-': '(PT > 750*MeV) & (P > 4000*MeV) & (MIPCHI2DV(PRIMARY) > 4)',
    'K+': '(PT > 750*MeV) & (P > 4000*MeV) & (MIPCHI2DV(PRIMARY) > 4)'
}
d0_comb = "(AMAXDOCA('') < 0.2*mm) & (ADAMASS('D0') < 100*MeV)"
# We can split long selections across multiple lines
d0_vertex = (
    '(VFASPF(VCHI2/VDOF)< 9)'
    '& (BPVDIRA > 0.9997)'
    "& (ADMASS('D0') < 70*MeV)"
)

d0 = CombineParticles('Combine_D0',
                      DecayDescriptor='([D0 -> pi- K+]CC)',
                      DaughtersCuts=d0_decay_products,
                      CombinationCut=d0_comb,
                      MotherCut=d0_vertex)

d0_sel = Selection('Sel_D0',
                   Algorithm=d0,
                   RequiredSelections=[Pions, Kaons])

dstar_decay_products = {'pi+': '(TRCHI2DOF < 3) & (PT > 100*MeV)'}
dstar_comb = "(ADAMASS('D*(2010)+') < 400*MeV)"
dstar_vertex = (
    "(abs(M-MAXTREE('D0'==ABSID,M)-145.42) < 10*MeV)"
    '& (VFASPF(VCHI2/VDOF)< 9)'
)
dstar_sel = SimpleSelection(
    'Sel_Dstar',
    ConfigurableGenerators.CombineParticles,
    [d0_sel, Pions],
    DecayDescriptor='[D*(2010)+ -> D0 pi+]cc',
    DaughtersCuts=dstar_decay_products,
    CombinationCut=dstar_comb,
    MotherCut=dstar_vertex
)

dstar_seq = SelectionSequence('Dstar_Seq', TopSelection=dstar_sel)

DaVinci().UserAlgorithms += [dstar_seq.sequence()]
