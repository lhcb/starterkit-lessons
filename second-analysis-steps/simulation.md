# The simulation framework
{% prereq "Requirements" %}

* Know how to run LHCb applications using `lb-run`
* Now how to set up and use `lb-dev`
* Be able to produce nTuples using `MCDecayTreeTuple`

{% endprereq %}

Gauss is LHCb's simulation framework that interfaces various generators to decay engines and then simulate the response of our detector.
This lesson will give a basic introduction to Gauss, focussing on the most common use-cases analysts might encounter. Additional information
and a collection of useful links can be found on the project's website: http://lhcbdoc.web.cern.ch/lhcbdoc/gauss/.

{% objectives "Learning Objectives" %}

* Understand how a signal decay sample is produced in the LHCb framework
* Produce generator level Monte Carlo, print the decay tree and produce nTuples
* Read a DecFile and understand what it produces, including generator level cuts
* Generate an event type with modified channels and generator level cuts
* Raise awareness about the available options for fast simulation

{% endobjectives %}

{% callout "Setup" %}

Before we continue, please set up a Gauss v49r12 `lb-dev`:
* Change your `CMTCONFIG` using this command: `LbLogin -c x86_64-slc6-gcc49-opt`.
* Set up Gauss: `lb-dev Gauss/v49r12`
* `cd ./GaussDev_v49r12`
* Get the `DecFiles` package for later: `git lb-clone-pkg Gen/DecFiles`
* Build it: `make`

Or simply copy and paste the following lines:
```shell
LbLogin -c x86_64-slc6-gcc49-opt
lb-dev Gauss/v49r12
cd ./GaussDev_v49r12
git lb-clone-pkg Gen/DecFiles
make
```
This configures a build environment for that latest `Sim09` version of Gauss.

{% endcallout %}

```eval_rst
.. toctree::
    :maxdepth: 3
    :caption: Contents:

    simulation-intro.md
    simulation-running-gauss.md
    simulation-dkfiles.md
    simulation-gencuts.md
    simulation-advanced-dkfiles.md
    simulation-fastsim.md
```
