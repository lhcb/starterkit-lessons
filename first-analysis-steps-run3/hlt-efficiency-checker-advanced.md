# Advanced `HltEfficiencyChecker` 




## How to measure efficiency for the HLT1 level

In the example above we have used an HLT1-filtered Monte Carlo sample. 
Now we will use a non-filtered Monte Carlo. 
Therefore, we change `trigger_level` from 2 to 1 and use an appropriate `testfiledb_key` for the correct input sample. 
Here one also has to specify HLT1 lines to run over the sample. 

```yaml
annotated_decay_descriptor:
    "${Bs}[B_s0 => ( J/psi(1S) => ${mup}mu+ ${mum}mu- ) ( phi(1020) => ${Kp}K+ ${Km}K- )]CC"
ntuple_path: &NTUPLE eff_ntuple.root
job:
    trigger_level: 1
    evt_max: 100
    testfiledb_key: Upgrade_BsPhiPhi_MD_FTv4_DIGI 
    options:
       - $HLTEFFICIENCYCHECKERROOT/options/options_template.py.jinja  
analysis:
    script: $HLTEFFICIENCYCHECKERROOT/scripts/hlt_line_efficiencies.py
    args:
        input: *NTUPLE
        reconstructible_children: mup,mum,Kp,Km
        legend_header: "B^{0}_{s} #rightarrow J/#Psi #phi"
        make_plots: true
        vars: "PT,Kp:PT,mu:PT"
        lines: Hlt1TwoTrackMVADecision,Hlt1TrackMVADecision
        true_signal_to_match_to: "Bs"
```



## How to measure efficiency for the HLT1 and HLT2 level

Sometimes you need to know in particular how many events would survive both HLT1 and HLT2 triggers.
To figure this out we start again with a non-HLT1 filtered Monte Carlo. 
The `trigger_level` will now be `both`.
You also now have to specify a denominator with the HLT1 lines for the HLT2 level. 
Then the HltEfficiencyChecker will define efficiency as 
`$ \varepsilon = \frac{\# Pass HLT2 and HLT1}{\# Pass HLT1} $`.

```yaml
annotated_decay_descriptor:
    "${Bs}[B_s0 => ( J/psi(1S) => ${mup}mu+ ${mum}mu- ) ( phi(1020) => ${Kp}K+ ${Km}K- )]CC"
ntuple_path: &NTUPLE eff_ntuple.root
job:
    trigger_level: both
    evt_max: 100
    testfiledb_key: upgrade_DC19_01_Bs2JPsiPhi_MD
    ft_decoding_version: 4
    lines_from: Hlt2Conf.lines.starterkit.hlt2_starterkit 
    allen_sequence: hlt1_pp_default
    run_reconstruction: true
    options:
        - $HLTEFFICIENCYCHECKERROOT/options/options_template.py.jinja  # first rendered with jinja2
analysis:
    script: $HLTEFFICIENCYCHECKERROOT/scripts/hlt_line_efficiencies.py
    args:
        input: *NTUPLE
        reconstructible_children: mup,mum,Kp,Km
        legend_header: "B^{0}_{s} #rightarrow #phi#phi"
        make_plots: true
        vars: "PT,Kp:PT"
        lines: Hlt2Topo2BodyLineDecision
        true_signal_to_match_to: "Bs"
        custom_denoms: "Hlt1trackmvas: Hlt1TrackMVADecision || Hlt1TwoTrackMVADecision" #Note the ' " '
```
## How to measure rate via a Moore option file directly

An experienced user might prefer to write the option files for the efficiency and rate measurement jobs themselves. 
First, one has to create a little option file `MooreAnalysis/HltEfficiencyChecker/options/starterkit/hlt2_lines_example.py` for Moore to know which lines should be called.

```python
from Moore import options #import an option configurable
from Hlt2Conf.lines.starterkit.hlt2_starterkit import all_lines #import a hlt2 lines dictionary from the line registration file

def make_lines(): 
    return [builder() for builder in all_lines.values()] #define a list of the line builders 

options.lines_maker = make_lines #set-up the lines list in the Moore options
```

Second, a little option file `MooreAnalysis/HltEfficiencyChecker/options/starterkit/hlt2_rate_example.py` to configure the sample:

```python
from Moore import options
from HltEfficiencyChecker.config import run_moore_with_tuples #running Moore control flow (same as run_moore()) and create tuple
from RecoConf.reconstruction_objects import reconstruction #calling the HLT2 reconstruction

options.set_input_and_conds_from_testfiledb('upgrade_minbias_hlt1_filtered') #setting the information on the sample from the test sample database (TestDB). This sets all the CondDB, DDDB, inputtype information 
options.input_raw_format = 4.3 #setup the input raw format version. Here it is LDST -> 4.3
options.evt_max = 100 #number of events to run over

options.ntuple_file = "rate_ntuple.root" #the name of the output ntuple where the rate information is stored

from RecoConf.hlt1_tracking import default_ft_decoding_version #import the configurable for the FT decoding version
default_ft_decoding_version.global_bind(value=2) #setting FT decoding version to 2

from RecoConf.global_tools import stateProvider_with_simplified_geom 
with reconstruction.bind(from_file=False): #rerun reconstruction instead of getting it from the file
    run_moore_with_tuples(
        options, False, public_tools=[stateProvider_with_simplified_geom()]) 
```

With the above files, one can run a Moore job with the following command:
```bash
MooreAnalysis/run gaudirun.py MooreAnalysis/HltEfficiencyChecker/options/hlt2_lines_example.py MooreAnalysis/HltEfficiencyChecker/options/hlt2_rate_example.py
```

Here `MooreAnalysis/run` is an executable of the `MooreAnalysis` project and `gaudirun.py` is a standardized script that sets up an event loop for you.
After running the above command, a file `rate_ntuple.root` will be created. 

The next step is to run the analysis job to compute the rates:
```bash
MooreAnalysis/run MooreAnalysis/HltEfficiencyChecker/scripts/hlt_calculate_rates.py rate_ntuple.root --using-hlt1-filtered-MC --json Hlt2_rates.json
```
Note, that one has to specify `--using-hlt1-filtered-MC` to comunicate to the analysis job that the input Monte Carlo is HLT1 filtered. 

## How to measure efficiency via a Moore option file directly

The steps for the efficiency measurement via Moore option file directly are the same as for the rate measurement.
However, one has to use a different job option file for the efficiency.
Open a file `MooreAnalysis/HltEfficiencyChecker/options/starterkit/hlt2_efficiency_example.py` with your favorite text editor.
There, you should define the following options:
```python
from Moore import options
from HltEfficiencyChecker.config import run_moore_with_tuples
from RecoConf.reconstruction_objects import reconstruction

decay = (
    "${Bs}[B_s0 => ( J/psi(1S) => ${mup}mu+ ${mum}mu- ) ( phi(1020) => ${Kp}K+ ${Km}K- )]CC"
)
options.input_files = [
    # HLT1-filtered
    # Bs2JpsiPhi, 13144011
    # sim+std://MC/Upgrade/Beam7000GeV-Upgrade-MagDown-Nu7.6-25ns-Pythia8/Sim09c-Up02/Reco-Up01/Trig0x52000000/13144011/LDST
    "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000001_1.ldst",
    "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000002_1.ldst",
    "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000003_1.ldst",
    "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000004_1.ldst",
    "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000005_1.ldst",
    "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000006_1.ldst",
    "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000007_1.ldst",
    "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000064_1.ldst",
    "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000068_1.ldst",
]
options.input_type = 'ROOT'
options.input_raw_format = 4.3
options.evt_max = 100

options.simulation = True
options.data_type = 'Upgrade'
options.dddb_tag = 'dddb-20171126'
options.conddb_tag = 'sim-20171127-vc-md100'
options.ntuple_file = "eff_ntuple.root"

# needed to run over FTv2 data
from RecoConf.hlt1_tracking import default_ft_decoding_version
default_ft_decoding_version.global_bind(value=2)

from RecoConf.global_tools import stateProvider_with_simplified_geom
with reconstruction.bind(from_file=False):
    run_moore_with_tuples(
        options, False, decay, public_tools=[stateProvider_with_simplified_geom()]) #here False refers to the no CPU HLT1 

```
Where instead of using a TestDB `options.set_input_and_conds_from_testfiledb('<name_sample>')`, we directly specified `LFNs` (paths to the files) and sample configuration :
```python
options.input_files = [
    # HLT1-filtered
    # Bs2JpsiPhi, 13144011
    # sim+std://MC/Upgrade/Beam7000GeV-Upgrade-MagDown-Nu7.6-25ns-Pythia8/Sim09c-Up02/Reco-Up01/Trig0x52000000/13144011/LDST
    "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000001_1.ldst",
    "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000002_1.ldst",
    "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000003_1.ldst",
    "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000004_1.ldst",
    "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000005_1.ldst",
    "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000006_1.ldst",
    "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000007_1.ldst",
    "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000064_1.ldst",
    "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000068_1.ldst",
]
options.input_type = 'ROOT'

options.simulation = True
options.data_type = 'Upgrade'
options.dddb_tag = 'dddb-20171126'
options.conddb_tag = 'sim-20171127-vc-md100'
```

[comment]: <> (TO DO : Add those samples to TestDB )

Using the options file, that defines the lines to be tested from the rate example `MooreAnalysis/HltEfficiencyChecker/options/starterkit/hlt2_lines_example.py`

With above files one can run a Moore job with the following command:
```bash
MooreAnalysis/run gaudirun.py MooreAnalysis/HltEfficiencyChecker/options/hlt2_lines_example.py MooreAnalysis/HltEfficiencyChecker/options/hlt2_eff_example.py
```

The next step is to run the analysis job to compute the efficiencies:
```bash
MooreAnalysis/run MooreAnalysis/HltEfficiencyChecker/scripts/hlt_line_efficiencies.py eff_ntuple.root --reconstructible-children=Kp,Km,mup,mum --true-signal-to-match-to Bs --legend-header="B_{s} #rightarrow J/#psi #phi" --make-plots
```

You can recognize here some of the `args` from the `yaml` configuration file. 
For example `--reconstructible-children` takes a list of the reconstructable daighters, `--true-signal-to-match-to` gives a way to match the Monte Carlo, `--legend-header` gives a legend header and `--make-plots` to create the efficiency plots.




