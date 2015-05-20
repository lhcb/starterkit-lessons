# SetupProject DaVinci v36r6
import sys

import GaudiPython as GP
from GaudiConf import IOHelper
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

def advance(decision='B02DKWSD2HHHBeauty2CharmLine'):
    """Advance until stripping decision is true, returns
    number of events by which we advanced"""
    n = 0
    while True:
        appMgr.run(1)
        n += 1
        dec=evt['/Event/Strip/Phys/DecReports']
        if dec.hasDecisionName("Stripping%sDecision"%decision):
            break

    return n


# Configure all the unpacking, algorithms, tags and input files
appConf = ApplicationMgr()
appConf.ExtSvc+= ['ToolSvc', 'DataOnDemandSvc', LoKiSvc()]

from Configurables import DaVinci
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
#LHCbApp().DDDBtag = ... 
#LHCbApp().CondDBtag  = ...

# Pass file to open as first command line argument
inputFiles = [sys.argv[-1]]
IOHelper('ROOT').inputFiles(inputFiles)
    

# Configuration done, run time!
appMgr = GP.AppMgr()
evt = appMgr.evtsvc()

print_decay = appMgr.toolsvc().create('PrintDecayTreeTool', interface="IPrintDecayTreeTool")

# New style decay descriptors, also known as LoKi decays
loki_decay_finder = appMgr.toolsvc().create('LoKi::Decay', interface="Decays::IDecay")
# Old style decay descriptors
old_decay_finder = appMgr.toolsvc().create("DecayFinder", interface="IDecayFinder")

# works
#decay_desc = "[[B0]cc -> (^D- => {^K- ^K+ ^pi-, ^K- ^pi+ ^pi-,^pi+ ^pi- ^pi-, ^K- ^K- ^pi+}) ^K-]cc"
# doesn't work
decay_desc = "[[B0]cc -> (^D- => {^K- ^K+ ^pi-, ^K- ^pi+ ^pi-,^pi+ ^pi- ^pi-}) ^K-]cc"
old_decay_finder.setDecay(decay_desc)

# process first event
appMgr.run(1)

# Get a small print out of what is there in the event
evt.dump()

# print out "all" TES locations
# prints out packed locations, so you need to know
# what the unpacked location is called to actually access it
print "-"*80
for node in nodes(evt):
    print node

# print out the stripping lines which fired for this event
print "-"*80
print evt['/Event/Strip/Phys/DecReports']


# Figure out why your decay descriptor does not work
# type the following into the python terminal
#advance()

#WS_candidates = evt['/Event/Bhadron/Phys/B02DKWSD2HHHBeauty2CharmLine/Particles']
#B = WS_candidates[0]
#print_decay.printTree(B)
# Can we find the decay?
#old_decay_finder.hasDecay(WS_candidates)

# Beware, you can not repeatedly call this. It somehow keeps track
# of how often it has been called/recursion stuff :-s
#head = Particle()
#old_decay_finder.findDecay(WS_candidates, head)
# head will contain the head of the decay tree
#print head
