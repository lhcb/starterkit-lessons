# The analysis flow and analysis preservation

{% objectives "Learning Objectives" %}

* Learn about analysis tools
* Learn about analysis preservation 

{% endobjectives %}

Most LHCb analyses share a similar structure and follow a similar procedure pipeline.
As a user you often start developing your analysis by retrieving the data and simulation samples that you need.   
Giving you tools to get those samples is the main goal of the first analysis steps lessons.  

{% callout "Reminder from the data flow lesson" %}

The output of the last stages of the [LHCb data flow](dataflow) - Stripping (Runs 1 and 2), Turbo stream (Run 2), Sprucing (Run 3) or Run 3 HLT2 lines - can be accessed by the user. 
Selections made at these last stages are also defined by users. 
Their outputs are saved to disk and later used to produce data files that will contain only the information that interests users.
This is done using the software package called DaVinci.

{% endcallout %}

### Getting data files

<<<<<<< HEAD
After preselecting data either in the stripping, sprucing or triggering step, user can produce a ROOT file, by running the DaVinci package.
ROOT datafiles usually contain either [TTree](https://root.cern.ch/doc/master/classTTree.html) of [_ntuple_](https://root.cern.ch/doc/v608/classTNtuple.html) data structures with information saved per event or per candidate, like mass of a candidate or trigger decision flags.
For the data sample information is coming from reconstruction, but for the simulation sample you can request both the reconstructed values of observables/variables or generator-level values.
=======
After preselecting data either in the Stripping, Sprucing or triggering step, users can produce ROOT files containing _ntuples_, running the DaVinci application.
An ntuple is a (often complex) data structure typically stored within a (ROOT) file, which contains information about events or candidates in the data sample, such as the candidate mass or trigger decision flags.
For data samples the available information is solely that coming from the reconstruction, but for simulation samples you can request both the reconstructed values of observables/variables and the generator-level (truth) values.
>>>>>>> 2c99fa5e4e3cc705ddbc2f60f4be01ea4a43cbea
The reconstructed values are the ones that passed the entire reconstruction pipeline with detector effects included and generator-level values are the original values that the event generator has produced.
Certain variables, for example, vertex quality or trigger decisions, can only be accessed on the reconstruction level.
This allows to study different reconstruction and detector effects for your analysis.

DaVinci can be run locally on `lxplus` (for small tasks) or on the Grid computing system via [Ganga](davinci-grid) or via [Analysis Productions](analysis-productions). 
Using Ganga will give you more control over the jobs (short scripts and macros you run on the remote machines) and also will allow you to do way more than just running DaVinci. 
Analysis Productions, however, are more user-friendly and provide a set of tests that ensure your jobs will be run as expected with minimal errors. 
Analysis Productions can be very useful for the preservation of your analysis.
We will discuss the concept of analysis preservation a bit later in this lesson.
<<<<<<< HEAD
=======
Unless you want to run some really unusual ntuple jobs with Ganga, you should always go for Analysis Productions. 
>>>>>>> 2c99fa5e4e3cc705ddbc2f60f4be01ea4a43cbea

In first analysis steps we cover both running DaVinci on [Ganga](https://lhcb.github.io/starterkit-lessons/first-analysis-steps/davinci-grid.html) and via [Analysis Productions](https://lhcb.github.io/starterkit-lessons/first-analysis-steps/analysis-productions.html).


### Useful high energy physics analysis tools

After getting the ntuples a user usually develops new analysis code or expands an existing code, that their collaborators use. 
Analysis code is usually based on the popular high-energy physics software tools or on the more general data analysis tools, like [numpy](https://numpy.org/) or [pandas](https://pandas.pydata.org/). 

{% callout "Popular data analysis tools in HEP" %}

There are multiple packages that collaborators use in LHCb.
Some of this packages are CERN supported, some are projects from the LHCb collaborators and some are supported by external community.  
This list is by no means exhaustive, so if there are any other tools you use often, please, feel free to add them by contributing to this lesson. 

* [ROOT](https://root.cern/) and its python twin sister pyROOT are open-source data analysis frameworks used extensively in HEP community. 
* [Scikit-HEP project](https://github.com/scikit-hep), a community project for a Python-based Big Data analysis ecosystem for Particle Physics. Consists of multiple complementary and interoperable Python modules, some of them are listed here:
   - [uproot](https://github.com/scikit-hep/uproot4) a python module that allows to read the root files (that are the output of the DaVinci package). 
   - [awkward-array](https://github.com/scikit-hep/awkward-1.0) a Python module for a friendly manipulation of data structures common in Particle Physics, i.e., nested, variable-sized data, including arbitrary-length lists, records, mixed types, and missing data, using NumPy-like idioms.
   - [boost histogram](https://github.com/scikit-hep/boost-histogram) a python version of the C++14 library [Boost::Histogram](https://www.boost.org/doc/libs/1_77_0/libs/histogram/doc/html/index.html) for histogramming. 
   - [iminuit](https://github.com/scikit-hep/iminuit) a jupyter-friendly python interface for minimization package [Minuit2](https://root.cern.ch/doc/master/Minuit2Page.html).
   - [mplhep](https://github.com/scikit-hep/mplhep) a set of high energy physics helpers for [matplotlib](https://matplotlib.org/).
   - etc.
* [RooFit](https://root.cern/manual/roofit/) a fitting, fit model defining, plotting and "toy model" generation package supported by CERN ROOT team. Can be used in both C++ and python.
* [zfit](https://github.com/zfit/zfit) a fitting and fit model defining Python package well integrated withthe [Scikit-HEP project](https://github.com/scikit-hep) analysis ecosystem.


{% endcallout %}

Discussions on the new analysis tools that might be useful for the LHCb community are held in the [Work Package 4](https://lhcb-dpa.web.cern.ch/lhcb-dpa/wp4/index.html) of the Data Processing & Analysis project (DPA). 

### Analysis Preservation

When the samples are ready one can proceed with developing the necessary macros and scripts to perform the analysis steps, such as applying additional selections, fitting distributions, computing efficiencies and acceptances, etc. 
Starting from the ntuples a typical analysis will consist of the following steps: 

1. Defining and applying selections, including kinematic, particle identification, multivariate analysis, signal/background separations by fitting.
2. Computing and applying callibrations and corrections. 
Some of the calibrations, like [TrackCalib](https://gitlab.cern.ch/lhcb/Urania/tree/master/TrackCalib) and [PIDCalib2](https://gitlab.cern.ch/lhcb-rta/pidcalib2), are calibrated centrally by collaborators and can be used by the users directly. 
Other corrections, like correcting the kinematics distributions in the simulated samples to match the data ones as good as possible, are usually estimated on per analysis basis.
3. Computing efficiencies, acceptances and measuring detector resolution effects. 
4. Putting all the above together to measure the parameter(s) of interest. 

After acquiring the parameter(s) of interest, one has to have their work being reviewed by [physics working group](http://lhcb.web.cern.ch/lhcb_page/collaboration/organization/lhcb-conv/Physics_history_and_Sub-structure.html) (PWG) and then by the LHCb collaboration. 

One of the requirements for the successful review is that your analysis is fully preserved and can be reproduced by another LHCb physicist.  
Scientific integrity means having transparency on each step of the research as well as reproducibility of the results.
Both LHCb and CERN have guidelines which one should always follow when conducting research usign LHCb or CERN facilities.
LHCb guidelines are discussed and implemented in the [work package 6](https://lhcb-dpa.web.cern.ch/lhcb-dpa/wp6/index.html) of DPA.
CERN guideline can be found [here](https://analysispreservation.cern.ch/login?next=/).

The first step is to document well-used procedures, techniques and meticulously report the results of your analysis.
This is done by writing a clear and extensive Analysis Note, that is then available to the LHCb collaborators via the [CDS system](https://cds.cern.ch/collection/LHCb%20Analysis%20Notes).

Secondly, your analysis code should be put in a [GitLab](https://gitlab.cern.ch/) repository in your physics working group's area. For example, the code of all recent analyses by the QEE working group can be found [here](https://gitlab.cern.ch/LHCb-QEE).
GitLab is useful for tracing your own changes and changes of your collaborators too, as well as, tracing possible bugs with the GitLab CI.
Refer to [this lesson on GitLab](https://hsf-training.github.io/analysis-essentials/git/README.html) for more.

However, it is not enough to just submit your code. 
It is important to write a set of instructions to execute the steps in the correct sequence.
You will quickly find yourself, that remembering how to run your own code even a couple of months after it was used for the last time can be really hard. 
Instructions can be written using [snakemake](https://snakemake.readthedocs.io/en/stable/) and supportive bash scripts.

Snakemake workflow management system allows you to write a pipeline for analysis that will automatically detect the hierarchy of the steps and changes in inputs (both samples and code). 
One should aim to start implementing the snakemake workflow from the beginning of analysis development.
Your snakemake workflow should then be committed to the gitlab repository with the final version of your code.

The samples you have produced should also be saved. 
Analysis Productions help you to save a tested version of the DaVinci option files, that will help you to reproduce the ntuples with the same software version, that was specified in the original option files. 

The ntuples used for your analysis have to be uploaded to the [eos area](eos-storage) of your working group. 

You should always preserve the package versions of the sofrware that you have being using.
Even if it is a commonly used tool such as ROOT or NumPy.
This can be done by creating a [docker container](https://www.docker.com/), [conda enviroment](https://github.com/conda-forge/miniforge/) or using the [`lb-conda` environment](https://gitlab.cern.ch/lhcb-core/lbcondawrappers/-/blob/master/README.md).
The latter uses the conda environments installed on the [CERN virtual machine file system](https://cernvm.cern.ch/fs/), aka CVMFS, and is supported by the LHCb community. 

