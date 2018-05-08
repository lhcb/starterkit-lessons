# What to do when something fails

{% objectives "Learning Objectives" %}
* Learn how to read the logs to know where things are breaking
* Learn how to get a glimpse of where algorithms are writing in the TES
{% endobjectives %} 

When chaining complex workflows (building particles, combining them, etc) we find that our ntuple is not written while we don't have any errors.
The first step is to look at the logs.
Let's first go back at what we learned [when building our own decays](/second-analysis-steps/building-decays-part0) and rerun again (saving the output to a log file!).

We can scroll through the log until we find our selections, where we will see something like this:

```
Sel_D0            SUCCESS Number of counters : 10
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "# D0 -> K-  K+ "                               |      1000 |         95 |   0.095000 |    0.29661 |      0.0000 |      2.0000 |
 | "# D~0 -> K+  K- "                              |      1000 |         95 |   0.095000 |    0.29661 |      0.0000 |      2.0000 |
 | "# K+"                                          |      1000 |        723 |    0.72300 |    0.89122 |      0.0000 |      6.0000 |
 | "# K-"                                          |      1000 |        691 |    0.69100 |    0.85995 |      0.0000 |      6.0000 |
 | "# Phys/StdAllLooseKaons"                       |      1000 |      20491 |     20.491 |     13.579 |      1.0000 |      90.000 |
 | "# input particles"                             |      1000 |      20491 |     20.491 |     13.579 |      1.0000 |      90.000 |
 | "# selected"                                    |      1000 |        190 |    0.19000 |    0.59321 |      0.0000 |      4.0000 |
 |*"#accept"                                       |      1000 |         94 |(  9.40000 +- 0.922843 )%|   -------   |   -------   |
 |*"#pass combcut"                                 |      1308 |        234 |(  17.8899 +- 1.05974  )%|   -------   |   -------   |
 |*"#pass mother cut"                              |       234 |        190 |(  81.1966 +- 2.55434  )%|   -------   |   -------   |
SelFilterPhys_S...SUCCESS Number of counters : 1
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 |*"#passed"                                       |        94 |         94 |(  100.000 +- 1.06383  )%|   -------   |   -------   |
Sel_Dstar         SUCCESS Number of counters : 14
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "# D*(2010)+ -> D0  pi+ "                       |        94 |         30 |    0.31915 |    0.48844 |      0.0000 |      2.0000 |
 | "# D*(2010)- -> D~0  pi- "                      |        94 |         31 |    0.32979 |    0.51340 |      0.0000 |      2.0000 |
 | "# D0"                                          |        94 |         95 |     1.0106 |    0.10259 |      1.0000 |      2.0000 |
 | "# D~0"                                         |        94 |         95 |     1.0106 |    0.10259 |      1.0000 |      2.0000 |
 | "# Phys/Sel_D0"                                 |        94 |        190 |     2.0213 |    0.20518 |      2.0000 |      4.0000 |
 | "# Phys/StdAllNoPIDsPions"                      |        94 |       3699 |     39.351 |     19.111 |      7.0000 |      95.000 |
 | "# input particles"                             |        94 |       3889 |     41.372 |     19.174 |      9.0000 |      99.000 |
 | "# pi+"                                         |        94 |       1767 |     18.798 |     8.8949 |      3.0000 |      42.000 |
 | "# pi-"                                         |        94 |       1743 |     18.543 |     9.4381 |      3.0000 |      49.000 |
 | "# selected"                                    |        94 |         61 |    0.64894 |    0.55940 |      0.0000 |      2.0000 |
 |*"#accept"                                       |        94 |         57 |(  60.6383 +- 5.03902  )%|   -------   |   -------   |
 |*"#pass combcut"                                 |      3601 |        843 |(  23.4102 +- 0.705629 )%|   -------   |   -------   |
 |*"#pass mother cut"                              |       843 |         61 |(  7.23606 +- 0.892333 )%|   -------   |   -------   |
 | "Error from IParticleCombiner, skip the combinat|         5 |          5 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |

```

Here we have information of the input containers, types of particles, etc, with all the counters corresponding to our run on 1000 events.

{% challenge "Understanding the log" %}
How many D* do we expect in our ntuple? Can you check it?
Can you change some cuts and see how the counters change? Try to free the D* mass and see if you get more of those.
{% endchallenge %}

Now, let's make the particle builder fail silently and see if we can debug this.
For example, imagine we forgot to add the Kaons as inputs in `Sel_D0`, and instead by mistake added the Pions:

```python
d0_sel = Selection('Sel_D0',
                   Algorithm=d0,
                   RequiredSelections=[Pions])
```

Then we get

```
Sel_D0            SUCCESS Number of counters : 10
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "# D0 -> K-  K+ "                               |      1000 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "# D~0 -> K+  K- "                              |      1000 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "# K+"                                          |      1000 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "# K-"                                          |      1000 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "# Phys/StdAllNoPIDsPions"                      |      1000 |      38848 |     38.848 |     21.700 |      1.0000 |      128.00 |
 | "# input particles"                             |      1000 |      38848 |     38.848 |     21.700 |      1.0000 |      128.00 |
 | "# selected"                                    |      1000 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 |*"#accept"                                       |      1000 |          0 |(  0.00000 +- 0.100000 )%|   -------   |   -------   |
 | "#pass combcut"                                 |         0 |          0 |     0.0000 |     0.0000 | 1.7977e+308 |-1.7977e+308 |
 | "#pass mother cut"                              |         0 |          0 |     0.0000 |     0.0000 | 1.7977e+308 |-1.7977e+308 |

```

It's easy to see we have 0 input kaons and we can see we only get input pions!

Another problem: we messed up with a cut, for example in building the $$D^*$$,

```python
dstar_mother = (
    "(abs(M-MAXTREE('D0'==ABSID,M)-145.42) < 10*MeV)"
    '& (VFASPF(VCHI2/VDOF) < 0)'
)
```

Running this, we get

```
Sel_Dstar         SUCCESS Number of counters : 14
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "# D*(2010)+ -> D0  pi+ "                       |        94 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "# D*(2010)- -> D~0  pi- "                      |        94 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "# D0"                                          |        94 |         95 |     1.0106 |    0.10259 |      1.0000 |      2.0000 |
 | "# D~0"                                         |        94 |         95 |     1.0106 |    0.10259 |      1.0000 |      2.0000 |
 | "# Phys/Sel_D0"                                 |        94 |        190 |     2.0213 |    0.20518 |      2.0000 |      4.0000 |
 | "# Phys/StdAllNoPIDsPions"                      |        94 |       3699 |     39.351 |     19.111 |      7.0000 |      95.000 |
 | "# input particles"                             |        94 |       3889 |     41.372 |     19.174 |      9.0000 |      99.000 |
 | "# pi+"                                         |        94 |       1767 |     18.798 |     8.8949 |      3.0000 |      42.000 |
 | "# pi-"                                         |        94 |       1743 |     18.543 |     9.4381 |      3.0000 |      49.000 |
 | "# selected"                                    |        94 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 |*"#accept"                                       |        94 |          0 |(  0.00000 +- 1.06383  )%|   -------   |   -------   |
 |*"#pass combcut"                                 |      3601 |        843 |(  23.4102 +- 0.705629 )%|   -------   |   -------   |
 |*"#pass mother cut"                              |       843 |          0 |(  0.00000 +- 0.118624 )%|   -------   |   -------   |
 | "Error from IParticleCombiner, skip the combinat|         5 |          5 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
```

And we would get suspicious about the `MotherCut`...

The final trick is to use the `StoreExplorerAlg`, which shows us the state of the TES in the middle of execution.
We can configure it very easily and insert it in the DaVinci sequence to see what is happening.

```python
from Configurables import StoreExplorerAlg

DaVinci().UserAlgorithms += [StoreExplorerAlg("Before"),
                             dstar_seq.sequence(),
                             StoreExplorerAlg("After")]
```

And then run it:

```
EventSelector     SUCCESS Reading Event record 1. Record number within stream 1: 1
...
Before               INFO ========= /Event[0x2f009e70@EventDataSvc]:
Before               INFO +--> /Event [Address: CLID=0x1 Type=0x2]  DataObject
Before               INFO | +--> /Gen [Address: CLID=0x1 Type=0x2]  (Unloaded) 
Before               INFO | +--> /PrevPrev [Address: CLID=0x1 Type=0x2]  (Unloaded) 
Before               INFO | +--> /Prev [Address: CLID=0x1 Type=0x2]  (Unloaded) 
Before               INFO | +--> /Next [Address: CLID=0x1 Type=0x2]  (Unloaded) 
Before               INFO | +--> /NextNext [Address: CLID=0x1 Type=0x2]  (Unloaded) 
Before               INFO | +--> /MC [Address: CLID=0x1 Type=0x2]  (Unloaded) 
Before               INFO | +--> /Link [Address: CLID=0x1 Type=0x2]  (Unloaded) 
Before               INFO | +--> /pSim [Address: CLID=0x1 Type=0x2]  (Unloaded) 
Before               INFO | +--> /Rec [Address: CLID=0x1 Type=0x2]  DataObject
Before               INFO | | +--> /Header [Address: CLID=0x69 Type=0x2]  LHCb::RecHeader
Before               INFO | | +--> /Status [Address: CLID=0x1389 Type=0x2]  (Unloaded) 
Before               INFO | | +--> /Summary [Address: CLID=0x6a Type=0x2]  (Unloaded) 
Before               INFO | | +--> /ProtoP [No Address]  DataObject
Before               INFO | | | +--> /Charged [No Address]  KeyedContainer<LHCb::ProtoPartic [0x1d]
Before               INFO | | +--> /Muon [No Address]  DataObject
Before               INFO | | | +--> /MuonPID [No Address]  KeyedContainer<LHCb::MuonPID,Con [0x17]
Before               INFO | | +--> /Track [No Address]  DataObject
Before               INFO | | | +--> /Best [No Address]  KeyedContainer<LHCb::Track,Conta [0x45]
Before               INFO | | | +--> /FittedHLT1VeloTracks [No Address]  KeyedContainer<LHCb::Track,Conta [0x20]
Before               INFO | | +--> /Rich [No Address]  DataObject
Before               INFO | | | +--> /PIDs [No Address]  KeyedContainer<LHCb::RichPID,Con [0x18]
Before               INFO | | +--> /Vertex [No Address]  DataObject
Before               INFO | |   +--> /Primary [No Address]  KeyedContainer<LHCb::RecVertex,C [0x3]
Before               INFO | +--> /Calo [Address: CLID=0x1 Type=0x2]  (Unloaded) 
Before               INFO | +--> /Unstripped [Address: CLID=0x1 Type=0x2]  (Unloaded) 
Before               INFO | +--> /HC [Address: CLID=0x1 Type=0x2]  (Unloaded) 
Before               INFO | +--> /Tracker [Address: CLID=0x1 Type=0x2]  (Unloaded) 
Before               INFO | +--> /Velo [Address: CLID=0x1 Type=0x2]  (Unloaded) 
Before               INFO | +--> /Muon [Address: CLID=0x1 Type=0x2]  (Unloaded) 
Before               INFO | +--> /Rich [Address: CLID=0x1 Type=0x2]  (Unloaded) 
Before               INFO | +--> /Trigger [Address: CLID=0x1 Type=0x2]  DataObject
Before               INFO | | +--> /RawEvent [Address: CLID=0x3ea Type=0x2]  LHCb::RawEvent
Before               INFO | +--> /pRec [Address: CLID=0x1 Type=0x2]  DataObject
Before               INFO | | +--> /Track [Address: CLID=0x1 Type=0x2]  DataObject
Before               INFO | | | +--> /Best [Address: CLID=0x60e Type=0x2]  LHCb::PackedTracks
Before               INFO | | | +--> /FittedHLT1VeloTracks [Address: CLID=0x60e Type=0x2]  LHCb::PackedTracks
Before               INFO | | | +--> /Muon [Address: CLID=0x60e Type=0x2]  (Unloaded) 
Before               INFO | | +--> /Rich [Address: CLID=0x1 Type=0x2]  DataObject
Before               INFO | | | +--> /PIDs [Address: CLID=0x619 Type=0x2]  LHCb::PackedRichPIDs
Before               INFO | | +--> /Muon [Address: CLID=0x1 Type=0x2]  DataObject
Before               INFO | | | +--> /MuonPID [Address: CLID=0x623 Type=0x2]  LHCb::PackedMuonPIDs
Before               INFO | | +--> /Calo [Address: CLID=0x1 Type=0x2]  (Unloaded) 
Before               INFO | | +--> /ProtoP [Address: CLID=0x1 Type=0x2]  DataObject
Before               INFO | | | +--> /Charged [Address: CLID=0x610 Type=0x2]  LHCb::PackedProtoParticles
Before               INFO | | | +--> /Neutrals [Address: CLID=0x610 Type=0x2]  (Unloaded) 
Before               INFO | | +--> /Vertex [Address: CLID=0x1 Type=0x2]  DataObject
Before               INFO | | | +--> /Primary [Address: CLID=0x611 Type=0x2]  LHCb::PackedRecVertices
Before               INFO | |   +--> /V0 [Address: CLID=0x612 Type=0x2]  (Unloaded) 
Before               INFO | +--> /Turbo [Address: CLID=0x1 Type=0x2]  (Unloaded) 
Before               INFO | +--> /Strip [Address: CLID=0x1 Type=0x2]  (Unloaded) 
Before               INFO | +--> /AllStreams [Address: CLID=0x1 Type=0x2]  (Unloaded) 
Before               INFO | +--> /DAQ [No Address]  DataObject
Before               INFO | | +--> /ODIN [No Address]  LHCb::ODIN
Before               INFO   +--> /Phys [No Address]  DataObject
Before               INFO   | +--> /StdAllLooseKaons [No Address]  DataObject
Before               INFO   | | +--> /Particles [No Address]  KeyedContainer<LHCb::Particle,Co [0x9]
Before               INFO   | | +--> /decayVertices [No Address]  KeyedContainer<LHCb::Vertex,Cont [0]
Before               INFO   | | +--> /Particle2VertexRelations [No Address]  LHCb::Relation1D<LHCb::Particle,
Before               INFO   | | +--> /_RefitPVs [No Address]  KeyedContainer<LHCb::RecVertex,C [0]
Before               INFO   | +--> /Sel_D0 [No Address]  DataObject
Before               INFO   | | +--> /Particles [No Address]  KeyedContainer<LHCb::Particle,Co [0]
Before               INFO     +--> /TupleDstToD0pi_D0ToKpi [No Address]  DataObject
Before               INFO       +--> /Particles [No Address]  KeyedContainer<LHCb::Particle,Co [0]
After                INFO ========= /Event[0x2f009e70@EventDataSvc]:
After                INFO +--> /Event [Address: CLID=0x1 Type=0x2]  DataObject
After                INFO | +--> /Gen [Address: CLID=0x1 Type=0x2]  (Unloaded) 
After                INFO | +--> /PrevPrev [Address: CLID=0x1 Type=0x2]  (Unloaded) 
After                INFO | +--> /Prev [Address: CLID=0x1 Type=0x2]  (Unloaded) 
After                INFO | +--> /Next [Address: CLID=0x1 Type=0x2]  (Unloaded) 
After                INFO | +--> /NextNext [Address: CLID=0x1 Type=0x2]  (Unloaded) 
After                INFO | +--> /MC [Address: CLID=0x1 Type=0x2]  (Unloaded) 
After                INFO | +--> /Link [Address: CLID=0x1 Type=0x2]  (Unloaded) 
After                INFO | +--> /pSim [Address: CLID=0x1 Type=0x2]  (Unloaded) 
After                INFO | +--> /Rec [Address: CLID=0x1 Type=0x2]  DataObject
After                INFO | | +--> /Header [Address: CLID=0x69 Type=0x2]  LHCb::RecHeader
After                INFO | | +--> /Status [Address: CLID=0x1389 Type=0x2]  (Unloaded) 
After                INFO | | +--> /Summary [Address: CLID=0x6a Type=0x2]  (Unloaded) 
After                INFO | | +--> /ProtoP [No Address]  DataObject
After                INFO | | | +--> /Charged [No Address]  KeyedContainer<LHCb::ProtoPartic [0x1d]
After                INFO | | +--> /Muon [No Address]  DataObject
After                INFO | | | +--> /MuonPID [No Address]  KeyedContainer<LHCb::MuonPID,Con [0x17]
After                INFO | | +--> /Track [No Address]  DataObject
After                INFO | | | +--> /Best [No Address]  KeyedContainer<LHCb::Track,Conta [0x45]
After                INFO | | | +--> /FittedHLT1VeloTracks [No Address]  KeyedContainer<LHCb::Track,Conta [0x20]
After                INFO | | +--> /Rich [No Address]  DataObject
After                INFO | | | +--> /PIDs [No Address]  KeyedContainer<LHCb::RichPID,Con [0x18]
After                INFO | | +--> /Vertex [No Address]  DataObject
After                INFO | |   +--> /Primary [No Address]  KeyedContainer<LHCb::RecVertex,C [0x3]
After                INFO | +--> /Calo [Address: CLID=0x1 Type=0x2]  (Unloaded) 
After                INFO | +--> /Unstripped [Address: CLID=0x1 Type=0x2]  (Unloaded) 
After                INFO | +--> /HC [Address: CLID=0x1 Type=0x2]  (Unloaded) 
After                INFO | +--> /Tracker [Address: CLID=0x1 Type=0x2]  (Unloaded) 
After                INFO | +--> /Velo [Address: CLID=0x1 Type=0x2]  (Unloaded) 
After                INFO | +--> /Muon [Address: CLID=0x1 Type=0x2]  (Unloaded) 
After                INFO | +--> /Rich [Address: CLID=0x1 Type=0x2]  (Unloaded) 
After                INFO | +--> /Trigger [Address: CLID=0x1 Type=0x2]  DataObject
After                INFO | | +--> /RawEvent [Address: CLID=0x3ea Type=0x2]  LHCb::RawEvent
After                INFO | +--> /pRec [Address: CLID=0x1 Type=0x2]  DataObject
After                INFO | | +--> /Track [Address: CLID=0x1 Type=0x2]  DataObject
After                INFO | | | +--> /Best [Address: CLID=0x60e Type=0x2]  LHCb::PackedTracks
After                INFO | | | +--> /FittedHLT1VeloTracks [Address: CLID=0x60e Type=0x2]  LHCb::PackedTracks
After                INFO | | | +--> /Muon [Address: CLID=0x60e Type=0x2]  (Unloaded) 
After                INFO | | +--> /Rich [Address: CLID=0x1 Type=0x2]  DataObject
After                INFO | | | +--> /PIDs [Address: CLID=0x619 Type=0x2]  LHCb::PackedRichPIDs
After                INFO | | +--> /Muon [Address: CLID=0x1 Type=0x2]  DataObject
After                INFO | | | +--> /MuonPID [Address: CLID=0x623 Type=0x2]  LHCb::PackedMuonPIDs
After                INFO | | +--> /Calo [Address: CLID=0x1 Type=0x2]  (Unloaded) 
After                INFO | | +--> /ProtoP [Address: CLID=0x1 Type=0x2]  DataObject
After                INFO | | | +--> /Charged [Address: CLID=0x610 Type=0x2]  LHCb::PackedProtoParticles
After                INFO | | | +--> /Neutrals [Address: CLID=0x610 Type=0x2]  (Unloaded) 
After                INFO | | +--> /Vertex [Address: CLID=0x1 Type=0x2]  DataObject
After                INFO | | | +--> /Primary [Address: CLID=0x611 Type=0x2]  LHCb::PackedRecVertices
After                INFO | |   +--> /V0 [Address: CLID=0x612 Type=0x2]  (Unloaded) 
After                INFO | +--> /Turbo [Address: CLID=0x1 Type=0x2]  (Unloaded) 
After                INFO | +--> /Strip [Address: CLID=0x1 Type=0x2]  (Unloaded) 
After                INFO | +--> /AllStreams [Address: CLID=0x1 Type=0x2]  (Unloaded) 
After                INFO | +--> /DAQ [No Address]  DataObject
After                INFO | | +--> /ODIN [No Address]  LHCb::ODIN
After                INFO   +--> /Phys [No Address]  DataObject
After                INFO   | +--> /StdAllLooseKaons [No Address]  DataObject
After                INFO   | | +--> /Particles [No Address]  KeyedContainer<LHCb::Particle,Co [0x9]
After                INFO   | | +--> /decayVertices [No Address]  KeyedContainer<LHCb::Vertex,Cont [0]
After                INFO   | | +--> /Particle2VertexRelations [No Address]  LHCb::Relation1D<LHCb::Particle,
After                INFO   | | +--> /_RefitPVs [No Address]  KeyedContainer<LHCb::RecVertex,C [0]
After                INFO   | +--> /Sel_D0 [No Address]  DataObject
After                INFO   | | +--> /Particles [No Address]  KeyedContainer<LHCb::Particle,Co [0]
After                INFO     +--> /TupleDstToD0pi_D0ToKpi [No Address]  DataObject
After                INFO       +--> /Particles [No Address]  KeyedContainer<LHCb::Particle,Co [0]

```

Here we can see where our decays are being put and we can debug problems with inputs and outputs.
It can also be useful to know where things are written for accessing them interactively, if we want to further explore and debug.

{% callout "Configuring the algorithm" %}
The `StoreExplorerAlg` has the same print frequency as `DaVinci`, but it can be configured by modifying `PrintFreq` (fraction of events that are printed) or `PrintEvt`.
Have a look at the [class Doxygen](http://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/moore/releases/v14r2p5/doxygen/d2/d3b/class_store_explorer_alg.html) to see what they do.
{% endcallout %}
