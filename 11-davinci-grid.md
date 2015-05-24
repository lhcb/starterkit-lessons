---
layout: page
title: First Steps in LHCb
subtitle: Minimal DaVinci on the grid 
minutes: 10
---
> ## Learning Objectives {.objectives}
>
> * Create a ganga job
> * Submit a ganga job
> * Waiting for ganga
> * Find the job output

This lesson will teach you how to take our [minimal DaVinci
job](09-minimal-dv-job.html) and run it on the grid.

`ganga` is a program which you can use to interact with your grid
jobs. Start it with:

```bash
$ SetupProject Ganga
$ ganga
```

After `ganga` has started you will be dropped into something that
looks very much like an `ipython` session. `ganga` is built on top of
`ipython` so you can type anything that is legal `python` in addition
to some special commands provided by `ganga`.

To create your first `ganga` job type the following:

```python
j = Job(application=DaVinci(version='v36r6'))
j.backend = Dirac()
j.name = "First ganga job"
j.inputdata = j.application.readInputData('data/MC_2012_27163003_Beam4000GeV2012MagDownNu2.5Pythia8_Sim08e_Digi13_Trig0x409f0045_Reco14a_Stripping20NoPrescalingFlagged_ALLSTREAMS.DST.py')
j.application.optsfile = 'code/11-davinci-grid/ntuple_options_grid.py'
```

This will create a `Job` object that will execute `DaVinci` using a
backend called `Dirac`, which is "the grid". Instead of specifying the
files to process as part of the options file you have now tell the
`Job` about it. This means allows `ganga` to split your job up,
processing different files simultaneously. The final property to set
is the `j.application.optsfile` which specifies the options file to
use to configure the job.

Now you have created your first job, however it has not started
running yet. To submit it type `j.submit()`. Now `ganga` will do the
equivalent of `SetupProject DaVinci v36r6`, prepare your job and then
ship it off to the grid.

While it runs, let's submit an identical job via slightly different
method. Having to type in the details of each job every time you want
to run it is error prone and tedious. Instead you can place all the
lines that define a job in a file and simply run that. Quit `ganga`
by pressing Ctrl-d (Control - d). It will ask if you really want to
quit, so say Yes.

Place the following in a file called [`first-job.py`](code/11-davinci-grid/first-job.py):

```python
j = Job(application=DaVinci(version='v36r6'))
j.backend = Dirac()
j.name = "First ganga job"
j.inputdata = j.application.readInputData('data/MC_2012_27163003_Beam4000GeV2012MagDownNu2.5Pythia8_Sim08e_Digi13_Trig0x409f0045_Reco14a_Stripping20NoPrescalingFlagged_ALLSTREAMS.DST.py')
j.application.optsfile = 'code/11-davinci-grid/ntuple_options_grid.py'
j.submit()
```

Which you can execute and submit like so:

```bash
$ ganga first-job.py
```

This will print output similar to submitting the job from with in
`ganga`. Once it is done, start `ganga` again.

You can check on your jobs by typing `jobs` into a `ganga`
console. This will list all of your jobs, their status, what kind of
application they are and more.

You can get more detailed information about your job by typing
`jobs($jobid)`. Replacing `$jobid` with the `id` of the job you are
interested in. For concretness we will assume you are interested in
a job with jobid 787 in this example.

Once your job has finished its status will be `completed`. Check this
by typing `jobs` or by printing out the status of one particular job:

```python
print "Status of my job:", jobs(787).status
```

The next thing to do is to find the output of your job. Two things can
happen to files your job creates:

* They get downloaded by `ganga`, or
* they are stored "on the grid".

By default `ganga` will download most files below a size of XX MB. The
rest will remain on the grid. Log files will almost always be downloaded.

To find where the files `ganga` downloaded are you can check the `outputdir`
property of your job.

```python
output = job(787).outputdir
print "Job output stored in:", output
```

Take a look at the contents of this directory.

> ## Using the Shell from IPython {.callout}
>
> IPython lets you execute shell commands from within the `ganga` session.
> This means you can list the contents of a directory without leaving ganga
> by typing `!ls /tmp/`. This will list the contents of the `/tmp` directory.
> In our case we can use this to list the contents of the job output directory
> with `!ls $output` as we stored the path in the variable `output`.
