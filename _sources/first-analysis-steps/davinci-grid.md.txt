# Running DaVinci on the grid

{% objectives "Learning Objectives" %}

* Create a ganga job
* Submit a ganga job
* Waiting for ganga
* Find the job output

{% endobjectives %} 

This lesson will teach you how to take our [minimal DaVinci
job](minimal-dv-job) and run it on the grid.

`ganga` is a program which you can use to interact with your grid
jobs. 

Before creating your first `ganga` job, open the script `ntuple-options.py`, obtained in the [previous lesson](minimal-dv-job), and comment out the lines taking the local input data: we will now use the data stored on grid.

Also, you need to know the path to your data from Bookkeeping.
In our case the path is:
```
/MC/2016/Beam6500GeV-2016-MagDown-Nu1.6-25ns-Pythia8/Sim09c/Trig0x6138160F/Reco16/Turbo03/Stripping28r1NoPrescalingFlagged/27163002/ALLSTREAMS.DST
```
Note, that here the event type number should be located at the end of the path, which is not the case if you browse the bookkeeping by `Event type`.

Finally, launch your grid proxy typing `lhcb-proxy-init` and enter your *grid certificate* password. 

Great! Now you are ready to start ganga! Do it with:

```bash
$ ganga
```

After `ganga` has started you will be dropped into something that
looks very much like an `ipython` session. `ganga` is built on top of
`ipython` so you can type anything that is legal `python` in addition
to some special commands provided by `ganga`.

To create your first `ganga` job, type the following:

```python
j = Job(name='First ganga job')
myApp = prepareGaudiExec('DaVinci','v45r1', myPath='.')
j.application = myApp
j.application.options = ['ntuple-options.py']
j.application.platform = 'x86_64-centos7-gcc8-opt'
bkPath = '/MC/2016/Beam6500GeV-2016-MagDown-Nu1.6-25ns-Pythia8/Sim09c/Trig0x6138160F/Reco16/Turbo03/Stripping28r1NoPrescalingFlagged/27163002/ALLSTREAMS.DST'
data  = BKQuery(bkPath, dqflag=['OK']).getDataset()
j.inputdata = data[0:2]     # access only the first 2 files of data
j.backend = Dirac()
j.splitter = SplitByFiles(filesPerJob=1)
j.outputfiles = [LocalFile('DVntuple.root')]
```

This will create a `Job` object that will execute `DaVinci` configured 
with the option files given in `j.application.options` using a
backend called `Dirac`, which is "the grid". Instead of specifying the
files to process as part of the options file you have now to tell the
`Job` about it. This allows `ganga` to split your job up by setting `j.splitter`,
processing different files simultaneously. More details about the splitter are given in the [next lesson](split-jobs). Note that data will be accessed using its path in the bookkeeping `bkPath`. In order to speed-up our job, only the first 2 elements (files) of `data` will be accessed; we don't need to look at much data here. 

{% callout "DaVinciDev folder" %}

When you create a job using `prepareGaudiExec('DaVinci','v45r1', myPath='.')`
you get the following message:
```
INFO     Set up App Env at: ./DaVinciDev_v45r1
```
`ganga` has created a folder with a local copy of the DaVinci v45r1 release.
The content of it will be sent to the grid to ensure your job runs with 
exactly this configuration.
We will use this folder for the following jobs and you will learn more about
this in the [Developing LHCb Software](lhcb-dev) lesson.

{% endcallout %} 

Now you have created your first job, however it has not started
running yet. To submit it type `j.submit()`. Now `ganga` will do the
equivalent of `lb-run DaVinci/v45r1`, prepare your job and then
ship it off to the grid.

{% callout "Picking up a right platform" %}

The default platform on most lxplus machines is `x86_64-centos7-gcc8-opt` with gcc compiler version 8.
However some older DaVinci version are not compiled for `x86_64-centos7-gcc8-opt`.

The list of platforms available for a certain DaVinci version (let's try the DaVinci version we are using `v45r1`), can be viewed by
```bash
$ lb-sdb-query listPlatforms DaVinci v45r1
```
The default compiler platform for GaudiExec applications is `x86_64-slc6-gcc62-opt`.
So for some DaVinci versions, including the latest `DaVinci v45r1` a few additional actions are needed to set up your ganga job properly.

When setting up your ganga job, add the following line after declaring the `j.application`:
```python
j.application.platform = 'x86_64-centos7-gcc8-opt'
```

{% endcallout %}

While it runs, let's submit an identical job via slightly different
method. Having to type in the details of each job every time you want
to run it is error prone and tedious. Instead you can place all the
lines that define a job in a file and simply run that.

Place the following in a file called [first-job.py](code/davinci-grid/first-job.py):

```python
j = Job(name='First ganga job')
myApp = GaudiExec()
myApp.directory = "./DaVinciDev_v45r1"
j.application = myApp
j.application.options = ['ntuple_options.py']
j.application.platform = 'x86_64-centos7-gcc8-opt'
bkPath = '/MC/2016/Beam6500GeV-2016-MagDown-Nu1.6-25ns-Pythia8/Sim09c/Trig0x6138160F/Reco16/Turbo03/Stripping28r1NoPrescalingFlagged/27163002/ALLSTREAMS.DST'
data  = BKQuery(bkPath, dqflag=['OK']).getDataset()
j.inputdata = data[0:2]
j.backend = Dirac()
j.splitter = SplitByFiles(filesPerJob=1)
j.outputfiles = [LocalFile('DVntuple.root')]
j.submit()
```

Which you can execute and submit like so, from within a `ganga`
session:

```python
%ganga first-job.py
```

This will print an output similar to the one you saw when submitting the job from within
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
Tip: this can be done from ganga using command `jobs(787).peek()`.

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
$ lb-run DaVinci/v45r1 $SHELL
$ root -l path/to/the/job/output
```

You need to setup `DaVinci` as we need ROOT version 6 to read the nTuple.

{% callout "Getting help with ganga" %}

To find out more take a look at the [Ganga 
FAQ](https://twiki.cern.ch/twiki/bin/view/LHCb/FAQ/GangaLHCbFAQ)

{% endcallout %} 
