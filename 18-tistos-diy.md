---
layout: page
title: First steps in LHCb
subtitle: TisTos DIY
minutes: 45
---

> ## Learning Objectives {.objectives}
>
> * Learn what TisTos is and why it's useful
> * Use an interactive python session to look at TisTos on a local DST

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
D2hhPromptDst2D2RS selection, which can be retrieved thusly:

~~~ {.python}
candidates = evt['AllStreams/Phys/D2hhPromptDst2D2RSLine/Particles']
candidates.size()
~~~

It could be that there are more than one candidates, which are unlikely to all be
real. MC matching could be used to find the real one when running on
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
[other lesson](http://lhcb.github.io/first-analysis-steps/05-interactive-dst.html), but use instance-specific names that
correspond to the configuration we just added: `Hlt1TriggerTisTos` and
`Hlt2TriggerTisTos`. The tools use `ITriggerTisTos` as an interface.

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

The `set` calls reset the internal storage of candidate or trigger information,
and the `addTo` calls then add the things we are interested in.

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

$\epsilon_{\mathrm{Tos}}=N_{\mathrm{Tis}\&\mathrm{Tos}} / {N_{\mathrm{Tis}}}$

Loop over the events in the DST and calculate the efficiency of
Hlt1TrackAllL0. You can add some more Hlt1 selecitons when checking for Tis,
which ones would make sense?

There is a third classification, which is called Tob. This is the case if the
overlap — as defined for Tis and Tos — is between 1% and 70%.

To determine if a candidate is a combination of Tis, Tos and Tob or none of
these, an LHCb software tool has been created that calculates the overlaps and
classifies candidates with respect to trigger selection. This tool is called
TriggerTisTos and it implements the ITriggerTisTos interface.
