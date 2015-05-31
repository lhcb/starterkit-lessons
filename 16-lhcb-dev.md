---
layout: page
title: First Steps in LHCb
subtitle: Developing the LHCb Software
minutes: 10
---

> ## Learning Objectives {.objectives}
> * Learn the basics of how to work with and modify LHCb software packages

This lesson introduces you to two commands:

 - `lb-dev` for setting up a new development environment
 - `getpack` for downloading LHCb software packages

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
Successfully created the local project DaVinciDev in .

To start working:

  > cd ./DaVinciDev
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
For example, if you want to write a custom stripping selection, execute the following in the `DaVinciDev` directory:

```bash
getpack Phys/StrippingSelection head
```

Under the hood, `getpack` will `svn checkout` (≈ `git clone`) the corresponding SVN repository.
The first argument to `getpack` is the name of the package you want to checkout, while the second argument allows you to choose a specific branch.
`head` is usually the one one that contains the newest development changes and he one you should commit new changes to.

You can now modify the `StrippingSelection` package and run `make` to build it with your changes.
You can test your changes with the `./run` script.
It works similar to `lb-run`, without the need to specify a package and version:
```bash
./run gaudirun.py options.py
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

If you have made changes that are supposed to be integrated into the official LHCb repositories, you can use `svn commit`.
Be advised that you should always communicate with the package maintainers before committing changes!

If you just want to take a look at a source file, without checking it out, you can comfortably access the repository through two different web UIs.

 * [Trac](https://svnweb.cern.ch/trac/lhcb/)
 * [SVNweb](http://svnweb.cern.ch/world/wsvn/lhcb)

(which one to use? It is just a matter of taste, pick the one that looks nicest)

To get an idea of how a certain component of the LHCb software works, you can access its doxygen documentation.
There is a page for each project, a list of which can be found here: [https://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/davinci/latest_doxygen/index.html](https://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/davinci/latest_doxygen/index.html).

