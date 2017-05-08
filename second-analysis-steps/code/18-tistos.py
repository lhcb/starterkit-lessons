import sys

import GaudiPython as GP
from GaudiConf import IOHelper
from Configurables import DaVinci
from Configurables import ToolSvc, TriggerTisTos
from Configurables import LHCbApp, ApplicationMgr, DataOnDemandSvc
from Configurables import SimConf, DigiConf, DecodeRawEvent
from Configurables import CondDB, DstConf, PhysConf
from Configurables import LoKiSvc

# Some name shortcuts
MCParticle = GP.gbl.LHCb.MCParticle
Particle = GP.gbl.LHCb.Particle
Track = GP.gbl.LHCb.Track
MCHit = GP.gbl.LHCb.MCHit


def nodes(evt, node=None):
    nodenames = []

    if node is None:
        root = evt.retrieveObject('')
        node = root.registry()

    if node.object():
        nodenames.append(node.identifier())
        for l in evt.leaves(node):
            # skip a location that takes forever to load
            # XXX How to detect these automatically??
            if "Swum" in l.identifier():
                continue

            temp = evt[l.identifier()]
            nodenames += nodes(evt, l)

    else:
        nodenames.append(node.identifier())

    return nodenames


def advance(decision='D2hhPromptDst2D2RSLine'):
    """Advance until stripping decision is true, returns
    number of events by which we advanced"""
    n = 0
    while True:
        appMgr.run(1)
        if not evt['/Event']:
            n = 0
            break
        n += 1
        dec = evt['/Event/Strip/Phys/DecReports']
        if dec.hasDecisionName("Stripping%sDecision" % decision):
            break

    return n


def advance_hlt(decision):
    """Advance until HLT decision is true, returns
    number of events by which we advanced"""
    n = 0
    while True:
        appMgr.run(1)
        if not evt['/Event']:
            n = 0
            break
        n += 1
        found = False
        for loc in ['Hlt1', 'Hlt2']:
            reports = evt[loc + "/DecReports"]
            dec = "%sDecision" % decision
            rep = reports.decReport(dec)
            if rep and rep.decision():
                found = True
                break
        if found:
            break
    return n

# Configure all the unpacking, algorithms, tags and input files
appConf = ApplicationMgr()
appConf.ExtSvc += ['ToolSvc', 'DataOnDemandSvc', LoKiSvc()]

dv = DaVinci()
dv.DataType = "2012"

# disable for older versions of DV
# generally it seems in older versions of DV
# this whole script 'breaks' at places
# raising exceptions and yet works ...
dre = DecodeRawEvent()
dre.DataOnDemand = True

lhcbApp = LHCbApp()
lhcbApp.Simulation = True
CondDB().Upgrade = False
# don't really need tags for looking around
# LHCbApp().DDDBtag = ...
# LHCbApp().CondDBtag  = ...

# Pass file to open as first command line argument
inputFiles = [sys.argv[-1]]
IOHelper('ROOT').inputFiles(inputFiles)

# Configure two instances of the TriggerTisTos tool to pick things up from the
# split Hlt1 and Hlt2 locations.
for stage in ('Hlt1', 'Hlt2'):
    ToolSvc().addTool(TriggerTisTos, stage + "TriggerTisTos")
    ttt = getattr(ToolSvc(), stage + "TriggerTisTos")
    ttt.HltDecReportsLocation = stage + '/DecReports'
    ttt.HltSelReportsLocation = stage + '/SelReports'

# Configuration done, run time!
appMgr = GP.AppMgr()
evt = appMgr.evtsvc()

# Instantiate the TisTos tools we configured above
hlt1TisTosTool = appMgr.toolsvc().create('TriggerTisTos/Hlt1TriggerTisTos',
                                         interface="ITriggerTisTos")
hlt2TisTosTool = appMgr.toolsvc().create('TriggerTisTos/Hlt2TriggerTisTos',
                                         interface="ITriggerTisTos")

# process some events
n = None
n_tos = 0
n_tis = 0
n_tistos = 0

while advance():
    # Get our candidates
    candidates = evt['AllStreams/Phys/D2hhPromptDst2D2RSLine/Particles']
    candidate = candidates[0]

    # Set the things we want to compare for TOS
    hlt1TisTosTool.setOfflineInput()
    hlt1TisTosTool.addToOfflineInput(candidate)
    hlt1TisTosTool.setTriggerInput()
    hlt1TisTosTool.addToTriggerInput("Hlt1TrackAllL0Decision")
    tos_result = hlt1TisTosTool.tisTosTobTrigger()
    n_tos += tos_result.tos()

    # Add some lines for TIS
    hlt1TisTosTool.addToTriggerInput("Hlt1TrackMuonDecision")
    hlt1TisTosTool.addToTriggerInput("Hlt1DiMuonHighMassDecision")
    tis_result = hlt1TisTosTool.tisTosTobTrigger()
    n_tis += tis_result.tis()
    n_tistos += (tos_result.tos() and tis_result.tis())
