# The simulation framework
{% prereq "Requirements" %}
* Know how to run LHCb application using `lb-run`
* Now how to set up and use `lb-dev`
* Produce nTuples using an `MCDecayTreeTuple`
{% endprereq %} 

Gauss is LHCb's simulation framework that interfaces various generators to decay engines and allows to simulate the response of our detector.
This lesson will give a basic introduction to Gauss, focussing on the most common use-cases analysts might encounter. Additional information
and a collection of useful links can be found on the project's website: http://lhcbdoc.web.cern.ch/lhcbdoc/gauss/.

{% objectives "Learning Objectives" %}
* Understand how a signal decay sample is produced in the LHCb framework
* Produce generator level Monte Carlo, print the decay tree and produce nTuples
* Read a DecFile and understand what it produces, including generator level cuts
* Generate a sample without/with modified generator cuts
* Awareness about existing fast simulation options
{% endobjectives %} 

{% callout "Setup" %}
Before we continue, please set up a Gauss v49r9 `lb-dev`:
* Change your `CMTCONFIG` to `x86_64-slc6-gcc49-opt`.
* Get the `DecFiles` package for later: `git lb-clone-pkg  Gen/DecFiles`
* Build it!
{% endcallout %} 
