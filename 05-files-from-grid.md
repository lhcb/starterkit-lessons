---
layout: page
title: First Steps in LHCb
subtitle: Using screen to keep things running
minutes: 10
---

> ## Learning Objectives {.objectives}
>
> * Obtain a {x,µ,}dst file from the grid

> ## Prerequisites {.prereq}
>
> * Access to lxplus
> * A working grid certificate
> * Completed section 3

In section ["Finding a Dataset in the Bookkeeping"](03-.html), we obtained a file called `....py` which looks a bit like this:

```python
FILES = []
```

This is a list of files that make up the dataset we are interested in. Each of the files contains a number of individual events so if we just want to take a quick look at the dataset, it is sufficient to just obtain one of those files.

Before we can download the file, we need to set up our connection with the grid and load the Dirac software:
```bash
lhcb-proxy-init
SetupLHCbDirac
```

Initialisation of the proxy might take a while and should ask you for your certificate password.

Once we have a working Dirac installation, getting the file is as easy as

```bash
dirac-dms-get-file filename
```
