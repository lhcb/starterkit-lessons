# Running DaVinci on the grid

{% objectives "Learning Objectives" %}
* Create a ganga job
* Submit a ganga job
* Waiting for ganga
* Find the job output
{% endobjectives %} 

This lesson will teach you how to take our [minimal DaVinci
job](minimal-dv-job.html) and run it on the grid.

`ganga` is a program which you can use to interact with your grid
jobs. Start it with:

```bash
$ ganga
```

After `ganga` has started you will be dropped into something that
looks very much like an `ipython` session. `ganga` is built on top of
`ipython` so you can type anything that is legal `python` in addition
to some special commands provided by `ganga`.

To create your first `ganga` job type the following:

```python
j = Job(name='First ganga job')
myApp = prepareGaudiExec('DaVinci','v42r6p1', myPath='.')
j.application = myApp
j.application.options = ['ntuple_options.py']
j.application.readInputData('MC_2016_27163002_Beam6500GeV2016MagDownNu1.625nsPythia8_Sim09b_Trig0x6138160F_Reco16_Turbo03_Stripping28NoPrescalingFlagged_ALLSTREAMS.DST.py')
j.backend = Dirac()
j.outputfiles = [LocalFile('DVntuple.root')]
```

This will create a `Job` object that will execute `DaVinci` configured 
with the option files given in `j.application.options` using a
backend called `Dirac`, which is "the grid". Instead of specifying the
files to process as part of the options file you have now to tell the
`Job` about it. This allows `ganga` to split your job up,
processing different files simultaneously.

{% callout "DaVinciDev folder" %}
When you create a job using `prepareGaudiExec('DaVinci','v42r6p1', myPath='.')`
you get the following message:
```
INFO     Set up App Env at: ./DaVinciDev_v42r6p1
```
`ganga` has created a folder with a local copy of the DaVinci v42r6p1 release.
The content of it will be sent to the grid to ensure your jobs runs with 
exactly this configuration.
We will use this folder for the following jobs and you will learn more about
this in the [Developing LHCb Software](lhcb-dev.html) lesson.
{% endcallout %} 

Now you have created your first job, however it has not started
running yet. To submit it type `j.submit()`. Now `ganga` will do the
equivalent of `lb-run DaVinci/v42r6p1`, prepare your job and then
ship it off to the grid.

While it runs, let's submit an identical job via slightly different
method. Having to type in the details of each job every time you want
to run it is error prone and tedious. Instead you can place all the
lines that define a job in a file and simply run that.

Place the following in a file called [`first-job.py`](code/davinci-grid/first-job.py):

```python
j = Job(name='First ganga job')
myApp = GaudiExec()
myApp.directory = "./DaVinciDev_v42r6p1"
j.application = myApp
j.application.options = ['ntuple_options.py']
j.application.readInputData('MC_2016_27163002_Beam6500GeV2016MagDownNu1.625nsPythia8_Sim09b_Trig0x6138160F_Reco16_Turbo03_Stripping28NoPrescalingFlagged_ALLSTREAMS.DST.py')
j.backend = Dirac()
j.outputfiles = [LocalFile('DVntuple.root')]
j.submit()
```

Which you can execute and submit like so, from within a `ganga`
session:

```python
%ganga first-job.py
```

This will print output similar to submitting the job from with in
`ganga`.

You can check on your jobs by typing `jobs` into a `ganga`
console. This will list all of your jobs, their status, what kind of
application they are and more.

You can get more detailed information about your job by typing
`jobs($jobid)`, replacing `$jobid` with the `id` of the job you are
interested in. For concretness we will assume you are interested in
a job with jobid 787 in this example.

Once your job has finished its status will be `completed`. Check this
by typing `jobs` or by printing out the status of one particular job:

```python
print 'Status of my job:', jobs(787).status
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
output = jobs(787).outputdir
print 'Job output stored in:', output
```

Take a look at the contents of this directory.

{% callout "Using the Shell from IPython" %}
IPython lets you execute shell commands from within the `ganga` session.
This means you can list the contents of a directory without leaving ganga
by typing `!ls /tmp/`. This will list the contents of the `/tmp` directory.
In our case we can use this to list the contents of the job output directory
with `!ls $output` as we stored the path in the variable `output`.
{% endcallout %} 

To look at the `root` file produced by the job start a new terminal, and
type:

```bash
$ lb-run DaVinci/v42r6p1 $SHELL
$ root -l path/to/the/job/output
```

You need to setup `DaVinci` as we need ROOT version 6 to read the nTuple.

{% callout "Getting help with ganga" %}
To find out more take a look at the [Ganga 
FAQ](https://twiki.cern.ch/twiki/bin/view/LHCb/FAQ/GangaLHCbFAQ)
{% endcallout %} 
