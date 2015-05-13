
from GaudiConf import IOHelper
from Configurables import DaVinci, DecayTreeTuple
from DecayTreeTuple.Configuration import *
from StrippingConf.Configuration import StrippingConf, StrippingStream
from StrippingSettings.Utils import strippingConfiguration
from StrippingArchive.Utils import buildStreams
from StrippingArchive import strippingArchive

strip = 'stripping21'
streams = buildStreams(stripping=strippingConfiguration(strip), archive=strippingArchive(strip))

custom_stream = StrippingStream("CustomStream")
custom_line = 'StrippingD2hhCompleteEventPromptDst2D2RSLine'

for stream in streams:
    for line in stream.lines:
        if line.name() == custom_line:
            custom_stream.appendLines([line])

line = 'D2hhCompleteEventPromptDst2D2RSLine'

dtt = DecayTreeTuple('TupleDstToD0pi_D0ToKpi')
dtt.Inputs = [
    'Phys/{1}/Particles'.format(line)
]
dtt.Decay = '[D*(2010)+ -> ^(D0 -> ^K- ^pi+) ^pi+]CC'
dtt.addBranches({
    'Dst': '[D*(2010)+ -> (D0 -> K- pi+) pi+]CC',
    'Dst_pi': '[D*(2010)+ -> (D0 -> K- pi+) ^pi+]CC',
    'D0': '[D*(2010)+ -> ^(D0 -> K- pi+) pi+]CC',
    'D0_K': '[D*(2010)+ -> (D0 -> ^K- pi+) pi+]CC',
    'Dp_pi': '[D*(2010)+ -> (D0 -> K- ^pi+) pi+]CC'
})
dtt.ToolList = [
    'TupleToolGeometry',
    'TupleToolKinematic',
    'TupleToolPid',
    'TupleToolPrimaries',
    'TupleToolTrackInfo',
    'TupleToolEventInfo'
]
# Add the delta mass variable, dm = m(Kpipi) - m(Kpi)
dst_tt = dtt.Dst.addTupleTool('LoKi__Hybrid__TupleTool/DstTupleTool')
dst_tt.Variables = {
    'delta_M': 'M - M1'
}

DaVinci().UserAlgorithms += [dtt]
DaVinci().InputType = 'DST'
DaVinci().TupleFile = 'DVntuple.root'
DaVinci().PrintFreq = 1000
DaVinci().DataType = '2011'
DaVinci().Simulation = False
# Only ask for luminosity information when using collision data
DaVinci().Lumi = not DaVinci().Simulation
DaVinci().EvtMax = -1

IOHelper().inputFiles([
    './00041838_00047508_1.charmcompleteevent.dst'
], clear=True)

