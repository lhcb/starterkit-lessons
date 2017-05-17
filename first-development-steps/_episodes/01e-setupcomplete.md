---
title: "Setup Complete"
teaching: 20
exercises: 60
questions:
- "Can you run LHCb software from scratch?"
objectives:
- "Set up a development environment."
keypoints:
- "Usage of raw git commands."
---


> ## Prerequisites for building LHCb software
> 
> You should have a valid LHCb environment, using one of the ways previously mentioned.
> You may need to run `source /cvmfs/lhcb.cern.ch/group_login.sh` to set up your environment variables.
{: .prereq}

Before building, there are a few recommended customisations to prepare in your environment, besides those discussed in the previous lesson.

### Ninja (optional)

If you have ninja in your path, it will be used to (marginally) speed up the make process, and it is automatic; you still use `make ...` commands. To add it:
```
export PATH=/cvmfs/lhcb.cern.ch/lib/contrib/ninja/1.4.0/x86_64-slc6:$PATH
```

You can also add

```bash
export VERBOSE=
```

if you are a fan of reduced noise when building.

### CCache (optional)

If you rebuild often, you should be using CCache. This program stores the results of compiling and reuses them if nothing changes. You should be able to do something like this after you install CCache:

```bash
export CCACHE_DIR=$HOME/.ccache
export CMAKEFLAGS=-DCMAKE_USE_CCACHE=ON
```

If you installed CCache from source locally to $HOME/.local, then you'll need the local bin in your path, too:

```bash
export PATH=$HOME/.local/bin:$PATH
```

### Debug builds

If you want a debug build, you'll need to change some environment variables with the LbLogin program, which will start a new bash instance with the required variables using the configuration name in `$CMTDEB`:

```bash
LbLogin -c $CMTDEB
```

### Path

You should have a main development directory, and it should be in your CMTPROJECTPATH variable. If you use `$HOME/lhcbdev`, then you would do:

```bash
export CMTPROJECTPATH=$HOME/lhcbdev:$CMTPROJECTPATH
```

In VMs and Docker, sometimes the root directory `/workspace` is used.

# Git for LHCb

You can set up a development environment with complete LHCb packages, and build them from scratch. This allows you to make large scale changes without locating sub-packages online, and is similar to the standard procedures in other projects. The downside to this is that they take a while to compile, due to the size of LHCb projects. Since the following is very similar to standard git procedures, most online git tutorials are also helpful in understanding the meaning of the various
git command used.

> ## Help for the git novice
> 
> There are a lot of sites available for help with vanilla git commands. A few are listed here.
> 
> 1. [The git documentation](https://git-scm.com/docs/gittutorial). 
> 2. [Github's interactive tutorial series](https://try.github.io/levels/1/challenges/1). This is an excellent resource for running simple commands to get a feel for how git works on the command line, right in your browser.
> 3. [Atlassian's git guru tutorials](https://www.atlassian.com/git/tutorials/).
> 4. [TutorialsPoint's git tutorials](http://www.tutorialspoint.com/git/).
>
> There are always cheetsheets, too, such as [this one](http://www.cheat-sheets.org/saved-copy/git-cheat-sheet.pdf).
{: .discussion}



Before using git, you should go to your gitlab.cern.ch account and set up an ssh key. The procedure is the same as with github (and the public key is the same). In short, on Linux or Mac, make sure you have key pair in `~/.ssh/id_rsa.pub` and `~/.ssh/id_rsa`. If you don't have one, run

```bash
    ssh-keygen
```

and use the default location. If you don't put in a passphrase, you can directly push and pull without a password; if you do put a passphrase, you can run

```bash
    ssh-add
```

when you start a terminal to enter your passphrase once per session. Your computer keychain (Mac, Ubuntu, etc) can also store your password once per login.

> ## If you encounter problems
> 
> If you get an error about your ssh agent not running, you can execute the output of the ssh-agent command with `eval $(ssh-agent)` and try again.
{: .discussion}

Once you verify you have an ssh key pair, you need to go to the gitlab website, go to your profile (possibly hidden behind the hamburger button), then edit, then SSH Keys [(or just go here)](https://gitlab.cern.ch/profile/keys), and paste the contents of the `id_rsa.pub` file into the key edit box. You can give it a title that allows you to identify the computer or VM associated with the key.

## Setting up the projects

To get a complete project for development, the following command will work:

```bash
    git clone ssh://git@gitlab.cern.ch:7999/lhcb/Project.git
```

where `Project` is the name of the project. The location of this project on your computer is important! It should have the following structure:

* Main LHCb folder (any name, like `$HOME/lhcbdev`)
  * `Project` (git folder here)
  * `AnotherProject`

The package structure is this way to allow the package search system to find names and versions. The main folder should be listed in the `$CMTPROJECTPATH` environment variable.

Once you have cloned the repository, you should check out the branch that you want to build/work on with `git checkout -b BranchName`.

An complete example for the Lbcom package would look like this, using a destination for the git folder, since it will default to the wrong name:

```bash
mkdir ~/lhcbdev
cd ~/lhcbdev
git clone ssh://git@gitlab.cern.ch:7999/lhcb/LHCb.git
cd LHCb
git checkout -b upgradeTracking
```

Inside the project, the CMakeLists.txt file may reference other projects, like this:

```
gaudi_project(Project LocalVersion
              USE ReferredToProject RemoteVersion
              DATA OtherStuff)
```

You need to set the LocalVersion to your version (such as upgradeTracking), and you need to set any dependinces to the version names you've set, as well. When `gaudi_project` looks for a package, it will look for ones that follow the above naming scheme and have the correct, matching version.

> Note: You do not need to locally build all dependencies if there is a released version that will work; they will be found if you do not have a local version.

# Building

To build, you need to run the following commands in each package directory (starting with dependencies first):

```bash
lb-project-init
```

This will setup the makefile, and some other things. Then run:

```bash
make configure
```

to configure the run (it runs CMake behind the scenes). If this fails, you probably have a problem in your CMakeLists.txt or with your paths.

To build and move the results to the folder that CMT expects, run:

```bash
make install
```

This should take a long time the first time, and should be much faster after that.

> ## Future reading
>
> The twiki page [Git4LHCb](https://twiki.cern.ch/twiki/bin/view/LHCb/Git4LHCb) is currently the best source for git examples.
{: .callout}



