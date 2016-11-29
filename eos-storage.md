---
layout: page
title: First steps in LHCb
subtitle: Storing large files on EOS
minutes: 10
---

> ## Learning Objectives {.objectives}
>
> * Run a ganga job which puts output onto EOS
> * Open and view the files on EOS

During a real analysis the output of your jobs will quickly grow
beyond what fits onto your AFS space. CERN provides you with 2TB of
space on a set of hard drives called the [EOS
service](http://information-technology.web.cern.ch/services/eos-service) and
a grid storage quota of 2TB.

To retrieve a job outputfile, one can use three types of files:  
- `LocalFile`: the standard one with the output file directly downloaded to
the `gangadir`.  
- `DiracFile`: the output file is stored directly on the grid and be accessed
through the XRootD protocol.  

In this lesson, we will focus on the use of `DiracFile` in `ganga` to manage big
output files.

We can reuse what has been done to run [a DaVinci job on the
grid](https://lhcb.github.io/first-analysis-steps/davinci-grid.html) and adapt
the `j.outputfiles` part.

To add the DiracFile in the configuration of the job we just need:
```python
j = Job(application=DaVinci(version='v41r2'))
j.backend = Dirac()
j.name = 'First ganga job'
j.inputdata = j.application.readInputData('data/MC_2012_27163003_Beam4000GeV2012MagDownNu2.5Pythia8_Sim08e_Digi13_Trig0x409f0045_Reco14a_Stripping20NoPrescalingFlagged_ALLSTREAMS.DST.py')
j.application.optsfile = 'code/davinci-grid/ntuple_options_grid.py'
j.outputfiles = [
    DiracFile('DVntuple.root')
]
```

When the job is completed, no output is dowloaded but some interesting
information are provided by typing `j.outputfiles[0]`:

```python
DiracFile(namePattern='DVntuple.root',
          lfn='/lhcb/user/a/another/2016_11/146255/146255492/DVntuple.root',
          localDir='/afs/cern.ch/user/a/another/gangadir/workspace/vrenaudi/LocalXML/129/output')
```
Apart from the `namePattern` which was set during the configuration of the job,
we can retrieve the `localDir` which is the path in your gangadir to the output
of the job and its `lfn` which stands for Logical File Name.

This LFN can then be given as an argument in order to download the file. But
more important in most of the cases, you don't even need to download the file
thanks to the `accessURL` function which will give you the URL of your output
file by typing `j.outputfiles[0].accessURL()`:

```python
['root://eoslhcb.cern.ch//eos/lhcb/grid/user/lhcb/user/a/another/2016_11/146255/146255492/DVntuple.root']
```

This URL can be directly used in your ROOT script as follows:  

```python
TFile::Open("root://eoslhcb.cern.ch//eos/lhcb/grid/user/lhcb/user/a/another/2016_11/146255/146255492/DVntuple.root")  
```  


> ## Deprecation of the use of MassStorageFile {.callout}
> The use of `MassStorageFile` is deprecated as it is quite sensitive to network problems when ganga is downloading the output of the job to EOS. 

`ganga` needs configuring in order to know which files to store on
EOS, as well as where on EOS to store them. Open `~/.gangarc` in your
favourite editor and look for a line that starts with
`MassStorageFile`. In `ganga` a `MassStorageFile` represents a file
stored on something like EOS or CASTOR.  Change it to look like the
following:

```python
MassStorageFile = {'fileExtensions': [''],
                   'uploadOptions': {'path': '/eos/lhcb/user/a/another',
                                     'cp_cmd': '/afs/cern.ch/project/eos/installation/lhcb/bin/eos.select cp',
                                     'ls_cmd': '/afs/cern.ch/project/eos/installation/lhcb/bin/eos.select ls',
                                     'mkdir_cmd': '/afs/cern.ch/project/eos/installation/lhcb/bin/eos.select mkdir'
                                    },
                   'backendPostprocess': {'LSF': 'WN', 'Dirac': 'client',
                                          'LCG': 'client', 'ARC': 'client', 
                                          'CREAM': 'client', 'Localhost': 'WN',
                                          'Interactive': 'client'}
                  }
```

The line should look very similar to this already, the only thing that
needs changing is the `path` entry. You should change it to your EOS home directory
which is `/eos/lhcb/user/a/another` if your username is `another`.

> ## Automatically transfer files {.callout}
>
> You can use the `fileExtensions` entry to specify a list of file
> extensions that should be transfered to EOS by default. For the
> moment leave this set to `['']`.

Related to this there is an entry for `DiracFiles`, which represent
files stored on the Grid. By default any file ending in `.dst` will
not be downloaded nor stored on EOS. It will be stored on some storage
element on the Grid. For files which you do not plan to work with
interactively and instead feed into a different grid job it makes
sense to leave them on the grid.

Note that EOS is 'Grid storage', specifically that which is located at CERN. Your personal space, at /eos/lhcb/user/<your username's first letter>/<your username>, is just another location on EOS, and happens to be different to the default location used by DIRAC for 'grid storage'.

```python
DiracFile = {'fileExtensions': ['*.dst'],
             'uploadOptions': {},
             'backendPostprocess': {'LSF': 'WN', 'Dirac': 'WN',
                                    'LCG': 'WN', 'CREAM': 'WN',
                                    'Localhost': 'WN',
                                    'Interactive': 'WN'}
            }
```

Now that `ganga` is configured we will modify the [minimal DaVinci
job](minimal-dv-job.html) to store the nTuple it produces on EOS.

Make a copy of `first-job.py` and add the following three lines
before the `j.submit()` line:

```python
f = MassStorageFile('DVntuple.root')
f.outputfilenameformat = '/starterkit/{jid}_{fname}'
j.outputfiles = [f] 
```

`ganga` uses the string you pass to a `MassStorageFile` constructor to
match which files created by your job this `MassStorageFile` object
represents. In this case only files named `DVntuple.root` will match.

The `outputfilenameformat` tells ganga where inside your EOS area to
store the file and how to name it. The full path to this particular
file will be:

```bash
/eos/lhcb/user/a/another/starterkit/{jid}_{fname}
```

> ## Special outputfileformat patterns {.callout}
>
> Your `outputfilenameformat` string can contain several special
> strings which will be replaced on a file by file basis. The special
> strings are: `{jid}`, `{sjid}` and `{fname}`. They stand for job ID,
> subjob ID, and the filename of the matched file respectively.

> ## Subjobs and outputfileformat {.callout}
>
> When using subjobs it is important to make sure you include the
> `{sjid}` pattern in your `outputfilenameformat` string, otherwise
> all the subjobs will overwrite each others output.

The final line tells your `ganga` job that the `outputfiles` of this
job that need special treatment.

One important thing to note is that `ganga` has to be running after
your job has completed to copy the files to EOS. The job can not
copy things to EOS itself. You can leave `ganga` running in a
screen session and it will copy files as they
become available.

Once your job has completed and the files have been copied to
EOS by `ganga` you can access them from your terminal by mounting
your EOS area.

> ## Important! {.callout}
> Important: mounting eos is discouraged by the LHCb data management team,
> and no support will be provided by them for problems/questions related to it.

On `lxplus` EOS is mount under /eos like this:

```bash
$ ls -al /eos/user
```

```output
total 0
drwxr-xr-x. 1 daemon root 269935142640435 Oct  6  2014 .
drwxr-xr-x. 9 root   root               0 Oct 24 11:30 ..
drwxr-xr-x. 1 daemon root               0 Dec 15  2014 0
drwxr-xr-x. 1 daemon root  15407422735850 Oct 28 10:13 a
drwxr-xr-x. 1 daemon root   4213907467001 Oct 26 16:10 b
drwxr-xr-x. 1 daemon root   4592926796516 Oct 27 21:49 c
drwxr-xr-x. 1 daemon root   3951569840852 Oct 26 08:20 d
drwxr-xr-x. 1 daemon root   3104338965290 Oct 27 14:27 e
drwxr-xr-x. 1 daemon root   2243039535809 Oct 27 13:48 f
drwxr-xr-x. 1 daemon root   5916063359888 Oct 28 10:05 g
drwxr-xr-x. 1 daemon root   2778840054706 Oct 30 19:56 h
drwxr-xr-x. 1 daemon root    442304842811 Oct 26 10:35 i
drwxr-xr-x. 1 daemon root   8360665587286 Oct 30 13:46 j
```

If you list the contents of the `/eos` directory you should see

```output
lhcb ship user
```

If you used `/eos/lhcb/user/a/another/starterkit/{jid}_{fname}` when
configuring your `MassStorageFiles` there should now be files
visible here:

```bash
$ ls /eos/lhcb/user/a/another/starterkit
```

Once you have found your file you can open it in `ROOT` like this:

```bash
$ root /eos/lhcb/user/a/another/starterkit/myfavouritefile.root
```

> ## Direct access in ROOT {.callout}
>
> You can also open ROOT files on EOS directly from your ROOT
> script with:
> ```python
> TFile::Open('root://eoslhcb.cern.ch//eos/lhcb/user/a/another/starterkit/myfavouritefile.root')
> ```
