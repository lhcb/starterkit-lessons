# Setting up a Docker development environment

{% objectives "Using Docker" %}
* Download and prepare a Docker
{% endobjectives %}


The following setup is based on the Hackathon setup [here](https://gitlab.cern.ch/lhcb/upgrade-hackathon-setup).

## Prerequisites

* Docker: you should be able to run Docker containers. On MacOS you should have the latest version of Docker, which finally has built in virtualization. Be sure to use the most recent version of `upgrade-hackathon-setup`, as well, as several Mac related issues were fixed, and Mac is now fully supported with no (known) caveats.
* Fuse: You should have a very recent version of Fuse (or OSXFuse on MacOS)
* CVMFS: you should have access to /cvmfs/lhcb.cern.ch. On a Mac you will need [OSXFuse](http://osxfuse.github.io/) and [CernVM-FS](http://cernvm.cern.ch/portal/filesystem/cvmfs-2.3).

{% discussion "Mounting cvmfs manually" %}

If you want to mount CVMFS manually, such as for accessing it for other tasks, this is how you can do that:

Set your proxy, for example: 

```term
local:~ $ echo "CVMFS_HTTP_PROXY=DIRECT" | sudo tee -a /etc/cvmfs/default.local
```

Mount a CernVM-FS repository using the following steps:

```term
local:~ $ sudo mkdir /cvmfs/lhcb.cern.ch
local:~ $ sudo mount -t cvmfs lhcb.cern.ch /cvmfs/lhcb.cern.ch
local:~ $ sudo mkdir /cvmfs/lhcbdev.cern.ch
local:~ $ sudo mount -t cvmfs lhcbdev.cern.ch /cvmfs/lhcbdev.cern.ch
```

If you are on Mac, you will also need to add the `/cvmfs` directory to the shared directory list in the Docker applet.
{% enddiscussion %}

## Quick start
To get started, get the tools with:

```term
local:~ $ git clone ssh://git@gitlab.cern.ch:7999/lhcb/upgrade-hackathon-setup.git hackathon
```

{% discussion "Using your ssh key inside the container" %}

If you have loaded your identity, you can share to the container. To load it, type:

```term
local:~ $ test -z "$SSH_AGENT_PID" && eval $(ssh-agent)
local:~ $ ssh-add
```

This is also useful, since it keeps you from having to type a passphrase every time you try to use git. The `test -z` command is just ensuring that you don't run the agent multiple times. If you are using multiple identities on one account, you can tell `ssh-add` to add the correct one.
{% enddiscussion %}

Then from the `hackathon` directory just created invoke:

```term
local:hackathon $ ./lb-docker-run --home --ssh-agent --force-cvmfs
```

which will pull the latest image of the SLC6 Docker image we use to build our
software and start an interactive shell in the special directory `/workspace`,
mapped to the local `hackathon` directory.

{% discussion "Note about the options of the `lb-docker-run` command" %}

This command has quite a few options. You can see them with:

```term
local:hackathon $ ./lb-docker-run -h
```

The persistent home option used above may require that you own `/<username>` on your system as well.
 
The `--force-cvmfs` will mount `/cvmfs` on your system, and is not needed if you already have it mounted.
And the `--ssh-agent` option forwards your ssh identity to the container (if one was added).

{% enddiscussion %}


Building the whole stack the first time may take a lot of time, so you can
optionally get a pre-built image with:

```term
local:hackathon $ make pull-build
```

At this point you can build the software stack with (will build automatically
in parallel):

```term
local:hackathon $ make
```

If you didn't pull the pre-built image, this command will checkout the
code and build from scratch.

{% discussion "Using CVMFS" %}

Using CVMFS requires a few changes compared to working on LxPlus, where you have access to AFS. If you run an `lb-dev` or `lb-run` command on the nightlies, you should add `--nightly-cvmfs` to the command. For example:

```term
local:hackathon $ lb-run --nightly-cvmfs --nightly lhcb-head Kepler HEAD $SHELL
```

Would drop you into a shell with the CVMFS nightly head of the Kepler package.
{% enddiscussion %}

{% challenge "Building a project" %}

This is how you would run MiniBrunel:

```term
local:~ $ . /cvmfs/lhcb.cern.ch/lib/lhcb/LBSCRIPTS/dev/InstallArea/scripts/LbLogin.sh -c x86_64-slc6-gcc49-opt
local:~ $ lb-dev --nightly-cvmfs --nightly lhcb-future 282 Brunel/future
local:~ $ cd BrunelDev_future
local:BrunelDev_future $ git lb-use -q Brunel
local:BrunelDev_future $ git lb-use -q Rec
local:BrunelDev_future $ git lb-checkout Brunel/future Rec/Brunel
local:BrunelDev_future $ make
local:BrunelDev_future $ ./run gaudirun.py Rec/Brunel/options/MiniBrunel.py
```
{% endchallenge %}

