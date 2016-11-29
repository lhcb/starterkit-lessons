---
layout: page
title: First Steps in LHCb
subtitle: Developing LHCb Software
minutes: 10
---

> ## Learning Objectives {.objectives}
> * Learn the basics of how to work with and modify LHCb software packages

This lesson introduces you the commands:

 - `lb-dev` for setting up a new development environment
 - `git lb-use` and `git lb-checkout` for downloading LHCb software packages

If you want to make changes to a software package, you will need to set up a development environment. `lb-dev` is your friend here:

```bash
lb-dev --name DaVinciDev DaVinci/v41r2
```

The output should look similar to this:

```
Successfully created the local project DaVinciDev in .

To start working:

  > cd ./DaVinciDev
  > git lb-use DaVinci
  > git lb-checkout DaVinci/vXrY MyPackage

then

  > make
  > make test

and optionally (CMake only)

  > make install

You can customize the configuration by editing the files 'build.conf' and
'CMakeLists.txt' (see http://cern.ch/gaudi/CMake for details).
```

> ## `lb-dev` created local projects are Git repositories {.callout}
> When `lb-dev` creates the local project directory and create the initial files
> there, it also calls `git init` and commits to the local Git repository the
> first version of the files (try with `git log` in there).
>
> You can then use git to keep track of your development, and share your code
> with others (for example with a [new project in gitlab.cern.ch](https://gitlab.cern.ch/projects/new)).

Follow those instructions and once you've compiled your software run

```bash
./run bash
```

inside the directory. This will (similar to `lb-run`) give you a new bash session with the right environment variables.

Your new development environment won't be very useful without any software to modify and build.
So why not check out one of the existing LHCb packages, which are stored in the [LHCb Git repositories](https://gitlab.cern.ch/lhcb)?

Let's assume you have already used `lb-dev` to set up your development environment and you are currently inside it.
In order to obtain the source code of the package you want to work on, use the [Git4LHCb](https://twiki.cern.ch/twiki/bin/view/LHCb/Git4LHCb) scripts.
These are a set of aliases, starting with `git lb-`, that are designed to make developing LHCb software easier.
For example, if you want to write a custom stripping selection, execute the following in the `DaVinciDev` directory:

```bash
git lb-use Stripping
git lb-checkout Stripping/master Phys/StrippingSelections
make configure
```

Under the hood, `git lb-use` will add the [`Stripping`](https://gitlab.cern.ch/lhcb/Stripping) repository as a remote in git.
`git lb-checkout` will then perform a partial checkout of the master branch of the Stripping repository, only adding the files under [`Phys/StrippingSelections`](https://gitlab.cern.ch/lhcb/Stripping/tree/master/Phys/StrippingSelections).

> ## Which project to use in `git lb-use`? {.callout}
> The project name to pass to `git lb-use` depends on the directories you want
> to check out and work on, and not on the project name you passed to `lb-dev`.
> Moreover you can call `git lb-use` several times for different remote
> projects in the same local project:
>
> ```bash
> lb-dev --name DaVinciDev DaVinci/v40r2
> cd DaVinciDev
> git lb-use Analysis
> git lb-use Stripping
> git lb-use DaVinci
> ```

You can now modify the `StrippingSelections` package and run `make purge && make` to build it with your changes.
You can test your changes with the `./run` script.
It works similar to `lb-run`, without the need to specify a package and version:
```bash
./run gaudirun.py options.py
```

> ## What if `git` asks for my password? {.callout}
> Make sure you followed the [prerequisites](https://twiki.cern.ch/twiki/bin/view/LHCb/Git4LHCb#Prerequisites)
> section in the [Git4LHCb TWiki page](https://twiki.cern.ch/twiki/bin/view/LHCb/Git4LHCb).

If you just want to take a look at a source file, without checking it out, you can comfortably access the repository through
the [Gitlab web interface](https://gitlab.cern.ch/lhcb).

To get an idea of how a certain component of the LHCb software works, you can access the doxygen documentation.
One set of doxygen web pages is generated for several related projects, and is linked in all the
projects web sites, like https://cern.ch/LHCb-release-area/DOC/davinci/.
See also the the [LHCb Computing web page](http://cern.ch/lhcb-comp/) for a list of projects.

Another useful tool available on lxplus machines is `Lbglimpse`. It allows you to search for a given string in the source code of LHCb software.
```bash
Lbglimpse "PVRefitter" DaVinci v41r2
```
It works with every LHCb project and released version.

If you have made changes that are supposed to be integrated into the official LHCb repositories, you can use `git lb-push`.
But read the instructions in the [TWiki page](https://twiki.cern.ch/twiki/bin/view/LHCb/Git4LHCb#Using_Git_for_LHCb_development) first.

Depending on the project, you may be required to document your changes in the
release notes which are found in `doc/release.notes`.

After the call to `git lb-push`, go to the project repository web page to
create a new merge request, for example https://gitlab.cern.ch/lhcb/Stripping/merge_requests/new.

> ## Quick link to create a merge request {.callout}
> When pushing to a branch in a project in Gitlab you will see a message like:
>
> ```
> remote:
> remote: Create merge request for my-branch:
> remote:   https://gitlab.cern.ch/lhcb/Stripping/merge_requests/new?merge_request%5Bsource_branch%5D=my-branch
> remote:
> ```
>
> You can use the URL in the message to quickly create a merge request for the
> changes you just pushed.

It is advisable to test new developments on the so-called [nightly builds](https://lhcb-nightlies.cern.ch).
They take build all the projects with all pending merge requests.
You can use a nightly build version of a project with:
```bash
lb-dev --nightly lhcb-head DaVinci/HEAD
```
A more detailed description of the command is found here:

 * [SoftwareEnvTools](https://twiki.cern.ch/twiki/bin/view/LHCb/SoftwareEnvTools)

Sometimes mistakes happen and the committed code is either not compiling or does not do what it is supposed to do.
Therefore the nightly tests are performed. They first try to build the full software stack.
If that is successful, they run some reference jobs and compare the output of the new build with a reference file.
The results of the nightly builds can be found here.

* [Nightly builds summaries](https://lhcb-nightlies.cern.ch)

If the aim of the commit was to change the ouput, e.g. because you increased the
track reconstruction efficiency by a factor of two, mention it in the merge request
description, such that the manager of the affected project can update the reference file.
