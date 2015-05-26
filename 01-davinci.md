---
layout: page
title: First Steps in LHCb
subtitle: General Introduction to DaVinci
minutes: 5
---

> ## Learning Objectives {.objectives}
> * Which problems specific to HEP/LHCb is Gaudi designed to solve?
> * What are Algorithms and Tools?
> * What is an option file?
> * What is the relationship between DaVinci and Gaudi?
> * What are `lb-run` and `lb-dev`?
> * How do I run Gaudi using my option file?
> * How can I browse the code/documentation of the LHCb software?
> * How can I check out an LHCb package?

### Which problems specific to HEP/LHCb is Gaudi designed to solve?

### What are Algorithms and Tools?

### What is an option file?
An option file is a Python script that is used to configure a Gaudi instance.

### What is the relationship between DaVinci and Gaudi?

### What are `lb-run` and `lb-dev`?
Both are small programs that help you set up an environment for your LHCb software. If you just want to run software without making any modifications, `lb-run` is your tool of choice. In order to (for example) run `v36r6` of DaVinci, you just type


```bash
lb-run DaVinci v36r6 bash
```

and the script automatically starts a new bash session for you where all necessary environment variables are set for you. If you want to confirm that this indeed worked, just run `gaudirun.py`.

For an overview of all available versions of DaVinci type

```bash
lb-run --list DaVinci
```

If instead you want to make changes to the software, you will need to set up a development environment. `lb-dev` is your friend here:

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



### How do I run Gaudi using my option file?
On your terminal, after you have successfully initialised your environment type

```bash
gaudirun.py myoptionfile.py
```

### How can I browse the code/documentation of the LHCb software?

### How can I check out an LHCb package?
