---
layout: page
title: First steps in LHCb
subtitle: TisTos and what it all means
minutes: 45
---

> ## Learning Objectives {.objectives}
>
> * Learn a bit about the LHCb trigger
> * Learn what TisTos is and why it's useful
> * Use an interactive python session to look at TisTos on a local DST

The LHCb trigger reduces the input rate event rate of approximately 12 MHz
to 12.5 kHz of events that are written to offline tape storage. The rate
reduction is achieved in three steps: L0, HLT1 and HLT2. L0 is implemented in
custom FPGAs, has a fixed latency of 4 μs and a maximum output rate of 1 MHz.

HLT1 and HLT2 are implemented as a software applications that run on the online
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
"tistos.py". There are a few things in the script that we don't need and can be
removed, such as the print_decay and decay finder tools.

Like the stripping, the decisions of the HLT are saved in so-called
DecReports. You can find them in "Hlt/DecReports" and "Hlt2/DecReports", have a
look at what they contain.

~~~ {.python}
evt['Hlt1/DecReports']
evt['Hlt2/DecReports']
~~~

An important difference with the stripping is that for the HLT, all the
decisions are present, even if they are false. Copy the advance function to
advance_hlt, make it work on HLT decisions and make sure it really checks the
decision. This can be done using the "decision" member function of a
DecReport. Note that all names in the reports end with "Decision".

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

Bonus exercise: Make a plot of the transverse momentum distribution of
candidates accepted by the Hlt1TrackAllL0 selection.

The LHCbIDs of the (in this case) track can be retrieved using:

~~~ {.python}
report.substructure().substructure()[0].lhcbIDs()
~~~

Excercise: The LHCbIDs can tell you which sub-detector they belong too. Have a
look at the doxygen to see what methods you can use (hint: find "LHCbID (LHCb)
in the
[class index](http://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/davinci/latest_doxygen/classes.html)). And
get an idea of how many sub-detector hits are on the tracks. Does this match
what you would expect?

The LHCbIDs of the final state particles of the candidate that was created
offline (in the stripping or by your script) can be compared to those saved by
the HLT to find out if the offline candidate was accepted by the trigger. The
classification that results from this comparison is called TisTos (Trigger
independent of Signal/Trigger on Signal).

An offline candidate is considered to be Tos with respect to a trigger selection
if it was accepted by that trigger selection. In more formal terms, if the
LHCbIDs of each of the final state particles of the candidate accepted by the
trigger selection overlap for more than 70% with the LHCbIDs of final state
particles of the offline candidate.

For example, the Hlt1TrackAllL0 line accepts an event if there is at least one
track with a lot of PT and a large IPCHI2. If any of the tracks accepted by the
Hlt1TrackAllL0 line overlap for more than 70% with one of the tracks of the
offline candidate, it is Tos with respect to Hlt1TrackAllL0. If
Hlt1DiMuonHighMass is considered instead, then the LHCbIDs of both tracks that
make-up the Hlt1DiMuonHighMass candidates must overlap with the LHCbIDs of two
tracks that are part of the offline candidate.

To have a look at how this works, we'll use candidates from the
"D2hhPromptDst2D2RS" selection, which can be retrieved thusly:

~~~ {.python}
candidates = evt['AllStreams/Phys/D2hhPromptDst2D2RSLine/Particles']
candidates.size()
~~~

I could be that there are more than one candidates, which are unlikely to all be
"real". MC matching could be used to find the real one when running on
simulation and on data a single candidate can be selected, either randomly or
using some criterium. Dealing with multiple candidates correctly is beyond the
scope of this tutorial, so just always take the first one in the container.

Let's use the TriggerTisTos tool now. In preparation for Run-II, the Hlt1 and
Hlt2 DecReports and SelReports are now stored in different locations. That means
two TisTos tools will be needed, each configured to pick up information from
either HLT1 or HLT2. Since the tools we create are public tools, they have to be
configured in the following way (before the AppMgr is instantiated):

~~~ {.python}
from Configurables import ToolSvc, TriggerTisTos
ToolSvc().addTool(TriggerTisTos, "Hlt1TriggerTisTos")
ToolSvc().Hlt1TriggerTisTos.HltDecReportsLocation = 'Hlt1/DecReports'
ToolSvc().Hlt1TriggerTisTos.HltSelReportsLocation = 'Hlt1/SelReports'
~~~

Create the tools in the same way you created others during the
[other lesson](05-interactive-dst.html), but use instance-specific names that
correspond to the configuration we just added: "Hlt1TriggerTisTos" and
"Hlt2TriggerTisTos". The tools use ITriggerTisTos as an interface.

Use the advance function to find an event that has some candidates for the
chosen selection and set the TisTos tools to use our candidate and trigger selection:

~~~ {.python}
hlt1TisTosTool.setOfflineInput()
candidate = candidates[0]
hlt1TisTosTool.addToOfflineInput(candidate)
hlt1TisTosTool.setTriggerInput()
hlt1TisTosTool.addToTriggerInput("Hlt1TrackAllL0Decision")
result = hlt1TisTosTool.tisTosTobTrigger()
result.tos()
~~~

The "set" calls reset the internal storage of candidate or trigger information,
and the "addTo" calls then add the things we are interested in.

An offline candidate is considered to be Tis with respect to a trigger selection
if removing it from the event would still cause the trigger selection to accept
the event, i.e. if there is another particle in the event that was also
accepted by the trigger selection. In more formal terms, if the LHCbIDs of the
all of the final state particles of any of the candidates accepted by the
trigger selection overlap less than 1% with all of the LHCbIDs of the final
state particles of the offline candidate.

~~~ {.python}
result.tis()
~~~

Note that a candidate can be both Tis and Tos with respect to a trigger
selection, or Tos with respect to one selection, and Tis with respect to
another. To tell the tool to consider more trigger selections, use the following
(regexes are also supported), and try to find some events that are both Tos and
Tis:

~~~ {.python}
hlt1TisTosTool.setTriggerInput()
hlt1TisTosTool.addToTriggerInput("Hlt1TrackAllL0Decision")
hlt1TisTosTool.addToTriggerInput("Hlt1DiMuonHighMassDecision")
result = hlt1TisTosTool.tisTosTobTrigger()
~~~

The (Tos) trigger efficiency of a trigger selection can be calculated as:

eps\_{Tos} = N\_{Tis AND Tos} / N\_{Tis}

Loop over the events in the DST and calculate the efficiency of
Hlt1TrackAllL0. You can add some more Hlt1 selecitons when checking for Tis,
which ones would make sense?

There is a third classification, which is called Tob. This is the case if the
overlap — as defined for Tis and Tos — is between 1% and 70%.

To determine if a candidate is a combination of Tis, Tos and Tob or none of
these, an LHCb software tool has been created that calculates the overlaps and
classifies candidates with respect to trigger selection. This tool is called
TriggerTisTos and it implements the ITriggerTisTos interface.
