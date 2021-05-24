# Advanced Dirac

At this point we have used Ganga for a number of things related to DaVinci and basic file management. However, Ganga is a much more flexible tool than that. Using `Python` and `IPython` we can set up more complicated workflows that manage much of your analysis for you!

{% objectives "Learning Objectives" %}

* Learn core concepts of the GangaTasks package.
* Understand advanced Ganga features for job management.
* Apply these to real analysis issues and define a simple workflow.

{% endobjectives %}

{% callout "Pre-Requisites" %} 

This tutorial will be based on a couple of python files. Please download the following files and place them in a working directory on lxplus. [distro.py](code/advanced-dirac/distro.py), [compare.py](code/advanced-dirac/compare.py)

{% endcallout %}

### GangaTasks

The first and most important package to introduce is GangaTasks. This package is designed to stop busy analysts from spending more time managing GRID jobs than working on physics. It has the following core features.

* Automatically submits jobs and keeps a certain ammount running at all times.
* Automatically creates new jobs based on previous jobs in a chain.
* Automatically resubmits jobs up to a threshold number of resubmits (default=5).

So as you can imagine, Tasks is a powerful tool...time to play with it!

{% callout "Caveat" %} 

For Tasks to automatically resubmit and manage your job flow you need to have an active Ganga session open. This can be easily done via `tmux` and `screens` as to not disturb your local working.

{% endcallout %}

Currently you should have two files saved in a test folder - `distro.py` and `compare.py`. The first of these is an extremely simple distribution generator making one of three distributions (Gaussian, Normal, Poisson). The second lets you compare datasets in  histogram. So to get these stages running automatically lets get the generator working with Tasks in a `submit.py` file.

```python
# First create the overall Task
t = CoreTask()

# Now create the Transform
trf1 = CoreTransform()
trf1.application = Executable()
trf1.application.exe = File('my_script.sh')
trf1.inputfiles = [LocalFile("distro.py")]
trf1.outputfiles = [LocalFile("*.txt")]
trf1.backend = Local()

# change the environment  object for each Unit/Master Job
trf1.unit_splitter = GenericSplitter()
trf1.unit_splitter.attribute = "application.env"
trf1.unit_splitter.values = [{'MYENV':'slow_gaussian'}, {'MYENV':'poisson'}, {'MYENV':'flat'}]

#change the argument object for each subjob in the Unit
trf1.splitter = GenericSplitter()
trf1.splitter.attribute = 'application.args'
trf1.splitter.values = [*range(100,1000,100)]

# Append the transform
t.appendTransform(trf1)

# set the maximum number of active jobs to have running (allows for throttling)
t.float = 100

# run the Task
t.run()
```

There is a lot happening here so we can break it down. `t` is our `CoreTask()` object that will manage all our individual processes. Each distinct process is called a `Transform` and these can be further broken down into `Units` that are generally general configurations of `Transforms` and `subjobs` that are unique.

Hence, we are defining a `Task` to generate three different distributions (gaussian, poisson, flat) over a range of events. Here we take advantage of pythons `*` operator to unpack the `range` into an array. The final thing to note is that we are calling a bash script called `my_script.sh`. We need to define this in order to run python in the appropriate environment. This takes the form.

```
!/bin/bash

#run the python script that you uploaded
lb-conda default python distro.py $MYENV $1

``` 

{% callout ".env v .arg" %} 

It is important to note that the subjob splitter and the Unit splitter have to be on separate job parameters (in this case .env and .arg). Otherwise, the last splitter called will overwrite the changes of the first. You will still end up with the correct ammount of jobs but they will be running the wrong parameters! To inspect what parameters your jobs are running with you can use the usual Ganga syntax of
```
jobs(job_num) 
```
to quicky inspect what was submitted.

{% endcallout %}

Run this the same way you would any Ganga submission script using.
```
ganga -i submit.py
```

You can inspect your Task by looking at `tasks` in the ganga `IPython` window. Do not worry if nothing happens, the `tasks` monitor only refreshes every 30 real seconds! If everything has gone as intended shortly you will have some data files in your local gangadir relating to the generators. How do we add the second transform?

To add the second transform we need to append the following to the Ganga submission script before the run command.

```python
# Create the second transform
trf2 = CoreTransform()
trf2.application = Executable()
trf2.application.exe = File('my_other_script.sh')
trf2.inputfiles = [LocalFile("compare.py")]
trf2.outputfiles = [LocalFile("*.png")]

#specify transform dependencies
task_chain = TaskChainInput()
task_chain.input_trf_id = trf1.getID()
trf2.addInputData(task_chain)

trf2.splitter = GangaDatasetSplitter()
trf2.splitter.files_per_subjob = 3

t.appendTransform(trf2)
```

As you can see the new additions are very similar to what we have seen before. However, there are a couple of exceptions. The following code

```python
#specify transform dependencies
task_chain = TaskChainInput()
task_chain.input_trf_id = trf1.getID()
trf2.addInputData(task_chain)
```

lets us chain tasks together where the ID of the first Transform is a requirement to trigger the next. You can add the ID of multiple Transforms to this task chain if a Transform has more than one dependance. We also introduce the `GangaDatasetSplitter()`. This is a generic splitter that splits apart a job based on a list of filenames - in this case stored in `__GangaInputData__`. For data stored on the GRID it is preferable to use `SplitByFiles()`. This splitter object also carries your GRID proxy and several other utilites such as flags to `ignoremissing` that an analyst can use to improve the stability of dependant Transforms. The most common usage for this splitter is in DaVinci jobs as we will see later. 

We also need to define a second `.sh` script to manage the Transform. This is again a short file to ensure python can access all the necessary modules for the task.

```
#!/bin/bash

cat __GangaInputData.txt__

lb-conda default python compare.py

```

Now you can submit this task again and monitor the results. `cat __GangaInputData.txt__` is included so you can inspect which files are passed between Transforms.

{% callout "Units Changing Order" %} 

You should notice when running this script that the Unit `U:0` that generates the gaussian distribution is not the Unit that makes the histograms. This should be `U:2`. This is because Units are tied to their Transforms so `U:0` of the second Transform is simply the first Unit of that Transform to run. For a full breakdown of what Units are running each step, with what data, you can use

```
tasks(task_num).overview()
```

{% endcallout %}

### Alternative Backends - DIRAC (Python [bugged](https://github.com/ganga-devs/ganga/pull/1896))

So far we have only run Tasks on the `Localhost`. Naturally this will not be appropriate for many of the jobs you will need to do. So firstly lets get our python scripts running on `DIRAC` rather than `Localhost`. First we need to ensure that our `DIRAC` submission can access lb-conda. This is done using `Tags` which allow us to configure the behind the scenes of `DIRAC`. As such we need to add the following snippet to our code.

```python
trf1.backend = Dirac()
trf1.backend.diracOpts = '[j.setTag(["/cvmfs/lhcbdev.cern.ch/"])]'
```

This is because when the Transform generates a `DIRAC` job it creates a`job()` object called `j` for each Unit. Further to this we need to include the following in the relevant `.sh` executable.

```
source /cvmfs/lhcb.cern.ch/lib/LbEnv
```

Since any sites that are not at CERN will not source this by default.

### Alternative Backends - DIRAC (DaVinci)

As you can also imagine it is useful to be able to include DaVinci jobs as Transforms in certain analysis chains. As mentioned earlier Transforms have the following advantages over traditional jobs.

* Tasks will resubmit failed subjobs automatically.
* You can chain your Tuples into other Transforms before downloading them.

However, Transforms comes with a couple of caveats to achieve this. The first is that you should use a pre-built version of `DaVinci`. You cannot use `prepareGaudiExec()` as this will be called for each Unit you have running DaVinci and fail. Similarly, you should ensure that the Transform platform `trf.application.platform` matches your build. An example of a DaVinci implementation is shown below.

```python
trf1 = CoreTransform()
trf1.application = GaudiExec()
trf1.application.directory = "./DaVinciDev"
trf1.application.platform = "x86_64-centos7-gcc8-opt"
trf1.application.options = [options]
trf1.outputfiles = [DiracFile('*root')]
data = BKQuery(bkPath, dqflag="All").getDataset()
trf1.addInputData(data)
trf1.submit_with_threads = True
trf1.splitter = SplitByFiles(filesPerJob=5)
trf1.backend = Dirac()
```

For more details of how to prepare DaVinci jobs for GRID submission please refer to the [Running DaVinci on the GRID](../first-analysis-steps/davinci-grid.md) lesson.

### Alternative Backends - Condor

Transforms can also be set to run on the Condor backend. For those of you familiar with Condor you should recognise the `requirements` object that allows you to set requirements for host selection. These include `opsys`, `arch`, `memory` and others and can be inspected directly through the `IPython` interface. Changes to the choice of Condor universe can also be made by directly by changing the contents of `backend.universe`. An example of using the Condor backend is as follows.

```python
trf1.backend = Condor()
trf1.backend.getenv = "True"  # send the environment to the host
trf1.backend.requirements.memory = 1200
``` 

{% callout "Learning More?" %} 

At this point you should be a confident using the Ganga `IPython` shell to submit more advanced jobs. To learn more about other features and classes available to you please refer to the [Ganga Documentation](https://ganga.readthedocs.io/en/latest/UserGuide/index.html).

{% endcallout %}
