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
In each step, specific [Gaudi](http://lhcb.github.io/first-analysis-steps/01-davinci.html)-based applications process the data and produce the output for the next step.
The data flow is designed so the processing of real data and simulated one are as close to each other as possible, with small differences that will be discussed when applicable:

 1. (Only in simulation) The _pp_ collisions are generated using `Pythia` (currently `Pythia8`), decays of hadronic particles are described by `EvtGen` and the interaction of the generated particles with the detectors and its response are implemented using the `Geant` toolkit.
 The simulation step is managed by the `Gauss` application, while the digitization of the simulated events is performed by `Boole`.
 2. Data from the detector (or from `Boole`) are filtered through the *trigger*, which consists of L0, implemented in hardware, and HLT, implemented in software. The LHCb application responsible for the HLT is `Moore`, which will be discussed in further detail in the trigger lesson.
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


 5. Users can run their own analysis tools, such as [`DecayTreeTuple`](http://lhcb.github.io/first-analysis-steps/09-minimal-dv-job.html), to extract variables useful for analysis.
 The processing is slightly different between `DST` and `µDST`, since some calculations, such as isolations, need the other tracks in the event (not only the signal), which are not available in the latter format.

Steps 1 to 4 are performed centrally, while 5 is performed by the users.
Step 3, the reconstruction, is rarely performed, only when data are taken and when a new reconstruction configuration is available.
Step 4, the stripping, can be performed more often, since it runs on reconstructed data. Strippings are identified by a `SXrYpZ` version, which help identify the different types of strippings:

 - The digit `X` marks the major stripping version. This marks all major *restrippings*, in which the full list of lines is processed, with or without re-reconstruction.
 - The digit `Y` is the release version, whch was used during Run-I to mark the data type, that is, the year: 0 was used for 2012 and 1 for 2011. The legacy stripping, `S21` is used for 2012 and `S21r1` for 2011.
 - The digit `Z` marks the patch version, which correspond to *incremental strippings*, stripping campaigns based on a major version in which only a handful of lines is run (either for bug fixes or to add new ones). In the case of incremental strippings, no re-reconstruction is performed.

To get information on the stripping, the best resource is the [stripping Twiki](https://twiki.cern.ch/twiki/bin/view/LHCb/LHCbStripping).
In it we can find:

  - The status of the current strippings, eg, for [Stripping `S21r1`](http://lhcbproject.web.cern.ch/lhcbproject/Reprocessing/stats-re-stripping-fall14.html).
  - The configuration of all strippings, eg, for [Stripping `S21`](https://twiki.cern.ch/twiki/bin/view/LHCb/Stripping21Configuration)

Additionally, the information on all strippings can be found in the [stripping project website](http://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/stripping/), where you can see all the algorithms run and cuts applied in each line.
For example, if we wanted to understand the `StrippingD2hhCompleteEventPromptDst2D2RSLine` line, which we used in the [exploring a DST](http://lhcb.github.io/first-analysis-steps/05-interactive-dst.html) lesson from now on, we would go [here](http://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/stripping/config/stripping21/charmcompleteevent/strippingd2hhcompleteeventpromptdst2d2rsline.html).

> ## Understanding a stripping line {.challenge}
> Go to the `StrippingD2hhCompleteEventPromptDst2D2RSLine` line definition [here](http://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/stripping/config/stripping21/charmcompleteevent/strippingd2hhcompleteeventpromptdst2d2rsline.html) and try to understand what do the `CombineParticles` (`D2hhCompleteEventPromptD2KPiSel` and `D2hhCompleteEventPromptDst2D2RSLine`) in there do.
> How are they different from the decay we built in the [Build your own decay](06-building-decays.html) lesson?
> Do you understand all LoKi functors?
