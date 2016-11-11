---
title: "Setting up a local LHCb environment - Docker"
teaching: 5
exercises: 20
questions:
- "Do you want a integrated local install?"
objectives:
- "Get and configure Docker."
- "Get a development Docker instance running."
keypoints:
- "Run Docker."
---

The following setup is based on the Hackathon setup [here](https://gitlab.cern.ch/lhcb/upgrade-hackathon-setup).

## Prerequisites
* Docker: you should be able to run Docker containers. On a Mac you should have the latest version of Docker, which finally has built in virtualization.
* Fuse: You should have a very recent version of Fuse (or OSXFuse on MacOS)
* CVMFS: you should have access to /cvmfs/lhcb.cern.ch. On a Mac you will need [OSXFuse](http://osxfuse.github.io/) and [CernVM-FS](http://cernvm.cern.ch/portal/filesystem/cvmfs-2.3) (possibly a pre-release of 2.3.3 if you are on MacOS Sierra).

If you cannot get Docker or CVMFS running, you can use
the CernVM-based approach described later in the page.

> ## Mounting cvmfs manually
>
> If you want to mount CVMFS manually, such as for accessing it for other tasks, this is how you can do that:
>
> Set your proxy, for example: echo "CVMFS_HTTP_PROXY=DIRECT" | sudo tee -a /etc/cvmfs/default.local
>
> Mount a CernVM-FS repository using the following steps:
> 
> ~~~
sudo mkdir /cvmfs/lhcb.cern.ch
sudo mount -t cvmfs lhcb.cern.ch /cvmfs/lhcb.cern.ch

sudo mkdir /cvmfs/lhcbdev.cern.ch
sudo mount -t cvmfs lhcb.cern.ch /cvmfs/lhcbdev.cern.ch
~~~
> {: .input}
> 
> If you are on Mac, you will also need to add the /cvmfs directory to the shared directory list in the Docker applet.
{: .discussion}

## Quick start
To get started, get the tools with:

~~~
git clone ssh://git@gitlab.cern.ch:7999/lhcb/upgrade-hackathon-setup.git hackathon
~~~
{: .input}

Then from the `hackathon` directory just created invoke:

~~~
./lb-docker-run --home --ssh-agent --force-cvmfs
~~~
{: .input}

which will pull the latest image of the SLC6 Docker image we use to build our
software and start an interactive shell in the special directory `/workspace`,
mapped to the directory `hackathon`.

> ## Note:
>
> This command has quite a few options. You can see them with:
> 
> ~~~
./lb-docker-run -h
~~~
> {: .input}
> 
> The persistent home option used above may require that you own /<username> on your system as well.
> The force-cvmfs will mount /cvmfs on your system, and is not needed if you already have it mounted.
>
{: .callout}

> ## Logged in as root user?
>
> If you start out as root, you should switch to your user with:
> 
> ~~~
$ useradd username
$ su username
~~~
> {: .input}
> 
> You should see the appropriate log-on information when you change to the new user. Your home directory was already created and linked to a docker home directory if you used `--home`.
{: .discussion}

Building the whole stack the first time may take a lot of time, so you can
optionally get a pre-built image with:

~~~
make pull-build
~~~
{: .input}

At this point you can build the software stack with (will build automatically
in parallel):

~~~
make
~~~
{: .input}

If you didn't pull the pre-built image, this command will checkout the
code and build from scratch.


