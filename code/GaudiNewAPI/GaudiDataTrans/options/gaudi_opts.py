from Gaudi.Configuration import *
from Configurables import (
                           DataProducer,
                           DataConsumer,
                           DataConsumerProp)

ApplicationMgr().EvtMax = 10
ApplicationMgr().EvtSel = "NONE"

alg1 = DataProducer()
alg2 = DataConsumer()
alg3 = DataConsumerProp()

alg1.StartingValue = 12

ApplicationMgr().TopAlg.append(alg1)
ApplicationMgr().TopAlg.append(alg2)
ApplicationMgr().TopAlg.append(alg3)
