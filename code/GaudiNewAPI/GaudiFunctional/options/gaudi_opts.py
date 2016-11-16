from Gaudi.Configuration import *
from Configurables import (DataMaker,
                           DataConsumer)

ApplicationMgr().EvtMax = 5
ApplicationMgr().EvtSel = "NONE"


alg1 = DataMaker()
alg2 = DataConsumer()

ApplicationMgr().TopAlg.append(alg1)
ApplicationMgr().TopAlg.append(alg2)
