# Analysis Productions

{% objectives "Learning Objectives" %}

* Understand the reasons to use Analysis Productions
* Look at some pre-existing productions
* Create your own production

{% endobjectives %} 

Running DaVinci locally can be great for testing an options file, but is rarely appropriate for creating the full set of ROOT ntuples needed for an analysis. Historically, people would use the grid: one can send off a large number of DaVinci jobs to be handled by batch computing resources (in certain cases, this is still useful - see the [next lesson](davinci-grid) for how to do this). However, this still has some drawbacks, in particular:

* Large datasets (especially in run 3) would require a long period of monitoring your grid jobs
* Computing resources can be wasted if multiple analyses are independently producing similar ntuples
* Ntuples can be lost, or removed when analysists leave, which can be an issue for [analysis preservation](https://twiki.cern.ch/twiki/bin/view/LHCb/AnalysisPreservationReproducibility)

It is the goal of [Analysis Productions](https://gitlab.cern.ch/lhcb-datapkg/AnalysisProductions/-/blob/master/README.md) to centralise and automate much of the process of making ntuples, and to keep a record of how datasets were produced. Moving into run 3, this will usually be the preferred way to create ntuples for your analysis.

## Monitoring productions

Before we get into the how-to, let's first take a look at the end result of a real production. Open the [web-based book keeping](https://lhcb-portal-dirac.cern.ch/DIRAC/), and navigate to `Applications/Analysis Productions`. You should see a new menu appear, showing a list of working groups ('B2CC', 'SL', etc.). Under `SL` (the semi-leptonic working group), navigate to `AnaProd-RDs/v0r0p1735460`.

Each of these rows corresponds to one batch of jobs, and displays:

* Its unique ID number of that batch of jobs (left, eg. `69615`)
* Its name (centre, eg. `MC_13266069_2018_MagUp`)
* Its status (right)

To view more information, you can click on any one of these.

**TODO**: view the options file(s) used via the 'View on GitLab' button

**TODO**: find & view the `.root` files using the output pattern

## Creating your own production

For practice, we will now go through the first steps of creating a production. It should be noted that this process may change slightly, and the most up-to-date information is to be found on the well-documented [Analysis Productions GitLab page](https://gitlab.cern.ch/lhcb-datapkg/AnalysisProductions).

**TODO: Steps**

**Clone the repo**

**Create the right folders**

**Add the testing options file from before**

**Set up the .yaml file**

**Run the testing script**

**What you would do next if this was a real production (creating a merge request, reporting to your WG, etc.)**
