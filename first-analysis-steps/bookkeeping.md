# Finding data in the Bookkeeping
Knowing how data flows through the various Gaudi applications is crucial for 
knowing where to look for your data.

Data are catalogued in ‘the bookkeeping’, and are initially sorted in broad 
groups such as ‘real data for physics analysis’, ‘simulated data’, and ‘data 
for validation studies’.
After this, a tree of various application and processing versions will 
eventually lead to the data you need.

So, before we can run our first DaVinci job we need to locate some events. In 
this tutorial we will use the decay $$D^{*+} \to D^{0}\pi^{+}$$ as an example, 
where the $$D^{0}$$ decays to $$K^{-}\pi^{+}$$.

{% objectives "Learning Objectives" %}
* Find MC in the bookkeeping
* Find data in the bookkeeping
* Find the decay you want
* check reco
* ignore what (flagged) stripping is used
* run over those files with the strip21 module you want
* nTuples!
{% endobjectives %}

Navigate to the 
[bookkeeping](https://lhcb-portal-dirac.cern.ch/DIRAC/?view=tabs&theme=Grey&url_state=1|*LHCbDIRAC.BookkeepingBrowser.classes.BookkeepingBrowser:,) 
which lets you find both simulated
and real data.

At the bottom of the "Bookkeeping tree" tab there is a drop-down menu
labelled `Simulation Condition`, open it and change it to `Event
type`.

We will analyse 2012 data, and correspondingly use simulation for 2012
data. To navigate to the simulation, expand the folder icon in the
"Bookkeeping tree" window. Navigate to the `MC/2012` folder. This will
give you a very long list of all possible decay types for which there
is simulated data. We are looking for a folder which is named
`27163003 (Dst_D0pi,Kpi=DecProdCut)`. The number is a numerical
representation of the [event
type](https://cds.cern.ch/record/855452?ln=en).  The text is the human
readable version of that.

This sample of simulated events will only contain events where a $$D^{*+} \to 
D^{0}(\to K^{-}\pi^{+})\pi^{+}$$ was generated within the LHCb acceptance, 
although the decay might not have been fully reconstructed. (Not all simulated 
samples have the same requirements made on the signal decay.)

If you expand the `27163003 (Dst_D0pi,Kpi=DecProdCut)` folder you
will find several different subfolders to choose from. The names of these 
subfolders correspond to different data-taking conditions, such as magnet 
polarity (`MagDown` and `MagUp`), as well as different software versions used 
to create the different samples that are available. We will use 
`Beam4000GeV-2012-MagDown-Nu2.5-Pythia8`.

{% callout "So much choice!" %}
Often there are only one or two combinations of data-taking conditions and 
software versions to choose from, but sometimes there can be very many.
Generally newer versions are the best bet, but you should always ask the 
Monte Carlo liason of your working group for advice on what to use if you're 
not sure.
{% endcallout %}

Next we need to choose what version of the simulation you want to
use. Usually the latest available version is best, which is `Sim08e` in
this case. We also have to choose the version of the
digitisation and what configuration of the trigger and reconstruction
we want to have in the simulated sample. Usually
there is only one choice for these, which makes choosing easier.

We also have to select a version of the stripping. Choose any as long as it 
contains the word `Flagged`.

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
running our DaVinci job. Click it and select “Save as a
python file”. Clicking “Save” once again in the pop-up menu will start the
download. Save this file somewhere you can find it again.

A copy of the file we just downloaded is [available
here](data/MC_2012_27163003_Beam4000GeV2012MagDownNu2.5Pythia8_Sim08e_Digi13_Trig0x409f0045_Reco14a_Stripping20NoPrescalingFlagged_ALLSTREAMS.DST.py).

{% callout "Shortcut" %}
Once you get a bit of experience with navigating the bookkeeping you 
can take a shortcut! At the bottom of your browser window there is a 
text field next to a green "plus" symbol. You can directly enter a 
path here to navigate there directly. For example you could go 
straight to: 
`evt+std://MC/2012/27163003/Beam4000GeV-2012-MagDown-Nu2.5-Pythia8/` 
by typing this path and pressing the `Go` button.
{% endcallout %}

{% challenge "Find your own decay!" %}
Think of a decay and try to find a Monte Carlo sample for it. You could use 
the decay that your analysis is about, or if you don't have any ideas you 
could look for the semileptonic decay $$\Lambda_{b}^{0} \to 
\Lambda_{c}^{+}\mu^{-}\bar{\nu}_{\mu}$$, where the $$\Lambda_{c}^{+}$$ decays 
to $$pK^{-}\pi^{+}$$.

If you would like to find out more about how the event types define the 
signal decay, you can look at the [documentation for the DecFiles 
package](http://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/decfiles/).
{% endchallenge %}
