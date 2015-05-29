#!/usr/bin/env python
# -*- coding: utf-8 -*-
# =============================================================================
# @file   build_decays.py
# @author Albert Puig (albert.puig@cern.ch)
# @date   29.05.2015
# =============================================================================
"""Build D* -> D0 (->K pi) pi decays from scratch."""

from CommonParticles.StdAllNoPIDsPions import StdAllNoPIDsPions as Pions
from CommonParticles.StdAllLooseKaons import StdAllLooseKaons as Kaons

d0_daughters = {'pi-': '(PT > 750*MeV) & (P > 4000*MeV) & (MIPCHI2DV(PRIMARY) > 4)',
                'K+': '(PT > 750*MeV) & (P > 4000*MeV) & (MIPCHI2DV(PRIMARY) > 4)'}
d0_comb = "(AMAXDOCA('') < 0.2*mm) & (ADAMASS('D0') < 100*MeV)"
d0_mother = "(VFASPF(VCHI2/VDOF)< 9) & (BPVDIRA > 0.9997) & (ADMASS('D0') < 70*MeV)"

from Configurables import CombineParticles
d0 = CombineParticles("Combine_D0",
                      Decay='([D0 -> pi- K+]CC)',
                      DaughtersCuts=d0_daughters,
                      CombinationCut=d0_comb,
                      MotherCut=d0_mother)

from PhysSelPython.Wrappers import Selection
d0_sel = Selection("Sel_D0",
                   Algorithm=d0,
                   RequiredSelections=[Pions, Kaons])

from Configurables import FilterDesktop
soft_pion = FilterDesktop("Filter_SoftPi",
                          Code='(TRCHI2DOF < 3) & (PT > 100*MeV)')
soft_pion_sel = Selection("Sel_SoftPi",
                          Algorithm=soft_pion,
                          RequiredSelections=[Pions])

dstar = CombineParticles("CombineDstar",
                         Decay='[D*(2010)+ -> D0 pi+]cc',
                         CombinationCut="(ADAMASS('D*(2010)+') < 400*MeV)",
                         MotherCut="(abs(M-MAXTREE('D0'==ABSID,M)-145.42) < 10*MeV) & (VFASPF(VCHI2/VDOF)< 9)")
dstar_sel = Selection("Sel_Dstar",
                      Algorithm=dstar,
                      RequiredSelections=[d0_sel, soft_pion_sel])

from PhysSelPython.Wrappers import SelectionSequence
dstar_seq = SelectionSequence('Dstar_Seq', TopSelection=dstar_sel)

from Configurables import DaVinci
DaVinci().UserAlgorithms += [dstar_seq.sequence()]


# EOF
