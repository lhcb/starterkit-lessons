---
layout: page
title: First Steps in LHCb
subtitle: Developing the LHCb Software
minutes: 10
---

> ## Learning Objectives {.objectives}
> * Learn the basics of how to work with and modify LHCb software packages

This lesson introduces you to three commands:

 - `lb-run` for running LHCb software on the command line
 - `lb-dev` for setting up a new development environment
 - `getpack` for downloading LHCb software packages

If you just want to run the existing software without making any modifications to it, `lb-run` is your tool of choice.
In order to (for example) run version `v36r6` of DaVinci, you just type

```bash
lb-run DaVinci v36r6 bash
```

and the script automatically starts a new bash session for you where all necessary environment variables are set for you. If you want to confirm that this indeed worked, just run `gaudirun.py`.

For an overview of all available versions of DaVinci type

```bash
lb-run --list DaVinci
```

> ## Using SetupProject instead of lb-run {.callout}
> When reading through other tutorials, you will come across `SetupProject`.
> This is an older way of setting up a shell that is configured to run LHCb software.
> `lb-run` is the new way of doing things and has some nice benefits over `SetupProject`.
> For most purposes, `SetupProject DaVinci v36r6` is equivalent to
> ```bash
> lb-run DaVinci v36r6 $SHELL
> ```

If you want to make changes to a software package, you will need to set up a development environment. `lb-dev` is your friend here:

```bash
lb-dev --name DaVinciDev DaVinci v36r6
```

<!-- This doesn't seem to work atm
Alternatively, if you want to use the most recent nightly builds:

```bash
lb-dev --name DaVinciDev --nightly lhcb-head DaVinci
```
-->

The output should look similar to this:

```
Successfully created the local project DaVinciTest in .

To start working:

  > cd ./DaVinciTest
  > getpack MyPackage vXrY

then

  > make
  > make test
  > make QMTestSummary

and optionally

  > make install

You can customize the configuration by editing the files 'CMakeLists.txt'
(see http://cern.ch/gaudi/CMake for details).
```

Follow those instructions and once you've compiled your software run

```bash
./run bash
```

inside the directory. This will (similar to `lb-run`) give you a new bash session with the right environment variables.

Your new development environment won't be very useful without any software to modify and build.
So why not check out out one of the existing LHCb packages, which are stored in the LHCb SVN repository?

Let's assume you have already used `lb-dev` to set up your development environment and you are currently inside it.
In order to obtain the source code of the package you want to work on, use `getpack`.
This is an LHCb-aware wrapper around SVN.
For example, if you want to write a custom stripping selection, write

```bash
getpack Phys/StrippingSelection
# Make your local changes
make
./run bash
```

> ## What if getpack asks for my password 1000 times? {.callout}
> `getpack` might ask you for your password several times.
> To avoid this, you can create a kerberos token with
> ```
> kinit
> ```
> You will have to enter your password once, and further password prompts will be skipped
> 
> Alternatively, you can perform an anonymous checkout:
> ```
> getpack -p anonymous Phys/StrippingSelection
> ```

Under the hood, `getpack` will `svn checkout` (≈ `git clone`) the corresponding SVN repository.
If you have made changes that are supposed to be integrated into the official LHCb repositories, you can use `svn commit`.
Be advised that you should always communicate with the package maintainers before committing changes!

If you just want to take a look at a source file, without checking it out, you can comfortably access the repository through two different web UIs:

 * [Trac](https://svnweb.cern.ch/trac/lhcb/)
 * [SVNweb](http://svnweb.cern.ch/world/wsvn/lhcb)

To get an idea of how a certain component of the LHCb software works, you can access its doxygen documentation.
There is a page for each project, a list of which can be found here: [https://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/davinci/latest_doxygen/index.html](https://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/davinci/latest_doxygen/index.html).

