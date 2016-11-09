from Gaudi.Configuration import *
from Configurables import DataTrans
from Configurables import DataConsumer
from Configurables import DataProducer

ApplicationMgr().EvtMax = 10
ApplicationMgr().EvtSel = "NONE"

alg = DataTrans()
alg1 = DataProducer()
alg2 = DataConsumer()

alg1.StartingValue = 12

ApplicationMgr().TopAlg.append(alg)
ApplicationMgr().TopAlg.append(alg1)
ApplicationMgr().TopAlg.append(alg2)
