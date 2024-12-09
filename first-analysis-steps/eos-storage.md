# Storing large files on EOS

{% objectives "Learning Objectives" %}

* Open and view files on EOS

{% endobjectives %} 

EOS is a large area of disk space provided by CERN for users. See [here](http://information-technology.web.cern.ch/services/eos-service) for more information.

On `lxplus` EOS is mounted with a fuse mount under `/eos` .
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

You may also use the following path which is independent of the EOS mount:

```bash
$ root root://eoslhcb.cern.ch//eos/lhcb/user/a/another/myfavouritefile.root
```

In general the fuse mount of EOS is not very reliable and it is not recommended to use it for moving or accessing large numbers of files. Instead you can use the [eos commands](http://eos-docs.web.cern.ch/eos-docs/clicommands.html), for example `eos cp`, or the [xrootd protocol](https://xrootd.slac.stanford.edu/doc/xrdcl-docs/www/xrdcldocs.html#x1-40003), for example `xrdcp`.


If you are working with plenty of heavy files, at some point you might be interested in checking how much of your EOS quota is actually filled. This can be done using `eos quota` command with the following syntax:
```bash
$ eos quota /eos/lhcb/user/a/another
```

Checking the grid storage quota is more complicated. To do so, you need to run the LHCbDirac using your grid certificate:
```bash
$ lb-dirac bash --norc
$ lhcb-proxy-init
```
To check the current quota, one simply types
```bash
$ dirac-dms-user-quota
```
However, this does not show the actual amount of space used. To see it, one types
```bash
$ dirac-dms-storage-usage-summary --Directory /eos/lhcb/grid/user/lhcb/user/a/another
```
This will show a summary of your data on every grid site:
```output
Storage usage for directories ['/lhcb/user/a/another/']
------------------------------------------------
DIRAC SE             Size (TB)            Files
------------------------------------------------
CERN-USER            0.111                4272
CNAF-USER            0.132                2443
GRIDKA-USER          0.156                1332
IN2P3-USER           0.054                1453
PIC-USER             0.018                247
RAL-USER             0.121                3548
SARA-USER            0.051                815
Total (disk)         0.642                14110
------------------------------------------------
```
If you are using grid storage extensively, your qouta will run out and you will be requested to clean the storage.
To have a summary of locations of all your old files, type
```bash
$ dirac-dms-user-lfns --BaseDir=/lhcb/user/a/another
```
This command creates a `lhcb-user-a-another.lfns` file containing the list of LFNs for your old data. (This takes some time, if you have plenty of files).
You may also specify subdirectories in the option `--BaseDir` if you need to remove something specific.
There is a number of options allowing to select only files of certain name or produced before certain date, which may be reviewed in the following way:
```bash
$ dirac-dms-user-lfns --help
```
Having the `.lfns` file allows you to check the amount of space occupied by these files:
```bash
$ dirac-dms-data-size lhcb-user-a-another.lfns
```
```output
------------------------------
Files          |      Size (GB)
------------------------------
7120           |            5.2
------------------------------
```
Finally, to remove these files, one does
```bash
$ dirac-dms-remove-files lhcb-user-a-another.lfns
```
{% callout "Removal by direct path" %}

One may also simply remove a single specific file providing the path to it:
```bash
$ dirac-dms-remove-files /lhcb/user/a/another/2017_08/176410/176410700/Tuple.root
```
{% endcallout %}

Beware that the `dirac-dms-remove-files` command removes only the files, not the folders. You will have the empty folders left, however you should do nothing with this, they will be cleaned automatically after some time.
