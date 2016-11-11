from Gaudi.Configuration import *
from Configurables import (
                           DataProducer,
                           DataConsumer,
                           DataProducerProp,
                           DataConsumerProp)

ApplicationMgr().EvtMax = 5
ApplicationMgr().EvtSel = "NONE"

alg1 = DataProducer()
alg2 = DataConsumer()
alg1p = DataProducerProp()
alg2p = DataConsumerProp()

alg1.StartingValue = 12
alg1p.StartingValue = 17

ApplicationMgr().TopAlg.append(alg1)
ApplicationMgr().TopAlg.append(alg2)
ApplicationMgr().TopAlg.append(alg1p)
ApplicationMgr().TopAlg.append(alg2p)
