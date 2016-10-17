from Gaudi.Configuration import *
from Configurables import HelloWorld

alg = HelloWorld()

ApplicationMgr().TopAlg.append(alg)
