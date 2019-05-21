# More submit file options

{% objectives "Learning Objectives" %}
* Learn some more options you can specify for your jobs
* Be able to submit jobs with input/output files and arguments
* Be able to debug some common errors
{% endobjectives %} 

The options showcased in submit file used in the previous section are enough for certain simpler jobs, but for many purposes you will want to do more. This section of the lesson will cover a selection of some of the more common and/or useful ones, and the settings you'll likely want to use for your own jobs. For more information, a complete list can be found [in the documentation](https://research.cs.wisc.edu/htcondor/manual/v7.6/condor_submit.html) for `condor_submit`.

### Input files

Often, you'll want your job to process data that's stored in another file. Create a file `input.txt` and add some lines of text to it. To test this, you can use the following bash script, placed in `exec.sh`:

```
#!/bin/bash
echo "# Contents of file 'input.txt': #"
cat input.txt
```

Remember to make the script exectable, and to test it locally first. Then, go into your submit file. Here, you'll want to make sure that the `executable` option is set to run `exec.sh`, and also to add the following line to the body of your submit file:

```
transfer_input_files = input.txt
```

Once you've done this, you can submit your job. Behind the scenes, HTCondor has a remote temporary storage area where it runs your executable. The `transfer_input_files` option tells it to copy additional files to this storage area, so that the executable can access them.

{% callout "Transferring files from other directories" %} No matter where HTCondor copies a file from, by default it will place it in the same directory as the executable when it's running it. This means that if your file was stored in the directory `data/input.txt` instead, you would need to modify the path in the submit file to `transfer_input_files = data/input.txt`, but the executable above would not need to change.

You can access the remote area where your job is running using the command `condor_ssh_to_job -auto-retry <ClusterId>.<ProcID>`. Here, the cluster ID is the number it assigned to your job(s) upon submission (in the example from the previous section, this was 2875625), and the process ID is the number of the job within that cluster (for now, there is only one job, so this is just 0 - more on multiple jobs in the next section). From here, you can view the file structure. The executable is here, although it now has the name `condor_exec.exe`. Also, there are files `_condor_stdout` and `_condor_stderr` which contain the output to be transferred back to your output & error files later. The file `input.txt` can be found in this directory as well, regardless of where it was located in relation to your submit file. When you're finished, you can type `logout` to exit the ssh connection.
{% endcallout %}

You can use this to submit scripts in other formats, such as python scripts or C++ code. You will usually want to write a wrapper shell script to be your executable, and then to transfer the code you want to run as an input file. For example, let's create a python script `process_file.py`:

```
f = open('input.txt', 'r')
print('# Contents of file \'input.txt\' using python: #')
print(f.read())
```

You'll also need to add this script to the list of input files, so change the relevant line in your submit file to:

```
transfer_input_files = process_file.py, input.txt
```

Finally, let's modify the executable script, `exec.sh`, so that it will run this python script. Change it so that it reads:

```
#!/bin/bash
python3 process_file.py
```

Now you can submit the job, and check that this works as expected.

{% callout "Wrapper shell scripts" %} It may seem unnecessarily convoluted to use a shell script that sits in-between the submit file and the python script here, especially when it seems to only pass arguments straight to the python script. However, this step is good practice for good reasons. Most notably, for certain other jobs, you may need to use the shell script to set up the job environment correctly (for example, sourcing the correct software version from CVMFS).
{% endcallout %}

### Output files

HTCondor uses the name 'output file' to refer to different things in different contexts, so it's important to make it clear which is meant. One of these is the one that's been used so far, which refers to the one specified in the submit file (using the `output` option) which contains the STDOUT (terminal output) of your executable. The other use is for any new files that your executable may create as it runs. This part will cover the second of these.

Let's modify our python script so that, instead of displaying the file contents in the terminal, it puts all the text into upper-case and saves it to a new file. This new script will look something like:

```
f_in = open('input.txt', 'r')
f_out = open('result.txt', 'w+')
print('Converting input file to upper-case...')
f_out.write( f_in.read().upper() )
```

Now you can submit this job. Once it has finished you will find that HTCondor has successfully produced the file `result.txt`, and has automatically put it in your working directory, without even having to add anything to the submit file! This is the default behaviour of the `transfer_output_files` option - when it isn't specified, it automatically transfers back all files that have been created or modified in the job's temporary working directory.

However, while this behaviour is helpful, it's generally preferable to specify which output files you want HTCondor to transfer back to you. There are a couple of reasons for this, but the most important ones are:

* Files aren't automatically transferred when running using certain universes (notably, this applies to most grid universes).
* If you don't want all files to be transferred back, but only a subset of them.
* If the files created by the program are made inside of directories.

To do this, you simply need to add this line to the submit file:

```
transfer_output_files = result.txt
```

Some additional options that are useful to know are:

* `should_transfer_files`: enables/disables file transfer altogether. This has three possible values: `YES` (usually the default) always transfers files to the remote working directory; `IF_NEEDED` will access the files from a shared file system if possible, otherwise it will transfer the files; and `NO` disables file transfer. If you use this option, you must also specify...

* `when_to_transfer_output`: specifies when output files should be transferred back. This has two possible values: `ON_EXIT` transfers files when the job ends on its own; and `ON_EXIT_OR_EVICT` will do this, as well as if the job is evicted from a machine. This is useful for longer programs that have the capability to resume from where they exited. This has no default.

* `transfer_output_remaps`: allows you to transfer output files back with different names, and to locations other than the initial working directory.

### Arguments

If your executable takes command-line arguments, you need to specify them in the submit file using the `arguments` option. Anything you put here will be passed as an argument to the executable.

To test this out, let's modify our python script to add an argument that will let us choose whether to change the text to upper or lower-case. For convenience, you can use the `argparse` library:

```
import argparse
parser = argparse.ArgumentParser()
parser.add_argument('case', help='Choose what case to change the text to', type=str, choices=['upper','lower'])
args = parser.parse_args()

f_in = open('input.txt', 'r')
f_out = open('result.txt', 'w+')
print('Converting input file to {c}-case...'.format(c=args.case))
if args.case == 'upper':
	f_out.write( f_in.read().upper() )
elif args.case == 'lower':
	f_out.write( f_in.read().lower() )
```

You can now add the arguments to the submit file, by including the line:

```
arguments   = upper
```

This will pass the argument `upper` to our executable (equivalent to running `./my_script upper`). However, because this uses a wrapper shell script, there is one more step needed here in order to get it to pass these arguments to the python script. To do this, you can just modify your executable to be:

```
#!/bin/bash
python3 process_file.py "$@"
```

This will cause the script to pass all arguments it receives along to the python script.

Submitting the job now should give produce a file `result.txt` with all the text all in upper-case. You can also switch modes by changing the argument in the submit file from `upper` to `lower` - submitting this job should replace `result.txt` with a version in all lower-case.

### Resources and Requirements

You can specify running conditions of your jobs in the submit file. The system for doing this uses what are known as ClassAd attributes, which are essentially properties of some part of the system (machines, jobs, the scheduler, etc.). You can use these to tell HTCondor about the requirements and preferences you have for your jobs. These can be specified in the submit file using the `requirements` and `rank` options respectively.

A complete list of ClassAd attributes can be found [in the documentation](http://research.cs.wisc.edu/htcondor/manual/v8.8/ClassAdAttributes.html#x167-1231000A), but in this part a few particularly useful ones are highlighted. Note that, by using these, you are restricting how your job can run, which means that it will likely take longer to run. You should use these with care, and speak with the Batch support team for advice if you're not sure.

* `OpSysAndVer`: specifies the operating system (and version) that a machine is using. Currently, some of the grid resources are using SL6, while others have been migrated to CentOS7. For example, if you wanted to ensure that your program runs using CentOS7, you would need to add `requirements = (OpSysAndVer =?= "CentOS7")` to your submit file.

* `ClockMin` and `ClockDay`: the time of day (in minutes since midnight) and day of the week respectively. These can be used to give your jobs a preference for running outside of peak times - for example, you could use `rank = ( (ClockDay == 0) || (ClockDay == 6) ) && (ClockMin <= 480)` to encourage your job to run between midnight and 8am on weekends.

* `Memory`: the amount of physical memory the machine has, measured in megabytes. It is better to use this only in a `rank` option, such as `rank = (Memory >= 100)`, since there is a more preferable way to set memory & CPU requirements in your submit file: the `RequestMemory` and `RequestCpus` options. There is a 2GB/core limit

In addition to these, you should also always specify the duration of your job, which allows HTCondor to schedule things more effectively, and to give preference to shorter jobs. Importantly, it will also terminate jobs that go over their maximum runtime. Here, you are assigning a ClassAd attribute to the job rather than looking at the attributes of other systems or machines, so they are specified in a slightly different way - to set a maximum runtime of 1 hour (3600 seconds), you would need to add the following line to your submit file:

```
+MaxDuration = 3600
```

Equivalently, one could use the `JobFlavour` attribute instead:

```
+JobFlavour = "microcentury"
```

There are seven possible job flavours that one can use:

```
espresso     = 20 minutes
microcentury = 1 hour
longlunch    = 2 hours
workday      = 8 hours
tomorrow     = 1 day
testmatch    = 3 days
nextweek     = 1 week
```

### Notifications

If you want, you can tell HTCondor to send you emails updating you on the progress of your job. In your submit file, add the line:

```
notify_user = <your_email_address>
```

By default, this will send you an email when your job completes. You can change the conditions for receiving a notification using a second option:

```
notification = < Always | Complete | Error | Never >
```

The default setting is `Complete`. The `Error` setting is also extremely useful: imagine you have a long job you intend to leave running over the weekend - if something were to go wrong, you may want to be alerted so that you can resubmit it sooner rather than later.

{% challenge "Debug some jobs" %} TODO: [Here]() you can download a `.tar(.GZ??)` file containing the scripts for 3 jobs, each of which features some commonly-experimenced error or mistake. For each one, submit the job, see if you can figure out what went wrong with it (making use of the error files), and then try to fix it. To unpack the file, run `LINE`.

{% solution "Solution to problem 1" %}
The submit file here does not specify a log file, which is always required. To fix this, just add `log = log.log` to the submit file.
{% endsolution %}

{% solution "Solution to problem 2" %}
**Linear fitter** - this one actually has 2 errors, but only the first will cause an error message.

Firstly, the submit file contains the wrong argument for passing the input file. The `transfer_input_files` option correctly transfers it from its subdirectory to the remote storage area, but when it arrives it is no longer inside any subdirectory relative to the executable. When the job runs, the `arguments` option in the submit file transfers the file location `input/data.txt` to the executable, which in turn passes it to the python script, which finds no file at that location. To fix this, the line in the submit file should be changed to `arguments = data.txt`.

Secondly, the `plot.png` file containing the plotted graph is not returned to the user once the job has completed. This is because the python script saves it inside a subdirectory, and the `transfer_output_files` option has not been specified in the submit file (the default behaviour of this option means that files created inside subdirectories will not be transferred back). To fix this, add the line `transfer_output_files = images/plot.png`
{% endsolution %}

{% solution "Solution to problem 3" %}
**Prime factor finder** - this one is a tricky one, since it appears to run correctly until it completes and returns an empty file. The reason for this is not actually to do with the scripts, which are all correct - rather, the shell script `script.sh` has just not been made into an executable. To fix this, run `chmod +x script.sh` before submitting.
{% endsolution %}

{% endchallenge %}




