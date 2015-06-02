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
