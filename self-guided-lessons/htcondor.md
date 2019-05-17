# Batch submission using HTCondor

{% objectives "Learning Objectives" %}
* Understand some of the basic concepts of batch computing services
* Understand the architecture of the CERN batch system
{% endobjectives %} 

HTCondor is a high throughput computing service. Its purpose is to decide how and when jobs submitted by users should be run using the distributed computing service. It makes these decisions in a way that tries to share the available resources fairly between everyone using the system.

When you submit jobs to HTCondor, it adds them to a queue. Once they reach the front of the queue, they will automatically be sent to some of the worker nodes in the distributed computing service. The jobs are then run on the nodes HTCondor assigned them to. As each one completes, any output is returned to the user, along with any errors that occurred.

The position of your jobs in the queue will depend on factors including (but not limited to):

* Amount of resources requested - number of CPUs, etc.
* Expected duration of each job
* Quantity, and expected durations, of other users' jobs
* Share of resources being used by different experiments
* Your priority score *(discussed more later)*






 
