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
service](http://information-technology.web.cern.ch/services/eos-service).

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
job](08-minimal-dv-job.html) to store the nTuple it produces on EOS.

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
[screen session](02-screen.html) and it will copy files as they
become available.

Once your job has completed and the files have been copied to
EOS by `ganga` you can access them from your terminal by mounting
your EOS area.

On `lxplus` you can mount EOS like this:

```bash
$ mkdir ~/eos
$ eosmount eos
```

```output
warning: assuming you gave a relative path with respect to current working directory => mountpoint=eos
OK
===> Mountpoint   : /afs/cern.ch/user/a/another/eos
===> Fuse-Options : kernel_cache,attr_timeout=30,entry_timeout=30,max_readahead=131072,max_write=4194304,fsname=eoslhcb.cern.ch root://eoslhcb.cern.ch//eos/
===> xrootd ra             : 131072
===> xrootd cache          : 393216
===> fuse debug            : 0
===> fuse write-cache      : 1
===> fuse write-cache-size : 100000000
```

The first line creates a new directory in your home area, and the second line actually mounts
EOS there. If you list the contents of your `~/eos` directory you should see

```output
lhcb ship
```

If you used `/eos/lhcb/user/a/another/starterkit/{jid}_{fname}` when
configuring your `MassStorageFiles` there should now be files
visible here:

```bash
$ ls eos/lhcb/user/a/another/starterkit
```

Once you have found your file you can open it in `ROOT` like this:

```bash
$ root ~/eos/lhcb/user/a/another/starterkit/myfavouritefile.root
```

Before disconnecting from `lxplus` it is good practice to unmount
your EOS directory with `eosumount ~/eos`.

> ## Direct access in ROOT {.callout}
>
> You can also open ROOT files on EOS directly from your ROOT
> script with:
> ```python
> TFile::Open('root://eoslhcb.cern.ch//eos/lhcb/user/a/another/starterkit/myfavouritefile.root')
> ```
