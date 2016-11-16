from Gaudi.Configuration import *
from Configurables import GaudiTemplateAlg

# Might need data instead
ApplicationMgr().EvtMax = 10
ApplicationMgr().EvtSel = "NONE"

#Test your algorithm here
alg = GaudiTemplateAlg()
ApplicationMgr().TopAlg.append(alg)
