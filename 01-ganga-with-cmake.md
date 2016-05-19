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

The `lb-dev` command allows you to start hacking on LHCb code very quickly. The 
downside is that Ganga does not yet support the use of local projects created 
with `lb-dev` when you run an application.

In this lesson, we will go through a workaround that will allow you to use 
local projects with Ganga jobs, wherever they run.

> ## How things used to be {.callout}
>
> Before `lb-dev` was available, local projects would conventionally live in 
> the `~/cmtuser` folder. A local DaVinci v36r5 project, for example, would be 
> at `~/cmtuser/DaVinci_v36r5`, within which one could checkout sub-packages, 
> edit them, and compile the main package. Running things like `gaudirun.py` 
> would then pick up the local version of DaVinci, similarly to how the `./run` 
> command does things today.
>
> Ganga only supports this ‘cmtuser-style’ of local projects. If you create a 
> `Job` with a `DaVinci(version='v36r5')` application, Ganga will automatically 
> use the local version of DaVinci in `~/cmtuser`, and will even ship this to 
> the Grid of your job will run there. The Ganga developers are [working on 
> `lb-dev` support][ganga-lbdev].
>
> The use of ‘cmtuser-style’ projects created with `SetupProject` is 
> discouraged by everyone else in favour of `lb-dev`, so try not to use it if 
> at all possible.

We will create a local version of DaVinci with some modified algorithm, so that 
we can check that Ganga is using the local version when we submit a job.
First, we'll create a local DaVinci with `lb-dev`, and will then fetch the 
[`DecayTreeTuple`][dtt-package], which lives under the `Phys` hat.

```shell
$ lb-dev DaVinci v40r1p3
$ cd DaVinciDev_v40r1p3
$ getpack Phys/DecayTreeTuple v5r6
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

This will create a folder called `InstallArea`. Ganga needs to 
know about this folder a couple of others to use the local project. We will create [symbolic links][symlink] 
to these folders in the place that Ganga looks.

```shell
$ mkdir -p ~/cmtuser/DaVinci_v40r1p3
$ cd ~/cmtuser/DaVinci_v40r1p3
$ ln -s ~/DaVinciDev_v40r1p3/cmt
$ ln -s ~/DaVinciDev_v40r1p3/DaVinciDevSys
$ ln -s ~/DaVinciDev_v40r1p3/InstallArea
$ ls -la
drwxr-xr-x.  2 username z5 2048 May 19 09:05 .
drwxr-xr-x.  2 username z5 2048 May 19 09:05 ..
lrwxr-xr-x.  1 username z5   58 May 19 09:05 cmt -> /afs/cern.ch/user/a/apearce/DaVinciDev_v40r1p3/cmt
lrwxr-xr-x.  1 username z5   58 May 19 09:05 DaVinciDevSys -> /afs/cern.ch/user/a/apearce/DaVinciDev_v40r1p3/DaVinciDevSys
lrwxr-xr-x.  1 username z5   58 May 19 09:05 InstallArea -> /afs/cern.ch/user/a/apearce/DaVinciDev_v40r1p3/InstallArea
```

You can see the link in the output of `ls -la`. For example, the arrow `->` 
says that `InstallArea` is a link to our `InstallArea` created by `lb-dev`.

Now we can proceed as usual, defining a job running DaVinci in Ganga.

```shell
$ lb-run Ganga v601r19 ganga
```

```
j = Job(name='Local_DaVinci_Test')
j.application = DaVinci(version='v40r1p3')
j.applications.optsfile = [...]
j.inputdata = [...]
j.outputfiles = [LocalFile('*.root')]
j.submit()
```

You can then confirm that you have a `_2M` branch in your output ROOT file.

> ## You know what to do {.challenge}
>
> To test this, we need an options file that defines a `DecayTreeTuple`, and 
> some data to run over. You know how to write options files so you can do this 
> yourself. You could re-use an options file that we made previously, and use 
> the same data.

> ## Warning {.callout}
>
> This is a workaround and is not a permanent solution. You have to be careful 
> when using this method, as things other than Ganga also look in `~/cmtuser` 
> for existing projects when building and configuring. The `lb-dev` tool is one 
> of these things. The folder in `~/cmtuser` **should be deleted after 
> submitting the job** in Ganga to stop it interfering with anything else.

[first-ganga]: 01-managing-files-with-ganga.html
[ganga-lbdev]: https://github.com/ganga-devs/ganga/issues/73
[symlink]: https://kb.iu.edu/d/abbe
