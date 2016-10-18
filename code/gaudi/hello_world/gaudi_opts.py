from Gaudi.Configuration import *
from Configurables import HelloWorld

ApplicationMgr().EvtMax = 10
ApplicationMgr().EvtSel = "NONE"

alg = HelloWorld()
ApplicationMgr().TopAlg.append(alg)
