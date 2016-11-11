from Gaudi.Configuration import *
from Configurables import HelloWorldEx

alg = HelloWorldEx()

ApplicationMgr(
    EvtMax = 10,
    EvtSel = 'NONE',
    HistogramPersistency = 'NONE',
    TopAlg = [alg],
    # OutputLevel = DEBUG
)
