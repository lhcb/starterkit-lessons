# Measuring rates and efficiencies of HLT2 lines


{% objectives "Learning Objectives" %}

* Learn about `HltEfficiencyChecker`
* Configure rate measurement via a wizard
* Configure efficiency estimation via a wizard

{% endobjectives %}

In the previous lesson, we have learned how to develop an HLT2 line for the `$ B_{s}^{0} \to J/\psi \phi$` decay. 
The lesson provided some optimized cuts for you, and you just had to trust us on their efficiency.
However, when writing your own line, you will have to develop selections yourself. 
In order to fine-tune the selection, one would like to know then what would be rate and efficiency of a certain selection. 

To measure efficiency and rate, a special software package is developed, called `HltEfficiencyChecker`, which is a part of the `MooreAnalysis` project. 
The `HltEfficiencyChecker` package will run an HLT2 line over the Monte Carlo samples, save the trigger decisions and then give you an information on rate and efficiencies by running a configurable analysis script. 

## Measuring rates with `HltEfficiencyChecker`

Rate is just a number of events that a line would fire in the real data per second. 
Rate is measured on a `minimum bias` Monte Carlo sample, which is a full inclusive sample.
By definition, rate is an efficiency of the line on a `minimum bias` Monte Carlo sample multiplied by the total rate of collisions. 
If the minimum bias Monte Carlo is not HLT1 filtered, then the total rate of collisions is considered to be 30 MHz.
If the Monte Carlo is HLT1 filters, then the total rate of collisions is considered to be 1 MHz.

To measure the rates, one must configure `HltEfficiencyChecker` by providing the information about the lines, sample and level of software trigger to be run. 
The configuration can be done with `yaml` file that is passed to a wizard.
The `yaml` is a data serialization language and is widely used for configuration files. 
It is most close to `json` and python dictionaries in the structure. 


{% callout "Yaml file format" %}

Let's assume, you want to write a python dictionary in the `yaml` format: 
```python
my_dictionary = {
                'Experiment': "LHCb",
                'Year': 2022, 
                'Operating': true,
                'Subdetectors': ['Velo', 'UT', 'RICH1', 'RICH2', 
                                 'SciFi', 'ECal', 'HCal', 'Muon'],
                'Training': {
                       'Name' : 'Starterkit',
                       'Level' :  'PhD+',
                       'Established' :  2015
                       }
                }
```

To rewrite it in the `yaml` format, one should create a `my_dictionary.yaml` file, which would looks like:

```yaml
Experiment: "LHCb"
Year: 2022 
Operating: true
Subdetectors: 
    - Velo
    - UT
    - RICH1
    - RICH2
    - SciFi
    - ECal
    - HCal
    - Muon
Training: 
    Name: Starterkit
    Level: PhD+
    Established:  2015            
```

{% endcallout %}

The `yaml` format is rather strict in terms of formatting.
In case your `yaml` file does not compile, don't hesitate to use online services to cross-check the formatting: [yaml online validator](http://www.yamllint.com/).

First, check that the `MooreAnalysis` directory exists in you lhcb-strack directory:

```bash
ls MooreAnalysis
```

If you can not find the `MooreAnalysis` directory, please run in your stack directory: 
```bash
make fast/MooreAnalysis
```

Before we start, create a starterkit directory in `MooreAnalysis/HltEfficiencyChecker/options/`: 
```bash
mkdir MooreAnalysis/HltEfficiencyChecker/options/startekit
```
In the `MooreAnalysis` you can find a wizard script `MooreAnalysis/HltEfficiencyChecker/scripts/hlt_eff_checker.py`.
This wizard will generate an option file for you to run over the `Moore` output, provided you have created a configuration `yaml` file. 
The basic structure of the configuration `yaml` files consists of three parts: 
1. sample configuration : input sample definition
2. Moore job configuration : a job that runs the trigger lines over the input sample and saves trigger decisions into a ntuple
3. analysis job configuration : a job that uses trigger decisions to compute efficiency or rate

In the configuration `yaml` file for the rate computation, one has to provide the following information. 
Necessary information includes:

1. `ntuple_path`  - a path to the ntuple, where the trigger decisions will be saved.
2. `job` - descriptions of the Moore job, that includes:
    - `trigger_level` -  either 1 for HLT1 or 2 for HLT2 rates
    - `evt_max` - number of events the job is run on. In order to use the entire sample, set the `evt_max` to -1. 
    - input sample configuration: 
       - `testfiledb_key` - if using a [`TestDB`](https://gitlab.cern.ch/lhcb-datapkg/PRConfig/blob/master/python/PRConfig/TestFileDB.py) file, this is a name of the testing sample. By providing it you automatically set all the supporting sample information, like `CondDB` and `DDDB` tags. `TestDB` is a set of predefined testing samples, which are used to test LHCb software. 
       <center>OR</center>
       - `intput_files`  - an alternative to the `testfiledb_key`, where user can define manually paths to the samples as a `yaml` list. Requires `simulation` (if `simulation is `True`, then `conddb_tag`, `dddb_tag`), `input_raw_format`, `input_type`. 
       - `simulation` -  is the sample a Monte Carlo simulation. Either True or False. 
       - `conddb_tag` -  condition database tag for simulated samples
       - `dddb_tag`  - database tag for simulated samples
       - `input_type` - data file structure type. See the table below for the different file extension standards. 
    - `input_raw_format` - raw data format.  
    - `lines_from` - where should the lines be imported 
    - `run_recosntruction` - to run current HLT2 reconstruction (non-default yet). Requires `ft_deconsign_version` to be defined.  
    - `ft_decoding_version` - until the Fiber Tracker (aka SciFi) decoding version is not fixed, this has to match the version used in the simulation production.
    - `options` - a script that translates the `yaml` into the option file. Should be set to `$HLTEFFICIENCYCHECKERROOT/options/options_template.py.jinja`. 
2. `analysis` - description of an analysis job to be run after the trigger lines are run. 
    - `script` - analysis script to run. For rate calculation, use `$HLTEFFICIENCYCHECKERROOT/scripts/hlt_calculate_rates.py`. For efficiency, use `$HLTEFFICIENCYCHECKERROOT/scripts/hlt_line_efficiencies.py`.
    - `args` -  additional arguments for the analysis job: `$HLTEFFICIENCYCHECKERROOT/options/options_template.py.jinja  # first rendered with jinja2`
       - `input` - a name of the input ntuple file. To pick it up automatically the one produces in the Moore job, use `input: *NTUPLE`. 
       - `using_hlt1_filtered_MC` -  is the simulations sample on which the Moore job was run filtered with HLT1? This is important for the HLT2 rates. 
       - `json` - an output json to save the rate information. 

One can also configure additional options: 
1. In `job` :
    - `use_moore_as_hlt1` - use GPU based (Allen) or CPU based (Moore) HLT1. By default, false (i.e. Allen is used). If CPU HLT1 is used, the lines used must be specified via a separate trigger option file (example: `$HLTEFFICIENCYCHECKERROOT/options/hlt1_moore_lines_example.py`. 
    - `debug_mode` - debug level print-out.
2. In `analysis`: 
    - in `args` :
       - `lines` -  the lines to check. Note, that lines must be present in the `lines_from` path.  



{% callout "File extensions and `input_raw_format`" %}


| Syntax      | Description |
| ----------- | ----------- |
| Header      | Title       |
| Paragraph   | Text        |


Each file extension corresponds to a particular `input_raw_format`.
| Extension                                                | Purpose                                                  |`input_raw_format`                                        | `input_type`                                             | HLT1 Filtering                                           |
| -------------------------------------------------------- | -------------------------------------------------------- | -------------------------------------------------------- | -------------------------------------------------------- | -------------------------------------------------------- |
| (X)DIGI                                                  | raw information after simulation without reconstruction  | 0.5                                                      | `ROOT`                                                   |    no                                                    |
| MDF/RAW                                                  | raw information after data taking without reconstruction | 0.5                                                      | `MDF`                                                    |                                                          |
| LDST                                                     | reconstruction and selection algorithms output           | 4.3                                                      | `ROOT`                                                   |    yes                                                   |


More information on file extensions can be found [here](https://lhcbdoc.web.cern.ch/lhcbdoc/moore/master/tutorials/different_samples.html).

{% endcallout %}


Using the information above one can create `MooreAnalysis/HltEfficiencyChecker/options/startekit/hlt2_rate_example.yaml` file with:

```yaml
ntuple_path: &NTUPLE rate_ntuple.root
job:
    trigger_level: 2
    evt_max: -1
    testfiledb_key: upgrade_minbias_hlt1_filtered
    input_raw_format: 4.3
    lines_from: Hlt2Conf.lines.starterkit.hlt2_starterkit
    run_reconstruction: True
    ft_decoding_version: 2
    options:
        - $HLTEFFICIENCYCHECKERROOT/options/options_template.py.jinja  # first rendered with jinja2
analysis:
    script: $HLTEFFICIENCYCHECKERROOT/scripts/hlt_calculate_rates.py
    args:
        input: *NTUPLE
        using_hlt1_filtered_MC: True
        json: Hlt2_rates.json
```

To get the rate :


```bash
MooreAnalysis/run MooreAnalysis/HltEfficiencyChecker/scripts/hlt_eff_checker.py MooreAnalysis/HltEfficiencyChecker/options/startekit/hlt2_rate_example.yaml
```

The above command requests to run over the entire dataset and will take some time to finish.
If run on the full sample the output will be:

```bash


```




## Measuring efficiencies with `HltEfficiencyChecker`


Efficiency can be defined in two way: as a number of events that have passed the selection with respect to the expected number of events passing the selection and as a number of events that passed the selection and were truth matched 


The configuration `yaml` file must provide the following information for efficiency measurement in addition to the one provided for the rate computation:
1. `annotated_decay_descriptor` - a decay descriptor used for the truth matching. If one wants to save information on particles properties, like $p_T$ of a daughter, etc., one must provide a proxy (annotation) for such a particle to be used by the analysis script. This is done with `${prefix}` to the left from the particle in the decay descriptor. 
2. In `analysis` the `script` must be set to `$HLTEFFICIENCYCHECKERROOT/scripts/hlt_line_efficiencies.py`

In `analysis` on can provide additional `args` to get efficiencies as a function of different variables. To achieve this the following information should be provided:
   - `reconstructible_children` - a list of the proxies (annotations) from the `annotated_decay_descriptor` of the daughter particles that can be reconstructed. 
   - `legend_header` -  a latex expression for the legend header of the plots. 
   - `make_plots` - a flag to set creation of the efficiency plots. 
   - `vars` - variables efficiency can be plotted against. By default, they are associated with the mother particle. If one wants to use a variable from the daughter particles, the proxy (annotation) must be specified in the following manner: `<annotation>:<variable>`. To specify multiple variables: `<variable1>, <variable2>`.
   - `true_signal_to_match_to` -  particles that should be matched to see truth matched efficiency. 
    

   

```yaml
annotated_decay_descriptor:
    "${Bs}[B_s0 => ( J/psi(1S) => ${mup}mu+ ${mum}mu- ) ( phi(1020) => ${Kp}K+ ${Km}K- )]CC"
ntuple_path: &NTUPLE eff_ntuple.root
job:
    trigger_level: 2
    evt_max: 100
    input_type: ROOT
    lines_from: Hlt2Conf.lines.starterkit.hlt2_starterkit # NOTE only works with Hlt2 lines - you cannot specify a Hlt1 line at this time.
    input_files:
        # HLT1-filtered
        # Bs2JpsiPhi, 13144011
        # sim+std://MC/Upgrade/Beam7000GeV-Upgrade-MagDown-Nu7.6-25ns-Pythia8/Sim09c-Up02/Reco-Up01/Trig0x52000000/13144011/LDST
        - "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000001_1.ldst"
        - "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000002_1.ldst"
        - "root://eoslhcb.cern.ch//eos/lhcb/grid/prod/lhcb/MC/Upgrade/LDST/00076706/0000/00076706_00000003_1.ldst"
    simulation: True
    conddb_tag: 'sim-20171127-vc-md100'
    dddb_tag: 'dddb-20171126'
    input_raw_format: 4.3
    ft_decoding_version: 2
    run_reconstruction: True
    options:
        - $HLTEFFICIENCYCHECKERROOT/options/options_template.py.jinja  # first rendered with jinja2
analysis:
    script: $HLTEFFICIENCYCHECKERROOT/scripts/hlt_line_efficiencies.py
    args:
        input: *NTUPLE
        reconstructible_children: mup,mum,Kp,Km #Note absence of the spaces
        legend_header: "B^{0}_{s} #rightarrow J/#Psi#phi"
        true_signal_to_match_to: "Bs"
        make_plots: true
```





