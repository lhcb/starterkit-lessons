# Fun with LoKi Functors

{% objectives "Learning Objectives" %}

* Find out how the physics information can be obtained from the DST
* Understand what LoKi functors are
* Use LoKi functors interactively
* Be able to find functors that do what we want

{% endobjectives %} 

Probably most of you have seen somewhere in a Stripping line, a trigger line or a DaVinci options file an expression like this:

```python
'(PT > 750*MeV) & (P > 4000*MeV) & (MIPCHI2DV(PRIMARY) > 4)'.
```

Usually these strings represent so-called *LoKi functors*. In this lesson we will explore why they exist, how one can use them interactively and some of the technical details behind them.

LoKi functors are designed to flexibly compute and compare properties of the 
current decay, from simple quantities such as the transverse momentum of a 
particle or the impact parameter with respect to the primary vertex to even more complicated ones like helicity angles in a multi-body decay.
Internally, functors are implemented as C++ classes that take an object of type `TYPE1` and return another of `TYPE2`.
They can be used both in C++ and in Python code, and can be combined with each other using logical operations. 

According to `TYPE2` there are 3 types of functors:

 - *Functions*, which return `double`.
 - *Predicates*, which return a `bool`.
 - *Streamers*, which return a `std::vector` of some other type `TYPE3`.

When filling tuples, the most used functors are functions, while predicates are typically used for selections.

According to `TYPE1`, there are many types of functors, the most important of which are (you can find a full list in the [LoKi FAQ](https://twiki.cern.ch/twiki/bin/view/LHCb/FAQ/LoKiFAQ#How_to_code_own_LoKi_functor)):

 - *Particle functors*, which take `LHCb::Particle*` as input.
 - *Vertex functors*, which take `LHCb::VertexBase*` as input.
 - *MC particle functors*, which take `LHCb::MCParticle*` as input.
 - *MC vertex functors*, which take `LHCb::MCVertex*` as input.
 - *Array particle functors*, which take a `LoKi::Range_` (an array of particles) as input.
 - *Track functors*, which take `LHCb::Track` as input.

{% callout "C++ classes" %}

Things like `LHCb::Particle` are C++ classes that usually represent some 
physical object. You will interact with the C++ objects directly very rarely, 
if ever, also due to the existence of LoKi functors.

{% endcallout %} 

To understand what we can do with LoKi functors, we will pick up from where we 
left off [exploring a DST interactively](interactive-dst).
First open the DST as we did previously:

```bash
$ lb-run --ext=ipython DaVinci/v45r8 ipython -i explore.py 00070793_00000001_7.AllStreams.dst
```

Get the first candidate in the `D2hhPromptDst2D2KKLine` line:

```python
advance('D2hhPromptDst2D2KKLine')
candidates = evt['/Event/AllStreams/Phys/D2hhPromptDst2D2KKLine/Particles']
candidate = candidates[0]
```

The object `candidate `, loaded from the DST, is of type `LHCb::Particle` and we are looking at its representation via python bindings. 
We can do `help(candidate)` to find out which functions are available.
We can try to get very simple properties of the `$ D^{* +} $` candidate. Let's start from the components of its momentum.
This can be done calling the function `momentum()` for our candidate in the following way:
```python
p_x = candidate.momentum().X()
p_y = candidate.momentum().Y()
p_z = candidate.momentum().Z()
print p_x, p_y, p_z
```

This is inconvenient when [running DaVinci with Python options files](minimal-dv-job): there's no way of calling the `momentum()` method.
Instead, we can use the corresponding LoKi particle functors:

```python
from LoKiPhys.decorators import PX, PY, PZ
print PX(candidate)
print PY(candidate)
print PZ(candidate)
```

You will see an error when loading the functors:
```
LoKiSvc.REPORT      ERROR LoKi::AuxDesktopBase: 	loadDesktop(): unable to load IPhysDesktop! StatusCode=FAILURE
LoKiSvc.REPORT      ERROR The   ERROR message is suppressed : 'LoKi::AuxDesktopBase: 	loadDesktop(): unable to load IPhysDesktop!' StatusCode=FAILURE
```
This is related to the fact that some functors need to run in the `DaVinci` 
‘scope’, and they are all loaded in the `LoKiPhys.decorators` module. It's 
harmless in the examples we will use.
If the import is made *before* the instantiation of the `ApplicationMgr`, there 
will be no warnings.

{% challenge "Does it make sense?" %}

Compare the output of `PX` functor with the result of calling the function `candidate.momentum().X()`. 

{% endchallenge %} 

Math operations are also allowed:
```python
p_components_sum = PX + PY + PZ
p_components_sum(candidate)
```

There exist specific LoKi functors for all the most important properties of the particle. For example, the transverse momentum and mass:

```python
from LoKiPhys.decorators import PT, M
print PT(candidate)
print M(candidate)
```

{% challenge "Some practice" %}

Retrieve the momentum magnitude using functors `PX`, `PY` and `PZ`. There is also a specific functor `P` which does the job. Compare the results. 

Now, retrieve the transverse momentum and invariant mass (you will probably need the energy functor `E`), and see if it matches what the `PT` and `M` functors return.

{% endchallenge %} 

{% callout "A note about units" %}

By the [convention](https://lhcb-comp.web.cern.ch/lhcb-comp/Support/Conventions/units.pdf), the LHCb default units are MeV, millimeters and nanoseconds. It is easy to print the values of interest in other units:
```python
from LoKiPhys.decorators import GeV
print PT(candidate)/GeV
```

{% endcallout %} 

If we want to get the properties of the `$ D^{* +} $` vertex, for example its fit 
quality (`$ \chi^2 $`), we need to pass a vertex object to the vertex functor.

```python
from LoKiPhys.decorators import VCHI2
print VCHI2(candidate.endVertex())
```

Again, this is inconvenient when [running DaVinci with Python options 
files](minimal-dv-job), since in that case we don't have any way of 
calling the `endVertex` method.
Instead, we can use the `VFASPF` *adaptor* functor, which allows us to use 
vertex functors as if they were particle functors (note how the functor is 
built by combining two functors).

```python
from LoKiPhys.decorators import VFASPF
VCHI2(candidate.endVertex()) == VFASPF(VCHI2)(candidate)
```

{% challenge "Functions of functions of functions of…" %}

Make sure you understand what `VFASPF(VCHI2)(candidate)` means. It may help to play 
around in Python, creating a function that takes another function as an 
argument, for example:
```python
def create_greeting(salutation):
    def greet(name):
        print '{0}, {1}!'.format(salutation, name)
    return greet
```
What would `create_greeting('Hello')` return? What about 
`create_greeting('Howdy')('partner')`? Why is doing this useful?

{% endchallenge %} 

Calculation of some of the properties, such as the impact parameter (IP) or cosine of the 
direction angle (DIRA), requires the knowledge of the primary vertex (PV) 
associated to the candidate.
In `GaudiPython`, we can get the PVs ourselves.

```python
pv_finder_tool = appMgr.toolsvc().create(
    'GenericParticle2PVRelator<_p2PVWithIPChi2, OfflineDistanceCalculatorName>/P2PVWithIPChi2',
    interface='IRelatedPVFinder'
)
pvs = evt['/Event/Rec/Vertex/Primary']
best_pv = pv_finder_tool.relatedPV(candidate, pvs)
```
Now, we can get the cosine of the direction angle for the candidate given the primary vertex: 
```python
from LoKiPhys.decorators import DIRA
print DIRA(best_pv)(candidate)
```

Given that this is a very common operation, we have the possibility of using, in the context of a `DaVinci` application (Stripping, for example), a special set of functors, starting with the `BPV` prefix (for Best PV), which will get the PV for us.
Some functors also end with the suffix `DV`, which means they can only be used in the `DaVinci` context.

To get the quality of impact parameter of the candidate, one needs as well to call a distance calculator:
```python
from GaudiPython.Bindings import gbl
distCal = appMgr.toolSvc().create("LoKi::DistanceCalculator", interface=gbl.IDistanceCalculator)
ipTool = gbl.LoKi.Vertices.ImpactParamTool(distCal)
```
Now, we evaluate the quality of impact parameter of the candidate, given the primary vertex, and using the provided calculator:
```python
from LoKiPhys.decorators import IPCHI2
print IPCHI2(best_pv, ipTool)(candidate)
```

In the context of `DaVinci` application, e.g. the Stripping, the things become much simplier since the calculator instances are loaded automatically, and the syntax for calling the `IPCHI2` functor becomes `IPCHI2(best_pv,geo())(cand)`, where `geo()` is the geometry calculator tool.

{% callout "Finding LoKi functors" %}

The full list of defined LoKi functors can be found in the `LoKi::Cuts` 
namespace in the 
[doxygen](https://lhcb-doxygen.web.cern.ch/lhcb-doxygen/davinci/latest/d7/dae/namespace_lo_ki_1_1_cuts.html).
They are quite well documented with examples on how to use them.
The list can be overwhelming, so it's also worth checking a more curated selection of functors in the TWiki, [here](https://twiki.cern.ch/twiki/bin/view/LHCb/LoKiHybridFilters) and [here](https://twiki.cern.ch/twiki/bin/view/LHCb/LoKiParticleFunctions).

{% endcallout %} 

So far we've only looked at the properties of the head of the decay (that is, 
the `$ D^{* +} $`), but what if we want to get information about its decay products? As 
an example, let's get the largest transverse momentum of the final state 
particles.
A simple solution would be to navigate the tree and calculate the maximum 
`$ p_{\text{T}} $`.

```python
def find_tracks(particle):
    tracks = []
    if particle.isBasicParticle():
        proto = particle.proto()
        if proto:
            track = proto.track()
            if track:
                try:
                    tracks.append(particle.data())
                except AttributeError:
                    tracks.append(particle)
    else:
        for child in particle.daughters():
            tracks += find_tracks(child)
    return tracks

max_pt = max([PT(child) for child in find_tracks(candidate)])
```
{% callout "A note about the try/except" %}

If you import LoKi before running this example, it magically removes the 
`.data()` function and allows the particle to be used directly. The code above 
is made general using the `try`/`except` block and will work in either case.

{% endcallout %} 


However, LoKi offers functions for performing such operations, namely `MAXTREE` and `MINTREE`, which get as parameters the selection criteria, the functor to calculate and a default value.
In our example,

```python
from LoKiPhys.decorators import MAXTREE, ISBASIC, HASTRACK
MAXTREE(ISBASIC & HASTRACK, PT, -1)(candidate) == max_pt
```

In this example, we have used two selection functors, `ISBASIC` and `HASTRACK`, which return true if the particle doesn't have children and is made up by a track, respectively.
We can see that they do the same thing as `particle.isBasicParticle()` and `particle.proto().track()` in a more compact way.

{% callout "Combining LoKi cuts" %}

You might have noticed above we used the `&` operator (bitwise AND) to 
combine the `ISBASIC` and `HASTRACK` cuts above.
This is because Python doesn't allow LoKi to override the behaviour of `and` and `or` ("logical AND/OR"), so if we use them
the Python interpreter tries to combine the two cuts straight away, before we have even passed in our candidate:
```python
In [1]: ((M>1200) or (PT > 500))
Out[1]:  (M>1200)
```
the result is that our `PT` cut vanishes!
If we use the `|` operator ("bitwise OR") then LoKi correctly builds a functor representing the `OR` of our cuts:
```python
In [2]: ((M>1200) | (PT > 500))
Out[2]:  ( (M>1200) | (PT>500) )
```
This is why you should **always** use `&` and `|` when combining LoKi functors, and **never** use `and` and `or`.

{% endcallout %} 

Similarly, the `SUMTREE` functor allows us to accumulate quantities for those children that pass a certain selection:
```python
from LoKiPhys.decorators import SUMTREE, ABSID
print SUMTREE(321 == ABSID, PT)(candidate)
print SUMTREE('K+' == ABSID, PT)(candidate)
```
In this case, we have summed the transverse momentum of the charged kaons in the tree.
Note the usage of the `ABSID` functor, which selects particles from the decay 
tree using either their [PDG Monte Carlo ID](http://pdg.lbl.gov/2019/reviews/rpp2018-rev-monte-carlo-numbering.pdf) or their name.
If you would like to consider only the kaons of one specific charge in the selection requirement, consider the `ID` functor which does exactly the same thing, however has a sign which is positive for particles and negative for antiparticles. 

Another very useful LoKi functor is `CHILD`, which allows us to access a 
property of a single child of the particle.
To specify which child we want, its order is used, so we need to know how the candidate was built.
For example, from
```
In [10]: candidate.daughtersVector()
Out[10]:

 0 |->D0                           M/PT/E/PX/PY/PZ: 1.8653/ 2.5848/ 31.32/ 2.508/-0.6267/ 31.15 [GeV]  #  0 
                                       EndVertex  X/Y/Z: 1.053/-0.2006/-29.13 [mm]  Chi2/nDoF 0.2349/1 #  0 
 1    |->K+                        M/PT/E/PX/PY/PZ: 0.4937/ 2.1334/ 20.26/ 2.129/0.1371/ 20.14 [GeV]  #  5 
 1    |->K-                        M/PT/E/PX/PY/PZ: 0.4937/ 0.8534/ 11.06/0.3795/-0.7643/ 11.01 [GeV]  # 10 
 0 |->pi+                          M/PT/E/PX/PY/PZ: 0.1396/ 0.2558/ 3.101/0.2451/-0.0733/ 3.088 [GeV]  #  4 

```
we know that `D0` is the first child and `pi+` is the second.
Therefore, to access the mass of the `$ D^{0} $` we have 2 options:
```python
from LoKiPhys.decorators import CHILD
# Option 1
mass = M(cand.daughtersVector()[0])
# Option 2
mass_child = CHILD(M, 1)(cand)
# Do they agree?
mass == mass_child
```

{% challenge "Child vertex?" %}

Evaluate the quality of the D0 decay vertex.

{% endchallenge %} 

In the similar way, we may access properties of child of the child: for example, a kaon from the `$ D^{0} $` decay:
```python
from LoKiPhys.decorators import CHILD
mass_kaon = CHILD(CHILD(M, 1),1)(candidate)
```

{% challenge "Tracks and PID" %}

For the particles having tracks, we may exploit track functors to get the corresponding track properties. For instance, the track quality is given by functor `TRCHI2`.

What happens if we call `TRCHI2(candidate)`? Explain the result.

Evaluate the track quality for the first and second kaon, also independently of that retrieve (in a single line) the worst of two.

Then, evaluate the probability that each kaon is really a kaon (`PROBNNk`) or rather a misidentified pion (`PROBNNpi`).

{% endchallenge %} 

The usage of LoKi functors extends much further than in the interactive 
`GaudiPython` world we've been exploring here.

They constitute the basis of particle filtering in the *selection framework*, 
discussed in the [Building your own decay chain](/second-analysis-steps/building-decays-part0) 
lesson in 
[second-analysis-steps](/second-analysis-steps/README).
Selecting particles means using LoKi *predicates*, functors that give a `bool` 
output, like `ISBASIC` and `HASTRACK`.
Amongst these, a key functor is `in_range`, which returns `True` if the value 
of the given *function* functor (that is, the functor that returns a `double`) 
is within the given lower and upper limit.
It helps writing CPU-efficient functors and thus is very important when building time-critical software like trigger or stripping lines.

```python
from LoKiCore.functions import in_range
in_range(2000, M, 2014)(candidate)
in_range(1860, CHILD(M, 1), 1870)(candidate)
```
{% callout "Understanding the cuts in the stripping lines" %}

Have a look at the stripping line 
[D2hhPromptDst2D2KKLine](http://lhcbdoc.web.cern.ch/lhcbdoc/stripping/config/stripping28r1/charm/strippingd2hhpromptdst2d2kkline.html) which is used in our example. Open a `CombineParticles/D2hhPromptDst2D2KKLine` section, and explain which requirements are coded in the 'MotherCut', 'DaughterCuts' and 'CombinationCut' sections. 
(More details about `CombineParticles` algorithm are explained in the [lesson of second analysis steps](/second-analysis-steps/building-decays-part1.md).)

{% endcallout %} 


Additionally, LoKi functors can be used directly inside our `DaVinci` jobs to store specific bits of information in our ntuples without the need for a complicated C++-based algorithms.
This second option will be discussed in the [TupleTools and branches lesson](add-tupletools).

{% callout "Debugging LoKi functors" %}

If you write complicated LoKi functors, typically in the context of selections, 
you need functions for debugging when things go wrong.
LoKi provides wrapper functors that evaluate a functor (or functor expression), print debugging information and return the result;
the most important of these are:

 - `dump1`, which prints the input object and returns the calculated functor value,
  ```python
  from LoKiCore.functions import dump1
  debug_p_components_sum = dump1(p_components_sum)
  debug_p_components_sum(candidate)
  ```
 - `monitor` which prints the input the functor string and returns the calculated functor value,
  ```python
  from LoKiCore.functions import monitor
  monitor_p_components_sum = monitor(p_components_sum)
  monitor_p_components_sum(candidate)
  ```

{% endcallout %} 
