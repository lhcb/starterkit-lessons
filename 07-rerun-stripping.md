---
layout: page
title: First Steps in LHCb
subtitle: Running a specific stripping version on a DST
minutes: 15
---
> ## Learning Objectives {.objectives}
>
> * Modify the minimal DecayTreeTuple example to apply a different stripping version to an MC sample

Ideally, our simulated samples should feature the same stripping cuts as the real data we want to work with.
We can be sure of this if the same stripping version has been used when processing the simulated and real data.

But often, our simulated sample will have a different version of the stripping applied to it.
For example, what if our data sample uses Stripping 21, while our MC sample uses Stripping 20?

In this case, we simply need to rerun our stripping line of choice from the correct stripping version.

[This example](code/07-rerun-stripping/options.py) is an extended version of the minimal DaVinci DecayTreeTuple job that additionally runs the corresponding stripping line from Stripping 21.

Take a look at the file and try to find out what has changed compared to the [minimal DaVinci example](./code/09-ntuple_options.py).

