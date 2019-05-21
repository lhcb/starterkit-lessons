# Batch submission using HTCondor

{% objectives "Learning Objectives" %}
* Understand some of the basic concepts of batch computing services
* Understand the architecture of the CERN batch system
{% endobjectives %} 

HTCondor is a high throughput computing service. Its purpose is to decide how and when jobs submitted by users should be run using the distributed computing service. It makes these decisions in a way that tries to share the available resources fairly between everyone using the system.

When you submit jobs to HTCondor from lxplus, it adds them to a queue. Once they reach the front of the queue, they will automatically be sent to some of the worker nodes in the distributed computing service. The jobs are then run on the nodes HTCondor assigned them to. As each one completes, any output is returned to the user, along with any errors that occurred.

<!-- Diagram of batch system architecture here! -->

<!-- More about the architecture here - how remote sites can access AFS/EOS/CVMFS(read-only) as well -->

In order to decide where to place your jobs in the queue, HTCondor considers factors including (but not limited to):

* Amount of resources requested - number of CPUs, etc.
* Expected duration of each job
* Quantity, and expected durations, of other users' jobs
* Share of resources allotted to different experiments
* Your priority score

Your priority score is determined based on your recent usage. Submitting lots of intensive jobs will cause your score to increase quickly, but if you wait a little while afterwards it will gradually reduce back down *(N.B. a lower score means your jobs will have a higher priority)*.

### Other useful resources

While putting this lesson together, the following guides, tutorials, and pages proved highly useful, and may be useful to you as well if you need more information:

* [HTCondor v8.8.2 Manual](https://research.cs.wisc.edu/htcondor/manual/v8.8/) - University of Wisconsin-Madison
* [CERN Batch Service User Guide](http://batchdocs.web.cern.ch/batchdocs/index.html)
* [HTCondor User Tutorial](https://indico.cern.ch/event/611296/contributions/2604376/attachments/1471164/2276521/TannenbaumT_UserTutorial.pdf) - Todd Tannenbaum, 6th June 2017


 
