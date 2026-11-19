{% objectives "Learning Objectives" %}

* Know how to request an official (centrally produced) simulation sample for your analysis
* Know who submits a request, where it goes, and where the up-to-date documentation lives

{% endobjectives %}

# Requesting official MC samples

Running Gauss yourself, as in the previous lessons, is fine for tests and generator-level studies.
The simulated samples for an analysis are produced centrally on the Grid, and practically every analysis
requests its own samples (signal, then backgrounds and control modes) with an **MC request**.

Each Physics Working Group has one or two **MC liaisons** (see the
[list of WG liaisons](https://twiki.cern.ch/twiki/bin/viewauth/LHCbPhysics/LHCbWGLiaisons)). They review and approve every request, so
get to know yours early. Contact them before preparing a request if a feature you need is marked ⚠️ in
the [capabilities tables](https://lhcb-simulation.web.cern.ch/WPP/lbmcsubmit-capabilities/).

The normal path is:

1. **Prepare the DecFiles.** Each sample is identified by an 8-digit event type defined by a DecFile
   (see the previous lessons and the [DecFiles viewer](https://lhcb-decfiles.docs.cern.ch/)).
   A new DecFile must be merged *and released* before it can be requested: an open merge request is not enough.
2. **Write a short "Stage 0" YAML file.** It says which event types you want, how many events, and in which
   conditions (years or periods, simulation version, trigger/Sprucing, output format, ...).
   Check the [capabilities tables](https://lhcb-simulation.web.cern.ch/WPP/lbmcsubmit-capabilities/) for what is
   available in the periods you need.
3. **Give the Stage 0 file to your MC liaison.** They open a merge request in
   [mc-requests](https://gitlab.cern.ch/lhcb-simulation/mc-requests), where the CI expands the file into the full
   request and runs small test productions. The liaison and the WG conveners then approve it, and merging submits
   the production.
4. **Follow the production** in the GitLab issue that is opened automatically when the request is submitted.

{% callout "DecFile vs Stage 0" %}

The DecFile describes the *physics*: what is generated and how it decays, including generator-level cuts.
The Stage 0 file is the *order*: how many events of which event types, under which conditions.
The event type number links the two.

{% endcallout %}

{% callout "mc-requests, not lbmcsubmit" %}

[LbMCSubmit](https://gitlab.cern.ch/lhcb-simulation/lbmcsubmit) is the *tool* that turns a
Stage 0 file into a production request: it is the backend of mc-requests.
Requests never go to the lbmcsubmit repository: merge requests are opened in
[mc-requests](https://gitlab.cern.ch/lhcb-simulation/mc-requests).

{% endcallout %}

## A minimal Stage 0 file

This illustrative example asks for two event types in 2016 conditions:

```yaml
sim-version: 10
name: Ds2KKpi
inform:
  - auser
WG: Charm

samples:
  - event-types:
      - 23103005
      - 23103006
    data-types:
      - 2016
    num-events: 100_000
```

Defaults (generator, polarities, output format, ...) change between LbMCSubmit releases, and Run 3 requests need
extra options (for example an explicit `file-format`). Do not copy this example blindly: take the syntax from
the documentation below.

## Where to find the details

* [Run 3 MC samples](https://lhcb-simulation.web.cern.ch/WPP/MCsamples/): the different samples that can be requested for Run 3
* [What can I request with LbMCSubmit?](https://lhcb-simulation.web.cern.ch/WPP/lbmcsubmit-capabilities/): available options per data-taking period
* [Request preparation guide](https://lhcb-simulation.web.cern.ch/liaisons/): Stage 0 examples, filtering, Sprucing and the approval workflow
* [LbMCSubmit YAML schema reference](https://lhcb-simulation.web.cern.ch/WPP/schema-reference/): all fields and allowed values
* [DecFiles viewer](https://lhcb-decfiles.docs.cern.ch/): find existing event types
* [List of WG liaisons](https://twiki.cern.ch/twiki/bin/viewauth/LHCbPhysics/LHCbWGLiaisons): find the MC liaison of your WG
