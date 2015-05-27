---
layout: page
title: First steps in LHCb
subtitle: TisTos and what it all means
minutes: 10
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

For example, the Hlt1TrackMVA line accepts and event if there is at least one
track with a lot of PT, a large IPCHI2, or a combination of these. If any of the
tracks accepted by the Hlt1TrackMVA line overlap for more than 70% with one of
the tracks of the offline candidate, it is Tos with respect to Hlt1TrackMVA. If
Hlt1DiMuonHighMass is considered instead, then the LHCbIDs of both tracks that
make-up the Hlt1DiMuonHighMass candidates must overlap with the LHCbIDs of two
tracks that are part of the offline candidate.

An offline candidate is considered to be Tis with respect to a trigger selection
if removing it from the event would still cause the trigger selection to accept
the event, i.e. if there is another particle in the event that was also
accepted by the trigger selection. In more formal terms, if the LHCbIDs of the
all of the final state particles of any of the candidates accepted by the
trigger selection overlap less than 1% with all of the LHCbIDs of the final
state particles of the offline candidate.

Note that a candidate can be both Tis and Tos with respect to a trigger
selection, or Tos with respect to one selection, and Tis with respect to
another.

The (Tos) trigger efficiency of a trigger selection can be calculated as:
eps\_{Tos} = N\_{Tis AND Tos} / N\_{Tos}

There is a third classification, which is called Tob. This is the case if the
overlap — as defined for Tis and Tos — is between 1% and 70%.

To determine if a candidate is a combination of Tis, Tos and Tob or none of
these, an LHCb software tool has been created that calculates the overlaps and
classifies candidates with respect to trigger selection. This tool is called
TriggerTisTos and it implements the ITriggerTisTos interface.

We'll now learn how to use this tool to compare candidates with candidates saved
by the HLT in Run I.
