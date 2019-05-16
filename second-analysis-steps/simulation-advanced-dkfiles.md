{% objectives "Learning Objectives" %}
* Learn advanced configuration options to configure `EvtGen`
{% endobjectives %} 

## Advanced: Modifying the decay

##Two-body decays - getting angular momentum right
EvtGen has specific models for each two body spin configuration, for example Scalar to Vector+Scalar (SVS), and Vector to lepton+lepton(VLL)
```
#
Decay B+sig
  1.000     MyJ/psi  K+                   SVS;
Enddecay
CDecay B-sig
#
Decay MyJ/psi
  1.000     mu+  mu-                      PHOTOS  VLL;
Enddecay
```
For decays to two vectors, there is a more complicated polarization structure which needs to be specified - for example here the fraction and phase for each helicity are set according to measured values:
```
Define Hp 0.159
Define Hz 0.775
Define Hm 0.612
Define pHp 1.563
Define pHz 0.0
Define pHm 2.712
#
Alias      MyJ/psi    J/psi
Alias      MyK*0      K*0
Alias      Myanti-K*0 anti-K*0
ChargeConj MyK*0      Myanti-K*0
ChargeConj MyJ/psi    MyJ/psi
#
Decay B0sig
  1.000         MyJ/psi   MyK*0          SVV_HELAMP Hp pHp Hz pHz Hm pHm;
Enddecay
Decay anti-B0sig
  1.000         MyJ/psi   Myanti-K*0     SVV_HELAMP Hm pHm Hz pHz Hp pHp;
Enddecay
```

## 3+ multi-body decays
For 3+ bodies the physics models get more complicated. For a fully hadronic final state, typically a Dalitz model will be specified, e.g:
```
# D_DALITZ includes resonances contributions (K*(892), K*(1430), K*(1680))
Decay MyD-
  1.000    K+        pi-    pi-          D_DALITZ;
Enddecay
CDecay MyD+
```
Any time you see a 3+ body decay with the PHSP model, you know it will be very far from reality. If you have no other information sometimes this is the best you can do, though.

A semileptonic decay would typically be produced according to some form factor model, e.g
```
Decay B0sig 
# FORM FACTORS as per HFAG PDG10
   1   MyD*-        mu+  nu_mu         PHOTOS  HQET 1.20 1.426 0.818 0.908;
  #
Enddecay
CDecay anti-B0sig
```
here the numbers correspond to measured values for the form factor parameters. 
## Cocktail decays
Often you will want to simulate more than one decay mode in a sample, e.g:
```
Decay MyD_s+
 0.0259 phi      mu+     nu_mu                      PHOTOS  ISGW2;
 0.0267 eta      mu+     nu_mu                      PHOTOS  ISGW2;
 0.0099 eta'     mu+     nu_mu                      PHOTOS  ISGW2;
 0.0037 K0       mu+     nu_mu                      PHOTOS  ISGW2;  
 0.0018 K*0      mu+     nu_mu                      PHOTOS  ISGW2;
 0.0020 f_0      mu+     nu_mu                      PHOTOS  ISGW2; 
 0.0059 mu+      nu_mu                              PHOTOS   SLN; 
Enddecay
CDecay MyD_s-
```
Note that the fractions will always be renormalised to sum to 1 - you can directly use PDG branching fractions without having to rescale by hand.

## Final state radiation
After generating the decay, final state radiation is added using PHOTOS. Note that PHOTOS is enabled by default, even though many decfiles explicitly specify it. It needs to be explicitly removed via "noPhotos"


## Changing particle masses / lifetimes/ widths
Sometimes you need to change the mass or lifetime of a particle, either because the initial values are wrong, or the particle you actually want doesn't exist in EvtGen, and you need to adapt an existing particle.
This can be done with python code inserted in the header:

```
# InsertPythonCode:
#from Configurables import LHCb__ParticlePropertySvc
#LHCb__ParticlePropertySvc().Particles = [ 
# "N(1440)+              636       12212   1.0      1.4400000      2.194041e-24                 N(1440)+           21440      0.00",
# "N(1440)~-             637      -12212  -1.0      1.4400000      2.194841e-24                   anti-N(1440)-           -21440      0.00",
#"N(1520)+              420        2124   1.0      1.52000000      5.723584e-24                   N(1520)+           21520      0.00",
# "N(1520)~-             421       -2124  -1.0      1.52000000     5.723584e-24                   anti-N(1520)-           -21520      0.00",
#"N(1535)+              713       22212   1.0      1.53500000      4.388081e-24                   N(1535)+           21535      0.00",
#"N(1535)~-             714      -22212  -1.0      1.53500000      4.388081e-24                   anti-N(1535)-           -21535      0.00",
#"N(1720)+              775       32124   1.0      1.72000000      2.632849e-24                   N(1720)+           21720      0.00",
#"N(1720)~-             776      -32124  -1.0      1.72000000      2.632849e-24                   anti-N(1720)-           -21720      0.00"
#]
# EndInsertPythonCode
```
The format is: 
```
#                    GEANTID    PDGID  CHARGE   MASS(GeV)      TLIFE(s)                    EVTGENNAME    PYTHIAID    MAXWIDTH
```
