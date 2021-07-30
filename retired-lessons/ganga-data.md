# More Ganga

{% objectives "Learning Objectives" %}

* Set the input data with BKQuery
* Use LHCbDatasets
* Set the location of the output of our jobs
* Set the location of your .gangadir
* Access output stored on the grid

{% endobjectives %} 

As you already saw in the [previous lesson](davinci-grid), the input data can be specified for your job with the BKQuery tool. The path for the data can be found using the online Dirac portal and passed to the `BKQuery` to get the dataset. For example, to run over the Stripping 21 MagUp, Semileptonic stream

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
