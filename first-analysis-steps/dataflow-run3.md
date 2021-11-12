# LHCb data flow in Run 3

{% objectives "Learning Objectives" %}

* Understand the changes to the LHCb data flow for Run 3
* Learn the key concepts on the HLT, sprucing

{% endobjectives %}

## LHCb upgrade

In Run 1 & 2 LHCb proved itself not only to be a high-precision
heavy flavour physics experiment, but also extended the core physics programme to many different areas such as electroweak physics and fixed-target experiments. This incredible precision led to
over 500 papers including breakthroughs such as the first discovery of $C\!P$-violation in charm and the first observation of $B_s^0\to \mu^+\mu^-$ among many others.

In order to reach even higher precision the experiment aims to take $50\,\mathrm{fb}^{-1}$ of
data in Run 3 by increasing the instantaneous luminosity by a factor of five.
To be capable of dealing with the higher detector occupancy the experiment will be equipped with an entire new set of tracking detectors with higher granularity and improved radiation tolerance.

Additionally, the front-end and readout electronics of all sub-detectors will be replaced, to be able
to operate at bunch crossing rate of $40\,\mathrm{MHz}$, as well as the photodetectors of the RICH1 detector. 

Moreover, the LHCb upgrade comprises significant changes to the data flow, both for the online and offline processing.

## Upgrade of the LHCb trigger system

The trigger layout for Run 3 will look like this:

!["Data processing chain for Run 3"](img/hidef_RTA_dataflow_widescreen.png)

The LHCb trigger system will be fully redesigned by removing the L0 hardware trigger and moving to a fully-software based trigger. This means that the full detector readout as well as running the HLT1 needs to be enabled at the average non-empty bunch crossing rate in LHCb of $30\,\mathrm{MHz}$.

For this purpose the partial event reconstruction performed by HLT1 will be implemented as part of the ```Allen project``` and run on GPUs to make simple trigger decisions based on the kinematic and topological selections possible. HLT1 will reduce the data rate by a factor of 30.

The raw data of events selected by HLT1 is passed on to the buffer system and stored there. The buffering of events enables to run the real-time alignment and calibration before events are entering HLT2. This is crucial, because in this way calibration and alignment constants obtained from dedicated calibration samples collected in HLT1 can be used in the full event reconstruction performed in HLT2.

The total bandwidth that can be saved from HLT2 to tape is limited to $10\,\mathrm{GB}/s$. An important change in the HLT2 selections with respect to the Run 2 will be the increased use of the Turbo model. Wherever possible, the Turbo will be the baseline, so that in total for approximately 2/3 of data only the data of signal candidate (raw and reconstructed) will be saved and no further offline reconstruction will be possible. This results in significantly smaller event sizes, so that more events can be saved.

More details about HLT selections can be found in the [Moore documentation](https://lhcbdoc.web.cern.ch/lhcbdoc/moore/master/index.html) and the readme's of the [Allen project](https://gitlab.cern.ch/lhcb/Allen).

### Selective persistency

Besides the events that are processed by the Turbo stream, there is many applications for persisting extra information of the raw data. For some decay modes it cannot be avoided to save the full raw data, whereas for others few extra information is needed. In case of persisting the full raw and reconstructed data of the event HLT2 lines can be run with setting the flag `persistreco=True`. When full event information is not required objects can be persisted selectively by defining `extra_outputs`, which can be among the following: 

* extra reconstructed tracks
* ECAL clusters for neutral objects
* ...

## Offline data processing

Events that have passed HLT2 and have been saved to tape are further processed offline by running through the sprucing application. After passing the sprucing events are saved on disk. For events from the Turbo stream the default model is to run through the sprucing only in a passthrough sprucing selection, whereas others can undergo a more tighter selection by running through a sprucing line. This is neccessary because the total bandwidth for events that can be saved on disk is only 3.6GB/s, whereas for saving data from HLT2 to tape 10GB/s are available. Data that is saved to disk will be provided in streams dedicated to specific physics applications.

HLT2 and sprucing selections share the same code base, so that selection lines can be interchanged among the two flexibly.

![](https://lhcbproject.web.cern.ch/lhcbproject/Publications/f/p/Directory_LHCb-FIGURE-2020-016/hidef_DPA_dataflow.png)

### Changes to DaVinci

As in Run2 the DaVinci application is used to produce tuples for offline user analysis from the different physics streams. DaVinci itself is being redesigned to address the following issues:

* storage and computing resources are limited and saving irrelevant variables should be avoided, especially because there will be much more data in Run3
* consistency between online and offline selection frameworks in using ThOr functors

For this purpose the tupling tool is changed to ```FunTuple``` which allows more flexibility for users to choose only variables that are needed for the analysis and not take all variables from a specific TupleTool. Additionally, at the moment FunTuple supports both LoKi and ThOr functors until all relevant functors are migrated to the ThOr framework. 

For detailed information and implementation examples see the [DaVinciExamples](https://gitlab.cern.ch/lhcb/DaVinci/-/blob/master/DaVinciExamples/python/DaVinciExamples/tupling/example-tupling-advanced-run-mc.py) and [DPA documentation](https://lhcb-dpa.web.cern.ch/lhcb-dpa/index.html).