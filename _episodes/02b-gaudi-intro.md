---
title: "Intro to the Gaudi framework"
teaching: 20
exercises: 30
questions:
- "What does Gaudi do?"
objectives:
- "Learn the basic concepts of Gaudi."
keypoints:
- "Learn to use Gaudi."
---

# What is the Gaudi framework?

The Gaudi framework runs over a list of events, providing ways to process them and store data in a new format. It creates and manages **Data Objects**, which can hold a variety of data. A **Transiant Event Store (TES)** stores data objects in a way to make them accessible to the rest of the framework, and parts of it can be made persistent in a ROOT format file. The data in the TES is created and accessed by **Algorithms**, which produce data objects and process data objects. Gaudi also
provides **Services**, which provide useful tools, such as histograms. The Application Manager manages these components.


## Algorithms

This is the most important component of the framework for an user to know. Algorithms are called once per physics event, and (traditionally) implement three methods beyond constructor/destructor: `initialize`, `execute`, and `finalize` (see the details on the upgrade). Also, `beginRun` and `endRun` are available, though be careful not to misuse state.





> ## Code
>
> The code for these files is here:
> 
> * [README.txt](/DevelopKit/code/gaudi/data_trans/README.txt)
> * [CMakeLists.txt](/DevelopKit/code/gaudi/data_trans/CMakeLists.txt)
> * [DataTrans.cpp](/DevelopKit/code/gaudi/data_trans/DataTrans.cpp)
> * [DataTrans.h](/DevelopKit/code/gaudi/data_trans/DataTrans.h)
> * [gaudi_opts.py](/DevelopKit/code/gaudi/data_trans/gaudi_opts.py)
> 
{: .callout}

> ## Futher reading
> 
> There are further examples in the repoository for [Gaudi/GaudiPython/src/Test](https://gitlab.cern.ch/lhcb/Gaudi/tree/future/GaudiPython/src/Test).
> 
{: .callout}
