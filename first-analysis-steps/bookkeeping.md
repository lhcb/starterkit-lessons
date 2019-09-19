# Finding data in the Bookkeeping

Knowing how data flows through the various Gaudi applications is crucial for 
knowing where to look for your data.

Data are catalogued in ‘the bookkeeping’, and are initially sorted in broad 
groups such as ‘real data for physics analysis’, ‘simulated data’, and ‘data 
for validation studies’.
After this, a tree of various application and processing versions will 
eventually lead to the data you need.

So, before we can run our first DaVinci job we need to locate some events. In 
this tutorial we will use the decay $$D^{* +} \to D^{0}\pi^{+}$$ as an example, 
where the $$D^{0}$$ decays to $$K^{-} K^{+}$$.

{% objectives "Learning Objectives" %}
* Find MC in the bookkeeping
* Find data in the bookkeeping
* Find the decay you want
{% endobjectives %}

Navigate to the 
[bookkeeping](https://lhcb-portal-dirac.cern.ch/DIRAC/) 
under Data / Bookeeping Browser, which lets you find both simulated
and real data.

At the bottom of the "Bookkeeping tree" tab there is a drop-down menu
labelled `Simulation Condition`, open it and change it to `Event
type`. This changes the way the bookkeeping tree is sorted, making it easier for us to locate files by event.

We will analyse 2016 data, and correspondingly use simulation for 2016
data. To navigate to the simulation, expand the folder icon in the
"Bookkeeping tree" window. Navigate to the `MC/2016` folder. This will
give you a very long list of all possible decay types for which there
is simulated data. We are looking for a folder which is named
`27163002 (Dst_D0pi,KK=DecProdCut)`. The number is a numerical
representation of the [event
type](https://cds.cern.ch/record/855452?ln=en).  The text is the human
readable version of that.

This sample of simulated events will only contain events where a $$D^{* +} \to 
D^{0}(\to K^{-}K^{+})\pi^{+}$$ was generated within the LHCb acceptance, 
although the decay might not have been fully reconstructed. (Not all simulated 
samples have the same requirements made on the signal decay.)

If you expand the `27163002 (Dst_D0pi,KK=DecProdCut)` folder you
will find a couple different subfolders to choose from. The names of these 
subfolders correspond to different data-taking conditions, such as magnet 
polarity (`MagDown` and `MagUp`), as well as different software versions used 
to create the samples that are available. We will use 
`Beam6500GeV-2016-MagDown-Nu1.6-25ns-Pythia8`.

{% callout "So much choice!" %}
Often there are only one or two combinations of data-taking conditions and 
software versions to choose from, but sometimes there can be very many.
Generally newer versions are the best bet, but you should always ask the 
Monte Carlo liason of your working group for advice on what to use if you're 
not sure.
{% endcallout %}

Next we need to choose what version of the simulation you want to
use. There are two available in our case, `Sim09b` and `Sim09c`, but usually the latest available version is the best when there are more than one.
We also have to choose what configuration of the trigger (`Trig0x6138160F` in our case) and reconstruction
we want to have in the simulated sample. Usually
there is only one choice for these, which makes choosing easier.

Two final steps are versions of `Turbo` (only for Run 2 samples, as discussed in the [previous lesson](run-2-data-flow.html)) and `Stripping`. In order to make the samples as useful as possible for the largest number of analyses, each of the Run 2 samples is processed via both Turbo and Stripping frameworks, so it is usable for either of two choices.
Usually, there is only one version of `Turbo`, but there can be multiple versions of the `Stripping`. Choose any as long as it contains the word `Flagged`.

{% callout "Flagged and filtered samples" %}
In the usual data-taking flow, the trigger and stripping are run in 
*filtering* mode, whereby events that don't pass any trigger line or any 
stripping line are thrown away. In the simulation, it's often useful to keep 
such events so that the properties of the rejected events can be studied.
The trigger and stripping are then run in _flagging_ mode, such that the 
decisions are only recorded for later inspection.
Filtered Monte Carlo can be produced for analyses that need lots of events.
{% endcallout %}

After all this, you will be presented with a `ALLSTREAMS.DST` entry. By
clicking on it we finally see a list of files that we can
process. At the bottom right of the page there is a “Save” button
which will let us download a file specifying the inputs that we'll use for 
running our DaVinci job. Click it, select “Save as a
python file”, and add `.py` to the end of the text in “Save As...”. Clicking “Save” once again in the pop-up menu will start the
download. Save this file somewhere you can find it again.

A copy of the file we just downloaded is [available
here](data/MC_2016_27163002_Beam6500GeV2016MagDownNu1.625nsPythia8_Sim09c_Trig0x6138160F_Reco16_Turbo03_Stripping28r1NoPrescalingFlagged_ALLSTREAMS.DST.py).

{% callout "Shortcut" %}
Once you get a bit of experience with navigating the bookkeeping you 
can take a shortcut! At the bottom of your browser window there is a 
text field next to a green "plus" symbol. You can directly enter a 
path here to navigate there directly. For example you could go 
straight to:
`evt+std://MC/2016/27163002/Beam6500GeV-2016-MagDown-Nu1.6-25ns-Pythia8/`
by typing this path and pressing the `Go` button.
{% endcallout %}

{% challenge "Find your own decay!" %}
Think of a decay and try to find a Monte Carlo sample for it. You could use 
the decay that your analysis is about, or if you don't have any ideas you 
could look for the semileptonic decay $$\Lambda_{b}^{0} \to 
\Lambda_{c}^{+}\mu^{-}\bar{\nu}_{\mu}$$, where the $$\Lambda_{c}^{+}$$ decays 
to $$pK^{-}\pi^{+}$$.
{% endchallenge %}

If you would like to find out more about how the event types define the 
signal decay, you can look at the [documentation for the DecFiles 
package](http://lhcbdoc.web.cern.ch/lhcbdoc/decfiles/).
