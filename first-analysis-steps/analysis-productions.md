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

Before we get into the how-to, let's first take a look at the end result of a real production. Open the [DIRAC web interface](https://lhcb-portal-dirac.cern.ch/DIRAC/), and navigate to `Applications/Analysis Productions`. You should see a new menu appear, showing a list of working groups ('B2CC', 'SL', etc.). Under `SL` (the semi-leptonic working group), navigate to `AnaProd-RDs/v0r0p1735460`.

Each of these rows corresponds to one batch of jobs, and displays:

* Its unique ID number of that batch of jobs (left, eg. `69615`)
* Its name (centre, eg. `MC_13266069_2018_MagUp`)
* Its status (right)

To view more information, you can click on any one of these.

**TODO**: view the options file(s) used via the 'View on GitLab' button

**TODO**: find & view the `.root` files using the output pattern

## Creating your own production

For practice, we will now go through the steps of creating a production. It should be noted that this process may change slightly, and the most up-to-date information is to be found on the well-documented [Analysis Productions GitLab page](https://gitlab.cern.ch/lhcb-datapkg/AnalysisProductions).

Start by creating a folder to work in, and into it clone the [Analysis Productions repository](https://gitlab.cern.ch/lhcb-datapkg/AnalysisProductions).

```bash
git clone ssh://git@gitlab.cern.ch:7999/lhcb-datapkg/AnalysisProductions.git
cd AnalysisProductions
```

Before making any edits, you should create a branch for your changes, and switch to it:

```bash
git checkout -b ${USER}/starterkit-practice
```

Now we need to create a folder to store all the things we're going to add for our new production. For this practice production, we'll continue with the $ D^{0} \to K^{+}K^{-} $ decays used in the previous few lessons, so we should name the folder appropriately:

```bash
mkdir D02HH_Starterkit
```

Let's enter that new directory (`cd D02HH_Starterkit`), and start adding the files we'll need - first is the DaVinci options file. If you have the options file you created during the previous lessons, copy it here, and open it with your text editor of choice. The only thing you should need to change is to remove the lines about using the local input data (that's everything to do with `IOHelper()`), since we're going to be using remotely hosted data instead. If you don't have that options file available, or are having trouble, you can [use this file](code/analysis-productions/ntuple_options.py).

The other file needed is a `.yaml` file, which will be used to configure the jobs. Create a new file named `info.yaml`, and add the following to it:

```yaml
2016_MagDown_PromptMC_D02KK:
    application: DaVinci/v45r4
    wg: Charm
    inform:
        - your.email.here@cern.ch
    input:
        bk_query: "/MC/2016/Beam6500GeV-2016-MagDown-Nu1.6-25ns-Pythia8/Sim09c/Trig0x6138160F/Reco16/Turbo03/Stripping28r1NoPrescalingFlagged/27163002/ALLSTREAMS.DST"
    options:
        - ntuple_options.py
    output: CHARM_PROMPTMC_D02KK.ROOT
```

The top line defines the name of your job - you can set this to be whatever you want, but ideally it should be something clear that contains all the relevant information about that job. The lines indented beneath it set up all the options for that job. The options we're using here are:

* **application**: the version of DaVinci to use. Here we choose v45r4, the latest at the time of writing.
* **wg**: the working group this production is a part of. Since this is a $ D^{0} \to K^{+}K^{-} $ decay, we'll use `Charm`.
* **inform**: optionally, you can enter your email address to receive updates on the status of your jobs.
* **input**: the bookkeeping path of the data you're running over. This is what you located during the [bookkeeping lesson](bookkeeping).
* **options**: the list of options files to use. Behind the scenes, these will get passed to DaVinci.
* **output**: the name of the output `.root` ntuples. These will get registered in bookkeeping as well.

For a full list of the available options, and information on their allowed values, in the [AP documentation](https://gitlab.cern.ch/lhcb-datapkg/AnalysisProductions/-/blob/master/README.md).

{% challenge "Add a magnet-up job" %}

Currently, this will create ntuples for 2016 magnet-down MC data only. See if you can add to your `info.yaml` file to create a job for 2016 magnet-up data as well. *Hint:* you will need to find the location of the correct `.DST` file using bookkeeping.

{% solution "Solutions" %}

One way to do this is to copy and paste the first block, replacing `MagDown` with `MagUp` in the name, and replacing the bookkeeping path with the appropriate magnet-up one. An example of this can be found [here](code/analysis-productions/info.yaml).

For an alternate solution with less repetition, one can make use of the special job name `defaults`. This allows you to avoid duplicating options which are the same for all jobs (eg. `inform` and `wg`) - see the [AP documentation](https://gitlab.cern.ch/lhcb-datapkg/AnalysisProductions/-/blob/master/README.md) for more on this.

{% endsolution %}

{% endchallenge %}

### Testing your production locally

**TEMPORARY:** need to use the "unstable" LbEnv. `echo testing > ~/.lbenv_flavour` & re-log-in to lxplus.

Now we've got both of the files we need, we should test the production to make sure it works as expected. All of this will be done using the `lb-ap` command. Navigate up one level to the base directory of the respository (`AnalysisProductions`), and run `lb-ap`, which should display the following:

```
Usage: lb-ap [OPTIONS] COMMAND [ARGS]...

  Command line tool for the LHCb AnalysisProductions

Options:
  --version
  --help     Show this message and exit.

Commands:
  list       List the available production folders by running lb-ap list...
  render     Render the info.yaml for a given production
  validate   Validate the configuration for a given production
  test       Execute a job locally
  debug      Start an interactive session inside the job's environment
  reproduce  Reproduce an existing online test locally
  parse-log  Read a Gaudi log file and extract information
```

This command `lb-ap` will allow us to perform a number of different tests. Let's start with `lb-ap list`, which will display all of the productions. Hopefully you should see your new production (`D02HH_Starterkit`) on this list! You can also use this to list all of the jobs within a given production, by running `lb-ap list D02HH_Starterkit`. If you added a second job for magnet-up earlier, the output of this command should look like this:

```
The available jobs for D02HH_Starterkit are: 
* 2016_MagDown_PromptMC_D02KK
* 2016_MagUp_PromptMC_D02KK
```

The most important test is if the production actually runs successfully, and creates the desired ntuples - `lb-ap` can be used for this as well. To test the magnet-down job, run this command:

```bash
lb-ap test D02HH_Starterkit 2016_MagDown_PromptMC_D02KK
```

This will automatically run DaVinci, using the data and options files you specified in `info.yaml`. You should see the output from DaVinci similar to what you saw when you ran it locally, followed by a completion message similar to this:

```
Success! Output can be found in /afs/cern.ch/work/d/djwhite/StarterKit/AP_lesson_testing/AnalysisProductions/local-tests/D02HH_Starterkit-2020-09-22-19-08-55/output
```

You should find that a `local-tests` directory has been created, and inside it are a record of all the tests you've run. Navigate to the `output` folder of your test, and check what files have been created. There are assorted log files, as well as a `.ROOT` file called something like `00012345_00006789_1.CHARM_PROMPTMC_D02KK.ROOT`. Let's open this and check if everything worked correctly - run `root -l 00012345_00006789_1.CHARM_PROMPTMC_D02KK.ROOT` to open ROOT with that ntuple loaded, and view the contents by running `TBrowser b` (or your preferred way of viewing `.root` files).

### Creating a merge request

Now that we've tested all of our changes and are sure that everything's working as intended, we can prepare to submit them to the main repository by creating a merge request. Start by commiting the changes:

```bash
git add ntuple_options.py
git add info.yaml
git commit -m "Add D02KK MC production for starterkit"
```

TODO FINISH THIS - make sure to include 'starterkit' in the description so these MRs can be removed later!

**View the output of tests**

**What to do next (reporting to your WG, etc.)**

