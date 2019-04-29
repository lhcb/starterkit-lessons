# Reuse particles from a decay tree

{% objectives "Learning Objectives" %}
* Learn how to extract particles from a decay tree
* Build a new particle from the extracted particles
{% endobjectives %}

Sometimes we want to extract a portion of the decay tree in order to build a different decay.
To do that, we need to put the particles we're interested in in a new container so they can afterwards be used as inputs to a `CombineParticles` instance (as we saw in [the selection framework lesson](/second-analysis-steps/building-decays-part-0.md)).
To achieve this we can use the `FilterInTrees` algorithm, a simple variation of `FilterDesktop` ([doxygen](http://lhcb-doxygen.web.cern.ch/lhcb-doxygen/davinci/latest/d0/d0c/class_filter_desktop.html)).

Let's start from the example in [the selection framework lesson](/second-analysis-steps/building-decays-part0.md) and let's check that the $$K^+$$ child of the $$D^0$$ does not come from a $$K^{*}(892)^{0} \to K^{+}\pi^{-}$$.
To do that, we have to extract the $$K^+$$ from `([D0 -> K+ K-]CC)` and combine it with all pions in `Phys/StdAllNoPIDsPions/Particles`.

Using `FilterInTrees` is done in the same way we would use `FilterDesktop`:

```python
from Configurables import FilterInTrees
from PhysSelPython.Wrappers import Selection, DataOnDemand

stream = 'AllStreams'
line = 'D2hhPromptDst2D2KKLine'
tesLoc = '/Event/{0}/Phys/{1}/Particles'.format(stream, line)

kaons_from_d0 = FilterInTrees('kaons_from_d0_filter', Code="('K+' == ABSID)")
kaons_from_d0_sel = Selection("kaons_from_d0_sel",
                            Algorithm=kaons_from_d0,
                            RequiredSelections=[DataOnDemand(Location=tesLoc)])
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

Add the `kst_sel` to the example from [building your own decay](/second-analysis-steps/building-decays-part0.md):

```python
#Combine D and pions into Dstar                                                                                                              
dstar_sel = CombineSelection(
    'Sel_Dstar',
    [d0_sel, soft_pion_sel, kst_sel],
    DecayDescriptor='[D*(2010)+ -> D0 pi+]cc',
    CombinationCut=dstar_comb,
    MotherCut=dstar_mother
)
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
