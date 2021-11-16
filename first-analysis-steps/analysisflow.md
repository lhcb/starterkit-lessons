# The analysis flow and analysis preservation

{% objectives "Learning Objectives" %}

* Learn about analysis tools
* Learn about analysis preservation 

{% endobjectives %}

Most LHCb data analyses share similar structure and follow similar procedure pipeline.
As a user you start developing your analysis from retrieving the data and simulation samples that you need for your research.   
Giving you tools to get those samples is the main goal of the first analysis steps lessons.  

{% callout "Reminder from the data flow lesson" %}

The output of the last stages of the [LHCb data flow](dataflow), namely stripping (Run 1 and Run 2), Turbo stream (Run 2), sprucing (Run 3) and Run 3 HLT2 lines can be accessed by user. 
Selections made at these last stages are also defined by users. 
Their output is saved to disk and later is used to produce a data file that will contain only the information that interests users.
This is done using the software package called DaVinci.

{% endcallout %}

### Getting data files

After preselecting data either in the stripping, sprucing or triggering step, user can produce a data file, called ntuple by running DaVinci package.
The ntuple is a data file that contains information about events or candidates in the data sample, like mass of a candidate or trigger decision flags.
For the data sample information is coming from reconstruction, but for the simulation sample you can request both the reconstructed values of observables/variables or generator-level values.
The reconstructed values are the ones that passed the entire reconstruction pipeline with detector effects included and generator-level values are the original values that the event generator has produced.
This allows to study different reconstruction and detector effects for your analysis.

DaVinci can be run using CERN grid computing system directly via ganga or via Analysis Productions. 
Using ganga will give you more control over the jobs (little scrips and macros you run on the remote machines) and also will allow you to do way more than just running DaVinci. 
Analysis Productions, however, are more user-friendly and provide
Analysis Productions are also extremely important for the preservation of your analysis.
We will discuss the concept of analysis preservation a bit later in this lesson.
Unless you want to run some really unusual ntuple jobs on ganga, you should always go for Analysis Productions. 

In first analysis steps we cover both running DaVinci on [ganga](https://lhcb.github.io/starterkit-lessons/first-analysis-steps/davinci-grid.html) and via [Analysis Productions](https://lhcb.github.io/starterkit-lessons/first-analysis-steps/analysis-productions.html).


### Useful high energy physics analysis tools

After getting the ntuples a user usually develops new analysis code or expands an existing code, that their collaborators use. 
Analysis code is usually based on the popular high-energy physics software tools or on the more general data analysis tools, like [numpy](https://numpy.org/) or [pandas](https://pandas.pydata.org/). 

{% callout "Popular data analysis tools in HEP" %}

There are multiple packages that collaboratos use in LHCb.
Some of this packages are CERN supported, some are projects from the LHCb collaborators and some are supported by external community.  

* [ROOT](https://root.cern/) and it's python twin sister pyROOT are open-source data analysis frameworks used extensively in HEP community. 
* [scikit-HEP project](https://github.com/scikit-hep) a metaproject for python based particle physics analysis. Consists of multiple useful python modules, some of them are listed here:
   - [uproot](https://github.com/scikit-hep/uproot4) a python module that allows to read the root files (that are the output of the DaVinci package). 
   - [awkward arrays](https://github.com/scikit-hep/awkward-1.0) a python module for the numpy-like particle physics friendly array, that supports nested, variable-sized data arrays.
   - [boost histogram](https://github.com/scikit-hep/boost-histogram) a python version of the C++14 library [Boost::Histogram](https://www.boost.org/doc/libs/1_77_0/libs/histogram/doc/html/index.html) for histogramming. 
   - [iminuit](https://github.com/scikit-hep/iminuit) a jupyter-friendly python interface for minimization package [Minuit2](https://root.cern.ch/doc/master/Minuit2Page.html).
   - [mplhep](https://github.com/scikit-hep/mplhep) a set of high energy physics helpers for [matplotlib](https://matplotlib.org/).
   - etc.
* [RooFit](https://root.cern/manual/roofit/) a fitting, fit model defining, plotting and "toy model" generation package supported by CERN ROOT team. Can be used in both C++ and python.
* [zFit](https://github.com/zfit/zfit) a fitting and fit model defining python module integrated with [scikit-HEP project](https://github.com/scikit-hep) ecosystem.

{% endcallout %}

Discussions on the new analysis tools that might be useful for the LHCb community are held in the [work package 4](https://lhcb-dpa.web.cern.ch/lhcb-dpa/wp4/index.html) of the Data Processing and Analysis project (DPA). 

### Analysis Preservation

After one gets the necessary samples and writes the necessary macros and scripts to perform the analysis steps, like applying additional selections, fitting distributions, computing efficiencies and acceptances, etc. 
After acquiring results, one has to have their work being review by physics working group (PWG) and then by collaboration. 

One of the requirements for the successful review is that your analysis is fully preserved.  
Scientific integrity means having transparency on each step of the research as well as reproducibility of the results.
Both LHCb and CERN have guidelines which one should always follow when collecting research usign LHCb or CERN facilities.
LHCb guidelines are discussed and implemented in the [work package 6](https://lhcb-dpa.web.cern.ch/lhcb-dpa/wp6/index.html) of DPA.
CERN guideline can be found [here](https://analysispreservation.cern.ch/login?next=/).

First step is to document well-used procedures, techniques and meticulously report the results of the analysis.
This is done by writing a clear and extensive Analysis Note, that is then available to the collaborators.

Secondly, all your code should have a [gitlab](https://about.gitlab.com/) repository.
Gitlab is useful for tracing your own changes and changes of your collaborators too, as well as, tracing possible bugs with gitlab-CI.
A lesson on gitlab can be found [here](https://hsf-training.github.io/analysis-essentials/git/README.html).

However, it is not enough to just submit your code. 
It is important to write a set of instructions to execute the steps in the correct sequence.
You will quickly find yourself, that remembering how to run your own code even a couple of month after it was used for the last time can be really hard. 
Instructions can be written using [snakemake](https://snakemake.readthedocs.io/en/stable/) and supportive bash scripts.

Snakemake workflow management system allows you to write a pipeline for analysis that will automatically detect the hierarchy of the steps and changes in inputs (both samples and code). 
One should aim to start implementing the snakemake workflow from the beginning of analysis development.
Your snakemake workflow then should be committed to the gitlab repository with the final version of the code.

The samples you have produced should also be saved.
Analysis Productions help you to save a tested version of the DaVinci option files, that will help you to reproduce the ntuples with the same software version, that was specified in the original option files. 

The resulting samples have to be uploaded to your working group eos. 

You should always preserve the package versions of the sofrware that you have being using.
Even if it is a commonly used tool like ROOT or numpy.
This can be done by creating a [docker container](https://www.docker.com/), [miniconda enviroment](https://docs.conda.io/en/latest/miniconda.html) or using the [`lb-conda` environment](https://gitlab.cern.ch/lhcb-core/lbcondawrappers/-/blob/master/README.md).
The latter uses the conda environments installed on [Cern virtual machine file system](https://cernvm.cern.ch/fs/) or CVMFS and is supported by LHCb community. 

