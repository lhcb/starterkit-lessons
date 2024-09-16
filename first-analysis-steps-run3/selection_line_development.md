# Selection lines in Moore
{% objectives "Learning Objectives" %}

* Basics of line development
* How to produce a DST by running Moore 

{% endobjectives %} 
## Prerequisites

Before starting the development of a new line it can be useful to create and checkout a new branch in the Moore project:

```bash
git branch <user_name>-Starterkit-Bs2JpsiPhi
git checkout <user_name>-Starterkit-Bs2JpsiPhi
```

## Writing an HLT2 line

In the [dataflow Run 3] lesson we have seen that in Run 3 most analyses will have to move their selection to the HLT2 stage. 
This means that for new analyses the first step will often be to develop the corresponding HLT2 line. An HLT2 line defines a sequence of algorithm to build your analysis candidate.

In this tutorial we will use the decay $B_s^0 \to J\!/\!\psi(1S) \phi(1020)$ where the $J\!/\!\psi$ furthers decays into a pair of muons and the $\phi$ into pair of kaons as an example for the development of an HLT2 selection lines. Selection lines are defined in this directory in the [Moore project](https://gitlab.cern.ch/lhcb/Moore):

```bash
Hlt/Hlt2Conf/python/Hlt2Conf/lines
```

Here every physics analysis working group has a directory for their set of selection lines. You can find the relevant directory of your working group by looking at the directory name in this table:

| Purpose     | directory     |
| ----------- | --------------- |
| B2CC        | b_to_charmonia  |
| B2OC        | b_to_open_charm |
| B2noC       |                 |
| B&Q         | bandq           |
| Charm       | charm           |
| QEE         | qee             |
| RD          | rd              |
| SL          | semileptonic    |

Furthermore in current setup there is additional line for dedicated purposes such ```jets```, ```monitoring```, ```pid```, ```topological_b```.

For the tutorial we create another directory ```starterkit``` for our line developments:

```bash
mkdir Hlt/Hlt2Conf/python/Hlt2Conf/lines/starterkit
cd Hlt/Hlt2Conf/python/Hlt2Conf/lines/starterkit
```
The HLT2 and sprucing lines are written in python. Therefore we want to place new python files ```starterkit_lines.py```, ```hlt2_starterkit.py```, ```spruce_starterkit.py``` in our directory. The first one will be used for the selection itself, the second and third for registration of the selection lines as HLT2 or sprucing line.

## Writing a selection
We start with developing code for ```starterkit_lines.py```.

Selections can be applied to single particles or a set of particles building a combination. ```ParticleFilter``` and ```ParticleCombiner``` are used to apply these selections. Another algorithm we will need is ```require_all``` which enables to apply a selection with a set of different in a more elegant way. 

They can be imported from [```algorithms_thor.py```](https://gitlab.cern.ch/lhcb/Moore/-/blob/master/Hlt/Hlt2Conf/python/Hlt2Conf/algorithms_thor.py). Additionally we have to import ThOr functors to enable cuts on various variables.

```python
from Hlt2Conf.algorithms_thor import ParticleFilter, ParticleCombiner, require_all
import Functors as F
from Functors.math import in_range
```

 In the case of `$B_s^0 \to J\!/\!\psi(1S) \phi(1020)$` we can start from building `$\mu^+ \mu^-$` and `$K^+K^-$` combinations to build `$J\!/\!\psi(1S)$` and `$\phi(1020)$` composites.

To allow for flexibility and maintainabilty of the Moore framework it is highly recommended to share code for commonly used selections. While [```standard_particles.py```](https://gitlab.cern.ch/lhcb/Moore/-/blob/master/Hlt/Hlt2Conf/python/Hlt2Conf/standard_particles.py) provides builders for many commmon final-state and intermediate particles, there are additional builders for working specific purposes provided. It is very important build up on these commonly used code bases. 

For demonstration purposes we will first start to develop our own builders for intermediate particles and later switch to importing already existing ones. 

To select $B_s^0 \to J\!/\!\psi(1S)( \to \mu^+ \mu^-) \phi(1020)(\to K^+ K^-)$ decays we first need to select the final-state particles, i.e. muons and kaons. There is a variety of muon and kaon builders in ```standard_particles.py```.

We can for example start with muons from ```make_ismuon_long_muon``` and kaons from ```make_has_rich_long_kaons``` and further preselect these by using the ```ParticleFilter``` algorithm:

```python
from Hlt2Conf.standard_particles import make_ismuon_long_muon, make_has_rich_long_kaons

def make_selected_muons():
        muons = make_ismuon_long_muon()
        cut = F.ALL
        return ParticleFilter(muons, F.FILTER(cut))

def make_selected_kaons():
        kaons = make_has_rich_long_kaon()
        cut = F.ALL
        return ParticleFilter(kaons, F.FILTER(cut))
```

```F.ALL``` is a functor that does not apply any cuts.

Now we can start to apply cuts on the kinematics and PID using ThOr functors. An exhaustive list of all currently available functors can be found [here](https://gitlab.cern.ch/lhcb/Rec/-/blob/master/Phys/FunctorCore/python/Functors/__init__.py). We also need to import units for kinematics cuts.

```python
from GaudiKernel.SystemOfUnits import GeV, MeV
```

```python
def make_selected_muons(pt_min=250. * MeV,
                    p_min=2.0 * GeV,
                    pid=0.):
    kaons = make_has_rich_long_kaon()
    cut = require_all(F.PT > pt_min,
                      F.P > p_min,
                      F.PID_MU > pid)   
    return ParticleFilter(kaons, F.FILTER(cut))

def make_selected_kaons(pt_min=250. * MeV,
                    p_min=2.0 * GeV,
                    pid=0.):
    
    muons = make_ismuon_long_muon()
    cut = require_all(F.PT > pt_min,
                      F.P > p_min,
                      F.PID_K > pid)                 

    return ParticleFilter(muons, F.FILTER(cut))
```

Some of the variables you may want to cut on require PV information such as the $\text{IP}$ or $\chi^2_\text{IP}$. Before we can apply such a cut, we need to import and call an algorithm that is creating PVs.

```python
from RecoConf.reconstruction_objects import make_pvs_v2 as make_pvs

def make_selected_muons(pt_min=250. * MeV,
                    p_min=2.0 * GeV,
                    pid=0.,
                    ipchi2_min = 0.):
    
    pvs = make_pvs()
    kaons = make_has_rich_long_kaon()
    cut = require_all(F.PT > pt_min,
                     F.P > p_min,
                     F.PID_MU > pid,
                     F.MINIPCHI2(pvs) > ipchi2_min)

    return ParticleFilter(kaons, F.FILTER(cut))

def make_selected_kaons(pt_min=250. * MeV,
                    p_min=2.0 * GeV,
                    pid=0., 
                    ipchi2_min=0.):
    
    pvs = make_pvs()
    muons = make_ismuon_long_muon()
    cut = require_all(F.PT > pt_min,
                     F.P > p_min,
                     F.PID_K > pid,
                     F.MINIPCHI2(pvs) > ipchi2_min)

    return ParticleFilter(muons, F.FILTER(cut))
```
Disclaimers: 
1. ```MINIPCHI2``` can look very different from what you were used to in Run 2 since the number of PVs per event will be much higher in the next data takings.
2. ```make_pvs_v2``` is a temporary solution where ```v2``` depicts the the ThOr-compatible version 2 of the event model.

An attentive learner will quickly spot the the code above is just twice a replica of itself with a different standard particle list passed to `ParticleFilter` and a different PID cut.

We want to avoid code replication as much as possible, therefore the above lines could be easily rewritten as a `make_selected_tracks`.

```python
def make_selected_tracks(particles,
                 pvs = make_pvs,
                 pt_min=250. * MeV,
                 p_min=2.0 * GeV,  
                 pid = (F.PID_K > 0.),
                 ipchi2_min=0.):

    cut =  require_all( F.PT > pt_min, 
                         F.P > p_min,
                         pid,                    
                         F.MINIPCHI2(pvs) > ipchi2_min,

    return ParticleFilter(particles, F.FILTER(cut))
```
We can then build kaons and muons by writing
```python
kaons = filter_tracks()
muons = filter_tracks(make_long_muons, pid = (F.PID_MU > 0.)) 
``` 

The next step would be to build and select intermediate composite particles, that the $B_s^0$ decay into: $J\!/\!\psi(1S)$ and $\phi(1020)$.
To achieve this we define `make_phi_to_kk` and `make_jpsi2mumu` functions.
The functions that build composite particles, i.e. particles that are not the final tracks in the decay, are often refered to as `builders`. 

```python
def make_jpsi_to_mumu(pvs=make_pvs):

        muons = make_selected_tracks(make_ismuon_long_muons(),
                                        pvs=pvs,
                                        pt_min = 500*MeV,
                                        p_min = 0*GeV,
                                        pid = (F.PID_MU > -5.),
                                        m_min = 2950*MeV,
                                        m_max = 3250*MeV,
                                        max_docachi2 = 20,
                                        pt_sum = 0.,
                                        pt_jpsi_min = 500*MeV,
                                        max_vertexchi2 = 16)


        combination_cut = require_all(in_range(m_min, F.MASS, m_max), F.MAXDOCACHI2CUT < max_docachi2, F.SUM(F.PT) > pt_sum)
        vertex_cut = require_all(F.PT > pt_jspi_min, F.CHI2 < max_vertexchi2)

        return ParticleCombiner(
                Inputs=[muons,muons],
                DecayDescriptor="J/psi(1S) -> mu+ mu-",
                CombinationCut=combination_cut,
                CompositeCut=vertex_cut
                )

def make_phi_to_kk(pvs=make_pvs,
                 pt_k_min = 500*MeV,
                 m_min = 980*MeV,
                 m_max = 1060*MeV,
                 max_docachi2 = 30,
                 pt_phi = 500*MeV,
                 max_vertexchi2 = 25,
                 pt_sum = 0):

        kaons = make_selected_tracks(make_has_rich_long_kaons(),
                                        pvs=pvs,
                                        pt_min = pt_k_min)
        combination_cut = require_all(in_range(m_min, F.MASS, m_max), F.MAXDOCACHI2CUT < max_docachi2, F.SUM(F.PT) > pt_sum)
        vertex_cut = require_all(F.PT > pt_phi, F.CHI2 < max_vertexchi2)
        return ParticleCombiner(
                Inputs=[kaons,kaons],
                DecayDescriptor="phi(1020) -> K+ K-",
                CombinationCut=combination_cut,
                CompositeCut=vertex_cut)
  
```

Here we utilized a helper wrapper `in_range`, that represents cuts in form of `a_min < var < a_max` and the `ParticleCombiner` algorithm to create the $J\!/\!\psi(1S)$ and $\phi(1020)$ candidates. 
`ParticleCombiner` takes as an inputs both `CombinationCut` and `CompositeCut`.

* The `CombinationCut` is applied on the combinatoric combinations of the input particles. 
The `CompositeCut` is applied on the composite particle, i.e. the $J\!/\!\psi(1S)$ and $\phi(1020)$ in our case, after the vertex fit is performed.
* Fitting vertices can be computationally intensive and time-consuming. Therefore it is important to reduce the number of combinations producing unwanted background candidates beforehand by applying an efficient `CombinationCut`.

{% callout Rules for `DecayDescriptor` %}
* The order of the particle containers parsed as arguments to ```Inputs``` has to match the particles used in the ```DecayDescriptor```.
* particles should be sorted by their rareness of occurence, so that the combinatorics of the `ParticleCombinber` are minimal.
* Equal particles in the `DecayDescriptor` need equal particles containers.
{% endcallout %} 


Finally we need to write a builder function for the $B_s^0$ candidates taking the $J\!/\!\psi(1S)$ and $\phi(1020)$ as inputs.

```python

def make_Bs0_to_jpsiphi(m_min=5150*MeV,
                        m_max=5550*MeV,
                        min_bpvdira=-10,
                        vtx_chi2dof=20):

        pvs = make_pvs()

        jpsi = make_jpsi_to_mumu(pvs=pvs)
        phi = make_phi_to_kk(pvs=pvs)

        combination_cut = require_all(in_range(m_min, F.MASS, m_max))
        vertex_cut = require_all(F.BPVDIRA > min_bpvdira, F.CHI2DOF < vtx_chi2dof)

        return ParticleCombiner(
                Inputs=[jpsi,phi],
                DecayDescriptor="B_s0 -> J/psi(1S) phi(1020)",
                CombinationCut=combination_cut,
                CompositeCut=vertex_cut)
```

The last step is to register an HLT2 line that calls `make_Bs0_to_jpsiphi`. We will do this in separate file called `hlt2_starterkit.py`. For this we need to import the $B_s^0 \to J\!/\!\psi(1S) \phi(1020)$ builder we have developed from `starterkit_lines.py`. Furthermore we will need the `HLT2Line` algorithm to create the line object and `register_line_builder` that is used as python decorator to register the HLT2 line to a dictionary of `all_lines`. In our case it is trivial since we only have one line, but on working group level this simplifies the syntax in an elegant way. 

```python
from Hlt2Conf.lines.StarterkitExamples import starterkit_lines
from Moore.config import Hlt2Line, register_line_builder


all_lines = {}

@register_line_builder(all_lines)
def BsToJpsiPhi_line(name='Hlt2_BsToJpsiPhi_StarterkitLine',
                     prescale=1,
                     persistreco=False):
    
    line_alg = starterkit_lines.make_Bs0_to_jpsiphi()
    return Hlt2Line(
        name=name,
        prescale=prescale,
        algs=line_alg,
        persistreco=persistreco)
```

We will also need to call the 
`upfront_reconstruction` algorithm that is telling the HLT2 line that the reconstruction has to be run.
In addition commonly used builders and filters there can also be working group specific prefilters to events before the actual line is build. These can be passed as a list object to the `HLT2Line` via the `algs` argument. Such prefilters can for example require that the event has at least one reconstructed PV. We can also place the `upfront_reconstruction` into the prefilters.


```python
from RecoConf.reconstruction_objects import upfront_reconstruction
from RecoConf.reconstruction_objects import make_pvs_v2 as make_pvs
```

```python
@register_line_builder(all_lines)
def BsToJpsiPhi_line(name='Hlt2_BsToJpsiPhi_StarterkitLine',
                     prescale=1,
                     persistreco=False):
    prefilters = upfront_reconstruction() + [make_pvs()]
    line_alg = starterkit_lines.make_Bs0_to_jpsiphi()
    return Hlt2Line(
        name=name,
        prescale=prescale,
        algs=prefilters+line_alg,
        persistreco=persistreco)
```

Be mindful of HLT2 lines naming conventions that might exist and always document your lines, describing lines. 
In case of doubt contact your RTA/DPA liason or a Trigger/migration coordinator. 

You can find the full definition of the `BsToJpsiPhi_Line`.

{% callout "Things to keep in mind while developing selection lines" %}

1. The **signal efficiency** is a measure of how many true signal candidates would pass you selection. 
   Naturally, you want this to be as high as possible. 
   To measure your signal efficiency you need to request an Upgrade signal Monte Carlo for your corresponding channel. 

2. The **signal Rate** is a measure of how many events (of any kind) would pass your selection.
   Rate is measure using a minimum bias Monte Carlo sample (EventType 30000000). 
   Minimum bias Monte Carlo contains all possible outcomes of the pp collisions.
   There is no hard requirement on the rate value allowed per HLT2 line. 
   However, there are constraints from the computing side on how many events we can possibly process. 
   This means that one should aim for having just enough rate for their HLT2 line. 
   In order to figure out the acceptable rate for you HLT2 line, you should first estimate the expected rate. 
   To do this you need to know the instantaneous luminosity (targeted to be $\mathcal{L}= 2 \cdot 10^{33} \text{cm}^{-2} \text{s}^{-1}$ during stable runs), 
   the production cross-section of the parent particle and branching fraction of your exclusive decay channel.

   Assuming a perfect 100% trigger efficiency one can estimate the signal rate to be O(10Hz) using this formula:

  $\text{signal rate} =  2 \cdot \sigma(pp \to bbX) \cdot f_s \cdot \mathcal{L}_\text{inst}  \cdot \text{BR} (B_s^0 \to J\!/\!\psi(1S) \phi(1020)) \cdot \text{BR}(J\!/\!\psi(1S) \to \mu^+ \mu^-) \cdot \text{BR}(\phi(1020) \to K^+K^-) \cdot \varepsilon_\text{geom} \cdot \varepsilon_\text{trig}$  

   Apart from this you have to keep in mind that the HLT2 output rate not only contains signal but also background candidates which can result in total rates quite different from the signal rate.

   Since the full menu of trigger selections is not yet finalised it is not possible to give an exact rate limit. The rule of thumb assuming a very loose selection is to not exceed $100\,\text{Hz}$ for a line going to the Turbo stream.

3. The **size of a saved event** is another extremely important factor for the computing resources distribution. 
   By default only the raw and reconstructed candidate information will be preserved if the trigger line fired. 
   An average candidate size is about 5 kB for Bs2JpsiPhi. 
   If one would want to preserve additional information on e.g. Flavour Tagging, then the size of one candidate would be about three times bigger. 
   Saving the full raw event, i.e. all the information necessary in case one needs to re-reconstruct this events, would cost one about 40 times more space. 
   Since the total bandwidth for Run 3 is and is limited to ?? and shared between estimately more than 400 exclusive lines you have to think carefully about what information is really necessary to be saved.

4. The **time it takes to process a candidate** should be reduced as much as possible.
   There are a couple of useful tips to get there:
     * When defining your decay desciptor add the rarest particles first.
        You can see the rarity of candidates by looking at the output of running your line in Moore.
     * All the cuts that you can apply at the child level, should be applied there.
     * Combinatorics should be reduced as much as possible prior the vertex fit is run.

{% endcallout %} 
## Running Moore
Now that we have developed an HLT2 line, we can run `Moore` including our line on MC samples to check the implementation and evaluate its physics and computing performance. For this we will need to define and tell `Moore` the options we want to run with. We will place these in a dedicated options file that we call `hlt2_starterkit_options.py` and place in

```bash
Moore/Hlt/Hlt2Conf/options/
```

The Moore options can look like this for a test run on a Minimum Bias sample that we retrieve from [test file database](https://gitlab.cern.ch/lhcb-datapkg/PRConfig/-/blob/master/python/PRConfig/TestFileDB.py).

```python
from Hlt2Conf.lines.StarterkitExamples.hlt2_starterkit import BsToJpsiPhi_line

from Moore import options, run_moore
from RecoConf.global_tools import stateProvider_with_simplified_geom
from RecoConf.reconstruction_objects import reconstruction

def all_lines():
        return [BsToJpsiPhi_line()]

public_tools = [stateProvider_with_simplified_geom()]

options.set_input_and_conds_from_testfiledb('Upgrade_MinBias_LDST')
options.input_raw_format = 4.3
options.evt_max = 100
options.output_file = "hlt2_starterkit.dst"
options.output_type = "ROOT"

from Moore.tcks import dump_hlt2_configuration
config = run_moore(options, all_lines, public_tools)

dump_hlt2_configuration(config, "hlt2_starterkit.tck.json")
```
With these options we can run `Moore` with the following command from the stack:

```bash
./Moore/run gaudirun.py '$MOOREROOT/tests/options/default_input_and_conds_hlt2.py' ./Moore/Hlt/Hlt2Conf/options/hlt2_starterkit_options.py
```
Since we are running on only 100 MinBias MC events, this is very unlikely to produce a Bs2JpsiPhi candidate passing our selection. To evaluate the physics performance of our line we will have to switch to a dedicated Bs2JpsiPhi signal MC sample, which can be found in the bookkeeping [here]().

### Running on signal MC

Current signal MC samples are usually in the XDIGI format, which means that there was no reconstruction applied to them, while (L)DST usually have some outdated reconstruction run on them. We will therefore need to configure the Moore options to enable a real-time reconstruction. This can be done by

```python
from RecoConf.reconstruction_objects import reconstruction

with reconstruction.bind(from_file=False):
    config = run_moore(options, all_lines, public_tools)
```

To run on XDIGI files you will answer need to change some of the `Moore` options and define the right input files and formats and add the corresponding database tags:

```python
options.input_type = "ROOT"
options.input_raw_format = 0.3

options.input_files = "<Bs2JpsiPhi.xdigi>"
options.conddb_tag = "sim-20210617-vc-md100"
options.dddb_tag = "dddb-20210617"
```

After you have successfully run Moore on the signal MC with candidates passing your selection you will find the two new files `hlt2_starterkit.dst` and `hlt2_starterkit.tck.json` where the first one contains the persisted raw and reconstructed data of the selected events in the TES format and the second the configuration of the run, that will be needed later to retrieve information about the trigger lines that were run.