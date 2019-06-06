# Using the queue command

{% objectives "Learning Objectives" %}
* Be able to submit multiple jobs at once by adding to the `queue` command
* Be able to use variables in a submit file to automatically change the behaviour of each job
* Learn some different options for queuing multiple jobs
{% endobjectives %} 

So far, we have ended all of our submit files by simply writing `queue` to tell HTCondor to schedule a single execution of the job that we defined. However, it wouldn't be batch computing without being able to submit large numbers of jobs at once!

Let's create another simple job using a python script, `my_script.py`. We want both file input and output - for this example, let's have it reverse the input text, and save it to a new file. We also want to specify both files as arguments. Such a script might look something like:

```
import argparse
parser = argparse.ArgumentParser()
parser.add_argument('inputfile', help='Specify an input file', type=str)
parser.add_argument('outputfile', help='Specify an output file', type=str)
args = parser.parse_args()

f_in = open(args.inputfile, 'r')
f_out = open(args.outputfile, 'w+')
f_out.write( ''.join( reversed(f_in.read()) ) )
```

As before, we'll use an executable shell script `exec.sh` as a wrapper:

```
#!/bin/bash
python3 my_script.py "$@"
```

And finally, the all-important submit file:

```
executable  = exec.sh
universe    = vanilla
arguments   = data.txt result.txt
+JobFlavour = "espresso"

transfer_input_files = my_script.py, input/data.txt
transfer_output_files = result.txt
should_transfer_files = IF_NEEDED
when_to_transfer_output = ON_EXIT

output      = output.out
error       = error.err
log         = log.log
queue 4
```

Most of this should look familiar at this point, with the most significant new addition being the final line: `queue 4`. If you create a `data.txt` file in the `input` directory, submit this, and then look at the queue, you will see that HTCondor has indeed scheduled 4 separate jobs. You can see these jobs in the queue individually by running `condor_q -nobatch`:

```
 ID         OWNER            SUBMITTED     RUN_TIME ST PRI SIZE CMD
2908604.0   djwhite         5/22 12:07   0+00:00:00 I  0    0.0 exec.sh data.txt result.txt
2908604.1   djwhite         5/22 12:07   0+00:00:00 I  0    0.0 exec.sh data.txt result.txt
2908604.2   djwhite         5/22 12:07   0+00:00:00 I  0    0.0 exec.sh data.txt result.txt
2908604.3   djwhite         5/22 12:07   0+00:00:00 I  0    0.0 exec.sh data.txt result.txt

4 jobs; 0 completed, 0 removed, 4 idle, 0 running, 0 held, 0 suspended
```

This shows a bit more information about the jobs than the standard `condor_q`. In particular, note that all of these jobs have the same cluster ID, but each has a different process ID ranging from 0 to 3. In almost all cases, when you submit multiple jobs using a single queue command, they will share the same cluster ID, and have sequential process IDs starting from 0.

There are a couple of issues with the above script. Firstly, we're running the exact same job (with the exact same input) multiple times, which is rarely what we need. Secondly, once the jobs have all completed, notice that you only have one `output.out` file - each time one completes, it overwrites any existing output file with its own output because each one has the same name. In order to fix these issues, we need to be able to distinguish each job created by a single submit file. We can do this using the variables `$(ClusterId)` and `$(ProcId)` (or, equivalently, `$(Cluster)` and `$(Process)` respectively). These variables are automatically-defined for each job individually. Using this, you can modify your submit file to read:

```
executable  = exec.sh
universe    = vanilla
arguments   = data.txt result_$(ProcID).txt
+JobFlavour = "espresso"

transfer_input_files = my_script.py, input/data.txt
transfer_output_files = result_$(ProcID).txt
should_transfer_files = IF_NEEDED
when_to_transfer_output = ON_EXIT

output      = output/output.$(ClusterId).$(ProcId).out
error       = error/error.$(ClusterId).$(ProcId).err
log         = log/log.$(ProcId).log
queue 4
```

If you first create the folders `output`, `error`, and `log`, and then submit this job, HTCondor will automatically create 4 separate output and error files, and will transfer back 4 separate result files.

{% challenge "Multiple input files" %} The result files from the above script all contain the same information, since the same input file is being used and the script is deterministic. How could you modify it to allow for the use of multiple input files?

{% solution "Solution" %}
If you create multiple input files (e.g. `input/data_0.txt`, `input/data_1.txt`, ...), you can modify the `transfer_input_files` option to read:

```
transfer_input_files = my_script.py, input/data_$(ProcId).txt
```
{% endsolution %}

{% endchallenge %}

<!-- 2 typical ways to manage files for lots of jobs: 1) folders for output, error, logs, input, etc., with different (numbered) filenames 2) different (numbered) folders for each job, with the same filenames (use initialdir = job$(ProcId)) -->

### More on variables

As you've seen above, the variables that HTCondor automatically defines provide a lot of potential for the automisation of large numbers of jobs. To expand on this, you are also able to define your own variables within your submit file using very similar syntax. Among other things, this is particularly useful for avoiding repetition - in the example above, the input and output file names had to be written in two places. Equivalent functionality can be achieved by using:

```
INPUTFN = data.txt
OUTPUTFN = result_$(ProcID).txt
# ...
arguments = $(INPUTFN) $(OUTPUTFN)
# ...
transfer_input_files = my_script.py, input/$(INPUTFN)
transfer_output_files = $(OUTPUTFN)
```

### Other ways to queue





