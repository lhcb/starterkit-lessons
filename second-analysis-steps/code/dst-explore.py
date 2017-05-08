import sys

import GaudiPython as GP
from GaudiConf import IOHelper
from Configurables import (
    LHCbApp,
    ApplicationMgr,
    DataOnDemandSvc,
    SimConf,
    DigiConf,
    DecodeRawEvent,
    CondDB,
    DstConf,
    PhysConf,
    LoKiSvc,
    DaVinci
)

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
            if 'Swum' in l.identifier():
                continue

            temp = evt[l.identifier()]
            nodenames += nodes(evt, l)

    else:
        nodenames.append(node.identifier())

    return nodenames


def advance(decision):
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
        if dec.hasDecisionName('Stripping{0}Decision'.format(decision)):
            break

    return n


# Configure all the unpacking, algorithms, tags and input files
appConf = ApplicationMgr()
appConf.ExtSvc += ['ToolSvc', 'DataOnDemandSvc', LoKiSvc()]

dv = DaVinci()
dv.DataType = '2012'


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

# Configuration done, run time!
appMgr = GP.AppMgr()
evt = appMgr.evtsvc()

print_decay = appMgr.toolsvc().create(
    'PrintDecayTreeTool', interface='IPrintDecayTreeTool'
)

decay_finder = appMgr.toolsvc().create(
    'LoKi::Decay', interface='Decays::IDecay'
)

# process first event
appMgr.run(1)

# Get a small print out of what is there in the event
evt.dump()

# print out "all" TES locations
# prints out packed locations, so you need to know
# what the unpacked location is called to actually access it
print '-'*80
for node in nodes(evt):
    print node

# print out the stripping lines which fired for this event
print '-'*80
print evt['/Event/Strip/Phys/DecReports']


"""
To try and figure out why your decay descriptor does not work,
type the following into the python terminal

>>> advance()
>>> stream = '$MYSTREAM'
>>> line = '$MYLINE'
>>> candidates = evt['/Event/{0}/Phys/{1}/Particles'.format(stream, line)]
>>> head = candidates[0]
>>> print_decay.printTree(head)

Can we find match the candidate with the decay descriptor?

>>> decay_finder.hasDecay(candidates)

Beware, you can not repeatedly call this. It somehow keeps track
of how often it has been called/recursion stuff :-s
"""
