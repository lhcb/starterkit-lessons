---
layout: page
title: First Steps in LHCb
subtitle: Download a file from the grid
minutes: 10
---

> ## Learning Objectives {.objectives}
>
> * Obtain a {,x,µ}dst file from the grid

In section ["Finding a Dataset in the Bookkeeping"](03-bookkeeping.html), we obtained a file called `MC_2012_27163003_Beam4000GeV2012MagDownNu2.5Pythia8_Sim08e_Digi13_Trig0x409f0045_Reco14a_Stripping20NoPrescalingFlagged_ALLSTREAMS.DST.py` which contains the following section:

```python
IOHelper('ROOT').inputFiles(['LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00035742/0000/00035742_00000001_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00035742/0000/00035742_00000002_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00035742/0000/00035742_00000003_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00035742/0000/00035742_00000004_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00035742/0000/00035742_00000005_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00035742/0000/00035742_00000006_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00035742/0000/00035742_00000007_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00035742/0000/00035742_00000008_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00035742/0000/00035742_00000009_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00035742/0000/00035742_00000010_1.allstreams.dst'
], clear=True)
```
which is just a collection of **L**ogical **F**ile **N**ames on the grid.

This is a list of files that make up the dataset we are interested in. Each of the files contains a number of individual events so if we just want to take a quick look at the dataset, it is sufficient to just obtain one of those files.

Before we can download the file, we need to set up our connection with the grid and load the Dirac software:
```bash
lhcb-proxy-init
lb-run LHCbDirac $SHELL
```

Initialisation of the proxy might take a while and should ask you for your certificate password.

Once we have a working Dirac installation, getting the file is as easy as

```bash
dirac-dms-get-file LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00035742/0000/00035742_00000002_1.allstreams.dst
```

Again this will take a while but afterwards you should have a file called `00035742_00000002_1.allstreams.dst` in the directory where you called the command.

Since those tuples tend to be quite large, you might want to use your AFS work directory instead of your AFS user directory to store files.

If you want to obtain all the files, you can copy and paste the list of file names from the file you got from the bookkeeping and paste them into the following python script for convenience.

```python
# Your list of file names here
FILES = []

if __name__ == '__main__':
    from subprocess import call
    from sys import argv

    n_files = len(FILES)
    if len(argv) > 1:
        n_files = int(argv[1])

    files = FILES[:n_files]
    for f in files:
        print('Getting file {0}.'.format(f))
        call('dirac-dms-get-file {0}'.format(f), shell=True)
    print('Done getting {0} files.'.format(n_files))
```

Save it as `getEvents.py` and use it via `python getEvents.py [n]`. If you specify `n`, the script will only get the first n files from the grid.
