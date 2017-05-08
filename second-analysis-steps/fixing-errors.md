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
Sel_D0            SUCCESS Number of counters : 13
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "# D0 -> pi-  K+ "                              |      1000 |         43 |   0.043000 |    0.20286 |      0.0000 |      1.0000 |
 | "# D~0 -> pi+  K- "                             |      1000 |         55 |   0.055000 |    0.23233 |      0.0000 |      2.0000 |
 | "# K+"                                          |      1000 |       1161 |     1.1610 |     1.9155 |      0.0000 |      21.000 |
 | "# K-"                                          |      1000 |       1135 |     1.1350 |     2.1248 |      0.0000 |      29.000 |
 | "# Phys/StdAllLooseKaons"                       |      1000 |      24448 |     24.448 |     19.363 |      1.0000 |      136.00 |
 | "# Phys/StdAllNoPIDsPions"                      |      1000 |      42328 |     42.328 |     26.566 |      3.0000 |      181.00 |
 | "# input particles"                             |      1000 |      66776 |     66.776 |     45.704 |      4.0000 |      317.00 |
 | "# pi+"                                         |      1000 |       1962 |     1.9620 |     2.3436 |      0.0000 |      26.000 |
 | "# pi-"                                         |      1000 |       1912 |     1.9120 |     2.5401 |      0.0000 |      32.000 |
 | "# selected"                                    |      1000 |         98 |   0.098000 |    0.31368 |      0.0000 |      3.0000 |
 |*"#accept"                                       |      1000 |         94 |(  9.40000 +- 0.922843 )%|   -------   |   -------   |
 |*"#pass combcut"                                 |     10403 |        254 |(  2.44160 +- 0.151318 )%|   -------   |   -------   |
 |*"#pass mother cut"                              |       254 |         98 |(  38.5827 +- 3.05439  )%|   -------   |   -------   |
Sel_Dstar         SUCCESS Number of counters : 14
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "# D*(2010)+ -> D0  pi+ "                       |        94 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "# D*(2010)- -> D~0  pi- "                      |        94 |          1 |   0.010638 |    0.10259 |      0.0000 |      1.0000 |
 | "# D0"                                          |        94 |         43 |    0.45745 |    0.49819 |      0.0000 |      1.0000 |
 | "# D~0"                                         |        94 |         55 |    0.58511 |    0.51384 |      0.0000 |      2.0000 |
 | "# Phys/Sel_D0"                                 |        94 |         98 |     1.0426 |    0.24904 |      1.0000 |      3.0000 |
 | "# Phys/StdAllNoPIDsPions"                      |        94 |       4481 |     47.670 |     25.661 |      7.0000 |      153.00 |
 | "# input particles"                             |        94 |       4579 |     48.713 |     25.705 |      8.0000 |      154.00 |
 | "# pi+"                                         |        94 |       2175 |     23.138 |     11.977 |      4.0000 |      58.000 |
 | "# pi-"                                         |        94 |       2091 |     22.245 |     12.936 |      1.0000 |      90.000 |
 | "# selected"                                    |        94 |          1 |   0.010638 |    0.10259 |      0.0000 |      1.0000 |
 |*"#accept"                                       |        94 |          1 |(  1.06383 +- 1.05816  )%|   -------   |   -------   |
 |*"#pass combcut"                                 |      2240 |        586 |(  26.1607 +- 0.928634 )%|   -------   |   -------   |
 |*"#pass mother cut"                              |       586 |          1 |( 0.170648 +- 0.170503 )%|   -------   |   -------   |
 | "Error from IParticleCombiner, skip the combinat|         6 |          6 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
```

Here we have information of the input containers, types of particles, etc, with all the counters corresponding to our run on 1000 events.

{% challenge "Understanding the log" %}
How many $$D^*$$ do we expect in our ntuple? Can you check it?
Can you change some cuts and see how he counters change? Try to free the $$D^*$$ mass and see if we get more of those.
{% endchallenge %}

Now, let's make the particle builder fail silently and see if we can debug this.
For example, imagine we forgot to add the Kaons as inputs in `Sel_D0`:

```python
d0_sel = Selection('Sel_D0',
                   Algorithm=d0,
                   RequiredSelections=[Pions])
```

Then we get

```
Sel_D0            SUCCESS Number of counters : 12
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "# D0 -> pi-  K+ "                              |      1000 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "# D~0 -> pi+  K- "                             |      1000 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "# K+"                                          |      1000 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "# K-"                                          |      1000 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "# Phys/StdAllNoPIDsPions"                      |      1000 |      42328 |     42.328 |     26.566 |      3.0000 |      181.00 |
 | "# input particles"                             |      1000 |      42328 |     42.328 |     26.566 |      3.0000 |      181.00 |
 | "# pi+"                                         |      1000 |       1962 |     1.9620 |     2.3436 |      0.0000 |      26.000 |
 | "# pi-"                                         |      1000 |       1912 |     1.9120 |     2.5401 |      0.0000 |      32.000 |
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
 | "# D0"                                          |        94 |         43 |    0.45745 |    0.49819 |      0.0000 |      1.0000 |
 | "# D~0"                                         |        94 |         55 |    0.58511 |    0.51384 |      0.0000 |      2.0000 |
 | "# Phys/Sel_D0"                                 |        94 |         98 |     1.0426 |    0.24904 |      1.0000 |      3.0000 |
 | "# Phys/StdAllNoPIDsPions"                      |        94 |       4481 |     47.670 |     25.661 |      7.0000 |      153.00 |
 | "# input particles"                             |        94 |       4579 |     48.713 |     25.705 |      8.0000 |      154.00 |
 | "# pi+"                                         |        94 |       2175 |     23.138 |     11.977 |      4.0000 |      58.000 |
 | "# pi-"                                         |        94 |       2091 |     22.245 |     12.936 |      1.0000 |      90.000 |
 | "# selected"                                    |        94 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 |*"#accept"                                       |        94 |          0 |(  0.00000 +- 1.06383  )%|   -------   |   -------   |
 |*"#pass combcut"                                 |      2240 |        586 |(  26.1607 +- 0.928634 )%|   -------   |   -------   |
 |*"#pass mother cut"                              |       586 |          0 |(  0.00000 +- 0.170648 )%|   -------   |   -------   |
 | "Error from IParticleCombiner, skip the combinat|         6 |          6 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
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
Before               INFO ========= /Event[0x224961d0@EventDataSvc]:
Before               INFO +--> /Event [Address: CLID=0x1 Type=0x2]  DataObject
Before               INFO | +--> /Gen [Address: CLID=0x1 Type=0x2]  (Unloaded)
Before               INFO | +--> /MC [Address: CLID=0x1 Type=0x2]  (Unloaded)
Before               INFO | +--> /Link [Address: CLID=0x1 Type=0x2]  (Unloaded)
Before               INFO | +--> /pSim [Address: CLID=0x1 Type=0x2]  (Unloaded)
Before               INFO | +--> /Rec [Address: CLID=0x1 Type=0x2]  (Unloaded)
Before               INFO | +--> /Trigger [Address: CLID=0x1 Type=0x2]  (Unloaded)
Before               INFO | +--> /Calo [Address: CLID=0x1 Type=0x2]  (Unloaded)
Before               INFO | +--> /Muon [Address: CLID=0x1 Type=0x2]  (Unloaded)
Before               INFO | +--> /Rich [Address: CLID=0x1 Type=0x2]  (Unloaded)
Before               INFO | +--> /Other [Address: CLID=0x1 Type=0x2]  (Unloaded)
Before               INFO | +--> /pRec [Address: CLID=0x1 Type=0x2]  (Unloaded)
Before               INFO | +--> /Prev [Address: CLID=0x1 Type=0x2]  (Unloaded)
Before               INFO | +--> /PrevPrev [Address: CLID=0x1 Type=0x2]  (Unloaded)
Before               INFO | +--> /Next [Address: CLID=0x1 Type=0x2]  (Unloaded)
Before               INFO | +--> /NextNext [Address: CLID=0x1 Type=0x2]  (Unloaded)
Before               INFO | +--> /DAQ [Address: CLID=0x1 Type=0x2]  (Unloaded)
Before               INFO | +--> /Strip [Address: CLID=0x1 Type=0x2]  (Unloaded)
Before               INFO   +--> /AllStreams [Address: CLID=0x1 Type=0x2]  (Unloaded)
...
After                INFO ========= /Event[0x224961d0@EventDataSvc]:
After                INFO +--> /Event [Address: CLID=0x1 Type=0x2]  DataObject
After                INFO | +--> /Gen [Address: CLID=0x1 Type=0x2]  (Unloaded)
After                INFO | +--> /MC [Address: CLID=0x1 Type=0x2]  (Unloaded)
After                INFO | +--> /Link [Address: CLID=0x1 Type=0x2]  (Unloaded)
After                INFO | +--> /pSim [Address: CLID=0x1 Type=0x2]  (Unloaded)
After                INFO | +--> /Rec [Address: CLID=0x1 Type=0x2]  DataObject
After                INFO | | +--> /Header [Address: CLID=0x69 Type=0x2]  LHCb::RecHeader
After                INFO | | +--> /Status [Address: CLID=0x1389 Type=0x2]  (Unloaded)
After                INFO | | +--> /Summary [Address: CLID=0x6a Type=0x2]  (Unloaded)
After                INFO | | +--> /ProtoP [No Address]  DataObject
After                INFO | | | +--> /Charged [No Address]  KeyedContainer<LHCb::ProtoPartic [38]
After                INFO | | +--> /Muon [No Address]  DataObject
After                INFO | | | +--> /MuonPID [No Address]  KeyedContainer<LHCb::MuonPID,Con [27]
After                INFO | | +--> /Track [No Address]  DataObject
After                INFO | | | +--> /Best [No Address]  KeyedContainer<LHCb::Track,Conta [70]
After                INFO | | +--> /Rich [No Address]  DataObject
After                INFO | | | +--> /PIDs [No Address]  KeyedContainer<LHCb::RichPID,Con [46]
After                INFO | | +--> /Vertex [No Address]  DataObject
After                INFO | |   +--> /Primary [No Address]  KeyedContainer<LHCb::RecVertex,C [2]
After                INFO | +--> /Trigger [Address: CLID=0x1 Type=0x2]  DataObject
After                INFO | | +--> /RawEvent [Address: CLID=0x3ea Type=0x2]  LHCb::RawEvent
After                INFO | +--> /Calo [Address: CLID=0x1 Type=0x2]  (Unloaded)
After                INFO | +--> /Muon [Address: CLID=0x1 Type=0x2]  (Unloaded)
After                INFO | +--> /Rich [Address: CLID=0x1 Type=0x2]  (Unloaded)
After                INFO | +--> /Other [Address: CLID=0x1 Type=0x2]  (Unloaded)
After                INFO | +--> /pRec [Address: CLID=0x1 Type=0x2]  DataObject
After                INFO | | +--> /Track [Address: CLID=0x1 Type=0x2]  DataObject
After                INFO | | | +--> /Best [Address: CLID=0x60e Type=0x2]  LHCb::PackedTracks
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
After                INFO | +--> /Prev [Address: CLID=0x1 Type=0x2]  (Unloaded)
After                INFO | +--> /PrevPrev [Address: CLID=0x1 Type=0x2]  (Unloaded)
After                INFO | +--> /Next [Address: CLID=0x1 Type=0x2]  (Unloaded)
After                INFO | +--> /NextNext [Address: CLID=0x1 Type=0x2]  (Unloaded)
After                INFO | +--> /DAQ [Address: CLID=0x1 Type=0x2]  DataObject
After                INFO | | +--> /ODIN [No Address]  LHCb::ODIN
After                INFO | +--> /Strip [Address: CLID=0x1 Type=0x2]  (Unloaded)
After                INFO | +--> /AllStreams [Address: CLID=0x1 Type=0x2]  (Unloaded)
After                INFO   +--> /Phys [No Address]  DataObject
After                INFO   | +--> /StdAllNoPIDsPions [No Address]  DataObject
After                INFO   | | +--> /Particles [No Address]  KeyedContainer<LHCb::Particle,Co [16]
After                INFO   | | +--> /decayVertices [No Address]  KeyedContainer<LHCb::Vertex,Cont [0]
After                INFO   | | +--> /Particle2VertexRelations [No Address]  LHCb::Relation1D<LHCb::Particle,
After                INFO   | | +--> /_RefitPVs [No Address]  KeyedContainer<LHCb::RecVertex,C [0]
After                INFO   | +--> /StdAllLooseKaons [No Address]  DataObject
After                INFO   | | +--> /Particles [No Address]  KeyedContainer<LHCb::Particle,Co [7]
After                INFO   | | +--> /decayVertices [No Address]  KeyedContainer<LHCb::Vertex,Cont [0]
After                INFO   | | +--> /Particle2VertexRelations [No Address]  LHCb::Relation1D<LHCb::Particle,
After                INFO   | | +--> /_RefitPVs [No Address]  KeyedContainer<LHCb::RecVertex,C [0]
After                INFO     +--> /Sel_D0 [No Address]  DataObject
After                INFO       +--> /Particles [No Address]  KeyedContainer<LHCb::Particle,Co [0]
```

Here we can see where our decays are being put and we can debug problems with inputs and outputs.
It can also be useful to know where things are written for accessing them interactively, if we want to further explore and debug.

{% callout "Configuring the algorithm" %}
The `StoreExplorerAlg` has the same print frequency as `DaVinci`, but it can be configured by modifying `PrintFreq` (fraction of events that are printed) or `PrintEvt`.
Have a look at the [class Doxygen](http://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/moore/releases/v14r2p5/doxygen/d2/d3b/class_store_explorer_alg.html) to see what they do.
{% endcallout %}
