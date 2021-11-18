# Batch submission using HTCondor

{% callout "LHCb fair-usage of Grid resources" %}

LHCb has a policy document on the [fair-usage of Grid computing resources](https://twiki.cern.ch/twiki/pub/LHCb/LHCbNationalComputingBoard/LHCb_Grid_usage_policy.pdf), which explains how job resource accounting information are monitored by DIRAC. This enables the fair-usage policy to be enforced automatically at the job submission stage, but only if those jobs were submitted using DIRAC (i.e. via `ganga`). By submitting jobs to the Grid this way, you can be sure that you are using the resources fairly. 

Computing nodes used in the CERN LXBATCH system are effectively part of LHCb Grid resources. However, using HTCondor to submit jobs to these nodes (e.g. on lxplus) will not take the fair-usage policy into account, and so should not be used!

You may still like to read this tutorial, as your home institute may have its own HTCondor batch system.

{% endcallout %}


{% objectives "Learning Objectives" %}

* Understand some of the basic concepts of batch computing services
* Understand the architecture of the CERN batch system

{% endobjectives %}

HTCondor is a high throughput computing service. Its purpose is to decide how and when jobs submitted by users should be run using the distributed computing service. It makes these decisions in a way that tries to share the available resources fairly between everyone using the system.

When you submit jobs to HTCondor from lxplus, it adds them to a queue. Once they reach the front of the queue, they will automatically be sent to some of the worker nodes in the distributed computing service. The jobs are then run on the nodes HTCondor assigned them to. As each one completes, any output is returned to the user, along with any errors that occurred.

In order to decide where to place your jobs in the queue, HTCondor considers factors including (but not limited to):

* Amount of resources requested - number of CPUs, etc.
* Expected duration of each job
* Quantity, and expected durations, of other users' jobs
* Share of resources allotted to different experiments
* Your priority score

Your priority score is determined based on your recent usage. Submitting lots of intensive jobs will cause your score to increase quickly, but if you wait a little while afterwards it will gradually reduce back down *(N.B. a lower score means your jobs will have a higher priority)*.

```eval_rst
.. toctree::
    :maxdepth: 3
    :caption: Contents:

    htcondor-first-job.md
    htcondor-more-options.md
    htcondor-queue.md
```

## System architecture

When you connect to lxplus, you have read/write access to both the AFS and EOS filesystems, as well as read-only access to CVMFS for accessing experiment software. Worker notes in the batch system have the same permissions, which is what allows them to read input files from, and write output files to, your user areas.

Each worker node has its own remote storage space, which is limited to 20GB. When a job starts, the executable is copied to this storage area, usually along with any input data files required. Any files created by the script should be created there as well, where they can later be transferred back to your user area. When a job completes, the worker node cleans out its storage area, so it's important to tell HTCondor to transfer any output files you want to keep.

See [here](https://batchdocs.web.cern.ch/concepts/dataflow.html) for a more detailed explanation (and a helpful diagram).

## Other useful resources

While putting this lesson together, the following guides, tutorials, and pages proved highly useful, and may be useful to you as well if you need more information:

* [HTCondor v8.8.2 Manual](https://research.cs.wisc.edu/htcondor/manual/v8.8/) - University of Wisconsin-Madison
* [CERN Batch Service User Guide](https://batchdocs.web.cern.ch/index.html)
* [HTCondor User Tutorial](https://indico.cern.ch/event/611296/contributions/2604376/attachments/1471164/2276521/TannenbaumT_UserTutorial.pdf) - Todd Tannenbaum, 6th June 2017
* [Mattermost](https://mattermost.web.cern.ch/lhcb) - you can join the 'condor' and 'Distributed Analysis' channels
* [lhcb-distributed-analysis@cern.ch](mailto:lhcb-distributed-analysis@cern.ch) mailing list
