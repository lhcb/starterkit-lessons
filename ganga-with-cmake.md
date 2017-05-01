---
layout: page
title: Second analysis steps
subtitle: Using Ganga with local projects
minutes: 15
---

> ## Learning Objectives {.objectives}
>
> * Submit jobs using Ganga that use local LHCb projects containing your own 
>   modifications

Before starting, you should read the [first Ganga lesson][first-ganga], which 
gives some advice about choosing a Ganga version, making a clean environment 
for running Ganga in, and where to go to get help when things in Ganga go 
wrong.


> ## How things used to be {.callout}
>
> Before `lb-dev` was available, local projects would conventionally live in 
> the `~/cmtuser` folder. A local DaVinci v36r5 project, for example, would be 
> at `~/cmtuser/DaVinci_v36r5`, within which one could checkout sub-packages, 
> edit them, and compile the main package. Running things like `gaudirun.py` 
> would then pick up the local version of DaVinci, similarly to how the `./run` 
> command does things today.
>
> Ganga now supports lb-dev packages, so the `SetupProject` style of use is deprecated as of
> version 6.2, and will quickly become unsupported.

We will create a local version of DaVinci with some modified algorithm, so that 
we can check that Ganga is using the local version when we submit a job.
First, we'll create a local DaVinci with `lb-dev`, and will then fetch the 
[`DecayTreeTuple`][dtt-package], which lives under the `Phys` hat.

```shell
$ lb-dev DaVinci v41r2p1
$ cd DaVinciDev_v41r2p1
$ git lb-use Analysis
$ git lb-checkout Analysis/v17r3 Phys/DecayTreeTuple
```

We will do something silly as a test: add a `_2M` branch to 
`TupleToolKinematic` that fills twice the invariant mass of the particle. To do 
this, we just need to edit the implementation file at 
`src/TupleToolKinematic.cpp`.
Line 76 of that file reads like this:

```cpp
test &= tuple->column( prefix+"_M", P->momentum().M() );
```

Here, `tuple` is a object representing the ntuple to be filled, and the 
`column` method tells the ntuple to fill the branch named by the first argument 
(a `string`) with the value of the second argument.
Create a line just below this one, using `_2M` as the branch name suffix as 
twice the particle mass as the value.

```cpp
test &= tuple->column( prefix+"_2M", 2*P->momentum().M() );
```

Then we just need to compile our local DaVinci package. You must be in the 
`DaVinci_v40r1p3` when executing the following commands:

```shell
# Using -j8 runs up to 8 tasks in parallel, speeding up the build
$ make -j8
$ make install
```

This will create a folder called `InstallArea`. This is the folder that Ganga will provide on the Grid.

Now we can proceed as usual, defining a job running DaVinci in Ganga. But instead of
using the built in version of our app (`DaVinci()`), we will use `GaudiExec()` to make a 
custom application:

```shell
$ lb-run Ganga v602r2 ganga
```

```
j = Job(name='Local_DaVinci_Test')
myApp = GaudiExec()
myApp.directory = "/path/to/DaVinciDev_v41r2p1"
myApp.options = ["/path/to/DaVinciDev_v41r2p1/myOptions.py"]
j.application = myApp
j.inputdata = [...]
j.outputfiles = [LocalFile('*.root')]
j.submit()
```

> ## Details of GaudiExec {.callout}
>
> Here, `.options` is a list of arguments that are passed to `gaudirun.py`. The actual command
> looks like:
>
> ```bash
$ ./run gaudirun.py [options contents] [inputdata contents]
```
>
> If you want to run a program directly without the `gaudirun.py` script, you can add `j.application.useGaudiRun = False`.
>
> For more info, do `help(GaudiExec)` in Ganga.

After running this job, confirm that you have a `_2M` branch in your output ROOT file.



> ## You know what to do {.challenge}
>
> To test this, we need an options file that defines a `DecayTreeTuple`, and 
> some data to run over. You know how to write options files so you can do this 
> yourself. You could re-use an options file that we made previously, and use 
> the same data.


[first-ganga]: 01-managing-files-with-ganga.html
[ganga-lbdev]: https://github.com/ganga-devs/ganga/issues/73
[symlink]: https://kb.iu.edu/d/abbe
