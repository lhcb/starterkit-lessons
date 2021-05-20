# Advanced Dirac

At this point we have used Ganga for a number of things related to Davinci and basic file management. However, Ganga is a much more flexible tool that. Using `Python` and `IPython` we can set up more complicated workflows that manage much of your analysis for you!

{% objectives "Learning Objectives" %}

* Learn core concepts of the GangaTasks package
* Understand advanced Ganga features for job management
* Apply these to real analysis issues and define a simple workflow

{% endobjectives %}

{% callout "Pre-Requisites" %} 

This tutorial will be based on a couple of python files. Please download the following files and place them in a working directory on lxplus. [distro.py](code/advanced-dirac/distro.py), [compare.py](code/advanced-dirac/compare.py)

{% endcallout %}

The first and most important package to introduce is GangaTasks. This package is designed to stop busy analysts from spending more time managing GRID jobs than working on physics. It has the following core features.

* Automatically submit jobs and keeps a certain ammount running at all times.
* Automatically create new jobs based on previous jobs in a chain.
* Automatically resubmits jobs up to a threshold number of resubmits (default=5).

So as you can imagine, Tasks is a powerful tool...time to play with it!

Currently you should have two files saved in a test folder - `distro.py` and `compare.py`. The first of these is an extremely simple distribution generator making one of three distributions (Gaussian, Normal, Poisson). The second lets you compare datasets in  histogram. So to get these stages running automatically lets get the generator working with Tasks in a `submit.py` file:

```
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

Hence, we are defining a `Task` to generate three different distributions (gaussian, poisson, flat) over a range of events. Here we take advantage of pythons `*` operator to unpack the `range` into an array. The final thing to note is that we are calling a bash script called `my_script.sh`. We need to define this in order to run python in the appropriate environment. This takes the form:

```
!/bin/bash

#run the python script that you uploaded
lb-conda default python distro.py $MYENV $1

``` 

{% callout ".env v .arg" %} 

it is important to note that the subjob splitter and the unit splitter have the be on separate job parameters (in this case .env and .arg) Otherwise the last splitter called will overwrite the changes of the first. You will still end up with the correct ammount of jobs but they will be running the wrong parameters! To inspect what parameters your jobs are running with you can use the usual Ganga syntax of
```
jobs(job_num) 
```
to quicky inspect what was submitted.

{% endcallout %}

Run this the same way you would any ganga script using.
```
ganga -i gangaTasks.py
```

You can inspect your Task by looking at `tasks` in the ganga `IPython` window. Do not worry if nothing happens, the `tasks` monitor only refreshes every 30 real seconds! If everything has gone as intended shortly you will have some data files in your local gangadir relating to the generators. How do we add the second transform?

To add the second transform we need to append the following to the Ganga submission script before the run command:

```
# Create the second transform
trf2 = CoreTransform()
trf2.application = Executable()
trf2.application.exe = File('my_other_script.sh')
trf2.inputfiles = [LocalFile("compare.py")]
trf2.outputfiles = [LocalFile("*.png")]

#specify transform dependancies
task_chain = TaskChainInput()
task_chain.input_trf_id = trf1.getID()
trf2.addInputData(task_chain)

trf2.splitter = GangaDatasetSplitter()
trf2.splitter.files_per_subjob = 3

t.appendTransform(trf2)
```

As you can see the new additions are very similar to what we have seen before. However, there is an exception. The following code

```
#specify transform dependancies
task_chain = TaskChainInput()
task_chain.input_trf_id = trf1.getID()
trf2.addInputData(task_chain)
```

lets us chain tasks together where the ID of the first transform is a requirement to trigger the next. You can add the ID of multiple transforms to this task chain if a transform has more than one dependance. We also need to define a second `.sh` script to manage the transform. This is again a short file to ensure python can access all the necessary modules for the task.

```
#!/bin/bash

cat __GangaInputData.txt__

lb-conda default python compare.py

```

Now you can submit this task again and monitor the results. `cat __GangaInputData.txt__` is included so you can inspect which files are passed between transforms.

{% callout "Units Changing Order" %} 

You should notice when running this script that the unit `U:0` that generates the gaussian is not the unit that makes the histograms. This should be `U:2`. This is because units are tied to their Transforms so `U:0` of the second Transform is simply the second to run. For a full breakdown of what units are running each step, with what data, you can use
```
tasks(task_num).overview()
```
to see a full breakdown.
{% endcallout %}
