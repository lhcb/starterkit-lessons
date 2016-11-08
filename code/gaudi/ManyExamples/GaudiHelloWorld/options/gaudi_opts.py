from Gaudi.Configuration import *
from Configurables import HelloWorldEx

ApplicationMgr().EvtMax = 10
ApplicationMgr().EvtSel = "NONE"

alg = HelloWorldEx()
ApplicationMgr().TopAlg.append(alg)
