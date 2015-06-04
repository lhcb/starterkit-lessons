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
were very different. In Run II the reconstructions used in HLT2 and offline will
be identical, while selections might still be different. HLT1 uses a special,
fast, reconstruction that only searches for tracks with PT > 500 MeV, for example.

Once HLT1 or HLT2 has accepted an event, the candidates accepted by all trigger
lines are saved to the raw event in a stripped-down form. One of the things that
is saved are all the LHCbIDs of the final state particles of a decay tree.
LHCbIDs are unique identifiers for detector hits.

We will now have a look at the LHCbIDs of some of the stored candidates. We will
use the script we [created earlier](05-interactive-dst.html) as a starting
point, and the file you [downloaded before](05-files-from-grid.html). Fire up
your favourite editor, open the script and save a copy to work on as
`tistos.py`. There are a few things in the script that we don't need and can be
removed, such as the print_decay and decay finder tools.

Like the stripping, the decisions of the HLT are saved in so-called
DecReports. You can find them in `Hlt/DecReports` and `Hlt2/DecReports`, have a
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

The HLT1 selection that was most efficient for hadronic charm and beauty decays
in Run-I was called Hlt1TrackAllL0. Use the advance function to find an event
that was accepted by that trigger selection.

The DecReports only contain the decisions, the candidates are stored in the
SelReports ("Hlt{1,2}/SelReports"). Get the Hlt1 SelReports from the event store
and retrieve the one for Hlt1TrackAllL0 using the selReport function,
analogously to how the DecReport was retrieved above.

The SelReports store candidates in a tree of sub-structures, which can be
accessed using the substructure member funtion of a report. Any SelReport has at
least one level of sub-structure. The sub-structure is internally stored in
SmartRefs, which can be derefereced using their "data" method. Let's have a look
at a SelReport for Hlt1TrackAllL0.

~~~ {.python}
reports = evt['Hlt1/SelReports']
report = reports.selReport('Hlt1TrackAllL0Decision')
print report
report.substructure().size()
report.substructure().substructure()[0]
report.substructure().substructure()[0].data()
~~~

In addition to the LHCbIDs, some numbers are also stored, such as the momentum
of the track. These are stored in the numerical info dictionary that can be
retrieved using:

~~~ {.python}
report.substructure().substructure()[0].numericalInfo()
~~~

> ## Plot the transverse momentum distribution {.challenge}
>
> Make a plot of the total and transverse moment distributions of all candidates
> accepted by the Hlt1TrackAllL0 selection.

The LHCbIDs of the (in this case) track can be retrieved using:

~~~ {.python}
report.substructure().substructure()[0].lhcbIDs()
~~~

> ## Subdetector LHCb IDs. {.challenge}
>
> The LHCbIDs can tell you which sub-detector they belong too. Have a
> look at the doxygen to see what methods you can use (hint: find "LHCbID (LHCb)
> in the
> [class index](http://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/davinci/latest_doxygen/classes.html)).
> And get an idea of how many sub-detector hits are on the tracks. Does this match
> what you would expect?
