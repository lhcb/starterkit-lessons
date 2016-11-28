---
layout: page
title: First Steps in LHCb
subtitle: More Ganga
minutes: 10
---
> ## Learning Objectives {.objectives}
>
> * Set the input data with BKQuery
> * Use LHCbDatasets
> * Set the location of the output of our jobs
> * Set the location of your .gangadir
> * Access output stored on the grid

The input data can be specified for your job with the BKQuery tool. The path for the data can be found using the online Dirac portal and passed to the `BKQuery` to get the dataset. For example, to run over the Stripping 21 MagUp, Semileptonic stream

```python
Ganga In [3]: j.inputdata = BKQuery('/LHCb/Collision12/Beam4000GeV-VeloClosed-MagUp/Real Data/Reco14/Stripping21r0p1a/90000000/SEMILEPTONIC.DST').getDataset()
Ganga In [4]: j.inputdata
Ganga Out [4]: 
 LHCbDataset (
   depth = 0,
   treat_as_inputfiles = False,
   persistency = None,
   files = [3717 Entries of type 'DiracFile'] ,
   XMLCatalogueSlice =    LocalFile (
     namePattern = ,
     compressed = False,
     localDir = 
   ) 
 )
```
This is a list of `DiracFile`, the Ganga object for files stored on the grid. We can access one locally via the `accessURL`:
```python
Ganga In [5]: j.inputdata[0].accessURL()
Ganga Out [5]: ['root://bw32-4.grid.sara.nl:1094/pnfs/grid.sara.nl/data/lhcb/LHCb/Collision12/SEMILEPTONIC.DST/00051179/0000/00051179_00006978_1.semileptonic.dst']
```
The returned path can be used by Bender to explore the contents of the DST, as in the [Interactively exploring a DST](interactive-dst.html) lesson.

In the previous lesson we looked at the location of the ouput with `job(782).outputdir`. This location points us to the `gangadir` where ganga stores information about the jobs and the output. If we have lots of jobs with large files the file system where the gangadir is located will quickly fill up.

> ## Setting the gangadir location {.callout}
>
> The location of the `gangadir` can be changed in the configuration file '~/.gangarc'. Just search for the `gangadir` attribute and change it to where you like (on the CERN AFS the `work` area is a popular choice).

To avoid filling up the filespace it is wise to put the large files produced by your job somewhere with lots of storge - the grid. You can do so by setting the `outputfiles` attribute:

```python
j.outputfiles = [DiracFile('*.root'), LocalFile('stdout')]
```
The `DiracFile` will be stored in your user area on the grid (with up to 2TB personal capacity) from where you can access it with the `accessURL()` function as before. The wildcard means that any root file produced by your job will stay on the grid. `LocalFile` downloads the file to your `gangadir`, in this case the one called `stdout`.

Small files are downloaded as standard: `.root`, logfiles etc. Files that are expected to be large which have extensions .dst etc are by default kept on the grid as Dirac files. In general you are encouraged to keep your large files on the grid to avoid moving large amounts of data around through your work area.


> ## Getting help with ganga {.callout}
>
> To find out more take a look at the [Ganga FAQ](https://twiki.cern.ch/twiki/bin/view/LHCb/FAQ/GangaLHCbFAQ)
