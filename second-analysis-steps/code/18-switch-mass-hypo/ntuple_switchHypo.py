from GaudiConf import IOHelper
from Configurables import DaVinci, DecayTreeTuple, SubstitutePID
from PhysSelPython.Wrappers import Selection, SelectionSequence, DataOnDemand
from DecayTreeTuple.Configuration import *


# Stream and stripping line we want to use
stream = 'AllStreams'
line = 'D2hhPromptDst2D2KKLine'
tesLoc = '/Event/{0}/Phys/{1}/Particles'.format(stream, line)

# get the selection(s) created by the stripping
strippingSels = [DataOnDemand(Location=tesLoc)]

# configure an algorithm to substitute the Kaon in the D0-decay by a second
# pion
subs = SubstitutePID(
    'MakeD02Kpi',
    Code="DECTREE('[D*(2010)+ -> (D0 -> K- K+) pi+]CC')",
    # note that SubstitutePID can't handle automatic CC
    Substitutions={
        'Charm -> (D0 -> K- ^K+) Meson': 'pi+',
        'Charm -> (D~0 -> ^K- K+) Meson': 'pi-'}
)

# create a selection using the substitution algorithm
selSub = Selection(
    'Dst2D0pi_D02Kpi_Sel',
    Algorithm=subs,
    RequiredSelections=strippingSels
)
# in order to add the selection into the program make a sequence
selSeq = SelectionSequence('SelSeq', TopSelection=selSub)


# Create an ntuple to capture D*+ decays from the new selection
dtt = DecayTreeTuple('TupleDstToD0pi_D0ToKpi')
dtt.Inputs = [selSeq.outputLocation()]
dtt.Decay = '[D*(2010)+ -> ^(D0 -> ^K- ^pi+) ^pi+]CC'

# Configure DaVinci

# add our new selection and the tuple into the sequencer
seq = GaudiSequencer('MyTupleSeq')
seq.Members += [selSeq.sequence()]
seq.Members += [dtt]
DaVinci().appendToMainSequence([seq])

DaVinci().InputType = 'DST'
DaVinci().TupleFile = 'DVntuple.root'
DaVinci().PrintFreq = 1000
DaVinci().DataType = '2016'
DaVinci().Simulation = True
# Only ask for luminosity information when not using simulated data
DaVinci().Lumi = not DaVinci().Simulation
DaVinci().EvtMax = -1

# Use the local input data
IOHelper().inputFiles([
    'root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/2016/ALLSTREAMS.DST/00062514/0000/00062514\
_00000008_7.AllStreams.dst'], clear=True)
