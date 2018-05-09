# Reuse particles from a decay tree

{% objectives "Learning Objectives" %}
* Learn how to extract particles from a decay tree
* Build a new particle from the extracted particles
{% endobjectives %}

Sometimes we want to extract a portion of the decay tree in order to build a different decay.
To do that, we need to put the particles we're interested in in a new container so they can afterwards be used as inputs to a `CombineParticles` instance (as we saw in [the selection framework lesson](/second-analysis-steps/building-decays-part-0.md)).
To achieve this we can use the `FilterInTrees` algorithm, a simple variation of `FilterDesktop` ([doxygen](https://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/hlt/latest_doxygen/de/d8e/class_filter_in_trees.html)).

Let's start from the example in [the selection framework lesson](/second-analysis-steps/building-decays-part0.md) and let's check that the $$K^+$$ child of the $$D^0$$ does not come from a $$K^{*}(892)^{0} \to K^{+}\pi^{-}$$.
To do that, we have to extract the $$K^+$$ from `([D0 -> K+ K-]CC)` and combine it with all pions in `Phys/StdAllNoPIDsPions/Particles`.

Using `FilterInTrees` is done in the same way we would use `FilterDesktop`:

```python
from Configurables import FilterInTrees
from PhysSelPython.Wrappers import Selection, AutomaticData

stream = 'AllStreams'
line = 'D2hhPromptDst2D2KKLine'
tesLoc = '/Event/{0}/Phys/{1}/Particles'.format(stream, line)

kaons_from_d0 = FilterInTrees('kaons_from_d0_filter', Code="('K+' == ABSID)")
kaons_from_d0_sel = Selection("kaons_from_d0_sel",
                            Algorithm=kaons_from_d0,
                            RequiredSelections=[AutomaticData(Location=tesLoc)])
```

The output of `kaons_from_d0_sel` is a container with all the kaons coming from the $$D^0$$.

The final step is easy, very similar to [building your own decay](/second-analysis-steps/building-decays-part0.md):

```python
from Configurables import CombineParticles
from PhysSelPython.Wrappers import Selection, DataOnDemand

Pions = DataOnDemand('Phys/StdAllNoPIDsPions/Particles')
kst = CombineParticles('kst_particles',
                       DecayDescriptor="[K*(892)0 -> K+ pi-]cc",
                       CombinationCut="ADAMASS('K*(892)0') < 300*MeV",
                       MotherCut='(VFASPF(VCHI2/VDOF)< 9)')
kst_sel = Selection('kst_sel',
                    Algorithm=kst,
                    RequiredSelections=[kaons_from_d0_sel, Pions])
```


{% callout "An interesting detail" %}
One can use `FilterInTrees` and `FilterDecays` to select several particles at
once and obtain a flattened list.
For example, if we had a Stripping line that builds `[B-  -> (^D0 -> ^K- ^pi+) ^pi-]cc` and we wanted to combine the `D0` and `pi-` with an external `pi0` to build `[B- -> D0 pi- pi0]cc`, we could do
```python
flatlist = FilterInTrees ("FlatList", Code="('D0' == ABSID) | ('pi-' == ABSID)")
from Configurables import CombineParticles
add_pi0 = CombineParticles("MakeB",
                           DecayDescriptor = "[B- -> D0 pi- pi0]cc",
                           ...
                           Inputs=[flatlist, resolvedPi0])
```
`flatlist` contains both `D0` and `pi-`, which are then used to build the `B`.
{% endcallout %}
