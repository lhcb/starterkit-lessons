---
layout: page
title: First Steps in LHCb
subtitle: Introduction to the LHCb data flow
minutes: 10
---

> ## Learning Objectives {.objectives}
> * Understand the LHCb data flow
> * Learn the key concepts on the stripping

Collisions recorded by the LHCb detector go through a very specific data flow in order to ensure the maximum data-taking efficiency and data quality.
In each step, specific [Gaudi](01-davinci.html)-based applications process the data and produce the output for the next step.
The data flow is designed so the processing of real data and simulated one are as close to each other as possible, with small differences that will be discussed when applicable:

 1. (Only in simulation) The _pp_ collisions are generated using `Pythia` (currently `Pythia8`), decays of hadronic particles are described by `EvtGen` and the interaction of the generated particles with the detectors and its response are implemented using the `Geant` toolkit.
 The simulation step is managed by the `Gauss` application, while the digitization of the simulated events is performed by `Boole`.
 2. Data from the detector (or from `Boole`) are filtered through the *trigger*, which consists of L0, implemented in hardware, and HLT, implemented in software. The LHCb application responsible for the HLT is `Moore`, which will be discussed in further detail in the [trigger lesson](18-hlt-intro.html).
 In the case of real data, these are saved to disk and sent to CERN.
 3. Triggered, raw data are reconstructed to transform the detector hits into physical objects such as tracks, clusters, etc, through the `Brunel` application. These are stored into an output file in `DST` format.
 4. The reconstructed `DST` files are suitable for analysis, but they are not accessible to users due to computing restrictions. Data must be filtered futher through a set of selections called *stripping*, which write output either in `DST` or `µDST` format.

    > ## The output format {.callout}
    > A `DST` file is a ROOT-format file in which the full event information (reconstructed objects, raw data, etc) is stored. This takes typically around 150kB per event, so it's very desirable to try to store smaller files.
    > The `µDST` format was designed to achieve this purpose: in them, only the information concerning the build candidates (that is, the output of the stripping `SelectionSequences`) is stored; the raw event, which takes around 50kB per event, is discarded.
    > 
    > Since the objects saved are just those placed in TES locations, one can build a custom output file by specifying which locations need to be saved.
    > For example, one could save a `µDST` with raw information.
    >


 5. Users can run their own analysis tools, such as [`DecayTreeTuple`](09-minimal-dv-job.html), to extract variables useful for analysis.
 The processing is slightly different between `DST` and `µDST`, since some calculations, such as isolations, need the other tracks in the event (not only the signal), which are not available in the latter format.
