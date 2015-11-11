import sys

import GaudiPython as GP
from GaudiConf import IOHelper
from Configurables import DaVinci

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

dv = DaVinci()
dv.DataType = '2012'

# Pass file to open as first command line argument
inputFiles = [sys.argv[-1]]
IOHelper('ROOT').inputFiles(inputFiles)

appMgr = GP.AppMgr()
evt = appMgr.evtsvc()

print_decay = appMgr.toolsvc().create(
    'PrintDecayTreeTool', interface='IPrintDecayTreeTool'
)

decay_finder = appMgr.toolsvc().create(
    'LoKi::Decay', interface='Decays::IDecay'
)

appMgr.run(1)
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
