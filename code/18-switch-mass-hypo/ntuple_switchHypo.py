from GaudiConf import IOHelper
from Configurables import DaVinci, DecayTreeTuple, SubstitutePID
from PhysSelPython.Wrappers import Selection, SelectionSequence, DataOnDemand
from DecayTreeTuple.Configuration import *


# Stream and stripping line we want to use
stream = 'AllStreams'
line = 'D2hhCompleteEventPromptDst2D2RSLine'
tesLoc = '/Event/{0}/Phys/{1}/Particles'.format(stream, line)

# get the selection(s) created by the stripping
strippingSels = [DataOnDemand(Location=tesLoc)]

# configure an algorithm to substitute the Kaon in the D0-decay by a second
# pion
subs = SubstitutePID(
    'MakeD02pipi',
    Code="DECTREE('[D*(2010)+ -> (D0 -> K- pi+) pi+]CC')",
    # note that SubstitutePID can't handle automatic CC
    Substitutions={
        'Charm -> (D0 -> ^K- pi+) Meson': 'pi-',
        'Charm -> (D~0 -> ^K+ pi-) Meson': 'pi+'
    }
)

# create a selection using the substitution algorithm
selSub = Selection(
    'Dst2D0pi_D02pipi_Sel',
    Algorithm=subs,
    RequiredSelections=strippingSels
)
# in order to add the selection into the program make a sequence
selSeq = SelectionSequence('SelSeq', TopSelection=selSub)


# Create an ntuple to capture D*+ decays from the new selection
dtt = DecayTreeTuple('TupleDstToD0pi_D0Topipi')
dtt.Inputs = [selSeq.outputLocation()]
dtt.Decay = '[D*(2010)+ -> ^(D0 -> ^pi- ^pi+) ^pi+]CC'

# Configure DaVinci

# add our new selection and the tuple into the sequencer
seq = GaudiSequencer('MyTupleSeq')
seq.Members += [selSeq.sequence()]
seq.Members += [dtt]
DaVinci().appendToMainSequence([seq])

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
