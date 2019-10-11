# Storing large files on EOS

{% objectives "Learning Objectives" %}

* Run a ganga job which puts output onto EOS
* Open and view the files on EOS

{% endobjectives %} 

During a real analysis the output of your jobs will quickly grow
beyond what fits onto your AFS space. CERN provides you with 2TB of
space on a set of hard drives called the [EOS
service](https://information-technology.web.cern.ch/services/eos-service) and
a grid storage quota of 2TB.

To retrieve a job outputfile, one can use two types of files:  
- `LocalFile`: the standard one with the output file directly downloaded to
the `gangadir`.  
- `DiracFile`: the output file is stored directly on the grid and be accessed
through the XRootD protocol.  

In this lesson, we will focus on the use of `DiracFile` in `ganga` to manage big
output files.

We can reuse what has been done to run [a DaVinci job on the
grid](davinci-grid.md) and adapt the `j.outputfiles` part.

To add the DiracFile in the configuration of the job we just need:
```python
j = Job(name='First ganga job')
myApp = GaudiExec()
myApp.directory = "./DaVinciDev_v45r1"
j.application = myApp
j.application.options = ['ntuple-options.py']
j.application.platform = 'x86_64-centos7-gcc8-opt'
bkPath = '/MC/2016/Beam6500GeV-2016-MagDown-Nu1.6-25ns-Pythia8/Sim09c/Trig0x6138160F/Reco16/Turbo03/Stripping28r1NoPrescalingFlagged/27163002/ALLSTREAMS.DST'
data  = BKQuery(bkPath, dqflag=['OK']).getDataset()
j.inputdata = data[0:2]     # access only the first 2 files of data
j.backend = Dirac()
j.outputfiles = [
    DiracFile('DVntuple.root')
]
j.splitter = SplitByFiles(filesPerJob=5)
j.submit()
```

When the job is completed, no output is dowloaded but some interesting
information are provided by typing `j.outputfiles[0]`:

```python
DiracFile(namePattern='DVntuple.root',
          lfn='/lhcb/user/a/another/2018_11/146255/146255492/DVntuple.root',
          localDir='/afs/cern.ch/user/a/another/gangadir/workspace/another/LocalXML/129/output')
```
Apart from the `namePattern` which was set during the configuration of the job,
we can retrieve the `localDir` which is the path in your gangadir to the output
of the job and its `lfn` which stands for Logical File Name.

This LFN can then be given as an argument in order to download the file. But
more important in most of the cases, you don't even need to download the file
thanks to the `accessURL` function which will give you the URL of your output
file by typing `j.outputfiles[0].accessURL()`:

```python
['root://eoslhcb.cern.ch//eos/lhcb/grid/user/lhcb/user/a/another/2018_11/146255/146255492/DVntuple.root']
```

This URL can be directly used in your ROOT script with the help of the XRootD
protocol as follows:  

```python
TFile::Open("root://eoslhcb.cern.ch//eos/lhcb/grid/user/lhcb/user/a/another/2018_11/146255/146255492/DVntuple.root")  
```  

{% callout "Use of the XRootD protocol " %}

In order to access files on every grid site with the XRootD protocol, be sure
to have a valid proxy using `lhcb-proxy-init`.

{% endcallout %} 


On `lxplus` EOS is mount under `/eos` .
If you list the contents of the `/eos` directory you should see various experiments areas, and the user area:

```
lhcb ship user
```

There are two useful workspaces on EOS:
- `/eos/user` which is your private EOS space, also accessible via [CERNBox](https://cernbox.cern.ch/), and has 1TB of space;
- `/eos/lhcb` which is the LHCb area and contains the following sub-directories:
  - `/eos/lhcb/user` which is your user area, where you can store your tuples, and has 2TB of space;
  - `/eos/lhcb/wg` which contains the Working Group areas (e.g. for preserving tuples for completed analyses);
  - `/eos/lhcb/grid/` which is the path to the CERN grid site; and many others.

Let's inspect your user area:

```bash
$ ls -al /eos/lhcb/user/a/another
```

Files stored on EOS can be open in `ROOT` the usual way:

```bash
$ root /eos/lhcb/user/a/another/myfavouritefile.root
```

You may also use the following path which is independent of the EOS mount (the mount is in general not very reliable):

```bash
$ root root://eoslhcb.cern.ch//eos/lhcb/user/a/another/myfavouritefile.root
```
