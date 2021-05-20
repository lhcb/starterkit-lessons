# Advanced Dirac

At this point we have used Ganga for a number of things related toDavinci and basic file management. However, Ganga is a much more flexible tool that. Using `Python` and `IPython` we can set up more complicated workflows that manage much of your analysis for you!

{% objectives "Learning Objectives" %}

* Learn core concepts of the GangaTasks package
* Understand advanced ganga features for job management
* Apply these to real analysis issues and define a simple workflow

{% endobjectives %}

{% callout "Pre-Requisites" %} 

This tutorial will be based off a couple of python files. Please download the following files and place them in a working directory on lxplus [distro.py](code/advanced-dirac/distro.py), [compare.py](code/advanced-dirac/compare/py)

{% endcallout %}

The first and most important package to introduce is GangaTasks. This package is designed to help busy analysts from spending more time managing GRID jobs than working on physics. It has the following core features.

* Automatically submit jobs and keep a certain ammount running at alltimes.
* Automatically create new jobs based on previous jobs in a chain.
* Automatically resubmit jobs up to a threshold number of resubmits!

So as you can imagine, tasks is a powerful tool...time to play with it!

Currently you should have two files saved a test folder `distro.py` and `compare.py`. The first of these is an extremely simple pdf generator making one of three pdfs (Gaussian, Normal, Poisson). The second lets you compare datasets in  histogram. So to get these stages running automatically lets get the generator working with Tasks:

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

Hence, we are defining a `Task` to generate three different distributions (gaussian, poisson, flat) over a range of events. Here we take advantage of pythons `*` operator to unpack the `range` into an array. The final thing to note is that we are calling a bash script called `my_script.py`. We need to define this in order to run python in the appropriate environment. This takes the form:

```
!/bin/bash

#run the python script that you uploaded
lb-conda default python distro.py $MYENV $1

``` 

{% callout "Env v Arg" %} 

it is important to note that the subjob splitter and the unit splitter have the be on separate job parameters (in this case env and arg) Otherwise the last splitter called  will overwrite the changes of the first. You will end up with the correct ammount of jobs but they will be running the wrong parameters!

{% endcallout %}

Run this the same way you would any ganga script using.
```
ganga -i gangaTasks.py
```

You can inspect your task by looking at `Tasks` in the ganga IPython window. Do not worry if nothing happens the `Tasks` monitor only refreshes every 30 real seconds!


