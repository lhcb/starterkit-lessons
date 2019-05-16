# Batch submission using HTCondor

{% objectives "Learning Objectives" %}
* Understand some of the concepts of batch computing services
* Be able to submit a simple job to HTCondor
* Learn some techniques for submitting multiple, more complex jobs at once
* Monitor the progress of your jobs, both from the terminal and a browser
{% endobjectives %} 

HTCondor is a high throughput computing service. Its purpose is to decide how and when jobs submitted by users should be run using the distributed computing service. It makes these decisions in a way that tries to share the available resources fairly between everyone using the system.

When you submit jobs to HTCondor, it adds them to a queue. Once they reach the front of the queue, they will automatically be sent to some of the worker nodes in the distributed computing service. The position of these jobs in the queue depends on factors including (but not limited to):

* Resources requested - number of CPUs, etc.
* Expected duration
* Quantity, and expected durations, of other users' jobs
* Share of resources being used by different experiments
* Your priority score *(discussed more later)*

The jobs are then run on the nodes they were assigned to, and the output is returned to the user, along with any errors and a log of what took place.
