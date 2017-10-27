# Running a minimal DaVinci job locally

[Looping event-by-event](interactive-dst.html) over a file and [inspecting
interesting quantities with LoKi functors](loki-functors.html) is great for
exploration: to checking that the file contains the candidates you need, that
the topology makes sense, and so on.
It's impractical for most cases, though, where you want _all_ the candidates
your trigger/stripping line produced, which could be tens of millions of
decays.
In these cases we use DaVinci, the application for analysing high-level
information such as tracks and vertices, which we'll look at in this lesson to
produce a ROOT ntuple.

{% objectives "Learning Objectives" %}
* Run a DaVinci job over a local DST
* Inspect the ntuple output
* Set up the job to run in Ganga
{% endobjectives %} 

With some stripped data located, it's useful to store the information on the
selected particles inside an ntuple.
This allows for quick, local analysis with [ROOT](https://root.cern.ch/),
rather than always searching through a DST that contains lots of things we're
not interested in.

As well as being the application that runs the stripping,
[DaVinci](http://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/davinci)
allows you to access events stored in DSTs and copy the information to ROOT
ntuples.
You tell DaVinci what you want it to do through _options files_, written in
Python.
There are lots of things you can do with DaVinci options files, as there's lots
of information available on the DST, but for now we'll just work on getting the
bare essentials up and running.

Our main tool will be the `DecayTreeTuple` object, which we'll create inside a
file we will call `ntuple_options.py`:

```python
from Configurables import DecayTreeTuple
from DecayTreeTuple.Configuration import *

# Stream and stripping line we want to use
stream = 'AllStreams'
line = 'D2hhPromptDst2D2KKLine'

# Create an ntuple to capture D*+ decays from the StrippingLine line
dtt = DecayTreeTuple('TupleDstToD0pi_D0ToKK')
dtt.Inputs = ['/Event/{0}/Phys/{1}/Particles'.format(stream, line)]
dtt.Decay = '[D*(2010)+ -> (D0 -> K- K+) pi+]CC'
```

This imports the `DecayTreeTuple` class, and then creates an object called
`dtt` representing our ntuple.
Once DaVinci has run, the resulting ntuple will be saved in a folder within the
output ROOT file called `TupleDstToD0pi_D0ToKpi`.

The `Inputs` attribute specifies where `DecayTreeTuple` should look for
particles, and here we want it to look at the output of the stripping line
we're interested in.

As stripping lines can save many decays to a DST, the `Decay` attribute
specifies what decay we would like to have in our ntuple.
If there are no particles at the `Input` location, or the `Decay` string
doesn't match any particles at that location, the ntuple will not be filled.

{% callout "Decay descriptors" %}
There is a special syntax for the `Decay` attribute string, commonly called
'decay descriptors', that allow a lot of flexibility with what you accept.
For example, `D0 -> K- X+` will match any D0 decay that contains one
negatively charged kaon and one positively charged track of any species.
More information the decay descriptor syntax can be found on the [LoKi decay
finders TWiki
page](https://twiki.cern.ch/twiki/bin/view/LHCb/FAQ/LoKiNewDecayFinders).
{% endcallout %} 

Now we need to tell DaVinci how to behave.
The `DaVinci` class allows you to tell DaVinci how many events to run over,
what type of data is being used, what algorithms to run over the events, and so
on.

There are [many configuration
attributes](http://lhcb-release-area.web.cern.ch/LHCb-release-area/DOC/davinci/releases/v41r2/doxygen/py/dc/d2f/class_da_vinci_1_1_configuration_1_1_da_vinci.html#ac788f6a80f5f61d47056debe7b86ca71)
defined on the `DaVinci` object, but we will only set the ones that are
necessary for us.

```python
from Configurables import DaVinci

# Configure DaVinci
DaVinci().UserAlgorithms += [dtt]
DaVinci().InputType = 'DST'
DaVinci().TupleFile = 'DVntuple.root'
DaVinci().PrintFreq = 1000
DaVinci().DataType = '2016'
DaVinci().Simulation = True
# Only ask for luminosity information when not using simulated data
DaVinci().Lumi = not DaVinci().Simulation
DaVinci().EvtMax = -1
DaVinci().CondDBtag = 'sim-20170721-1-vc-mu100'
DaVinci().DDDBtag = 'dddb-20171005-3'
```

Nicely, a lot of the attributes of the `DaVinci` object are self-explanatory:
`InputType` should be `'DST'` when giving DaVinci DST files; `PrintFreq`
defines how often DaVinci should print its status; `DataType` is the year of
data-taking the data corresponds to, which we get from looking at the
bookkeeping path used to get the input DST; `Simulation` should be `True` when
using Monte Carlo data, `Lumi` defines whether to store information on the
integrated luminosity the input data corresponds to; and `EvtMax` defines how
many events to run over, where a value of `-1` means "all events".

The `CondDBtag` and `DDDBtag` attributes specify the exact detector conditions
that the Monte Carlo was generated with.
Specifying these tags is important, as without them you can end up with the
wrong magnet polarity value in your ntuple, amongst other Bad Things.
You can find the values for these tags in the [bookkeeping
file](data/MC_2012_27163003_Beam4000GeV2012MagDownNu2.5Pythia8_Sim08e_Digi13_Trig0x409f0045_Reco14a_Stripping20NoPrescalingFlagged_ALLSTREAMS.DST.py)
we downloaded earlier.

{% callout "Database tags" %}
Generally, the `CondDB` and `DDDB` tags are different for each dataset you
want to use, but will be the same for all DSTs within a given dataset.
When using simulated data, *always* find out what the database tags are for
your dataset!
For real collision data, you shouldn't specify these tags, as the default
tags are the latest and greatest, so just remove those lines from the options
file.
{% endcallout %} 

In order to run an algorithm that we have previously created, we need to add it
to the `UserAlgorithms` list.
The `TupleFile` attribute defines the name of the ROOT output file that DaVinci
will store any algorithm output in, which should be our ntuple.

{% callout "Being smart and efficient" %}
Typical stripping lines take only a small part of the stripped stream - so, a small fraction of events in the DST: actually, usually you care about a single TES location!
At the same time, event unpacking and running the DecayTreeTuple machinery for each event is time-consuming. 
Consequently, DSTs can be processed much faster if before unpacking we select *only* events which are likely to accomodate the desired TES location. This can be achieved, for example, by requiring a prefilter checking whether event passes a stripping requirement. You may also filter on trigger decisions - this is an idea behind the Turbo stream.
As a conclusion, it is *strongly* recommended to exploit the `EventPreFilters` method offered by `DaVinci`: this feature can save a lot of processing time and collaboration's computing resources when running over millions of events.
To require events to pass a specific stripping line requirement, one should add these lines to the options file:
```python
from PhysConf.Filters import LoKi_Filters
fltrs = LoKi_Filters (
    STRIP_Code = "HLT_PASS_RE('StrippingD2hhPromptDst2D2KKLineDecision')"
)
DaVinci().EventPreFilters = fltrs.filters('Filters')
```
Here we use the [LoKi functor `HLT_PASS_RE`](http://lhcb-doxygen.web.cern.ch/lhcb-doxygen/davinci/latest/d7/dae/namespace_lo_ki_1_1_cuts.html#aee4bba9ae8443acd970dd52e20e5b8c1) which checks for a positive decision on (in this case) the stripping line. 
You may investigate some of more advanced examples of `EventPreFilters` usage [here](https://twiki.cern.ch/twiki/bin/view/LHCb/FAQ/DaVinciFAQ#How_to_process_the_stripped_DSTs) and [here](https://gitlab.cern.ch/lhcb/Phys/blob/master/Phys/PhysConf/python/PhysConf/Filters.py).
{% endcallout %} 

All that's left to do is to say what data we would like to run over.
As we already have a data file [downloaded locally](files-from-grid.html), we
define that as our input data.

```python
from GaudiConf import IOHelper

# Use the local input data
IOHelper().inputFiles([
    './00062514_00000001_7.AllStreams.dst'
], clear=True)
```

This says to use the `.dst` file that is in the same directory as the options
file, and to clear any previous input files that might have been defined.

That's it! We're ready to run DaVinci.

In the same folder as your options file `ntuple_options.py` and your DST file
ending in `.dst`, there's just a single command you need run on `lxplus`.

```shell
$ lb-run DaVinci/v42r6p1 gaudirun.py ntuple_options.py
```

The full options file we've created, `ntuple_options.py`, is [available
here](./code/minimal-dv/ntuple_options.py).
A slightly modified version that uses remote files (using an XML catalog 
[described here](files-from-grid.html)) is [available
here](./code/minimal-dv/ntuple_options_xmlcatalog.py)

{% callout "Using a microDST" %}
A microDST (or µDST) is a smaller version of a DST.
Some stripping lines go to µDSTs, and some go to DSTs.
There are two things that need changing in our options file in order to have
it work when it is used with a stripping line that goes to a µDST:

1. The `DecayTreeTuple.Inputs` attribute should start at the word
   `Phys`; and
2. The `RootInTES` attribute on the `DaVinci` object has to be set to
   `/Event/$STREAM`

In context, the changes look like

```python
dtt.Inputs = ['Phys/{0}/Particles'.format(line)]
# ...
DaVinci().RootInTES = '/Event/{0}'.format(stream)
```
{% endcallout %} 
