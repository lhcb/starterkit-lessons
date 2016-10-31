---
title: "Setting up a local LHCb environment - Docker"
teaching: 30
exercises: 90
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
* Docker: you should be able to run Docker containers
* CVMFS: you should have access to /cvmfs/lhcb.cern.ch

If you cannot get Docker or CVMFS running (or you are on a Mac), you can use
the CernVM-based approach described later in the page.

## Quick start
To get started, get the tools with
```
git clone ssh://git@gitlab.cern.ch:7999/lhcb/upgrade-hackathon-setup.git hackathon
```
Then from the `hackathon` directory just created invoke
```
./lb-docker-run --home --ssh-agent
```
which will pull the latest image of the SLC6 Docker image we use to build our
software and start an interactive shell in the special directory `/workspace`,
mapped to the directory `hackathon`.

> ## Note:
>
> This command has quite a few options. You can see them with:
> 
> ```
./lb-docker-run -h
```
>
> The persistant home option used above may require that you own /<username> on your system as well.
{: .callout}

Building the whole stack the first time may take a lot of time, so you can
optionally get a prebuilt image with
```
make pull-build
```

At this point you can build the software stack with (will build automatically
in parallel)
```
make
```
Note that if you didn't pull the prebuilt image, this command will checkout the
code and build from scratch.



