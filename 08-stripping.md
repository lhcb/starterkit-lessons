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
 2. Data from the detector (or from `Boole`) are filtered through the *trigger*, which consists of L0, implemented in hardware, and HLT, implemented in software. The LHCb application responsible for the HLT is `Moore`, which will be discussed in further detail in []().
 In the case of real data, these are saved to disk and sent to CERN.
 3. Test
