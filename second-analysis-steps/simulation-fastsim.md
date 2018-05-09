{% objectives "Learning Objectives" %}
* Learn about existing fast simulation options
{% endobjectives %} 

# Why fast simulation is crucial

Our production queues are beyond full and nominal simulation for all requests is unsustainable for the future.
Using fast simulation or being smart about how you use simulated samples is unavoidable. Fortunately,
some fast simulation options already exist which are ready to be used and a very brief overview is given
below (excluding options still in development).

In general, Gauss is a very flexible and extensible framework and if you require a large simulated sample it is useful
to think about whether some change to the configuration, a reordering of steps or the event loop might net large
speed-ups basically for free.

## Speeding up the detector simulation

### Reduced detector geometry
It is possible to turn off parts of the detector that might not be necessary for the specific analysis.
Most prominently, the RICH detectors propagation of the optical photons can be deactivated which reduces
the overall time required to simulate an event by 30%. More aggressively, all but the tracking system can be turned of, netting an
increase in speed by about a factor of 10. However, the latter severely impacts the usability of the samples.

### Particle gun
Instead of generating a full Pythia event, a single signal particle is spawned whose kinematics can be configured
to follow various different distribution (with distributions as in Pythia8 available). The particle is then decayed
using EvtGen to the desired final state. The overall increase in speed is about a factor of 50. However, resolutions and efficiencies
are usually too good (compared to the full simulation) as the detector occupancy is much lower.

### ReDecay
Allows to re-use the underlying event but generates and simulates new signal decays every time. Depending on the studied signal decay multiplicity,
speed-ups by a factor between 10 and 20 are typically seen. Additionally, the same precision as in the nominal simulation is reached.
However, as the underlying event and the kinematics of the signal particle remain unchanged, correlations between different events are introduced whose
effect depends on the studied observables and need to be handled with care if significant.

## Speeding up the generation

### Split generator level cuts
Imagine you are studying the decay of a signal particle that is relatively rarely produced in Pythia 8 minimum bias events. In addition you apply tight generator level
cuts which do not depend on what Pythia did (i.e. Lorentz-invariant quantities such as some invariant mass combination of some children). We have the ability
to split the generator cuts: One part is applied as usual, triggering a reset of the generation phase if not passed. The other part is applied immediately after
EvtGen generated the signal decay. If the generated decay fails (e.g. the invariant mass combination of two children is below a threshold), the decay products are
removed and a new decay is generated until the cuts are passed. This avoids rerunning Pythia unnecessarily and can lead to substantial CPU savings when studying rare
particles (e.g. $$\Lambda_b$$).
