---
layout: page
title: First steps in LHCb
subtitle: HLT intro
minutes: 45
---

> ## Learning Objectives {.objectives}
>
> * Learn a bit about the LHCb trigger
> * Learn how the results of the HLT are stored in the raw event.
> * Use an interactive python session to explore the HLT results.

The LHCb trigger reduces the input rate event rate of approximately 12 MHz
to 12.5 kHz of events that are written to offline tape storage. The rate
reduction is achieved in three steps: L0, HLT1 and HLT2. L0 is implemented in
custom FPGAs, has a fixed latency of 4 μs and a maximum output rate of 1 MHz.

HLT1 and HLT2 are implemented as software applications that run on the online
farm; HLT1 runs in real-time and writes events to the local harddrives of the
farm machines, while HLT2 uses the rest of the available CPU (100% when there is
no beam) to process the events written by HLT1. Events accepted by HLT2 are sent
to offline storage.

In Run I, both the reconstructions and selections used by HLT1, HLT2 and offline
were very different. In Run 2 the reconstructions used in HLT2 and offline will
be identical, while selections might still be different. HLT1 uses a special,
fast, reconstruction that only searches for tracks with PT > 500 MeV, for example.

Once HLT1 or HLT2 has accepted an event, the candidates accepted by all trigger
lines are saved to the raw event in a stripped-down form. One of the things that
is saved are all the LHCbIDs of the final state particles of a decay tree.

> ## What is an LHCbID? {.callout}
> Every single sub-detector element has an LHCbID which is unique across the whole detector.
> Physics objects, such as tracks, can be defined as sets of LHCbID objects.
> When a trigger decision is made, the set of LHCbID objects which comprise the triggering
> object is stored in the SelReports. This allows objects reconstructed later, such as in Brunel,
> to be compared with the objects reconstructed in the trigger.

A new feature in Run 2 is the so-called Turbo stream. Since the reconstruction
available in HLT2 is the same as the offline reconstruction, physics analysis
can be done with the candidates created in HLT2. If a line is configured to be a
Turbo line, all information on the candidates that it selects is stored in the
raw event. These candidates can be resurrected later by the Tesla application and
written to a microDST. This is similar to stripping streams that go to 
microDST, where only candidates that are used in passing selections are 
available to analysts. The Turbo stream is different because information that 
is not saved is lost forever.

We will now have a look at some of the candidates stored by the HLT. We will use the script we
[used last time](http://lhcb.github.io/first-analysis-steps/05-interactive-dst.html)
as a starting point, and the file
`root://eoslhcb.cern.ch//eos/lhcb/user/r/raaij/Impactkit/00051318_00000509_1.turbo.mdst`.
This file contains some 2016 Turbo events from [run 
174252](http://lbrundb.cern.ch/rundb/run/174252/).  Fire up your
favourite editor, open the script and save a copy to work on as
`hlt_info.py`. There are a few things in the script that we don't need and can
be removed, such as the `print_decay` method and the decay finder tools.

Like the stripping, the decisions of the HLT are saved in so-called
DecReports. You can find them in `Hlt1/DecReports` and `Hlt2/DecReports`, have a
look at what they contain.

~~~ {.python}
evt['Hlt1/DecReports']
evt['Hlt2/DecReports']
~~~

An important difference with the stripping is that for the HLT, all the
decisions are present, even if they are false. Copy the `advance` function to
`advance_hlt`, make it work on HLT decisions and make sure it really checks the
decision. This can be done using the `decision` member function of a
`DecReport`. Note that all names in the reports end with `Decision`.

~~~ {.python}
reports = evt['Hlt1/DecReports']
report = reports.decReport('Hlt1TrackAllL0Decision')
print report.decision()
~~~

The HLT1 selections that are most efficient for hadronic charm and beauty decays
in Run 2 are called Hlt1TrackMVA and Hlt1TwoTrackMVA. Use the advance function
to find an event that was accepted by either of these trigger selections.

The DecReports only contains the decisions for each line, 1 or 0. The 
candidates themselves are stored in the
SelReports ("Hlt{1,2}/SelReports"). Get the HLT1 SelReports from the event store
and retrieve the one for one of the TrackMVA selections using the selReport function,
analogously to how the DecReport was retrieved above.

The SelReports store candidates in a tree of sub-structures, which can be
accessed using the `substructure` member funtion of a report. Any SelReport has at
least one level of sub-structure. The sub-structure is internally stored in
SmartRefs, which can be dereferenced using their "data" method. Let's have a look
at a SelReport for a TrackMVA selection.

~~~ {.python}
reports = evt['Hlt1/SelReports']
report = reports.selReport('Hlt1TrackMVADecision')
print report
report.substructure().size()
report.substructure()[0].substructure().size()
report.substructure()[0].substructure()[0]
report.substructure()[0].substructure()[0].data()
~~~

In addition to the LHCbIDs, some numbers are also stored, such as the momentum
of the track. These are stored in the numerical info dictionary that can be
retrieved using:

~~~ {.python}
report.substructure()[0].substructure()[0].numericalInfo()
~~~

> ## Plot the transverse momentum distribution {.challenge}
>
> Make a plot of the total and transverse moment distributions of all candidates
> accepted by the Hlt1TrackMVA selection. Then add Hlt1TwoTrackMVA and
> consider the difference.

The LHCbIDs of the (in this case) track can be retrieved using:

~~~ {.python}
report.substructure()[0].substructure()[0].lhcbIDs()
~~~

> ## Turbo candidates {.challenge}
>
> Now let's have a look at the same information that is stored for a candidate
> created by a Turbo line, for example Hlt2CharmHadDsp2KS0PimPipPip_KS0LLTurbo.
> Adapt the `advance_hlt` with an additional argument that allows specification
> of the location of `DecReports` it uses, then advance to an event that was selected by
> Hlt2CharmHadDsp2KS0PimPipPip_KS0LLTurbo, retrieve its `SelReport` and have a
> look at what's stored.
