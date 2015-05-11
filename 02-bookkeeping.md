---
layout: page
title: First Steps in LHCb
subtitle: Finding data to process
minutes: 10
---
> ## Learning Objectives {.objectives}
>
> * Find MC in the book-keeping
> * Find data in the book-keeping
> * Find the decay you want
> * check reco
> * ignore what (flagged) stripping is used
> * run over those files with the strip21 module you want
> * nTuples!

To feed simulated and/or real data to your DaVinci job you first
need to locate some events. In this tutorial we will use the
decay D* -> D(->K pi)pi as an example.

Navigate to the [book-keeping](https://lhcb-portal-dirac.cern.ch/DIRAC/?view=tabs&theme=Grey&url_state=1|*LHCbDIRAC.BookkeepingBrowser.classes.BookkeepingBrowser:,) which lets you find both simulated
and real data.

At the bottom of the "Bookkeeping tree" tab there is a dropdown menu
labelled `Simulation Condition`, open it and change it to `Event
type`.

We will analyse 2012 data, and correspondigly use simulation for 2012
data. To navigate to the simulation expand the folder icon in the
"Bookkeeping tree" window. Navigate to the `MC/2012` folder. This will
give you a very long list of all possible decay types for which there
is simulated data. We are looking for a folder which is named
`27163003 (Dst_D0pi,Kpi=DecProdCut)`. The number is a numerical
representation of the [event
type](https://cds.cern.ch/record/855452?ln=en).  The text is the human
readable version of that.

This sample of simulated events will only contain decays of the type
D* -> D(-> K pi)pi.

If you expand the `27163003 (Dst_D0pi,Kpi=DecProdCut)` folder you
will find a list of different configurations depending on the
magnet polarity (`MagDown` and `MagUp`) as well as different
versions of `Pythia` used to generate the events. We will use
`Beam4000GeV-2012-MagDown-Nu2.5-Pythia8`.

Next you need to choose what version of the simulation you want to
use. Usually you want to choose the latest one available. You will
also have to choose the version of the digitisation, what
configuration of the trigger and reconstruction version you want to
have in your simulated sample. However usually there is only one
choice for these. So go with that.

You will also have to select a version of the stripping. Here simply
choose any as long as it contains the word `Flagged`.

After all this you will be presented with a `ALLSTREAMS.DST` entry. By
clicking on it you finally get a list of files that you can
process. At the bottom right of the page there is a `Save` button
which will let you download a file specifying the inputs that you can
use for running your DaVinci job. Click it and select `Save as a
python file`. Clicking `Save` once again in the popupmenu will start a
download. Save this file somewhere you can find it again.

> ## Shortcut {.callout}
>
> Once you get a bit of experience with navigating the book-keeping you
> can take a shortcut! At the bottom of your browser window there is a
> text field next to a green "plus" symbol. You can directly enter a
> path here to navigate there directly. For example you could go
> straight to:
> `evt+std://MC/2012/27163003/Beam4000GeV-2012-MagDown-Nu2.5-Pythia8/`
> by typing this path and pressing the `Go` button.

