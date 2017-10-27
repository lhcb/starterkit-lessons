# Reuse particles from a decay tree

{% objectives "Learning Objectives" %}
* Learn how to extract particles from a decay tree
* Build a new particle from the extracted particles
{% endobjectives %}

Sometimes we want to extract a portion of the decay tree in order to build a different decay.
To do that, we need to put the particles we're interested in in a new container so they can afterwards be used as inputs to a `CombineParticles` instance (as we saw in [the selection framework lesson](/second-analysis-steps/building-decays-part-0.md)).
To achieve this we can use the `FilterInTrees` algorithm, a simple variation of `FilterDesktop` ([doxygen](https://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/hlt/latest_doxygen/de/d8e/class_filter_in_trees.html)).

Let's start from the example in [the selection framework lesson](/second-analysis-steps/building-decays-part0.md) and let's check that the $$\pi^-$$ child of the $$D^0$$ does not come from a $$\rho\to\pi^+\pi^-$$.
To do that, we have to extract the $$\pi^-$$ from `([D0 -> pi+ K-]CC)` and combine it with all pions in `Phys/StdAllNoPIDsPions/Particles`.

Using `FilterInTrees` is done in the same way we would use `FilterDesktop`:

```python
from Configurables import FilterInTrees
from PhysSelPython.Wrappers import Selection, DataOnDemand

decay_tree_location = 'Phys/D2hhCompleteEventPromptDst2D2RSLine/Particles'
d0_from_dst = FilterInTrees('d0_from_dst_filter', Code="DECTREE('[Charm -> pi+ K-]CC')")
d0_from_dst_sel = Selection("d0_from_dst_sel",
                            Algorithm=d0_from_dst,
                            RequiredSelections=[AutomaticData(Location=decay_tree_location)])
pions_from_d0 = FilterInTrees('pions_from_d0_filter',Code="('pi+' == ABSID)")
pions_from_d0_sel = Selection("pions_from_d0_sel",
                              Algorithm=pions_from_d0,
                              RequiredSelections=[d0_from_dst_sel])
```

The output of `pions_from_d0_sel` is a container with all the pions coming from the $$D^0$$.

{% callout "Question" %}
Do you see why we couldn't use something simple like this?
```python
pions_from_d0 = FilterInTrees('pions_from_d0_filter', Code="'pi+' == ABSID")
```
{% endcallout %}

Note how we had to do the process in two steps in order to avoid getting the soft pion from the $$D^*$$.
Sometimes this makes things quite difficult, but almost all problems can be solved with a smart use of the `DECTREE` container in an intermediate step.

{% challenge "Selecting the soft pion" %}
Can you find of a way of selecting the soft pion?
Hint: use the `FilterDecays` algorithm, in which you specify a decay descriptor as `Code`, marking the desired particle(s).
{% endchallenge %}

The final step is easy, very similar to [building your own decay](/second-analysis-steps/building-decays-part0.md):

```python
from Configurables import CombineParticles
from PhysSelPython.Wrappers import Selection, DataOnDemand

Pions = DataOnDemand('Phys/StdAllNoPIDsPions/Particles')
rho = CombineParticles('rho_particles',
                       DecayDescriptor=['rho(770)0 -> pi+ pi-'],
                       CombinationCut="ADAMASS('rho(770)0') < 300*MeV",
                       MotherCut='(VFASPF(VCHI2/VDOF)< 9)')
rho_sel = Selection('rho_sel',
                    Algorithm=rho,
                    RequiredSelections=[pions_from_d0_sel, Pions])
```

Unfortunately, the `CombineParticles` example we just wrote is not exactly what we meant, since it will actually build $$\rho$$ from all pions it gets as input, not using one from our `pions_from_d0` selection and one from `'Phys/StdAllNoPIDsPions/Particles'`.
How to solve this?
We have to get creative and use the tools at hand:
for example, we could use `SubstitutePID` from the previous lesson to change the PID of the pions in the `pions_from_d0` selection to kaon and build `[rho(770)0 -> K+ pi-]CC` and then change again the PID of the kaon to a pion.
Of course, if we were reconstructing $$K^{*}(892)^{0} \to K^{-}\pi^{+}$$ with `Phys/StdAllLooseKaons/Particles` instead, for example, we would already have everything we need since the ambiguity wouldn't exist.

{% callout "An interesting detail" %}
One can use `FilterInTrees` and `FilterDecays` to select several particles at
once and obtain a flattened list.
For example, if we had a Stripping line that builds `[B-  -> (^D0 -> ^K- ^pi+) ^pi-]cc` and we wanted to combine the $$D^0$$ and $$\pi^-$$ with an external $$\pi^0$$ to build `[B- -> D0 pi- pi0]cc`, we could do
```python
flatlist = FilterInTrees ("FlatList", Code="('D0' == ABSID) | ('pi-' == ABSID)")
from Configurables import CombineParticles
add_pi0 = CombineParticles("MakeB",
                           DecayDescriptor = "[B- -> D0 pi- pi0]cc",
                           ...
                           Inputs=[flatlist, resolvedPi0])
```
`flatlist` contains both $$D^0$$ and $$\pi^-$$, which are then used to build the $$B$$.
{% endcallout %}
