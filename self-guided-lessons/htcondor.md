# Batch submission using HTCondor

{% objectives "Learning Objectives" %}
* Understand some of the basic concepts of batch computing services
* Be able to submit a simple job to HTCondor
* Monitor the progress of your jobs, both from the terminal and a browser
{% endobjectives %} 

HTCondor is a high throughput computing service. Its purpose is to decide how and when jobs submitted by users should be run using the distributed computing service. It makes these decisions in a way that tries to share the available resources fairly between everyone using the system.

When you submit jobs to HTCondor, it adds them to a queue. Once they reach the front of the queue, they will automatically be sent to some of the worker nodes in the distributed computing service. The jobs are then run on the nodes HTCondor assigned them to. As each one completes, any output is returned to the user, along with any errors that occurred.

The position of your jobs in the queue will depend on factors including (but not limited to):

* Amount of resources requested - number of CPUs, etc.
* Expected duration of each job
* Quantity, and expected durations, of other users' jobs
* Share of resources being used by different experiments
* Your priority score *(discussed more later)*

### Submitting a simple job

To start with, you'll need to create an executable for the job you want to run. Create a very simple shell script in your user area on lxplus: in `my_first_script.sh`, add the following lines:

```
#!/bin/bash
echo "Hello world!"
```

Next, you need to make this script executable by running `chmod +x my_first_script.sh`. You can test that this has all worked correctly by running it locally using `./my_first_script.sh` and checking that it prints the line to the terminal.

In order to run the job, HTCondor needs another file that tells it all the information about the job that it needs. This is known as a submit file. Create a second file, `submit.sub`, and put the following into it:

```
executable  = my_first_script.sh
output      = output.out
error       = error.err
log         = log.log
queue
```

There are many more options that can be added to a submit file, but these ones are the most basic. Here is a short explanation of what each one is doing:

* **executable**: the executable/script/command that you want to run.
* **output**: the location of the file that will contain the terminal output (STDOUT) of the executable.
* **error**: similar to output, except for any errors (STDERR).
* **log**: the location of the file that will store the logging information created by HTCondor as it tracks your job.
* **queue**: tells HTCondor to add the job to the queue.

You can now submit your job by running `condor_submit submit.sub`. After a few seconds, it should have produced an output similar to the one shown below.

```
Submitting job(s).
1 job(s) submitted to cluster 2875625.
```

This tells you that the job has been submitted, and gives you the cluster ID (essentially, this is a unique reference number that HTCondor assigns to your job). If you look in the current directory, you should see that it has automatically created the three files specified in the submit file earlier (`output.out`, `error.err`, and `log.log`). As the job progresses, it will add to these files as appropriate.

We can now track the progress of the job if we wish. There are multiple ways to do this, but here are some of the most useful:

* `condor_q`: shows you the current queue, and the status of your job.

```
-- Schedd: bigbird11.cern.ch : <137.138.76.70:9618?... @ 05/16/19 18:22:43
OWNER   BATCH_NAME                 SUBMITTED   DONE   RUN    IDLE  TOTAL JOB_IDS
djwhite CMD: my_first_script.sh   5/16 18:22      _      _      1      1 2875625.0

1 jobs; 0 completed, 0 removed, 1 idle, 0 running, 0 held, 0 suspended
```

The above output is from shortly after submitting. The job starts out in the 'idle' state while it waits for HTCondor to schedule it. Even for a job as simple as this, this step can take up to a couple of minutes. Following this, the job's status will change to 'running', and eventually to 'done' when complete.

{% callout "Tracking the queue in real-time" %} By itself, this command will print the status of your jobs at the current time to the terminal. If you want to see how they're progressing, you may find it useful to prefix it with `watch` (e.g. `watch condor_q`). This will repeatedly run the command at regular intervals, allowing you to see how the output changes over time. You can press `Ctrl+c` to exit back to the terminal at any time.
{% endcallout %}

* `condor_wait -status <log_file>`: watches the log file and waits until the jobs have been completed before exiting.

{% challenge "Find some additional information" %} These commands only display a fraction of the posisble information. Look at the documentation for both `condor_q` and `condor_wait` (either online, or using the `-help` flag) to see what else is available.
{% endchallenge %}

Hopefully, by this point, the job should have finished! To check that it worked properly, take a look inside your output file, which should now contain the line `Hello world!`.

{% challenge "Output files and output files" %} The output file `output.out` specified in the submit file contains the terminal output of the executable. What happens if the executable creates a file as well as printing to the terminal?
{% endchallenge %}




 
