# Submitting a simple job

{% objectives "Learning Objectives" %}

* Be able to submit a simple job using HTCondor
* Monitor the progress of your jobs, from a terminal and a browser

{% endobjectives %} 

Now it's time to make and submit a job of our own. To start with, you'll need to create an executable for the job you want to run. Create a file in your AFS area *(currently, submitting jobs from EOS is not supported, although coming soon!)* on lxplus called `my_first_script.sh`, and add the following lines:

```
#!/bin/bash
echo "Hello world!"
```

Next, you need to make this script executable by running `chmod +x my_first_script.sh`. You can test that this has all worked correctly by running it locally using `./my_first_script.sh` and checking that it prints the line to the terminal.

In order to run this executable, HTCondor needs another file that gives it all the information it needs about the job. This is known as a submit file. Create a second file, `submit.sub`, and put the following into it:

```
executable  = my_first_script.sh
universe    = vanilla
output      = output.out
error       = error.err
log         = log.log
queue
```

There are many more options that can be added to a submit file, but these ones are the most basic. Here is a short explanation of what each one is doing:

* **executable**: the executable/script/command that you want to run.
* **universe**: the base runtime environment for the job.
* **output**: the location of the file that will contain the terminal output (STDOUT) of the executable.
* **error**: similar to output, except for any errors (STDERR).
* **log**: the location of the file that will store the logging information created by HTCondor as it tracks your job. Note that specifying a log file is required, and you will get an error if you try to submit a job without one.
* **queue**: tells HTCondor to add the job to the queue.

You can now submit your job by running `condor_submit submit.sub`. After a few seconds, it should have produced an output similar to the one shown below.

```
Submitting job(s).
1 job(s) submitted to cluster 2875625.
```

This tells you that the job has been submitted, and gives you the cluster ID (essentially, this is a unique reference number that HTCondor assigns to the job(s) you submitted). If you look in the current directory, you should see that it has automatically created the three files specified in the submit file earlier (`output.out`, `error.err`, and `log.log`). As the job progresses, it will add to these files as appropriate.

### Tracking your jobs

While we're waiting for the job to complete, we can track its progress from a terminal. There are multiple ways to do this, but here are some of the most useful:

* `condor_q`: shows you the current queue, and the status of your job.

```
OWNER   BATCH_NAME                 SUBMITTED   DONE   RUN    IDLE  TOTAL JOB_IDS
djwhite CMD: my_first_script.sh   5/16 18:22      _      _      1      1 2875625.0

1 jobs; 0 completed, 0 removed, 1 idle, 0 running, 0 held, 0 suspended
```

The above output is from shortly after submitting. A typical job starts out in the 'idle' state while it waits for HTCondor to schedule it. Even for a job as simple as this, this step can take up to a couple of minutes. Following this, the job's status will change to 'running', and eventually to 'completed' when finished.

The other three job states are less common. A job may be put into the 'hold' state if there is something wrong with the submit file, or the user uses the `condor_hold` command. Similarly, a user can suspend their job using `condor_suspend`, or remove it using `condor_rm`. A held job can be restarted by using `condor_release`.

{% callout "Tracking the queue in real-time" %}


 By itself, this command will print the status of your jobs at the current time to the terminal. If you want to see how they're progressing, you may find it useful to prefix it with `watch` (e.g. `watch condor_q`). This will repeatedly run the command at regular intervals, allowing you to see how the output changes over time. You can press `Ctrl+c` to exit back to the terminal at any time.

{% endcallout %}

* `condor_wait -status <log_file>`: watches the log file and waits until the jobs have been completed before exiting.

{% challenge "Find some additional information" %}


 These commands only display a fraction of the posisble information. Look at the documentation for both `condor_q` and `condor_wait` (either online, or using the `-help` flag) to see what else is available.

{% endchallenge %}

For a more holistic overview, we can also use [Grafana](https://monit-grafana.cern.ch/d/000000869/user-batch-jobs?orgId=5&refresh=5m), which is a browser-based monitoring system. Log in using your CERN SSO, and it will take you to the 'User Batch Jobs' dashboard. Go to the 'user' drop-down menu, and search for your username by typing it in.

This monitor displays a lot of different graphs and statistics. Importantly, in the one called 'Job Summary', you should see that you now have one job, either in the idle or running state. If it's not there, it should appear after a couple of minutes. It also displays information including when your jobs were submitted, when they started, what your user priority score is, and more.

{% challenge "Other dashboards" %}

 There are a whole host of dashboards available beyond 'User Batch Jobs'. Take a look and see what else you can find that might be useful (if you want, you can try starting with 'Cluster Batch Jobs').

{% endchallenge %}

Hopefully, by this point, the job should have finished! To check that it worked properly, take a look inside your output file, which should now read:

```
Hello world!
```




