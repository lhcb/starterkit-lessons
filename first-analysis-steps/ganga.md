# Ganga and Interfacing with DIRAC

## Running DaVinci on the grid

{% objectives "Learning Objectives" %}

* Create a ganga job
* Submit a ganga job
* Waiting for ganga
* Find the job output

{% endobjectives %} 

In certain cases, submitting an [Analysis Production](analysis-productions) may not be the best option.
This lesson will teach you how to take our [minimal DaVinci
job](minimal-dv-job) and run it on the grid yourself.

`ganga` is a program which you can use to interact with your grid
jobs. 

Before creating your first `ganga` job, open the script `ntuple_options.py`, obtained in an [earlier lesson](minimal-dv-job), and comment out the lines taking the local input data: we will now use the data stored on grid.

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


{% callout "Help in Ganga" %}

As with any python session you can get help on any object or method with `help`. For example `help(Job)` gives you all the information about the Ganga `Job` object.

{% endcallout %}

To create your first `ganga` job, type the following:

```python
j = Job(name='First ganga job')
myApp = prepareGaudiExec('DaVinci','v45r1', myPath='.')
j.application = myApp
j.application.options = ['ntuple_options.py']
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
processing different files simultaneously. More details about the splitter are given in the [next lesson](#SPLIT-JOBS). Note that data will be accessed using its path in the bookkeeping `bkPath`. In order to speed-up our job, only the first 2 elements (files) of `data` will be accessed; we don't need to look at much data here. 

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
print('Status of my job:', jobs(787).status)
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
print('Job output stored in:', output)
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

## Splitting a job into subjobs

{% objectives "Learning Objectives" %}

* Learn how to process many files in parallel on the grid
  by splitting a job into many subjobs

{% endobjectives %} 

In the [previous lesson](#DAVINCI-GRID), you've submitted a job to the LHC grid.
You will notice that the job will take a long time to finish.
This is because it has to process many gigabytes of data.

`ganga` provides several *splitters* that implement strategies for processing data in parallel.
The one we will use now is `SplitByFiles`, which spawns several subjobs, each of which only processes a certain number of files.

Apart from processing data faster, this will also allow you to work with datasets that are spread across several sites of the LHC grid.
This is because a job can only access datasets that are available on the site that it runs on.

To activate a splitter, assign it to the `.splitter` attribute of your job:
```python
j.splitter = SplitByFiles(filesPerJob=5)
```
Note that the specified number of files per job is only the allowed maximum.
You will often get jobs with fewer files.

{% callout "How do I choose the number of files per job?" %}

Choose fewer files per job if possible, as this will allow you to finish sooner 
and reduces the impact of jobs failing due to grid problems.
Setting `filesPerJob=5` should work well for real data, while `filesPerJob=1` should be good for signal MC.

{% endcallout %} 

{% callout "Splitter arguments" %}

The splitter has other useful arguments: 
- `maxFiles`      : the maximal total number of files. By default the splitter will run over all files in the dataset (which corresponds to the default value of -1)
- `ignoremissing` : boolean indicating whether it is appropriate to run if there are data files which are not accessible at the moment. This is important if it is necessary to make sure that the resulting ntuples correspond to the whole data/MC sample.

{% endcallout %} 

Now, when you run `j.submit()`, the job will automatically be split into several subjobs.
These can be displayed by entering

```python
jobs(787).subjobs
```
in `ganga`, where you have to replace 787 with the `id` of your main job.

You can access individual subjobs as in `jobs(787).subjobs(2)`.
For example, to resubmit a failed subjob, you would run
```python
jobs(787).subjobs(2).resubmit()
```

To access several subjobs at once, you can use the `.select` method:
```python
jobs(787).subjobs.select(status='failed').resubmit()
```
This will resubmit all failed subjobs.

If you want to do something more complex with each subjob, a regular `for`-loop can be used as well:
```python
for j in jobs(787).subjobs:
    print(j.id)
```

It's possible that some of your subjobs will be stuck in a certain state (submitting/completing/...).
If that is the case, try to reset the Dirac backend:
```python
jobs(787).subjobs(42).backend.reset()
```
If that doesn't help, try failing the job and resubmitting:
```python
jobs(787).subjobs(42).force_status('failed')
jobs(787).subjobs(42).resubmit()
```

It can take quite a while to submit all of your subjobs.
If you want to continue working in ganga while submitting jobs, you can use the `queues` feature to do just that.
Simply call `queues.add` with the `submit` function of a job without adding parentheses, like this:
```
queues.add(j.submit)
```
Ganga will then submit this job (and its subjobs) in the background.
Make sure not to close `ganga` before the submission is finished, or you will have to start submitting the rest of the jobs again later on.

{% challenge "Splitting your first job" %}

Try splitting the `ganga` job from our previous lesson with `splitByFiles=1` 
([reference code](code/split-jobs/first-job.py)) and submit it with `ganga`.

{% endchallenge %} 

## More Ganga

{% objectives "Learning Objectives" %}

* Set the input data with BKQuery
* Use LHCbDatasets
* Set the location of the output of our jobs
* Set the location of your .gangadir
* Access output stored on the grid

{% endobjectives %} 

As you already saw in the [previous lesson](#DAVINCI-GRID), the input data can be specified for your job with the BKQuery tool. The path for the data can be found using the online Dirac portal and passed to the `BKQuery` to get the dataset. For example, to run over the Stripping 21 MagUp, Semileptonic stream

```python
Ganga In [3]: j.inputdata = BKQuery('/LHCb/Collision12/Beam4000GeV-VeloClosed-MagUp/Real Data/Reco14/Stripping21r0p1a/90000000/SEMILEPTONIC.DST').getDataset()
Ganga In [4]: j.inputdata
Ganga Out [4]: 
 LHCbCompressedDataset (
  files = [ LHCbCompressedFileSet (
   lfn_prefix = '/lhcb/LHCb/Collision12/SEMILEPTONIC.DST',
   suffixes = [3716 Entries of type 'str']
 )]  ,
   persistency = None,
   depth = 0,
   XMLCatalogueSlice =    LocalFile (
     namePattern = ,
     localDir = ,
     compressed = False
   ) ,
   credential_requirements =    DiracProxy (
     group = lhcb_user,
     encodeDefaultProxyFileName = False,
     dirac_env = None,
     validTime = None
   )
 )
```
This is a container acting as a list of `DiracFile`s, the Ganga object for files stored on the grid. Each of these `DiracFile` objects are accessible by using the brackets, as if we try to access an element from a list. This we can then use to access one of the files locally via the `accessURL` method:
```python
Ganga In [5]: j.inputdata[0].accessURL()
Ganga Out [5]: ['root://bw32-4.grid.sara.nl:1094/pnfs/grid.sara.nl/data/lhcb/LHCb/Collision12/SEMILEPTONIC.DST/00051179/0000/00051179_00006978_1.semileptonic.dst']
```
The returned path can be used by Bender to explore the contents of the DST, as in the [Interactively exploring a DST](interactive-dst) lesson.

In the previous lesson we looked at the location of the ouput with `job(782).outputdir`. This location points us to the `gangadir` where ganga stores information about the jobs and their output. If we have lots of jobs with large files, the file system where the gangadir is located will quickly fill up.

{% callout "Setting the gangadir location" %}

The location of the `gangadir` can be changed in the configuration file 
'~/.gangarc'. Just search for the `gangadir` attribute and change it to where 
you like (on the CERN AFS the `work` area is a popular choice).

It is not recommended to have your `gangadir` on the FUSE mounted `EOS` area on lxplus. The connection may be slow and unreliable which will cause problems when running ganga.

{% endcallout %} 

To avoid filling up the filespace, it is wise to put the large files produced by your job somewhere with lots of storage - the grid. You can do so by setting the `outputfiles` attribute:

```python
j.outputfiles = [DiracFile('*.root'), LocalFile('stdout')]
```
The `DiracFile` will be stored in your user area on the grid (with up to 2TB personal capacity). The wildcard means that any root file produced by your job will stay on the grid. `LocalFile` downloads the file to your `gangadir`, in this case the one called `stdout`.
You can access your files stored on the grid with the `accessURL()` function as before. 
For example, to access the location of the output `.root` file of a specific subjob, one can use 

```python
jobs(787).subjobs(15).outputfiles[0].accessURL()
```
This is a very useful feature: you do not have to download your files from the grid in order to merge them locally! This takes a lot of time and disk space.
Instead, one can get a list of URLs from each subjob, and pass them to the `TChain` or use the `hadd` method of ROOT[^1]. 
Ganga has a shortcut to access the list of all `.root` files from all the subjobs of a given job:

```python
jobs(787).backend.getOutputDataAccessURLs()
```

Small files are downloaded as standard: `.root`, logfiles etc. Files that are expected to be large (with extensions `.dst` etc) are by default kept on the grid as Dirac files. In general, you are encouraged to keep your large files on the grid to avoid moving large amounts of data around through your work area. 


{% callout "Getting help with ganga" %}

To find out more take a look at the [Ganga 
FAQ](https://twiki.cern.ch/twiki/bin/view/LHCb/FAQ/GangaLHCbFAQ)

{% endcallout %} 

[^1]: Merging your files with `hadd` will be significantly faster if you run it with the option telling ROOT to use the same compression level in that output file as is used for the input files. This can be done using the `-fk` option. If running on lxplus you will need to get a newer ROOT version that supports this option by using: `lb-run ROOT hadd -fk output.root input/*.root`

